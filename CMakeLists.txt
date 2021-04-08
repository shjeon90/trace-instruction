cmake_minimum_required(VERSION 3.19)
project(trace_addr)

set(PIN_ROOT /home/horo90/Downloads/pin-3.18-98332-gaebd7b1e6-gcc-linux)

include_directories(${PIN_ROOT}/extras/xed-intel64/include/xed)
include_directories(${PIN_ROOT}/source/include/pin)
include_directories(${PIN_ROOT}/source/include/pin/gen)

set(CMAKE_CXX_STANDARD 17)

add_executable(trace_addr trace-instruction.cpp)