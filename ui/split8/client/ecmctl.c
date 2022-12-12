// Client side implementation of UDP client-server model 
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 

#include "common/eridan_cmd.h"

#define MAXLINE 1024 

int
connect_to_server(struct sockaddr_in *servaddr)
{
    int sockfd;

    // Creating socket file descriptor 
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    }

    memset(servaddr, 0, sizeof(servaddr));
        
    // Filling server information 
    servaddr->sin_family = AF_INET;
    servaddr->sin_port = htons(EC_SERVER_PORT);
    servaddr->sin_addr.s_addr = inet_addr("127.0.0.1");

    return sockfd;
}

int main()
{ 
    struct sockaddr_in     servaddr;
    char buffer[MAXLINE]; 
    eridan_cmd_hdr_t *hdr, *reply;
    eridan_cmd_req_t *req;

    int n, len;

    int sockfd = connect_to_server(&servaddr);

    hdr = malloc(sizeof(eridan_cmd_hdr_t));
    memset(hdr, 0, sizeof(eridan_cmd_hdr_t));
    hdr->cookie  = EC_MAGIC_COOKIE;
    hdr->version = 0;
    hdr->length  = 20;
    req   = malloc(sizeof(eridan_cmd_req_t));
    memset(req, 0, sizeof(eridan_cmd_req_t));
    req->reqid   = 0x3434;
    req->cmdid   = ERIDAN_CMD_GET_VERSION;

    sendto(sockfd,  (const char *)hdr, sizeof(eridan_cmd_hdr_t),
                    MSG_CONFIRM,
                    (const struct sockaddr *) &servaddr, sizeof(servaddr));
    sendto(sockfd,  (const char *)req, sizeof(eridan_cmd_req_t),
                    MSG_CONFIRM,
                    (const struct sockaddr *) &servaddr, sizeof(servaddr));
    printf("Hello message sent.\n");

    len = sizeof(servaddr);
    n = recvfrom(sockfd, (char *)buffer, sizeof(eridan_cmd_hdr_t),
                         MSG_WAITALL,
                         (struct sockaddr *) &servaddr, (unsigned int *)&len); 
    printf("Recved %d bytes from server\n", n);
    reply = (eridan_cmd_hdr_t *)buffer;
    printf("Server : %x\n", reply->cookie);
    
    close(sockfd); 
    return 0; 
}