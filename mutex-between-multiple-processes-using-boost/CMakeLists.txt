cmake_minimum_required(VERSION 3.10)
project(shared_memory_project)

set(CMAKE_CXX_STANDARD 11)

find_package(Boost REQUIRED)

add_executable(boost_writer src/writer.cpp)
target_include_directories(boost_writer PRIVATE ${Boost_INCLUDE_DIRS})

add_executable(boost_reader src/reader.cpp)
target_include_directories(boost_reader PRIVATE ${Boost_INCLUDE_DIRS})

add_executable(boost_mutex_cleanup src/cleanup.cpp)
target_include_directories(boost_mutex_cleanup PRIVATE ${Boost_INCLUDE_DIRS})
