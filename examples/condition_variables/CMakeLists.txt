# Find required packages
find_package(Threads REQUIRED)

# Add Demo executable
add_executable(Demo Demo.cpp)

# Link against pthread
target_link_libraries(Demo PRIVATE pthread)

# Set C++ standard
set_target_properties(Demo PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED ON
)

# Add include directories
target_include_directories(Demo PRIVATE
    ${CMAKE_SOURCE_DIR}/include
) 