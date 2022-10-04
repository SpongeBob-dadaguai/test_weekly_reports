#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

#define BACKLOG 5

int main(int argc, char **argv) 
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);  //create the sockfd
    if(sockfd == -1) {
        printf("error creating socket!\n");
        return -1;
    }
    
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(8000);
    
    if(bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1)  {
        printf("bind error\n");
        close(sockfd);
        return -1;
    }
    // printf("bind success\n");
    if(listen(sockfd, BACKLOG) != 0) {
        printf("Listening failed!\n");
        close(sockfd);
        return -1;
    }
    // printf("listen success\n");
    
    int clientfd;
    struct sockaddr_in client_addr;
    int socklen = sizeof(client_addr);
    printf("waiting for client connect.........\n");
    clientfd = accept(sockfd, (struct sockaddr*)&client_addr,
    (socklen_t*)&socklen);
    if(clientfd == -1) {
        perror("accept");
        close(clientfd);
        close(sockfd);
        return -1;
    }
    else {
        
        printf("connect with client: %s\n", inet_ntoa(client_addr.sin_addr));
    }

    char buffer[1024];
    while (1)
    {
    int iret;
    memset(buffer,0,sizeof(buffer));
    iret=recv(clientfd,buffer,sizeof(buffer),0);
    if (iret<=0) 
    {
       printf("iret=%d\n",iret); break;  
    }
    printf("receive :%s\n",buffer);
 
    strcpy(buffer,"ok");//reply cilent with "ok"
    if ( (iret=send(clientfd,buffer,strlen(buffer),0))<=0) 
    { 
        perror("send"); 
        break; 
    }
    printf("send :%s\n",buffer);
  }

    return 0;

}
