cmake_minimum_required(VERSION 3.10)
project(shared_memory_project)

set(CMAKE_CXX_STANDARD 11)

add_executable(writer src/writer.cpp)
add_executable(reader src/reader.cpp)
add_executable(cleanup src/cleanup.cpp)
