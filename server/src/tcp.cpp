#include "tcp.h"
#include "camera.h"
#include "codec.h"
#include "md5.h"
#include "udp.h"

#include <unistd.h>
#include <string.h>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

Tcp::Tcp(Config *config)
{
    this->config = config;
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        printf("create socket error: %s (errno: %d)\n", strerror(errno), errno);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(config->port);

    if (bind(sock, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1)
    {
        printf("bind socket error: %s (errno: %d)\n", strerror(errno), errno);
    }

    if (listen(sock, 10) == -1)
    {
        printf("listen socket error: %s (errno: %d)\n", strerror(errno), errno);
    }
    // connect redis
    redis.connect(config->redis, 6379);
    if (config->redis_password != "")
    {
        redis.auth(config->redis_password);
    }
    // udp service
    pthread_t pid;
    if (pthread_create(&pid, NULL, udp_thread, (void *)this) != 0)
    {
        printf("udp thread start failed.\n");
    }
}

void Tcp::Listen()
{
    while (1)
    {
        printf("\nbegin listen\n");
        if ((connection = accept(sock, (struct sockaddr *)NULL, NULL)) == -1)
        {
            printf("accept socket error: %s (errno: %d)", strerror(errno), errno);
            continue;
        }
        pthread_t pid;
        if (pthread_create(&pid, NULL, recv_thread, (void *)this) != 0)
        {
            printf("recv thread start failed.\n");
        }
        Camera camera(config);
        Codec codec(config);
        char *data;
        int length;
        FILE *file = fopen("camera.h264", "wb");
        int i = 0;
        while (i++ >= 0)
        {
            if (camera.Grab(&data))
            {
                if (codec.Encode(data, &data, &length) > 0)
                {
                    bool sent = false;
                    if (send(connection, (char *)(&i), 4, MSG_NOSIGNAL) > 0)
                    {
                        if (send(connection, (char *)(&length), 4, MSG_NOSIGNAL) > 0)
                        {
                            if (send(connection, data, length, MSG_NOSIGNAL) > 0)
                            {
                                sent = true;
                            }
                        }
                    }
                    if (!sent)
                    {
                        printf("\nsend data failed.\n");
                        break;
                    }
                    // fwrite(data, 1, length, file);
                }
            }
            usleep(1e6 / config->fps);
        }
        while (codec.Encode(&data, &length) > 0)
        {
            // fwrite(data, 1, length, file);
        }
        fclose(file);
        close(connection);
    }
}

bool exact_recv(int connection, int size, char *buffer)
{
    int current = 0;
    int n = recv(connection, buffer, size, 0);
    if (n <= 0)
    {
        return false;
    }
    while (current + n < size)
    {
        current += n;
        n = recv(connection, buffer + current, size - current, 0);
    }
    return true;
}

void *Tcp::udp_thread(void *arg)
{
    Tcp *ptr = (Tcp *)arg;
    Udp udp(ptr->config);
    udp.Listen();
    return 0;
}

void *Tcp::recv_thread(void *arg)
{
    Tcp *ptr = (Tcp *)arg;
    int size;
    while (1)
    {
        bool success = exact_recv(ptr->connection, 4, (char *)&size);
        if (success)
        {
            if (size < RECV_BUFFER_SIZE)
            {
                success = exact_recv(ptr->connection, size, ptr->buff);
                if (success)
                {
                    ptr->buff[size] = '\0';
                    if (size > 1)
                    {
                        printf("recv message: %s\n", ptr->buff);
                        MD5 md5;
                        md5.update(ptr->buff);
                        string code_msg = string(ptr->buff) + md5.toString().substr(0, 5);
                        vector<string> v;
                        // { "key":"web1", "data":"data_string" }
                        char key[20];
                        sprintf(key, "web%d", ptr->config->camera_index);
                        v.push_back(std::string("{ \"key\":\"") + key + std::string("\", \"data\":\"") + code_msg + std::string("\" }"));
                        ptr->redis.rpush("data_to_send", v, [](cpp_redis::reply &reply)
                                         { std::cout << "data_to_send: " << reply << std::endl; });
                        ptr->redis.sync_commit();
                    }
                }
            }
            else
            {
                printf("message size too large.\n");
                success = false;
            }
        }
        if (!success)
        {
            break;
        }
    }
    printf("exit recv thread.\n");
    return 0;
}

Tcp::~Tcp()
{
    close(sock);
}