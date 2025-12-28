cmake_minimum_required(VERSION 3.16)

project(TheOrderBook
    VERSION 1.0
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# for vim
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_subdirectory(apps)

# Google Benchmark (library only, no tests, no downloads)
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
set(BENCHMARK_ENABLE_GTEST_TESTS OFF CACHE BOOL "" FORCE)
set(BENCHMARK_USE_BUNDLED_GTEST OFF CACHE BOOL "" FORCE)
set(BENCHMARK_ENABLE_INSTALL OFF CACHE BOOL "" FORCE)
add_subdirectory(external/benchmark)


add_executable(orderbook_bench benchmarks/orderbook_bench.cpp)

target_include_directories(orderbook_bench
    PRIVATE
        ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(orderbook_bench
    PRIVATE
        benchmark::benchmark
        benchmark::benchmark_main
)

