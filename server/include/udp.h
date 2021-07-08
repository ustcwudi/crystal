#pragma once
#include "config.h"
#include <netinet/in.h>

class Udp
{
private:
    int sock_fd;
    Config *config;
    sockaddr_in addr_serv;

public:
    Udp(Config *config);
    void Listen();
    ~Udp();
};
