#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <arpa/inet.h>

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
    struct sockaddr_in client_addr;
    int len = sizeof(client_addr);
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

    int epoll_fd = epoll_create(BACKLOG);  //create epoll_fd
    struct epoll_event ev, events[BACKLOG];  //ev用于注册事件，events用于监听客户端

    ev.data.fd = sockfd;
    ev.events = EPOLLIN | EPOLLET;
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, sockfd, &ev);  //添加ev
    char msg[MaxMessageLength];  //接收数据
    while(1) {
        int num = epoll_wait(epoll_fd, events, BACKLOG, -1);
        for(int i = 0; i < num; i++) {
            if(events[i].data.fd == sockfd) {
                //有新的客户端连接进来
                int clientfd = accept(sockfd, (struct sockaddr *)&client_addr, (socklen_t *)&len);
                if(clientfd < 0) {
                    perror("accept");
                    continue;
                }
                printf("connect with %d\n", clientfd);

                ev.data.fd = clientfd;
                ev.events = EPOLLIN | EPOLLET;
                epoll_ctl(epoll_fd, EPOLL_CTL_ADD, clientfd, &ev);
            } else if(events[i].events & EPOLLIN && events[i].data.fd > 0)  {//接收数据
                memset(msg, 0, sizeof(msg));
                
                int clientfd = events[i].data.fd;
                if(clientfd < 0) {
                    continue;
                }

                int ret = recv(clientfd, msg, sizeof(msg), 0);
                if(ret < 0) {
                    perror("recv");
                    close(clientfd);
                    events[i].data.fd = -1;
                    continue;
                } else if(ret == 0) {
                    printf("connect with %d interrupt\n", clientfd);
                    close(clientfd);
                    events[i].data.fd = -1;
                    continue;
                }       
                printf("receive from %d: %s\n", events[i].data.fd, msg);
                ev.data.fd = clientfd;
                ev.events = EPOLLOUT | EPOLLET;
                epoll_ctl(epoll_fd, EPOLL_CTL_MOD, clientfd, &ev);         
            } else if(events[i].events & EPOLLOUT) {
                int clientfd = events[i].data.fd;
                int ret = send(clientfd, msg, sizeof(msg), 0);
                if(ret == -1) {
                    perror("send"); 
                }
                ev.data.fd = clientfd;
                ev.events = EPOLLIN | EPOLLET;
                epoll_ctl(epoll_fd, EPOLL_CTL_MOD, clientfd, &ev);   
            }
        }
    }
    close(sockfd);
    close(epoll_fd);
    return 0;
}

int socket_init(char IP[], char Port[], struct sockaddr_in *server_addr)
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);  //create the sockfd
    if(sockfd == -1) {
        printf("error creating socket!\n");
        return -1;
    }
    // printf("%s %s\n",IP, Port);
    memset(&(*server_addr), 0, sizeof(*server_addr));
    server_addr->sin_family = AF_INET;  //Dedicated to IPv4
    server_addr->sin_addr.s_addr = inet_addr(IP); //IP
    server_addr->sin_port = htons(atoi(Port));  //Port

    return sockfd;
}