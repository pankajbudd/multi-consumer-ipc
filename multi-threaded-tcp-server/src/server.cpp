#include <iostream>
#include <unistd.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <cstring>
#include <thread>
#include <vector>

constexpr int MAX_EVENTS = 10;
constexpr int PORT = 9090;
constexpr int NUM_THREADS = 4;

/**
 * Makes a socket non-blocking by setting the O_NONBLOCK flag
 * @param sockfd The socket file descriptor to modify
 * @return 0 on success, -1 on failure
 */
int make_socket_non_blocking(int sockfd) {
    int flags = fcntl(sockfd, F_GETFL, 0);
    return fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);
}

/**
 * Handles communication with a single client
 * This function runs in a separate thread for each connected client
 * @param client_fd The file descriptor for the client socket
 */
void handle_client(int client_fd) {
    char buf[1024] = {};  // Buffer for reading client data
    ssize_t n;
    while (true) {
        n = read(client_fd, buf, sizeof(buf) - 1);  // Leave space for null terminator
        if (n <= 0) {
            if (n == 0) {
                std::cout << "🔴 Client disconnected (fd: " << client_fd << ")\n";
                break;
            } else if (errno == EAGAIN || errno == EWOULDBLOCK) {
                // No data available right now, sleep briefly and try again
                usleep(100000);  // Sleep for 100ms
                continue;
            } else {
                perror("read");
                break;
            }
        } else {
            buf[n] = '\0';  // Null terminate the received data
            std::cout << "📨 Received from client: " << buf << std::endl;
            
            // Echo back to client
            ssize_t bytes_sent = write(client_fd, buf, n);
            if (bytes_sent == -1) {
                if (errno == EAGAIN || errno == EWOULDBLOCK) {
                    // Buffer full, sleep briefly and try again
                    usleep(100000);  // Sleep for 100ms
                    continue;
                } else {
                    perror("write");
                    break;
                }
            }
        }
    }
    close(client_fd);
}

/**
 * Main server function that sets up the TCP server and handles incoming connections
 * Uses epoll for efficient I/O multiplexing and creates a new thread for each client
 */
void run_server() {
    // Create TCP socket
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd == -1) {
        perror("socket");
        return;
    }

    // Set socket option to reuse address (prevents "Address already in use" errors)
    int opt = 1;
    setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // Configure server address structure
    sockaddr_in addr = {};
    addr.sin_family = AF_INET;           // IPv4
    addr.sin_addr.s_addr = INADDR_ANY;   // Listen on all available interfaces
    addr.sin_port = htons(PORT);         // Convert port to network byte order
    bind(listen_fd, (sockaddr*)&addr, sizeof(addr));

    // Make the listening socket non-blocking
    make_socket_non_blocking(listen_fd);

    // Start listening for connections
    listen(listen_fd, SOMAXCONN);

    // Create epoll instance for I/O multiplexing
    int epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        perror("epoll_create1");
        return;
    }

    // Register the listening socket with epoll
    epoll_event ev = {};
    ev.events = EPOLLIN;         // Monitor for incoming data
    ev.data.fd = listen_fd;      // Store the file descriptor
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listen_fd, &ev);

    // Array to store events returned by epoll_wait
    epoll_event events[MAX_EVENTS];
    // Vector to store client threads
    std::vector<std::thread> threads;

    std::cout << "🔌 Server listening on port " << PORT << "\n";

    // Main server loop
    while (true) {
        // Wait for events on registered file descriptors
        int nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        for (int i = 0; i < nfds; ++i) {
            if (events[i].data.fd == listen_fd) {
                // New connection request
                sockaddr_in client_addr;
                socklen_t client_len = sizeof(client_addr);
                int client_fd = accept(listen_fd, (sockaddr*)&client_addr, &client_len);
                if (client_fd != -1) {
                    make_socket_non_blocking(client_fd);
                    std::cout << "🟢 New client connected (fd: " << client_fd << ")\n";
                    // Create a new thread to handle this client
                    threads.push_back(std::thread(handle_client, client_fd));
                }
            }
        }

        // Clean up completed threads
        // Note: This is a simple implementation. In a production environment,
        // you might want to use a thread pool or more sophisticated thread management
        for (auto& t : threads) {
            if (t.joinable()) {
                t.join();
            }
        }
    }

    close(listen_fd);
}

int main() {
    run_server();
    return 0;
}

