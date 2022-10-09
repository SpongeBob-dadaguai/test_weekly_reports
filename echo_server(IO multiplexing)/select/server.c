#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/select.h>

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

    fd_set fds, tmp;
    FD_ZERO(&fds);
    FD_SET(sockfd, &fds);
    int maxfd = sockfd;

    while(1) {
        tmp = fds;
        int ret = select(maxfd + 1, &tmp, NULL, NULL, NULL);
        if(ret == -1) {
            perror("select");
            return -1;
        } else if(ret == 0) {
            continue;
        } else {
            if(FD_ISSET(sockfd, &tmp)) {  //new client connected
                struct sockaddr_in client_addr;
                int len = sizeof(client_addr);
                int clientfd = accept(sockfd, (struct sockaddr*)&client_addr, (socklen_t*)&len);

                FD_SET(clientfd, &fds);
                maxfd = maxfd > clientfd ? maxfd : clientfd;
            }
            for (int i = sockfd + 1; i <= maxfd; i++) {
                if(FD_ISSET(i, &tmp)) {
                    char msg[MaxMessageLength];
                    memset(msg, 0, sizeof(msg));
                    int ret = recv(i, msg, sizeof(msg), 0);
                    if(ret == -1) {
                        perror("receive");
                    } else if(ret == 0) {
                        //connect interrupt
                        printf("client %d is closed.....\n", i);
                        close(i);
                        FD_CLR(i, &fds);
                    } else {
                        printf("receive from client %d: %s\n", i, msg);
                        send(i, msg, sizeof(msg), 0);
                    }

                }
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