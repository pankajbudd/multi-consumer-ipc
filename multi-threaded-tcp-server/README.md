# 🚀 Multi-Threaded TCP Server with Epoll

This project demonstrates a high-performance TCP server implementation in C++ using **epoll** for I/O multiplexing and **multi-threading** for handling client connections. The server implements a simple echo service that efficiently handles multiple concurrent client connections.

---

## 📁 Components

### ✅ `server.cpp`
The main server implementation that includes:
- Non-blocking socket operations
- Epoll-based I/O multiplexing
- Multi-threaded client handling
- Simple echo server functionality

### ✅ `client.cpp`
An interactive TCP client that:
- Accepts optional command line arguments for server IP and port
- Uses default values (127.0.0.1:9090) if not specified
- Supports continuous chat with the server
- Gracefully handles Ctrl+C for clean shutdown
- Provides real-time echo responses

---

## 🛠️ Build Instructions

```bash
# Build the server
g++ -o server server.cpp -pthread

# Build the client
g++ -o client client.cpp
```

---

## 🚀 Run Instructions

### 1. Start the Server
```bash
./server
```

### 2. Run the Client
```bash
# Using default values (127.0.0.1:9090)
./client

# Or specify custom server and port
./client <server_ip> <port>
```

Examples:
```bash
# Connect to localhost with default port
./client

# Connect to specific IP and port
./client 192.168.1.10 9090
```

Or using netcat:
```bash
nc localhost 9090
```

---

## 🔍 Key Features

- **Non-blocking I/O**: All sockets are set to non-blocking mode for better performance
- **Epoll-based I/O Multiplexing**: Efficient handling of multiple file descriptors
- **Multi-threaded Architecture**: Each client connection is handled in a separate thread
- **Simple Echo Service**: Server echoes back any data received from clients
- **Interactive Client**: Continuous chat functionality with graceful shutdown
- **Signal Handling**: Clean shutdown on Ctrl+C
- **Default Configuration**: Easy local testing with default values

---

## 📌 Implementation Details

### Socket Configuration
```cpp
constexpr int MAX_EVENTS = 10;    // Maximum epoll events
constexpr int PORT = 9090;        // Server port
constexpr int NUM_THREADS = 4;    // Number of worker threads
```

### Client Configuration
```cpp
constexpr const char* DEFAULT_SERVER_IP = "127.0.0.1";  // Default server address
constexpr int DEFAULT_PORT = 9090;                      // Default port number
```

### Key Components
- **Non-blocking Sockets**: Using `fcntl` with `O_NONBLOCK` flag
- **Epoll Event Loop**: Efficient I/O event monitoring
- **Thread Management**: One thread per client connection
- **Buffer Management**: 1024-byte buffer for client data
- **Signal Handling**: SIGINT (Ctrl+C) handling for graceful shutdown

---

## 🧪 Example Output

Server:
```
🔌 Server listening on port 9090
🟢 New client connected (fd: 4)
📨 Received from client: Hello, Server!
📨 Received from client: How are you?
🔴 Client disconnected (fd: 4)
```

Client:
```
Connected to server at 127.0.0.1:9090
Type your messages (Ctrl+C to exit):
> Hello, Server!
Server: Hello, Server!
> How are you?
Server: How are you?
^C
Gracefully shutting down...
Closing connection...
```

---

## 📦 Possible Enhancements

- Implement a thread pool instead of creating a new thread for each client
- Add support for SSL/TLS encryption
- Implement proper error handling and logging
- Add configuration file support
- Implement graceful shutdown
- Add support for different protocols (HTTP, WebSocket, etc.)
- Implement connection pooling
- Add metrics and monitoring
- Add support for sending files
- Add timeout handling
- Add message history
- Implement readline support for better input handling
- Add support for multiple simultaneous connections
- Add command line argument parsing library (e.g., getopt)
- Add configuration file support for client settings

---

## 🔒 Security Considerations

- The current implementation is a basic echo server
- In a production environment, consider:
  - Adding input validation
  - Implementing rate limiting
  - Adding authentication
  - Using SSL/TLS for encryption
  - Implementing proper error handling
  - Validating client input
  - Implementing connection timeouts
  - Adding message encryption
  - Implementing user authentication
  - Validating IP addresses and ports

---

## 📄 License

This project is open-sourced for educational and demonstrative purposes. Feel free to use, modify, or extend it.

---

## 👨‍💻 Author

Made with ❤️ in C++.

---