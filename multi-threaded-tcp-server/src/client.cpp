#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <arpa/inet.h>

constexpr const char* SERVER_IP = "127.0.0.1";
constexpr int SERVER_PORT = 9090;

int main() {
    // Create socket
    int client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd == -1) {
        perror("socket");
        return 1;
    }

    sockaddr_in server_addr = {};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        perror("inet_pton");
        close(client_fd);
        return 1;
    }

    // Connect to server
    if (connect(client_fd, (sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("connect");
        close(client_fd);
        return 1;
    }

    std::cout << "🔌 Connected to server!\n";

    // Send and receive data
    const char* msg = "Hello, Server!";
    write(client_fd, msg, strlen(msg));

    char buf[1024] = {};
    ssize_t n = read(client_fd, buf, sizeof(buf));
    if (n > 0) {
        std::cout << "📨 Received from server: " << buf << "\n";
    }

    close(client_fd);
    return 0;
}

