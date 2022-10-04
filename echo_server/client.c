#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main(int argc, char** argv) 
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

    struct sockaddr_in serveraddr;
    memset(&serveraddr, 0 ,sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(atoi(argv[2])); //server`s port
    serveraddr.sin_addr.s_addr = inet_addr(argv[1]); //server`s IP

    int connectfd = connect(sockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr));

    if(connectfd != 0) {
        printf("connecting error!\n");
        close(sockfd);
        return -1;
    }
    printf("connecting success\n\n");
    char MSG[1024];
    while(1) {
        memset(MSG, 0, sizeof(MSG));
        printf("please input your message\n");
        scanf("%s", MSG);

        if (send(sockfd,MSG,strlen(MSG),0) <= 0) { // send message to server
            printf("sending error"); 
            break; 
        }
        printf("send: %s\n", MSG);
        printf("waiting for server's reply\n");
        memset(MSG, 0, sizeof(MSG));
        if ( (recv(sockfd,MSG,sizeof(MSG),0))<=0) {// receive server's reply 
            printf("recieve error\n"); 
            break;
        }
        printf("server's reply: %s\n", MSG);
        printf("\ncontinue to send message?yes or no\n");
        char choice[64];
        scanf("%s", choice);
        if(strcmp(choice, "no") == 0 || strcmp(choice, "NO") == 0 || strcmp(choice, "No") == 0) {
            break;
        }
    }
}