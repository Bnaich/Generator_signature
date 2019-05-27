#ifndef TASK_FILES_HPP
#define TASK_FILES_HPP

#include <fstream>
#include <iostream>

constexpr int kb_size = 1024;
constexpr int def_readblocksize_in_Kb = 1024 * 1024;
constexpr int def_maxreadblocksize = 256 * def_readblocksize_in_Kb;

bool InputFileSettings(std::string name, std::ifstream& fs_name)
{
    std::ios::iostate old_stream_settings = fs_name.exceptions();
    fs_name.exceptions(std::ios::failbit | std::ios::badbit);
    try
    {
        if (name.empty())
        {
            std::cout << "FULL path for inner file:" << std::endl;
            std::getline(std::cin, name);
        }
        fs_name.open(name.data(), std::ios::out | std::ios::binary);
    }
    catch (std::ios::ios_base::failure &err)
    {
        std::cerr << "ERROR: " << err.what() << " while trying to open Input file" << std::endl;
        fs_name.exceptions(old_stream_settings);

        return false;
    }
    catch (std::length_error &)
    {
        std::cout << "File name too long" << std::endl;

        return false;
    }
    fs_name.exceptions(old_stream_settings);
    return true;
}

bool OutputFileSettings(std::string& name, std::ofstream& fs_name)
{
    std::ios::iostate old_stream_settings = fs_name.exceptions();
    fs_name.exceptions(std::ios::failbit | std::ios::badbit);
    try
    {
        if (name.empty())
        {
            std::cout << "ENTER the FULL path to the output file: " << std::endl;
            std::getline(std::cin, name);
            if (name.empty())
            {
                fs_name.exceptions(old_stream_settings);
                return false;
            }
        }

        fs_name.open(name, std::ios::in | std::ios::binary | std::ios::app);
    }
    catch (std::ios::ios_base::failure &error)
    {
        std::cerr << "ERROR: " << error.what() << " while trying to open Input file" << std::endl;

        fs_name.exceptions(old_stream_settings);

        return false;
    }
    fs_name.exceptions(old_stream_settings);
    return true;
}

bool BlockSizeSettings(size_t& blockSize)
{
    try
    {
        if (blockSize == 0)
        {
            std::cout << "Please Enter size of block in Kb: ";
            std::cout << "(min = 1Kb; max = 262144Kb; default = 1024Kb)" << std::endl;

            std::cin  >> blockSize;
            if(blockSize == 0) // default
            {
                std::cout << "Blocksize set as default: 1Mb" << std::endl;
                blockSize = def_readblocksize_in_Kb;
                return true;
            }

            if (blockSize > def_maxreadblocksize / 1024)
            {
                std::cout << "Blocksize is too BIG" << std::endl;
                return false;
            }

            blockSize *= kb_size;
        }
    }
    catch (std::ios::ios_base::failure &error)
    {
        std::cerr << "Error: " << error.what() << std::endl;
        return false;
    }

    return true;
}

#endif //TASK_FILES_HPP

