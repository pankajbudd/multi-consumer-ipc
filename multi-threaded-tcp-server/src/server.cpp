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

int make_socket_non_blocking(int sockfd) {
    int flags = fcntl(sockfd, F_GETFL, 0);
    return fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);
}

void handle_client(int client_fd) {
    char buf[1024] = {};
    ssize_t n;
    while (true) {
        n = read(client_fd, buf, sizeof(buf));
        if (n <= 0) {
            if (n == 0) {
                std::cout << "🔴 Client disconnected (fd: " << client_fd << ")\n";
            } else {
                perror("read");
            }
            close(client_fd);
            return;
        } else {
            std::cout << "📨 Received from client: " << buf;
            write(client_fd, buf, n);  // Echo back to client
        }
    }
}

void run_server() {
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd == -1) {
        perror("socket");
        return;
    }

    int opt = 1;
    setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);
    bind(listen_fd, (sockaddr*)&addr, sizeof(addr));

    make_socket_non_blocking(listen_fd);

    listen(listen_fd, SOMAXCONN);

    int epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        perror("epoll_create1");
        return;
    }

    epoll_event ev = {};
    ev.events = EPOLLIN;
    ev.data.fd = listen_fd;
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listen_fd, &ev);

    epoll_event events[MAX_EVENTS];
    std::vector<std::thread> threads;

    std::cout << "🔌 Server listening on port " << PORT << "\n";

    while (true) {
        int nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        for (int i = 0; i < nfds; ++i) {
            if (events[i].data.fd == listen_fd) {
                sockaddr_in client_addr;
                socklen_t client_len = sizeof(client_addr);
                int client_fd = accept(listen_fd, (sockaddr*)&client_addr, &client_len);
                if (client_fd != -1) {
                    make_socket_non_blocking(client_fd);
                    std::cout << "🟢 New client connected (fd: " << client_fd << ")\n";
                    threads.push_back(std::thread(handle_client, client_fd));
                }
            }
        }

        // Join threads (simple, but could be optimized)
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

