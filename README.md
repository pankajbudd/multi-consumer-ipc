# Multi-Consumer IPC and Additional Modules

This repository contains implementations of various C++ modules, including inter-process communication (IPC), mutex-based synchronization, a multi-threaded TCP server, and a single-producer multiple-consumer model. Each module is organized into its respective directory for clarity and modularity.

## Directory Structure

```
.
├── mutex-between-multiple-processes/
│   ├── src/
│   │   ├── main.cpp              # Entry point for mutex synchronization demo
│   │   ├── mutex_manager.cpp     # Implementation of mutex-based synchronization
│   └── include/
│       └── mutex_manager.h       # Header file for mutex manager
│   └── tests/
│       └── test_mutex_manager.cpp # Unit tests for mutex manager
├── multi-threaded-tcp-server/
│   ├── src/
│   │   ├── main.cpp              # Entry point for TCP server
│   │   ├── tcp_server.cpp        # Multi-threaded TCP server implementation
│   └── include/
│       └── tcp_server.h          # Header file for TCP server
│   └── tests/
│       └── test_tcp_server.cpp   # Unit tests for TCP server
├── single-producer-multiple-consumer/
│   ├── src/
│   │   ├── main.cpp              # Entry point for producer-consumer demo
│   │   ├── producer.cpp          # Implementation of producer logic
│   │   ├── consumer.cpp          # Implementation of consumer logic
│   │   ├── ipc_manager.cpp       # Handles IPC mechanisms
│   └── include/
│       ├── producer.h            # Header file for producer
│       ├── consumer.h            # Header file for consumer
│       └── ipc_manager.h         # Header file for IPC manager
│   └── tests/
│       ├── test_producer.cpp     # Unit tests for producer
│       ├── test_consumer.cpp     # Unit tests for consumer
│       └── test_ipc.cpp          # Unit tests for IPC manager
├── utils/
│   ├── logger.cpp                # Logging utility implementation
│   └── logger.h                  # Header file for logging utility
├── docs/
│   └── design.md                 # Detailed design documentation
├── CMakeLists.txt                # Build configuration
└── README.md                     # Project documentation
```

## Features

- **Mutex Synchronization**: Demonstrates process synchronization using mutexes.
- **Multi-Threaded TCP Server**: Implements a scalable TCP server using multi-threading.
- **Single-Producer Multiple-Consumer Model**: Implements a producer-consumer pattern with IPC mechanisms.
- **Logging Utility**: Provides a centralized logging mechanism for debugging and monitoring.
- **Documentation**: Includes detailed design documentation for better understanding.
- **Unit Testing**: Includes unit tests for all major components.

## Build and Run

1. Clone the repository:
    ```bash
    git clone https://github.com/your-username/your-repo.git
    cd your-repo
    ```

2. Build the project using CMake:
    ```bash
    mkdir build
    cd build
    cmake ..
    make
    ```

3. Run the desired module:
    - For mutex synchronization:
        ```bash
        ./mutex_between_multiple_processes_demo
        ```
    - For multi-threaded TCP server:
        ```bash
        ./multi_threaded_tcp_server_demo
        ```
    - For single-producer multiple-consumer:
        ```bash
        ./single_producer_multiple_consumer_demo
        ```

## Testing

Run the unit tests:
```bash
cd build
ctest
```

## Documentation

Refer to the [docs/design.md](docs/design.md) file for detailed design and implementation notes.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.