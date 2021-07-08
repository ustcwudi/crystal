#pragma once
#include "config.h"
#include <string>
#include <netinet/in.h>
#include <pthread.h>
#include <cpp_redis/cpp_redis>

#define RECV_BUFFER_SIZE 4096

using namespace std;
using namespace cpp_redis;

class Tcp
{
private:
    static void *recv_thread(void *arg);
    static void *udp_thread(void *arg);
    int sock, connection;
    struct sockaddr_in servaddr;
    char buff[RECV_BUFFER_SIZE];
    Config *config;
    client redis;

public:
    Tcp(Config *config);
    void Listen();
    ~Tcp();
};
