#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <pthread.h>
   
#define BACKLOG 10  //Maximum number of simultaneous connections
#define MaxMessageLength 1024  //Maximum length of message


typedef struct client {
    char client_name[MaxMessageLength];  //name of the client
    pthread_t thread;    //create a new thread for the client
    struct sockaddr_in client_addr; 
    int clientfd;   //File descriptor for the client connected
    int length;   //The current number of client connections is stored only in the head
    struct client *next; 
}client, *Client;

Client head, tail;  //head and tail of the list
int Socket_init(char IP[], char Port[], struct sockaddr_in *server_addr);
void *new_thread(void *arg);  //create a new thread for the client
void *listen_or_wait(void *arg);  // Listen to the client and wait for connecting
void *Quit(void *arg);   //end the server thread

int main(int argc, char **argv) 
{
    if(argc < 3) {
        printf("Usage: %s <IP> <Port>\n", argv[0]);
        return -1;
    }
    
    struct sockaddr_in server_addr;
    int sockfd = Socket_init(argv[1], argv[2], &server_addr);  //Init the socket
    
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

    head = (Client)malloc(sizeof(struct client));
    head->length = 0;
    head->next = NULL;
    tail = head;
    
    printf("waiting for client connect......\n");

    pthread_t listen;
    int res = pthread_create(&listen, NULL, listen_or_wait, (void*)&sockfd);
    pthread_detach(listen);

    pthread_t quit;
    pthread_create(&quit, NULL, Quit, NULL);
    pthread_join(quit, NULL);

    printf("\nmain thread ending.....\n");
    return 0;

}

int Socket_init(char IP[], char Port[], struct sockaddr_in *server_addr)
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

void *new_thread(void *arg)  //create a new thread for the client
{
    Client cur_client = (Client)arg;  //Forced conversion
    // printf("clientfd: %d\n", clientfd);
    char MSG[MaxMessageLength];
    while(1) {
        memset(MSG, 0, sizeof(MSG));
        int sign = recv(cur_client->clientfd, MSG, sizeof(MSG), 0);
        if(sign <= 0) {
            printf("connecting with %s interrupt\n", cur_client->client_name);
            head->length--;
            break;
        }
        printf("receive from client %s: %s\n", cur_client->client_name, MSG);

        memset(MSG, 0, sizeof(MSG));
        strcpy(MSG, "Hi, I'm SpongeBob-dadaguai!");  //reply the client
        if((sign = send(cur_client->clientfd, MSG, sizeof(MSG), 0)) <= 0) { 
            perror("send");
            break;
        }
        printf("send: %s\n\n", MSG);
    }
    return NULL;
}

void *listen_or_wait(void *arg)  // Listen to the client and wait for connecting
{
    int sockfd = *(int*)(arg);
    int socklen = sizeof(struct sockaddr_in);

    while(head->length < BACKLOG){
        Client node = (Client)malloc(sizeof(struct client));
        tail->next = node;
        tail = tail->next;
        tail->next = NULL;
        int clientfd = accept(sockfd, (struct sockaddr*)&(node->client_addr), (socklen_t*)&socklen);
        if(clientfd != -1) {
            node->clientfd = clientfd;
            recv(node->clientfd, node->client_name, sizeof(node->client_name), 0);  //receive the name of the client
            // printf("%s\n", node->client_name);
            void *arg = (void *)node;
            int res = pthread_create(&(node->thread), NULL, new_thread, arg);   //create a new thread for the client
            if(res != 0) {
                perror("create thread");
            }
            pthread_detach(node->thread);  //detach the thread
            head->length++;
        }
    }
    return NULL;
}

void *Quit(void *arg)   //end the server thread
{
    char quit[MaxMessageLength];
    printf("if you want to quit, please input 'quit' or 'q' to end the main thread\n\n");
    while(1) {
        scanf("%s", quit);
        if(strcmp(quit, "q") == 0 || strcmp(quit, "quit") == 0) {
            printf("main thread will end in 5 seconds\n");
            for(int i = 5;i >= 1; i--) {
                printf("%d ", i);
                fflush(stdout);
                sleep(1);
            }
            break;
        }
    }
    return NULL;
}


