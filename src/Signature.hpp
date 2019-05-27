#ifndef TASK_SIGNATURE_HPP
#define TASK_SIGNATURE_HPP

#include <fstream>
#include <queue>
#include <map>
#include <thread>
#include <memory>
#include <vector>
#include <future>


struct Meta_data
{
    Meta_data();

    void init(std::ifstream&  inputFile,
              std::ofstream& outputFile,
              size_t blockSize,
              size_t threadCnt = std::thread::hardware_concurrency());
    void reading();
    void processing();
    void calculating_crc();
    void writing();
    struct Chunk
    {
        uint32_t                     n;
        std::unique_ptr<uint8_t[]>   data;
    };
    std::ifstream* inputFile_;
    std::ofstream* outputFile_;

    std::queue<Chunk> qData;         // read-queue
    std::map<uint32_t, uint32_t> crc_map; // crc write-map

    size_t block_size_;
    size_t threads_num_;

    size_t      input_file_size_;
    uint32_t    qData_maxcount_;     // max length of read-queue
    uint32_t    number_of_blocks_;
    /* MEMORY */
    std::queue<std::unique_ptr<uint8_t []>> memory_pool;
    std::mutex pool_mutex;
    std::condition_variable pool_cv;

    /* THREADS & GUARDS */
    std::future<void> read_task;
    std::future<void> write_task;
    std::vector<std::future<void>> calc_tasks;

    std::condition_variable queue_not_empty;
    std::condition_variable map_not_empty;
    std::mutex queue_mutex;
    std::mutex map_mutex;
};

#endif //TASK_SIGNATURE_HPP
