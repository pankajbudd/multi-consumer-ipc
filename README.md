# Multi-Consumer IPC Examples

This repository contains various examples demonstrating Inter-Process Communication (IPC) and thread synchronization mechanisms in C/C++.

## Project Structure

```
.
├── examples/
│   ├── condition_variables/     # Examples using pthread condition variables
│   ├── shared_memory/          # Examples using POSIX shared memory
│   └── semaphores/             # Examples using POSIX semaphores
├── include/                    # Common header files
├── src/                        # Common source files
├── tests/                      # Test files
└── build/                      # Build directory
```

## Examples

### 1. Condition Variables (examples/condition_variables/)

Demonstrates the use of pthread condition variables for thread synchronization:

- `Test1.cpp`: Basic producer-consumer pattern using condition variables
- `Test2.cpp`: Multiple consumer threads with condition variables
- `Test3.cpp`: Condition variable with shared memory between processes
- `Test5.cpp`: Multiple threads with broadcast signaling

### 2. Shared Memory (examples/shared_memory/)

Examples of inter-process communication using POSIX shared memory:

- Basic shared memory operations
- Shared memory with synchronization
- Producer-consumer pattern using shared memory

### 3. Semaphores (examples/semaphores/)

Examples demonstrating POSIX semaphores:

- Binary semaphores
- Counting semaphores
- Named semaphores for inter-process communication

## Building the Examples

```bash
# Create build directory
mkdir build
cd build

# Configure with CMake
cmake ..

# Build
make
```

## Running the Examples

### Condition Variables Example
```bash
# Run Test1
./examples/condition_variables/Test1

# Run Test2
./examples/condition_variables/Test2

# Run Test3 (requires two terminals)
# Terminal 1
./examples/condition_variables/Test3 producer
# Terminal 2
./examples/condition_variables/Test3 consumer

# Run Test5
./examples/condition_variables/Test5
```

## Requirements

- C++11 or later
- POSIX-compliant operating system
- CMake 3.10 or later
- pthread library

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.