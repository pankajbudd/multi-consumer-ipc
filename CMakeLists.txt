# CMakeLists.txt

cmake_minimum_required(VERSION 3.10)
project(MultiConsumerIPC)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add modules
add_subdirectory(multi-threaded-tcp-server)
add_subdirectory(mutex-between-multiple-processes)
add_subdirectory(single-producer-multiple-consumer)