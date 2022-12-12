/*
 * Server For Eridan Communication Miracle Radio Controller
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "common/eridan_cmd.h"

typedef enum ecm_ctrl_ {
    ECM_FAILURE = -1,
    ECM_SUCCESS
} ecm_ctrl_t;

fd_set fdset;
int    udpfd, lfd;

ecm_ctrl_t
ecm_ctrl_init(void)
{
    FD_ZERO(&fdset);

    return ECM_SUCCESS;
}

/*
 * Start UDP server
 */
ecm_ctrl_t
start_server(int port)
{
    int sockfd;
    int optval;
    struct sockaddr_in saddr, caddr;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        printf("ERROR opening socket");
        return (0);
    }

    /* setsockopt: Handy debugging trick that lets 
     * us rerun the server immediately after we kill it; 
     * otherwise we have to wait about 20 secs. 
     * Eliminates "ERROR on binding: Address already in use" error. 
     */
    optval = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, 
               (const void *)&optval , sizeof(int));

    memset((char *) &saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);
    saddr.sin_port = htons((unsigned short)port);

    if (bind(sockfd, (struct sockaddr *) &saddr, 
                     sizeof(saddr)) < 0)
    {
        printf("ERROR on binding");
        close(sockfd);
        return ECM_FAILURE;
    }

    udpfd = sockfd;
    FD_SET(udpfd, &fdset);

    return ECM_SUCCESS;
}

ecm_ctrl_t
handle_cmds(char *rbuf, int rlen, struct sockaddr *caddr)
{
    eridan_cmd_hdr_t *hdr, *reply;
    eridan_cmd_req_t *req;
    ssize_t n;
    int clen = sizeof(*caddr);

    printf("\n\nStarting to handle ecm_cmds\n");
    hdr = (eridan_cmd_hdr_t *)rbuf;
    printf("This is what we got:\n");
    printf("Got in cmd cookie : %x\n",  hdr->cookie);
    printf("Got in cmd version: %x\n",  hdr->version);
    printf("Got in cmd type: %x\n",     hdr->type);
    printf("Got in cmd length: %x\n",   hdr->length);
    printf("Got in cmd reserved: %x\n", hdr->reserved);

    req   = malloc(sizeof(eridan_cmd_req_t));
    memset(req, 0, sizeof(eridan_cmd_req_t));
    n = recvfrom(udpfd, (const char *)req, sizeof(eridan_cmd_req_t),
                        0,
                        (const struct sockaddr *)&caddr, (unsigned int *)&clen);

    printf("Got req from client\n");

    reply = malloc(sizeof(eridan_cmd_hdr_t));
    memset(reply, 0, sizeof(eridan_cmd_hdr_t));
    reply->cookie  = EC_MAGIC_COOKIE;
    reply->version = 0;
    reply->length  = 20;

    sendto(udpfd,  (const char *)reply, sizeof(eridan_cmd_hdr_t), 
                   MSG_CONFIRM,
                   (const struct sockaddr *)&caddr, sizeof(*caddr));

    return ECM_FAILURE;
}

/*
 * Handle cmds coming over UDP socket
 */
ecm_ctrl_t
handle_conns(void)
{
    struct  sockaddr_in saddr, caddr;
    char    rbuf[1024];
    ssize_t n;
    int     nready;

    for (;;) {
        printf("fdset :%hu udpfd:%hu\n", fdset, udpfd);
        nready = select(udpfd+5, &fdset, NULL, NULL, NULL);

        if (FD_ISSET(lfd, &fdset)) {
            printf("Unsupported right now");
        }
        if (FD_ISSET(udpfd, &fdset)) {
            int clen = sizeof(caddr);
            memset(rbuf, 0, sizeof(rbuf));
            n = recvfrom(udpfd, rbuf, sizeof(eridan_cmd_hdr_t), 0,
                         (struct sockaddr *)&caddr, (unsigned int *)&clen);
            handle_cmds(rbuf, sizeof(rbuf), (struct sockaddr *)&caddr);
        }
    }
}

// No need of handling args for now
// Create file for debug prints
// Read from socket
// parse cmds and reply
// test program on bats               DONE
int
main(int argc, char *argv[]) 
{
    printf("Hello, Welcome to Miracle Controller Server!!!\n");

    ecm_ctrl_init();
    start_server(EC_SERVER_PORT);
    handle_conns();

    exit(0);
}