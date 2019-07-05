#include "utility.h"

#include <string>

#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

uint64_t pow(uint64_t x, uint64_t y, uint64_t q)
{
    if (y == 0)
        return 1;
    auto tmp = pow(uint64_t(__uint128_t(x * x) % q), y / 2, q);
    // auto tmp2 = Integer(__uint128_t(tmp * tmp) % q);
    if (y % 2 == 1)
    {
        return uint64_t(__uint128_t(tmp * x) % q);
    }
    else
    {
        return tmp;
    }
}

void print_str(const std::string &str)
{
    for (auto ch : str)
    {
        printf("%x", (int)ch & 0xff);
    }
    printf("\n");
}

void send_msg(const std::string &msg, int port)
{
    struct sockaddr_in server;
    int ret;
    int len;
    int sockfd;
    int sendnum;
    int recvnum;
    char send_buf[2048];
    char recv_buf[2048];

    if (-1 == (sockfd = socket(AF_INET, SOCK_STREAM, 0)))
    {
        perror("can not create socket\n");
        exit(1);
    }

    memset(&server, 0, sizeof(struct sockaddr_in));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(port);

    if (0 != (ret = connect(sockfd, (struct sockaddr *)&server, sizeof(struct sockaddr))))
    {
        perror("connect error");
        close(sockfd);
        exit(1);
    }

    if (send(sockfd, msg.data(), msg.size(), 0) == -1)
    {
        perror("send");
        exit(1);
    }
    close(sockfd);
}
