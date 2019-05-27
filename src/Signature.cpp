#include "Signature.hpp"
#include "Logging.hpp"
#include "crc.hpp"
#include <iostream>
#include <exception>
#include <cstring> //memset

Meta_data::Meta_data() : inputFile_(nullptr),
                         outputFile_(nullptr),
                         threads_num_(0),
                         block_size_(0),
                         input_file_size_(0),
                         qData_maxcount_(0),
                         number_of_blocks_(0)
{
}

void Meta_data::init(std::ifstream &inputFile, std::ofstream &outputFile, size_t blockSize, size_t threadCnt)
{
    inputFile_ = &inputFile;
    outputFile_= &outputFile;
    block_size_   = blockSize;

    inputFile_->seekg(0, inputFile_->end);
    input_file_size_ = static_cast<size_t>(inputFile_->tellg());
    inputFile_->seekg(0, inputFile_->beg);

    number_of_blocks_ = input_file_size_ / block_size_;
    if (input_file_size_ % block_size_)
        ++number_of_blocks_;

    threads_num_ = threadCnt;
    qData_maxcount_ = threads_num_ * 2;
    calc_tasks.resize(threads_num_);

    try
    {
        for(auto i = 0; i < qData_maxcount_; ++i)
           memory_pool.push(std::unique_ptr<uint8_t[]>(new uint8_t[block_size_]));
    }
    catch (const std::exception &e)
    {
        std::rethrow_exception(std::make_exception_ptr(std::runtime_error( std::string("Unable to allocate a memory: ") + e.what())));
    }

}


void Meta_data::reading()
{
    LOG(INFO, "START READING");
    uint32_t idx = 0;

    while (idx < number_of_blocks_)
    {
        if (qData.size() < qData_maxcount_)
        {
            std::unique_ptr<uint8_t[]> buf;

            std::unique_lock<std::mutex> lock(pool_mutex);
            pool_cv.wait(lock, [this](){return !memory_pool.empty();});
            buf = std::move(memory_pool.front());
            memory_pool.pop();
            lock.unlock();

            size_t read_size = block_size_;
            inputFile_->read(reinterpret_cast<char *>(buf.get()), read_size);
            LOG(DEBUG, "READ BLOCK " << idx + 1 << "/" << number_of_blocks_);

            std::unique_lock<std::mutex> qlock(queue_mutex);
            qData.push({idx, std::move(buf)});
            qlock.unlock();
            queue_not_empty.notify_one();
            idx++;
        }
        if (qData.size() == qData_maxcount_)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            continue;
        }
    }
    LOG(INFO, "STOP READING");
}


void Meta_data::calculating_crc()
{
    std::unique_lock<std::mutex> qlock(queue_mutex);
    queue_not_empty.wait(qlock, [this](){ return !qData.empty();});
    auto block = std::move(qData.front());
    qData.pop();
    qlock.unlock();
    if (block.n == number_of_blocks_ - 1)
        memset(block.data.get(), 0, block_size_ - (input_file_size_ - (number_of_blocks_ - 1) * block_size_));
    auto crc = get_crc(block.data.get(), block_size_);

    std::unique_lock<std::mutex> pool_lock(pool_mutex);
    memory_pool.push(std::move(block.data));
    pool_cv.notify_one();
    pool_lock.unlock();

    LOG(DEBUG, "CALC BLOCK " << block.n + 1 << " crc: " << crc);
    std::unique_lock<std::mutex> mlock(map_mutex);
    crc_map.insert({block.n, crc});
    mlock.unlock();
    map_not_empty.notify_one();
}

void Meta_data::writing()
{
    LOG(INFO, "START WRITING");
    size_t idx = 0;

    while (idx < number_of_blocks_)
    {
        std::unique_lock<std::mutex> mlock(map_mutex);
        map_not_empty.wait(mlock, [this](){ return !crc_map.empty(); });
        auto chunk = crc_map.begin();
        if (chunk->first != idx)
        {
            mlock.unlock();
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            continue;
        }
        crc_map.erase(crc_map.begin());
        mlock.unlock();
        LOG(DEBUG,"WRITE BLOCK " << chunk->first + 1);
        outputFile_->write(reinterpret_cast<char *>(&chunk->second), sizeof(uint32_t));
        idx++;
    }
    LOG(INFO, "STOP WRITING");
}

void Meta_data::processing()
{
    try {
        LOG(NO_LOG, "START PROCESSING");
        read_task = std::async(std::launch::async, &Meta_data::reading, this);
        write_task = std::async(std::launch::async, &Meta_data::writing, this);

        size_t  idx = 0;
        while(idx < number_of_blocks_)
        {
            LOG(INFO, "START CALCULATING");
            for (auto &e : calc_tasks)
            {
                e = async(std::launch::async, &Meta_data::calculating_crc, this);
                idx++;
                if(idx == number_of_blocks_)
                    break;
            }
            for (auto &e : calc_tasks)
                if(e.valid())
                    e.get();
        }
        LOG(INFO, "STOP CALCULATING");
        read_task.wait();
        write_task.wait();
        LOG(NO_LOG, "STOP PROCESSING");
    }
    catch (...)
    {
        LOG(NO_LOG, "Error during processing");
        throw;
    }

}