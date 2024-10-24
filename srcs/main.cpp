#include "server.hpp"
# include "../include/utils.hpp"

int main(int ac, char **av) {
    int server_fd;
    setupServer(server_fd);
    if (parsing(ac, av)) {
        return 1;
    }
    while (true) {
        int poll_count = poll(poll_fds.data(), poll_fds.size(), -1);
        if (poll_count < 0) {
            perror("poll");
            exit(EXIT_FAILURE);
        }

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

void setupServer(int &server_fd) {
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

void handleNewConnection(int server_fd) {
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
    if (client_fd < 0) {
        perror("accept");
        return;
    }

    struct pollfd client_pollfd;
    client_pollfd.fd = client_fd;
    client_pollfd.events = POLLIN;
    poll_fds.push_back(client_pollfd);

    Client new_client = {client_fd, "", "", false, false};
    clients[client_fd] = new_client;
}

void handleClientMessage(int client_fd) {
    char buffer[512];
    memset(buffer, 0, sizeof(buffer));
    int bytes_read = read(client_fd, buffer, sizeof(buffer) - 1);
    if (bytes_read <= 0) {
        close(client_fd);
        poll_fds.erase(std::remove_if(poll_fds.begin(), poll_fds.end(), [client_fd](const struct pollfd &pfd) {
            return pfd.fd == client_fd;
        }), poll_fds.end());
        clients.erase(client_fd);
        return;
    }

    std::string message(buffer);
    Client &client = clients[client_fd];
    user *sender = _Get_user_by_fd(client_fd);
    if (!client.isAuthenticated) {
        authenticateClient(client, message);
    } else if (Check_command(client, res) == -2){
            _Remove_user(it);
            return;
    }
}
user    *server::_Get_user_by_fd(int fd)
{
    std::vector<channel *>::iterator it;
    for (unsigned int i = 0; i < _list_user.size(); i++)
    {
        if (_list_user[i]->Get_fd() == fd)
            return (_list_user[i]);
    }
}
void authenticateClient(Client &client, const std::string &message) {
    // je sais pas encore comment on va faire
    // ou si c est toi
}

void joinChannel(Client &client, const std::string &channelName) {
    //toi ou moi ?
}

void sendMessageToChannel(const std::string &channelName, const std::string &message, int sender_fd) {
    //toi ou moi ?
}

void handleOperatorCommand(Client &client, const std::string &command) {
    //toi ou moi ?
}