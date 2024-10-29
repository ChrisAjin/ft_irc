#include "../include/server.hpp"

int server::_init_server() {
    int server_fd;
    setupServer(server_fd);
    // partie parsing a faire
    // voir les errerus a eviter 
    while (true) {
        int poll_count = poll(poll_fds.data(), poll_fds.size(), -1);
        if (poll_count < 0) {
            perror("poll");
            exit(EXIT_FAILURE);
        }
        //il faut check tout les fds et break si polls.fds.begin != poll_fds.end
        for (size_t i = 0; i < poll_fds.size(); ++i) {
            if (poll_fds[i].revents & POLLIN) {
                if (poll_fds[i].fd == server_fd) {
                    handleNewConnection(server_fd);
                } else {
                    handleClientMessage(poll_fds[i].fd);
                }
            }
        }
    }
    close(server_fd);
    return 0;
}
void server::setupServer(int &server_fd) {
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);
    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, MAX_CLIENTS) < 0) {
        perror("listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    struct pollfd server_pollfd;
    server_pollfd.fd = server_fd;
    server_pollfd.events = POLLIN;
    poll_fds.push_back(server_pollfd);
}
// ADD user en vrai
void server::handleNewConnection(int server_fd) {
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
    if (client_fd < 0) {
        perror("Invalid client socket");
        return;
    }
    _list_user.push_back(new user(client_fd));
    struct pollfd client_pollfd;
    client_pollfd.fd = client_fd;
    client_pollfd.events = POLLIN;
    poll_fds.push_back(client_pollfd);
    std::cout << "New User on socket : " << client_fd << std::endl;
    //je sais pas ce que tu fais ici peut etre cest du chinois je sais pas
    // Client new_client = {client_fd, "", "", false, false};
    // clients[client_fd] = new_client;
}
void server::handleClientMessage(int client_fd) {
    char buffer[512];
    memset(buffer, 0, sizeof(buffer));
    int bytes_read = read(client_fd, buffer, sizeof(buffer) - 1);
    if (bytes_read <= 0) {
        close(client_fd);
        //la faut changer aussi
        // poll_fds.erase(std::remove_if(poll_fds.begin(), poll_fds.end(), [client_fd](const struct pollfd &pfd) {
        //     return pfd.fd == client_fd;
        // }), poll_fds.end());
        clients.erase(client_fd);
        return;
    }
    std::string message(buffer);
    //la aussi
    //Client &client = clients[client_fd];
    // if (!client.isAuthenticated) {
    //     authenticateClient(client, message);
    // } else {
    //     //parsing on verra ensemble
    // }
}
void server::authenticateClient(user &client, const std::string &message) {
    // je sais pas encore comment on va faire
    // ou si c est toi
}
void server::joinChannel(user &client, const std::string &channelName) {
    //toi ou moi ?
}
void server::sendMessageToChannel(const std::string &channelName, const std::string &message, int sender_fd) {
    //toi ou moi ?
}
void server::handleOperatorCommand(user &client, const std::string &command) {
    //toi ou moi ?
}