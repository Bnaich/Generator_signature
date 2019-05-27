#include "Logging.hpp"
#include "Files.hpp"
#include "Signature.hpp"
#include <iostream>
#include <exception>

int CONFIG_LOG_LEVEL;


class Timer
{
public:
    Timer() : start(std::chrono::steady_clock().now()) { }
    ~Timer()
    {
        std::chrono::duration<float> duration = std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::steady_clock().now() - start);
        LOG(NO_LOG, "DURATION in milliseconds: " << std::chrono::duration_cast<std::chrono::milliseconds>(duration).count());
    }
private:
    std::chrono::time_point<std::chrono::steady_clock> start;
};

bool LoggingSettings(size_t& loglevel)
{
    try
    {
        if (loglevel == 0)
        {
            std::cout << "Choose log level: ";
            std::cout << "(DEBUG = 1; INFO = 2; NO_LOG = 3 (default - minimum messages)" << std::endl;
            std::cin  >> loglevel;
            if(loglevel == 0 || loglevel == 3) // default
            {
                std::cout << "Loglevel set as default: NO_LOG" << std::endl;
                CONFIG_LOG_LEVEL = 3;
                return true;
            }

            else if (loglevel == 2)
            {
                std::cout << "Loglevel set as LOG" << std::endl;
                return true;
            }
            else if (loglevel == 1)
            {
                std::cout << "Loglevel set as DEBUG" << std::endl;
                return true;
            }
            else
                return false;
        }
    }
    catch (std::ios::ios_base::failure &error)
    {
        std::cerr << "Error: " << error.what() << std::endl;
        return false;
    }

    return true;
}


int main()
{
    try
    {
        std::string inputFileName;
        std::string outputFileName;

        std::ifstream inputFileStream;
        if (!InputFileSettings(inputFileName, inputFileStream))
        {
            std::cout << "No input file name" << std::endl;
            return 0;
        }

        std::ofstream outputFileStream;
        if (!OutputFileSettings(outputFileName, outputFileStream))
        {
            std::cout << "No output file name" << std::endl;
            return 0;
        }

        size_t blocksize = 0;
        if (!BlockSizeSettings(blocksize))
        {
            std::cout << "FAIL to set blocksize" << std::endl;
            return 0;
        }
        size_t loglevel = 0;
        if (!LoggingSettings(loglevel))
        {
            std::cout << "FAIL to set Loglevel" << std::endl;
            return 0;
        }

        Meta_data md;
        md.init(inputFileStream, outputFileStream, blocksize);
        auto timer = Timer();
        md.processing();

        inputFileStream.close();
        outputFileStream.close();

        return 0;
    }
    catch (const std::exception &e)
    {
        std::cerr << "ERROR: " << e.what();
    } catch (...)
    {
        std::cerr << "UNKNOWN ERROR";
    }

}
