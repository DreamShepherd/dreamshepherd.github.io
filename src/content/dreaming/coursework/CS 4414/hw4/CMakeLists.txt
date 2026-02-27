cmake_minimum_required(VERSION 3.10)
project(BignumProject VERSION 1.0)

execute_process(COMMAND getconf _NPROCESSORS_ONLN OUTPUT_VARIABLE NUM_CORES)
string(STRIP ${NUM_CORES} NUM_CORES)
set(CMAKE_BUILD_PARALLEL_LEVEL ${NUM_CORES})

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD 17)

add_executable(bignum main.cpp threadpool.cpp bignum.cpp)
