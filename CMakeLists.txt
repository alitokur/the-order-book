cmake_minimum_required(VERSION 3.16)

project(TheOrderBook
    VERSION 1.0
    LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# for vim
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Add include folder
include_directories(${CMAKE_SOURCE_DIR}/include)

# Add subdirectories
add_subdirectory(apps)

