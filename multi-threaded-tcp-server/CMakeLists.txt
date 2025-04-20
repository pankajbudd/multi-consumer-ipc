cmake_minimum_required(VERSION 3.10)
project(shared_memory_project)

set(CMAKE_CXX_STANDARD 11)

add_executable(server src/server.cpp)
add_executable(client src/client.cpp)
