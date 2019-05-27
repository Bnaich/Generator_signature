#ifndef __MY_LOG_H__
#define __MY_LOG_H__

#include <cstdio>
#include <unistd.h>

enum class LOG_LEVELS
{
    DEBUG = 1,
    INFO = 2,
    NO_LOG = 3
};

extern int CONFIG_LOG_LEVEL;
#define LOG(LEVEL, ...)                                                           \
do                                                                                \
{                                                                                 \
    if(static_cast<int>(LOG_LEVELS::LEVEL) >= CONFIG_LOG_LEVEL)                   \
    {                                                                             \
       std::cout << "MESSAGE: "  << __VA_ARGS__  <<  std::endl;                   \
       std::cout << "\tFile: " << __FILE__ << std::endl;                          \
       std::cout << "\tline: " << __LINE__ << std:: endl;                         \
    }                                                                             \
} while (0, 0)

#endif // __MY_LOG_H__