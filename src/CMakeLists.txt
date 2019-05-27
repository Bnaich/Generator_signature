cmake_minimum_required(VERSION 3.00)
project(generate_signature)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-pthread")
add_executable(generate_signature main.cpp Signature.hpp Signature.cpp Files.hpp crc.hpp crc.cpp Logging.hpp)
