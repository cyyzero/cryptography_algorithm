#include "server.h"

#include <thread>
#include <iostream>

#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

Server::Server(int port)
    : port_(port)
{
    std::thread thread([this]() {
        this->run();
    });
    thread.detach();
}

void Server::run() const
{
    struct sockaddr_in server;
    struct sockaddr_in client;
    socklen_t len;
    int listend;
    int connectd;
    int sendnum;
    int opt;
    int recvnum;
    char recv_buf[2048];

    if ((listend = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("create listen socket error\n");
        exit(1);
    }

    memset(&server, 0, sizeof(struct sockaddr_in));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(port_);

    if (-1 == bind(listend, (struct sockaddr *)&server, sizeof(struct sockaddr)))
    {
        perror("bind error\n");
        exit(1);
    }

    if (-1 == listen(listend, 5))
    {
        perror("listen error\n");
        exit(1);
    }

    while (1)
    {
        std::string msg;
        std::cout << "Wait for accept" << std::endl;
        if (-1 == (connectd = accept(listend, (struct sockaddr *)&client, &len)))
        {
            perror("create connect socket error\n");
            continue;
        }

        for(;;)
        {
            recvnum = recv(connectd, recv_buf, sizeof(recv_buf), 0);
            if (recvnum == -1)
            {
                perror("recv error\n");
                exit(-1);
            }
            else if (recvnum == 0)
            {
                std::cout << "call msg_callback: " << std::endl;
                msg_callback_(msg);
                break;
            }
            else
            {
                msg.append(recv_buf, recvnum);
            }
        }
        close(connectd);
    }
    close(listend);
}