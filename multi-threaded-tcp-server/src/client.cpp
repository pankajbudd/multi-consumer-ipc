#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <signal.h>
#include <string>
#include <fcntl.h>
#include <errno.h>

// Default configuration
constexpr const char* DEFAULT_SERVER_IP = "127.0.0.1";
constexpr int DEFAULT_PORT = 9090;

// Global variable to control the chat loop
volatile sig_atomic_t running = 1;

// Signal handler for Ctrl+C
void signal_handler(int signum) {
    if (signum == SIGINT) {
        std::cout << "\nGracefully shutting down...\n";
        running = 0;
    }
}

void print_usage(const char* program_name) {
    std::cout << "Usage: " << program_name << " [server_ip] [port]\n"
              << "Example: " << program_name << " 127.0.0.1 9090\n"
              << "If no arguments provided, defaults to " << DEFAULT_SERVER_IP << ":" << DEFAULT_PORT << "\n";
}

// Make socket non-blocking
int make_socket_non_blocking(int sockfd) {
    int flags = fcntl(sockfd, F_GETFL, 0);
    if (flags == -1) return -1;
    return fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);
}

int main(int argc, char* argv[]) {
    // Set up signal handler for Ctrl+C
    struct sigaction sa;
    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, nullptr);

    // Parse command line arguments with defaults
    const char* server_ip = (argc > 1) ? argv[1] : DEFAULT_SERVER_IP;
    int port = (argc > 2) ? std::stoi(argv[2]) : DEFAULT_PORT;

    // Create a TCP socket (IPv4, stream-based, default protocol)
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket");
        return 1;
    }

    // Make socket non-blocking
    if (make_socket_non_blocking(sockfd) == -1) {
        perror("fcntl");
        close(sockfd);
        return 1;
    }

    // Configure server address
    sockaddr_in server_addr = {};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    
    // Convert IP address from text to binary form
    if (inet_pton(AF_INET, server_ip, &server_addr.sin_addr) <= 0) {
        std::cerr << "Invalid IP address: " << server_ip << std::endl;
        close(sockfd);
        return 1;
    }

    // Connect to the server
    if (connect(sockfd, (sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        if (errno != EINPROGRESS) {
            perror("connect");
            close(sockfd);
            return 1;
        }
    }

    // Wait for connection to complete
    fd_set write_fds;
    FD_ZERO(&write_fds);
    FD_SET(sockfd, &write_fds);
    
    struct timeval timeout;
    timeout.tv_sec = 5;  // 5 seconds timeout
    timeout.tv_usec = 0;

    int ret = select(sockfd + 1, nullptr, &write_fds, nullptr, &timeout);
    if (ret == -1) {
        perror("select");
        close(sockfd);
        return 1;
    } else if (ret == 0) {
        std::cerr << "Connection timeout\n";
        close(sockfd);
        return 1;
    }

    // Check if connection was successful
    int error = 0;
    socklen_t len = sizeof(error);
    if (getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &error, &len) == -1 || error != 0) {
        std::cerr << "Connection failed\n";
        close(sockfd);
        return 1;
    }

    std::cout << "Connected to server at " << server_ip << ":" << port << std::endl;
    std::cout << "Type your messages (Ctrl+C to exit):\n";

    // Chat loop
    while (running) {
        std::string message;
        std::cout << "> ";
        std::getline(std::cin, message);

        if (!running) break;  // Check if we should exit

        std::cout << "Sending message: " << message << std::endl;

        // Send the message
        ssize_t bytes_sent = send(sockfd, message.c_str(), message.length(), 0);
        if (bytes_sent == -1) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                std::cerr << "Send buffer full, retrying...\n";
                usleep(100000);  // Sleep for 100ms
                continue;
            }
            perror("send");
            break;
        }

        // Receive the echo response
        char buffer[1024] = {};
        ssize_t bytes_received = recv(sockfd, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received == -1) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                // No data available, sleep briefly and try again
                usleep(100000);  // Sleep for 100ms
                continue;
            }
            perror("recv");
            break;
        } else if (bytes_received > 0) {
            buffer[bytes_received] = '\0';  // Null-terminate the received data
            std::cout << "Server: " << buffer << std::endl;
        }
    }

    // Clean up
    std::cout << "Closing connection...\n";
    close(sockfd);
    return 0;
}

