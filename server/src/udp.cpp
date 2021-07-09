#include "udp.h"
#include "mysql.h"
#include <string>
#include <vector>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

using namespace std;

void split(string &value, string delim, vector<string> *reslut)
{
    size_t last = 0;
    size_t index = value.find_first_of(delim, last);
    while (index != std::string::npos)
    {
        reslut->push_back(value.substr(last, index - last));
        last = index + 1;
        index = value.find_first_of(delim, last);
    }
    if (index - last > 0)
    {
        reslut->push_back(value.substr(last, index - last));
    }
}

Udp::Udp(Config *config)
{
    this->config = config;
    sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock_fd < 0)
    {
        perror("socket");
    }

    memset(&addr_serv, 0, sizeof(struct sockaddr_in));
    addr_serv.sin_family = AF_INET;
    addr_serv.sin_port = htons(config->port);
    addr_serv.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sock_fd, (struct sockaddr *)&addr_serv, sizeof(addr_serv)) < 0)
    {
        perror("bind error:");
    }
}

void Udp::Listen()
{
    int recv_num;
    int send_num;
    char send_buf[100];
    char recv_buf[100];
    struct sockaddr_in addr_client;
    Mysql mysql(config->mysql, config->mysql_username, config->mysql_password);

    while (1)
    {
        printf("udp wait:\n");
        int len = sizeof(addr_serv);
        recv_num = recvfrom(sock_fd, recv_buf, sizeof(recv_buf), 0, (struct sockaddr *)&addr_client, (socklen_t *)&len);

        if (recv_num < 0)
        {
            perror("recv udp error:");
        }

        recv_buf[recv_num] = '\0';
        printf("server receive %d bytes: %s\n", recv_num, recv_buf);

        string message(recv_buf);
        if (message.substr(0, 5).compare("login") == 0)
        {
            vector<std::string> login_message;
            split(message, ",", &login_message);
            // login
            if (login_message.size() == 3 && login_message[1] == config->username && login_message[2] == config->password)
            {
                sendto(sock_fd, "111", 4, 0, (struct sockaddr *)&addr_client, len);
            }
            else
            {
                printf("login failed: %s : %s\n", login_message[1].c_str(), login_message[2].c_str());
                sendto(sock_fd, "reject", 7, 0, (struct sockaddr *)&addr_client, len);
            }
        }
    }
}

Udp::~Udp()
{
    close(sock_fd);
}