#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

#define BACKLOG 5  //Maximum number of simultaneous connections

int main(int argc, char **argv) 
{
    if(argc < 3) {
        printf("Usage: %s <IP> <Port>", argv[0]);
        return -1;
    }
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);  //create the sockfd
    if(sockfd == -1) {
        printf("error creating socket!\n");
        return -1;
    }
    
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;  //Dedicated to IPv4
    server_addr.sin_addr.s_addr = inet_addr(argv[1]); //IP
    server_addr.sin_port = htons(atoi(argv[2]));  //Port
    
    //bind IP and Port to the socket file descriptor
    if(bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1)  {
        printf("bind error\n");
        close(sockfd);
        return -1;
    }

    //turn sockfd into passive
    if(listen(sockfd, BACKLOG) != 0) {
        printf("Listening failed!\n");
        close(sockfd);
        return -1;
    }
    
    int clientfd;
    struct sockaddr_in client_addr;
    int socklen = sizeof(client_addr);
    printf("waiting for client connect.........\n");
    clientfd = accept(sockfd, (struct sockaddr*)&client_addr, (socklen_t*)&socklen);
    if(clientfd == -1) {
        perror("accept");
        close(clientfd);
        close(sockfd);
        return -1;
    }
    else {
        printf("connect with client: %s\n\n", inet_ntoa(client_addr.sin_addr));
    }

    char MSG[1024];
    while(1) {
        memset(MSG, 0, sizeof(MSG));
        int sign = recv(clientfd, MSG, sizeof(MSG), 0);
        if(sign <= 0) {
            printf("Receiving interrupt\n");
            break;
        }
        printf("receive from server: %s\n", MSG);

        memset(MSG, 0, sizeof(MSG));
        strcpy(MSG, "Hi, I'm SpongeBob-dadaguai!");  //reply the client
        if((sign = send(clientfd, MSG, sizeof(MSG), 0)) <= 0) {
            perror("send");
            break;
        }
        printf("send: %s\n\n", MSG);
    }

    return 0;

}
