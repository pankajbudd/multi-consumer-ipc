# CMakeLists.txt

cmake_minimum_required(VERSION 3.10)
project(multi_consumer_ipc)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add modules
add_subdirectory(multi-threaded-tcp-server)
add_subdirectory(mutex-between-multiple-processes)
add_subdirectory(mutex-between-multiple-processes-using-boost)
add_subdirectory(single-producer-multiple-consumer)
add_subdirectory(examples/condition_variables)

# Find required packages
find_package(Threads REQUIRED)