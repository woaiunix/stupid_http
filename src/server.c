#include "../headers/server.h"
   
void create_server(int port) {
    int sockfd, connfd;
    struct sockaddr_in servaddr, cli;

    int iSetOption = 1;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char*)&iSetOption, sizeof(iSetOption));

    if (sockfd <  0) {
        perror("Socket creation failed");
        exit(1);
    }
 
    bzero(&servaddr, sizeof(servaddr));
    
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(port);
   
    if ((bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr))) < 0) {
        perror("Socket bind failed");
        exit(1);
    }

    if ((listen(sockfd, 5)) != 0) {
        perror("Listen failed");
        exit(1);
    }
    else
        printf("Server is listening on port %d...\n", port);
    
    socklen_t len = sizeof(cli);

    while(1) {
        pthread_t tid;
        connfd = accept(sockfd, (struct sockaddr*)&cli, &len);
        
        if(pthread_create(&tid, NULL, request_handler, (void*)&connfd) < 0)
            perror("Create thread error");
    } 
}