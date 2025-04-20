cmake_minimum_required(VERSION 3.10)
project(shared_memory_project)

set(CMAKE_CXX_STANDARD 11)

add_executable(producer src/producer.cpp)
add_executable(consumer src/consumer.cpp)
add_executable(consumer_cleanup src/cleanup.cpp)
add_executable(producerConsumerDemo src/producerConsumerDemo.cpp)
