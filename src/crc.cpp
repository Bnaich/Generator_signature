#include "crc.hpp"

uint32_t get_crc(const uint8_t * p, std::size_t block_size)
{
    boost::crc_32_type crc;
    for (int  i = 0; i < block_size; ++i)
        crc.process_byte(*(p + i));
    return crc.checksum();
}

