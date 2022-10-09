#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <poll.h>

#define BACKLOG 10
#define MaxMessageLength 1024

int socket_init(char IP[], char Port[], struct sockaddr_in *server_addr);


int main(int argc, char **argv)
{
    if(argc < 3) {
        printf("usage: %s <IP> <Port>\n", argv[0]);
        return -1;
    }

    struct sockaddr_in server_addr;
    int sockfd = socket_init(argv[1], argv[2], &server_addr);
    if(sockfd == -1) {
        printf("creating socket error\n");
        return -1;
    }

    if(bind(sockfd, (struct sockaddr*)&server_addr, sizeof(struct sockaddr)) == -1) {
        perror("bind");
        return -1;
    }

    if(listen(sockfd, BACKLOG) != 0) {
        perror("listen");
        return -1;
    }

    struct pollfd fds[BACKLOG];
    fds[0].fd = sockfd, fds[0].events = POLLIN;
    int length = 1;  //相当于fds当前的长度
    char msg[MaxMessageLength];

    while(1) {
        int ready_num = poll(fds, length, -1);  //已经准备好传输数据的客户端的数量
        for(int i = 0; i < length; i++) {
            if(fds[i].revents & POLLIN) {
                if(fds[i].fd == sockfd){
                    struct sockaddr_in client_addr;
                    int len = sizeof(client_addr);
                    int clientfd = accept(fds[i].fd, (struct sockaddr*)&client_addr, (socklen_t *)&len);

                    fds[length].fd = clientfd;
                    fds[length].events = POLLIN;
                    length++;

                    if(--ready_num == 0) {
                        // continue;
                        break;
                    }
                }
                else {
                    memset(msg, 0, sizeof(msg));
                    int ret = recv(fds[i].fd, msg, sizeof(msg), 0);

                    if(ret == 0) {  //断开连接
                        printf("connect with client %d interrupt\n", fds[i].fd);
                        close(fds[i].fd);
                        fds[i].fd = -1;
                        continue;
                    }
                    fds[i].events = POLLOUT;
                    printf("receive from cleint %d: %s\n", fds[i].fd, msg);
                }
            }
            else if(fds[i].revents & POLLOUT) {
                int ret = send(fds[i].fd, msg, sizeof(msg), 0);
                fds[i].events = POLLIN;
            }
        }
    }
    close(sockfd);
    return 0;
}

int socket_init(char IP[], char Port[], struct sockaddr_in *server_addr)
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);  //create the sockfd
    if(sockfd == -1) {
        printf("error creating socket!\n");
        return -1;
    }
    printf("%s %s\n",IP, Port);
    memset(&(*server_addr), 0, sizeof(*server_addr));
    server_addr->sin_family = AF_INET;  //Dedicated to IPv4
    server_addr->sin_addr.s_addr = inet_addr(IP); //IP
    server_addr->sin_port = htons(atoi(Port));  //Port

    return sockfd;
}