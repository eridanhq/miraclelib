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

#define  ERIDAN_PATH "/opt/eridan/user"

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

static inline eridan_cmd_hdr_t *
get_reply_hdr()
{
    eridan_cmd_hdr_t *reply;

    reply = malloc(sizeof(eridan_cmd_hdr_t));
    memset(reply, 0, sizeof(eridan_cmd_hdr_t));
    reply->cookie  = EC_MAGIC_COOKIE;
    reply->version = EC_VERSION;
    reply->type    = EC_TYPE_RESP;
    reply->length  = 20;

    return reply;
}

static inline eridan_cmd_resp_t *
get_reply_body(eridan_cmd_req_t *req, int num_args)
{
    eridan_cmd_resp_t *rbody;

    /* Note : use of flexible array in C */
    rbody = malloc(sizeof(eridan_cmd_resp_t)+num_args*EC_CHAR_STR_SIZE);
    memset(rbody, 0, sizeof(eridan_cmd_resp_t));
    rbody->reqid = req->reqid;
    rbody->cmdid = req->cmdid;

    return rbody;
}

ecm_ctrl_t
handle_sysinit(eridan_cmd_req_t *req, eridan_cmd_resp_t **presp)
{
    eridan_cmd_resp_t *resp;
    const char *replystr = "DONE";
    char cmd[1024];

    resp  = get_reply_body(req, 1);
    memset(cmd, 0, sizeof(cmd));
    snprintf(cmd, sizeof(cmd), "%s/%s", ERIDAN_PATH, "sysinit");
    system(cmd);
    resp->num_results = 1;
    strcpy(resp->cmd_results, replystr);

    *presp = resp;

    return ECM_SUCCESS;
}

ecm_ctrl_t
handle_get_freq(eridan_cmd_req_t *req, eridan_cmd_resp_t **presp)
{
    eridan_cmd_resp_t *resp;
    const char *replystr = "DONE";
    char cmd[1024];

    resp  = get_reply_body(req, 1);
    memset(cmd, 0, sizeof(cmd));
    snprintf(cmd, sizeof(cmd), "%s/%s", ERIDAN_PATH, "getfreq");
    system(cmd);
    resp->num_results = 1;
    strcpy(resp->cmd_results, replystr);

    *presp = resp;

    return ECM_SUCCESS;
}

ecm_ctrl_t
handle_get_stats(eridan_cmd_req_t *req, eridan_cmd_resp_t **presp)
{
    eridan_cmd_resp_t *resp;
    const char *replystr = "DONE";
    char cmd[1024];

    resp  = get_reply_body(req, 1);
    memset(cmd, 0, sizeof(cmd));
    snprintf(cmd, sizeof(cmd), "%s/%s", ERIDAN_PATH, "getstat");
    system(cmd);
    resp->num_results = 1;
    strcpy(resp->cmd_results, replystr);

    *presp = resp;

    return ECM_SUCCESS;
}

ecm_ctrl_t
handle_set_freq(eridan_cmd_req_t *req, eridan_cmd_resp_t **presp)
{
    eridan_cmd_resp_t *resp;
    const char *replystr = "DONE";
    char cmd[1024];

    resp  = get_reply_body(req, 1);
    memset(cmd, 0, sizeof(cmd));
    snprintf(cmd, sizeof(cmd), "%s/%s %s %s", ERIDAN_PATH, "setfreq", req->cmd_args, req->cmd_args+EC_CHAR_STR_SIZE);
    system(cmd);
    printf("Executing [%s]\n", cmd);
    resp->num_results = 1;
    strcpy(resp->cmd_results, replystr);

    *presp = resp;

    return ECM_SUCCESS;
}

ecm_ctrl_t
handle_get_pwr(eridan_cmd_req_t *req, eridan_cmd_resp_t **presp)
{
    eridan_cmd_resp_t *resp;
    const char *replystr = "DONE";
    char cmd[1024];

    resp  = get_reply_body(req, 1);
    memset(cmd, 0, sizeof(cmd));
    snprintf(cmd, sizeof(cmd), "%s/%s", ERIDAN_PATH, "getpwr");
    system(cmd);
    resp->num_results = 1;
    strcpy(resp->cmd_results, replystr);

    *presp = resp;

    return ECM_SUCCESS;
}

ecm_ctrl_t
handle_set_pwr(eridan_cmd_req_t *req, eridan_cmd_resp_t **presp)
{
    eridan_cmd_resp_t *resp;
    const char *replystr = "DONE";
    char cmd[1024];

    resp  = get_reply_body(req, 1);
    memset(cmd, 0, sizeof(cmd));
    snprintf(cmd, sizeof(cmd), "%s/%s %s %s", ERIDAN_PATH, "setfpwr", req->cmd_args, req->cmd_args+EC_CHAR_STR_SIZE);
    system(cmd);
    resp->num_results = 1;
    strcpy(resp->cmd_results, replystr);

    *presp = resp;

    return ECM_SUCCESS;
}

ecm_ctrl_t
handle_get_sample_rate(eridan_cmd_req_t *req, eridan_cmd_resp_t **presp)
{
    eridan_cmd_resp_t *resp;
    const char *replystr = "DONE";
    char cmd[1024];

    resp  = get_reply_body(req, 1);
    memset(cmd, 0, sizeof(cmd));
    snprintf(cmd, sizeof(cmd), "%s/%s", ERIDAN_PATH, "getsamplerate");
    system(cmd);
    resp->num_results = 1;
    strcpy(resp->cmd_results, replystr);

    *presp = resp;

    return ECM_SUCCESS;
}

ecm_ctrl_t
handle_set_sample_rate(eridan_cmd_req_t *req, eridan_cmd_resp_t **presp)
{
    eridan_cmd_resp_t *resp;
    const char *replystr = "DONE";
    char cmd[1024];

    resp  = get_reply_body(req, 1);
    memset(cmd, 0, sizeof(cmd));
    snprintf(cmd, sizeof(cmd), "%s/%s %s %s", ERIDAN_PATH, "setsamplerate", req->cmd_args, req->cmd_args+EC_CHAR_STR_SIZE);
    system(cmd);
    resp->num_results = 1;
    strcpy(resp->cmd_results, replystr);

    *presp = resp;

    return ECM_SUCCESS;
}

ecm_ctrl_t
handle_get_rxfreq(eridan_cmd_req_t *req, eridan_cmd_resp_t **presp)
{
    eridan_cmd_resp_t *resp;
    const char *replystr = "DONE";
    char cmd[1024];

    resp  = get_reply_body(req, 1);
    memset(cmd, 0, sizeof(cmd));
    //snprintf(cmd, sizeof(cmd), "%s/%s", ERIDAN_PATH, "sysinit");
    //system(cmd);
    // TODO add 
    resp->num_results = 1;
    strcpy(resp->cmd_results, replystr);

    *presp = resp;

    return ECM_SUCCESS;
}

ecm_ctrl_t
handle_set_rxfreq(eridan_cmd_req_t *req, eridan_cmd_resp_t **presp)
{
    eridan_cmd_resp_t *resp;
    const char *replystr = "DONE";
    char cmd[1024];

    resp  = get_reply_body(req, 1);
    memset(cmd, 0, sizeof(cmd));
    snprintf(cmd, sizeof(cmd), "%s/%s %s %s", ERIDAN_PATH, "setrxfreq", req->cmd_args, req->cmd_args+EC_CHAR_STR_SIZE);
    system(cmd);
    resp->num_results = 1;
    strcpy(resp->cmd_results, replystr);

    *presp = resp;

    return ECM_SUCCESS;
}

ecm_ctrl_t
handle_get_rxsamplerate(eridan_cmd_req_t *req, eridan_cmd_resp_t **presp)
{
    eridan_cmd_resp_t *resp;
    const char *replystr = "DONE";
    char cmd[1024];

    resp  = get_reply_body(req, 1);
    memset(cmd, 0, sizeof(cmd));
    //snprintf(cmd, sizeof(cmd), "%s/%s", ERIDAN_PATH, "sysinit");
    //system(cmd);
    resp->num_results = 1;
    strcpy(resp->cmd_results, replystr);

    *presp = resp;

    return ECM_SUCCESS;
}

ecm_ctrl_t
handle_set_rxsamplerate(eridan_cmd_req_t *req, eridan_cmd_resp_t **presp)
{
    eridan_cmd_resp_t *resp;
    const char *replystr = "DONE";
    char cmd[1024];

    resp  = get_reply_body(req, 1);
    memset(cmd, 0, sizeof(cmd));
    snprintf(cmd, sizeof(cmd), "%s/%s %s %s", ERIDAN_PATH, "setfpwr", req->cmd_args, req->cmd_args+EC_CHAR_STR_SIZE);
    system(cmd);
    resp->num_results = 1;
    strcpy(resp->cmd_results, replystr);

    *presp = resp;

    return ECM_SUCCESS;
}

ecm_ctrl_t
handle_get_rxgains(eridan_cmd_req_t *req, eridan_cmd_resp_t **presp)
{
    eridan_cmd_resp_t *resp;
    const char *replystr = "DONE";
    char cmd[1024];

    resp  = get_reply_body(req, 1);
    memset(cmd, 0, sizeof(cmd));
    //snprintf(cmd, sizeof(cmd), "%s/%s", ERIDAN_PATH, "sysinit");
    //system(cmd);
    resp->num_results = 1;
    strcpy(resp->cmd_results, replystr);

    *presp = resp;

    return ECM_SUCCESS;
}

ecm_ctrl_t
handle_set_rxgains(eridan_cmd_req_t *req, eridan_cmd_resp_t **presp)
{
    eridan_cmd_resp_t *resp;
    const char *replystr = "DONE";
    char cmd[1024];

    resp  = get_reply_body(req, 1);
    memset(cmd, 0, sizeof(cmd));
    snprintf(cmd, sizeof(cmd), "%s/%s %s %s", ERIDAN_PATH, "setfpwr", req->cmd_args, req->cmd_args+EC_CHAR_STR_SIZE);
    system(cmd);
    resp->num_results = 1;
    strcpy(resp->cmd_results, replystr);

    *presp = resp;

    return ECM_SUCCESS;
}

ecm_ctrl_t
handle_sysoff(eridan_cmd_req_t *req, eridan_cmd_resp_t **presp)
{
    eridan_cmd_resp_t *resp;
    const char *replystr = "DONE";
    char cmd[1024];

    resp  = get_reply_body(req, 1);
    memset(cmd, 0, sizeof(cmd));
    snprintf(cmd, sizeof(cmd), "%s/%s", ERIDAN_PATH, "sysoff");
    system(cmd);
    resp->num_results = 1;
    strcpy(resp->cmd_results, replystr);

    *presp = resp;

    return ECM_SUCCESS;
}

ecm_ctrl_t
handle_start_scp(eridan_cmd_req_t *req)
{
    return ECM_SUCCESS;
}

ecm_ctrl_t
handle_prep_scp(eridan_cmd_req_t *req)
{
    return ECM_SUCCESS;
}

ecm_ctrl_t
handle_reset_now(eridan_cmd_req_t *req, eridan_cmd_resp_t **presp)
{
    eridan_cmd_resp_t *resp;
    const char *replystr = "DONE";
    char cmd[1024];

    resp  = get_reply_body(req, 1);
    memset(cmd, 0, sizeof(cmd));
    snprintf(cmd, sizeof(cmd), "%s/%s", ERIDAN_PATH, "reboot");
    system(cmd);
    resp->num_results = 1;
    strcpy(resp->cmd_results, replystr);

    *presp = resp;

    return ECM_SUCCESS;
}

ecm_ctrl_t
handle_reset_done(eridan_cmd_req_t *req)
{
    return ECM_SUCCESS;
}

ecm_ctrl_t
handle_send_updates(eridan_cmd_req_t *req)
{
    return ECM_SUCCESS;
}

ecm_ctrl_t
handle_check_updates(eridan_cmd_req_t *req)
{
    return ECM_SUCCESS;
}

ecm_ctrl_t
handle_get_version(eridan_cmd_req_t *req, eridan_cmd_resp_t **presp)
{
    eridan_cmd_resp_t *resp;
    const char *verstr = "1.1";

    resp  = get_reply_body(req, 1);
    resp->num_results = 1;
    /* Note : use of flexible array in C */
    //memcpy(resp->cmd_results, verstr, strlen(verstr)+1);
    strcpy(resp->cmd_results, verstr);

    *presp = resp;

    return ECM_SUCCESS;
}

ecm_ctrl_t
handle_cmds(char *rbuf, int rlen, struct sockaddr *caddr)
{
    eridan_cmd_hdr_t *hdr, *reply;
    eridan_cmd_req_t *req;
    eridan_cmd_resp_t *resp;
    ssize_t n;
    int clen = sizeof(*caddr);

    printf("\n\nStarting to handle ecm_cmds\n");
    hdr = (eridan_cmd_hdr_t *)rbuf;
    printf("This is what we got:\n");
    printf("Got in cmd cookie : %x\n",  hdr->cookie);
    printf("Got in cmd version: %x\n",  hdr->version);
    printf("Got in cmd type: %x\n",     hdr->type);
    printf("Got in cmd length: %d\n",   hdr->length);
    printf("Got in cmd reserved: %x\n", hdr->reserved);

    req   = malloc(sizeof(eridan_cmd_req_t)+hdr->length);
    memset(req, 0, sizeof(eridan_cmd_req_t)+hdr->length);
    n = recvfrom(udpfd, (const char *)req, sizeof(eridan_cmd_req_t)+hdr->length,
                        0,
                        (const struct sockaddr *)&caddr, (unsigned int *)&clen);

    printf("Got req from client\n");

    switch (req->cmdid)
    {
        case ERIDAN_CMD_SYSINIT:
            handle_sysinit(req, &resp);
        break;

        case ERIDAN_CMD_GET_FREQ:
            handle_get_freq(req, &resp);
        break;

        case ERIDAN_CMD_GET_STATS:
            handle_get_stats(req, &resp);
        break;

        case ERIDAN_CMD_SET_FREQ:
            handle_set_freq(req, &resp);
        break;

        case ERIDAN_CMD_GET_PWR:
            handle_get_pwr(req, &resp);
        break;

        case ERIDAN_CMD_SET_PWR:
            handle_set_pwr(req, &resp);
        break;

        case ERIDAN_CMD_GET_SAMPLE_RATE:
            handle_get_sample_rate(req, &resp);
        break;

        case ERIDAN_CMD_SET_SAMPLE_RATE:
            handle_set_sample_rate(req, &resp);
        break;

        case ERIDAN_CMD_GET_RXFREQ:
            handle_get_rxfreq(req, &resp);
        break;

        case ERIDAN_CMD_SET_RXFREQ:
            handle_set_rxfreq(req, &resp);
        break;

        case ERIDAN_CMD_GET_RXSAMPLERATE:
            handle_get_rxsamplerate(req, &resp);
        break;

        case ERIDAN_CMD_SET_RXSAMPLERATE:
            handle_set_rxsamplerate(req, &resp);
        break;

        case ERIDAN_CMD_GET_RXGAINS:
            handle_get_rxgains(req, &resp);
        break;

        case ERIDAN_CMD_SET_RXGAINS:
            handle_set_rxgains(req, &resp);
        break;

        case ERIDAN_CMD_SYSOFF:
            handle_sysoff(req, &resp);
        break;

        case ERIDAN_CMD_START_SCP:
            handle_start_scp(req);
        break;

        case ERIDAN_CMD_PREP_SCP:
            handle_prep_scp(req);
        break;

        case ERIDAN_CMD_RESET_NOW:
            handle_reset_now(req, &resp);
        break;

        case ERIDAN_CMD_RESET_DONE:
            handle_reset_done(req);
        break;

        case ERIDAN_CMD_SEND_UPDATES:
            handle_send_updates(req);
        break;

        case ERIDAN_CMD_CHECK_UPDATES:
            handle_check_updates(req);
        break;

        case ERIDAN_CMD_GET_VERSION:
            handle_get_version(req, &resp);
        break;

        default:
        break;
    }


    reply = get_reply_hdr();
    reply->length = sizeof(*resp);
    //resp  = get_reply_body(req, 0);
    sendto(udpfd,  (const char *)reply, sizeof(eridan_cmd_hdr_t), 
                   MSG_CONFIRM,
                   (const struct sockaddr *)&caddr, sizeof(*caddr));
    sendto(udpfd,  (const char *)resp, sizeof(*resp)+resp->num_results*EC_CHAR_STR_SIZE,
                    MSG_CONFIRM,
                   (const struct sockaddr *) &caddr, sizeof(*caddr));

    free(req);
    free(reply);
    free(resp);

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

// No need of handling args for now   OK
// Create file for debug prints
// Read from socket                   DONE
// parse cmds and reply               DONE
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