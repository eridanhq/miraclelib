// Client side implementation of UDP client-server model 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>
#include <sys/time.h>
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

    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    }

    memset(servaddr, 0, sizeof(servaddr));

    servaddr->sin_family = AF_INET;
    servaddr->sin_port = htons(EC_SERVER_PORT);
    servaddr->sin_addr.s_addr = inet_addr("127.0.0.1");

    struct timeval tv;
    tv.tv_sec = 10;
    tv.tv_usec = 0;
    if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0) {
        perror("Error");
        exit(EXIT_FAILURE);
    }

    return sockfd;
}

static inline void
print_request(eridan_cmd_req_t *req)
{
    printf("Request no: %x\n", req->reqid);
    printf("Command was: %s\n", cmd_names[req->cmdid]);
    printf("Arguments: %s\n", req->cmd_args);

    return;
}

static inline void
print_response(eridan_cmd_resp_t *resp)
{
    if (resp == NULL)
    {
        printf("Response error\n");
        return;
    }
    printf("Response no: %x\n", resp->reqid);
    printf("Command was: %s\n", cmd_names[resp->cmdid]);
    printf("Response: %s\n", resp->cmd_results);

    return;
}

#define ECMCTL_REQID 0x3434

static inline ecm_ctrl_t
get_request(int cmdid, int num_args, eridan_cmd_hdr_t **phdr, eridan_cmd_req_t **preq)
{
    eridan_cmd_hdr_t *hdr;
    eridan_cmd_req_t *req;

    hdr = malloc(sizeof(eridan_cmd_hdr_t));
    memset(hdr, 0, sizeof(eridan_cmd_hdr_t));
    hdr->cookie  = EC_MAGIC_COOKIE;
    hdr->version = EC_VERSION;
    hdr->length  = sizeof(eridan_cmd_req_t);
    hdr->type    = EC_TYPE_REQ;
    req   = malloc(sizeof(eridan_cmd_req_t)+num_args*EC_CHAR_STR_SIZE);
    memset(req, 0, sizeof(eridan_cmd_req_t)+num_args*EC_CHAR_STR_SIZE);
    req->reqid   = ECMCTL_REQID;
    req->cmdid   = cmdid;

    *phdr = hdr;
    *preq = req;

    return ECM_SUCCESS;
}

static inline ecm_ctrl_t
send_request_out(int sockfd, struct sockaddr_in *servaddr, eridan_cmd_hdr_t *hdr, eridan_cmd_req_t *req)
{
    sendto(sockfd,  (const char *)hdr, sizeof(eridan_cmd_hdr_t),
                    MSG_CONFIRM,
                    (const struct sockaddr *) servaddr, sizeof(*servaddr));
    sendto(sockfd,  (const char *)req, hdr->length,
                    MSG_CONFIRM,
                    (const struct sockaddr *) servaddr, sizeof(*servaddr));

    return ECM_SUCCESS;
}

static inline ecm_ctrl_t
check_input_side(char *side)
{
    if (side == NULL)
        return ECM_FAILURE;

    if (strlen(side) > 5)
        return ECM_FAILURE;

    if (strlen(side) == 1 && (side[0] == '1' || side[0] == '2'))
        return ECM_SUCCESS;

    if (strlen(side) == 2 && (toupper(side[0]) == 'T' && (side[1] == '1' || side[1] == '2')))
        return ECM_SUCCESS;

    if (strlen(side) == 3 && (toupper(side[0]) == 'T' && toupper(side[1]) == 'X' && (side[2] == '1' || side[2] == '2') ))
        return ECM_SUCCESS;

    if (strlen(side) == 4 && (toupper(side[0]) == 'T' && toupper(side[1]) == 'R' && toupper(side[2]) == 'X' && (side[3] == '1' || side[3] == '2')))
        return ECM_SUCCESS;

    return ECM_FAILURE;
}

ecm_ctrl_t
send_request(int sockfd, struct sockaddr_in *servaddr, eridan_cmd_id_t cmdid)
{
    eridan_cmd_hdr_t *hdr;
    eridan_cmd_req_t *req;

    get_request(cmdid, 0, &hdr, &req);
    send_request_out(sockfd, servaddr, hdr, req);

    free(hdr);
    free(req);

    return ECM_SUCCESS;
}

eridan_cmd_resp_t *
get_response(int sockfd, struct sockaddr_in *servaddr)
{
    int n, len;
    eridan_cmd_hdr_t *req, *hdr, *reply;
    eridan_cmd_resp_t *resp;

    hdr = malloc(sizeof(eridan_cmd_hdr_t));
    memset(hdr, 0, sizeof(eridan_cmd_hdr_t));
    len = sizeof(servaddr);
    n = recvfrom(sockfd, hdr, sizeof(eridan_cmd_hdr_t), /* XXX timeout */
                         MSG_WAITALL,
                         (struct sockaddr *) &servaddr, (unsigned int *)&len);
    if (n <= 0) {
        return NULL;
    }
    printf("Recved %d bytes from server\n", n);
    //reply = (eridan_cmd_hdr_t *)buffer;
    resp = malloc(hdr->length);
    printf("Server : %x\n", hdr->cookie);
    printf("Size: %d\n", hdr->length);
    n = recvfrom(sockfd, (char *)resp, hdr->length+35, /* XXX */
                         MSG_WAITALL,
                         (struct sockaddr *) &servaddr, (unsigned int *)&len);
    //resp = (eridan_cmd_resp_t *)buffer;
    if (n <= 0) {
        free(hdr);
        return NULL;
    }

    free(hdr);
    return resp;
}

ecm_ctrl_t
do_sysinit(void)
{
    eridan_cmd_resp_t *resp;
    struct sockaddr_in  servaddr;
    int sockfd = connect_to_server(&servaddr);

    send_request(sockfd, &servaddr, ERIDAN_CMD_SYSINIT);
    printf("Sysinit message sent.\n");
    resp = get_response(sockfd, &servaddr);
    print_response(resp);

    if (resp == NULL) {
        close(sockfd);
        return ECM_FAILURE;
    }
    free(resp);
    close(sockfd);
    return ECM_SUCCESS;
}


ecm_ctrl_t
do_getfreq(char *side, char *none)
{
    eridan_cmd_resp_t *resp;
    struct sockaddr_in  servaddr;
    eridan_cmd_hdr_t *hdr;
    eridan_cmd_req_t *req;

    if (check_input_side(side) == ECM_FAILURE)
        return ECM_FAILURE;
    int sockfd = connect_to_server(&servaddr);
    get_request(ERIDAN_CMD_GET_FREQ, 1, &hdr, &req);
    hdr->length += EC_CHAR_STR_SIZE;
    req->num_args = 1;
    strcpy(req->cmd_args, side);
    send_request_out(sockfd, &servaddr, hdr, req);

    printf("Getfreq message sent.\n");
    resp = get_response(sockfd, &servaddr);
    print_response(resp);

    if (resp == NULL) {
        free(hdr);
        free(req);
        close(sockfd);
        return ECM_FAILURE;
    }
    free(hdr);
    free(req);
    free(resp);
    close(sockfd);
    return ECM_SUCCESS;
}

ecm_ctrl_t
do_getstats(char *side, char *none)
{
    eridan_cmd_resp_t *resp;
    struct sockaddr_in  servaddr;
    eridan_cmd_hdr_t *hdr;
    eridan_cmd_req_t *req;

    if (check_input_side(side) == ECM_FAILURE)
        return ECM_FAILURE;
    int sockfd = connect_to_server(&servaddr);
    get_request(ERIDAN_CMD_GET_STATS, 1, &hdr, &req);
    hdr->length += EC_CHAR_STR_SIZE;
    req->num_args = 1;
    strcpy(req->cmd_args, side);
    send_request_out(sockfd, &servaddr, hdr, req);

    printf("Getstats message sent.\n");
    resp = get_response(sockfd, &servaddr);
    print_response(resp);

    if (resp == NULL) {
        free(hdr);
        free(req);
        close(sockfd);
        return ECM_FAILURE;
    }
    free(hdr);
    free(req);
    free(resp);
    close(sockfd);
    return ECM_SUCCESS;
}

ecm_ctrl_t
do_setfreq(char *side, char *freq)
{

    eridan_cmd_resp_t *resp;
    struct sockaddr_in  servaddr;
    eridan_cmd_hdr_t *hdr;
    eridan_cmd_req_t *req;

    if (check_input_side(side) == ECM_FAILURE)
        return ECM_FAILURE;
    int sockfd = connect_to_server(&servaddr);
    get_request(ERIDAN_CMD_SET_FREQ, 2, &hdr, &req);
    req->num_args = 2;
    hdr->length += 2*EC_CHAR_STR_SIZE;
    strcpy(req->cmd_args, side);
    strcpy(req->cmd_args+EC_CHAR_STR_SIZE, freq);
    send_request_out(sockfd, &servaddr, hdr, req);

    printf("Setfreq message sent.\n");
    printf("Req length:%d\n", hdr->length);
    resp = get_response(sockfd, &servaddr);
    print_response(resp);

    if (resp == NULL) {
        free(hdr);
        free(req);
        close(sockfd);
        return ECM_FAILURE;
    }
    free(hdr);
    free(req);
    close(sockfd);
    return ECM_SUCCESS;
}

ecm_ctrl_t
do_getpwr(char *side, char *none)
{
    eridan_cmd_resp_t *resp;
    struct sockaddr_in  servaddr;
    eridan_cmd_hdr_t *hdr;
    eridan_cmd_req_t *req;

    if (check_input_side(side) == ECM_FAILURE)
        return ECM_FAILURE;
    int sockfd = connect_to_server(&servaddr);
    get_request(ERIDAN_CMD_GET_PWR, 1, &hdr, &req);
    hdr->length += EC_CHAR_STR_SIZE;
    req->num_args = 1;
    strcpy(req->cmd_args, side);
    send_request_out(sockfd, &servaddr, hdr, req);

    printf("Getpwr message sent.\n");
    resp = get_response(sockfd, &servaddr);
    print_response(resp);

    if (resp == NULL) {
        free(hdr);
        free(req);
        close(sockfd);
        return ECM_FAILURE;
    }
    free(hdr);
    free(req);
    free(resp);
    close(sockfd);
    return ECM_SUCCESS;
}

ecm_ctrl_t
do_setpwr(char *side, char *pwr)
{
    eridan_cmd_resp_t *resp;
    struct sockaddr_in  servaddr;
    int sockfd = connect_to_server(&servaddr);
    eridan_cmd_hdr_t *hdr;
    eridan_cmd_req_t *req;

    if (check_input_side(side) == ECM_FAILURE)
        return ECM_FAILURE;
    get_request(ERIDAN_CMD_SET_PWR, 2, &hdr, &req);
    hdr->length += EC_CHAR_STR_SIZE;
    req->num_args = 2;
    strcpy(req->cmd_args, side);
    strcpy(req->cmd_args+EC_CHAR_STR_SIZE, pwr);
    send_request_out(sockfd, &servaddr, hdr, req);

    printf("Setpwr message sent.\n");
    resp = get_response(sockfd, &servaddr);
    print_response(resp);

    if (resp == NULL) {
        free(hdr);
        free(req);
        close(sockfd);
        return ECM_FAILURE;
    }
    free(hdr);
    free(req);
    free(resp);
    close(sockfd);
    return ECM_SUCCESS;
}

ecm_ctrl_t
do_getsamplerate(char *side, char *none)
{
    eridan_cmd_resp_t *resp;
    struct sockaddr_in  servaddr;
    int sockfd = connect_to_server(&servaddr);
    eridan_cmd_hdr_t *hdr;
    eridan_cmd_req_t *req;

    if (check_input_side(side) == ECM_FAILURE)
        return ECM_FAILURE;
    get_request(ERIDAN_CMD_GET_SAMPLE_RATE, 1, &hdr, &req);
    hdr->length += EC_CHAR_STR_SIZE;
    req->num_args = 1;
    strcpy(req->cmd_args, side);
    send_request_out(sockfd, &servaddr, hdr, req);

    printf("Getsamplerate message sent.\n");
    resp = get_response(sockfd, &servaddr);
    print_response(resp);

    if (resp == NULL) {
        free(hdr);
        free(req);
        close(sockfd);
        return ECM_FAILURE;
    }
    free(hdr);
    free(req);
    free(resp);
    close(sockfd);
    return ECM_SUCCESS;
}

ecm_ctrl_t
do_setsamplerate(char *side, char *srate)
{
    eridan_cmd_resp_t *resp;
    struct sockaddr_in  servaddr;
    int sockfd = connect_to_server(&servaddr);
    eridan_cmd_hdr_t *hdr;
    eridan_cmd_req_t *req;

    if (check_input_side(side) == ECM_FAILURE)
        return ECM_FAILURE;
    get_request(ERIDAN_CMD_SET_SAMPLE_RATE, 2, &hdr, &req);
    hdr->length += 2*EC_CHAR_STR_SIZE;
    req->num_args = 2;
    strcpy(req->cmd_args, side);
    strcpy(req->cmd_args+EC_CHAR_STR_SIZE, srate);
    send_request_out(sockfd, &servaddr, hdr, req);

    printf("Setsamplerate message sent.\n");
    resp = get_response(sockfd, &servaddr);
    print_response(resp);

    if (resp == NULL) {
        free(hdr);
        free(req);
        close(sockfd);
        return ECM_FAILURE;
    }
    free(hdr);
    free(req);
    free(resp);
    close(sockfd);
    return ECM_SUCCESS;
}

ecm_ctrl_t
do_getrxfreq(char *side, char *none)
{
    eridan_cmd_resp_t *resp;
    struct sockaddr_in  servaddr;
    int sockfd = connect_to_server(&servaddr);
    eridan_cmd_hdr_t *hdr;
    eridan_cmd_req_t *req;

    if (check_input_side(side) == ECM_FAILURE)
        return ECM_FAILURE;
    get_request(ERIDAN_CMD_GET_RXFREQ, 1, &hdr, &req);
    hdr->length += EC_CHAR_STR_SIZE;
    req->num_args = 1;
    strcpy(req->cmd_args, side);
    send_request_out(sockfd, &servaddr, hdr, req);

    printf("Getrxfreq message sent.\n");
    resp = get_response(sockfd, &servaddr);
    print_response(resp);

    if (resp == NULL) {
        free(hdr);
        free(req);
        close(sockfd);
        return ECM_FAILURE;
    }
    free(hdr);
    free(req);
    free(resp);
    close(sockfd);
    return ECM_SUCCESS;
}

ecm_ctrl_t
do_setrxfreq(char *side, char *rxfreq)
{
    eridan_cmd_resp_t *resp;
    struct sockaddr_in  servaddr;
    int sockfd = connect_to_server(&servaddr);
    eridan_cmd_hdr_t *hdr;
    eridan_cmd_req_t *req;

    if (check_input_side(side) == ECM_FAILURE)
        return ECM_FAILURE;
    get_request(ERIDAN_CMD_SET_RXFREQ, 2, &hdr, &req);
    hdr->length += 2 * EC_CHAR_STR_SIZE;
    req->num_args = 2;
    strcpy(req->cmd_args, side);
    strcpy(req->cmd_args+EC_CHAR_STR_SIZE, rxfreq);
    send_request_out(sockfd, &servaddr, hdr, req);

    printf("Setrxfreq message sent.\n");
    resp = get_response(sockfd, &servaddr);
    print_response(resp);

    if (resp == NULL) {
        free(hdr);
        free(req);
        close(sockfd);
        return ECM_FAILURE;
    }
    free(hdr);
    free(req);
    free(resp);
    close(sockfd);
    return ECM_SUCCESS;
}

ecm_ctrl_t
do_getrxsamplerate(char *side, char *none)
{
    eridan_cmd_resp_t *resp;
    struct sockaddr_in  servaddr;
    int sockfd = connect_to_server(&servaddr);
    eridan_cmd_hdr_t *hdr;
    eridan_cmd_req_t *req;

    if (check_input_side(side) == ECM_FAILURE)
        return ECM_FAILURE;
    get_request(ERIDAN_CMD_GET_RXSAMPLERATE, 1, &hdr, &req);
    hdr->length += EC_CHAR_STR_SIZE;
    req->num_args = 1;
    strcpy(req->cmd_args, side);
    send_request_out(sockfd, &servaddr, hdr, req);

    printf("Getrxsamplerate message sent.\n");
    resp = get_response(sockfd, &servaddr);
    print_response(resp);

    if (resp == NULL) {
        free(hdr);
        free(req);
        close(sockfd);
        return ECM_FAILURE;
    }
    free(hdr);
    free(req);
    free(resp);
    close(sockfd);
    return ECM_SUCCESS;
}

ecm_ctrl_t
do_setrxsamplerate(char *side, char *rxsrate)
{
    eridan_cmd_resp_t *resp;
    struct sockaddr_in  servaddr;
    eridan_cmd_hdr_t *hdr;
    eridan_cmd_req_t *req;

    if (check_input_side(side) == ECM_FAILURE)
        return ECM_FAILURE;
    int sockfd = connect_to_server(&servaddr);
    get_request(ERIDAN_CMD_SET_RXSAMPLERATE, 2, &hdr, &req);
    hdr->length += 2*EC_CHAR_STR_SIZE;
    req->num_args = 2;
    strcpy(req->cmd_args, side);
    strcpy(req->cmd_args+EC_CHAR_STR_SIZE, rxsrate);
    send_request_out(sockfd, &servaddr, hdr, req);

    printf("Setrxsamplerate message sent.\n");
    resp = get_response(sockfd, &servaddr);
    print_response(resp);

    if (resp == NULL) {
        free(hdr);
        free(req);
        close(sockfd);
        return ECM_FAILURE;
    }
    free(hdr);
    free(req);
    free(resp);
    close(sockfd);
    return ECM_SUCCESS;
}

ecm_ctrl_t
do_getrxgains(char *side, char *none)
{
    eridan_cmd_resp_t *resp;
    struct sockaddr_in  servaddr;
    eridan_cmd_hdr_t *hdr;
    eridan_cmd_req_t *req;

    if (check_input_side(side) == ECM_FAILURE)
        return ECM_FAILURE;
    int sockfd = connect_to_server(&servaddr);
    get_request(ERIDAN_CMD_GET_RXGAINS, 1, &hdr, &req);
    hdr->length += EC_CHAR_STR_SIZE;
    req->num_args = 1;
    strcpy(req->cmd_args, side);
    send_request_out(sockfd, &servaddr, hdr, req);

    printf("Getrxgains message sent.\n");
    resp = get_response(sockfd, &servaddr);
    print_response(resp);

    if (resp == NULL) {
        free(hdr);
        free(req);
        close(sockfd);
        return ECM_FAILURE;
    }
    free(hdr);
    free(req);
    free(resp);
    close(sockfd);
    return ECM_SUCCESS;
}

ecm_ctrl_t
do_setrxgains(char *side, char *rxgains)
{
    eridan_cmd_resp_t *resp;
    struct sockaddr_in  servaddr;
    eridan_cmd_hdr_t *hdr;
    eridan_cmd_req_t *req;

    if (check_input_side(side) == ECM_FAILURE)
        return ECM_FAILURE;
    int sockfd = connect_to_server(&servaddr);
    get_request(ERIDAN_CMD_SET_RXGAINS, 2, &hdr, &req);
    hdr->length += 2 * EC_CHAR_STR_SIZE;
    req->num_args = 2;
    strcpy(req->cmd_args, side);
    strcpy(req->cmd_args+EC_CHAR_STR_SIZE, rxgains);
    send_request_out(sockfd, &servaddr, hdr, req);

    printf("Setrxgains message sent.\n");
    resp = get_response(sockfd, &servaddr);
    print_response(resp);

    if (resp == NULL) {
        free(hdr);
        free(req);
        close(sockfd);
        return ECM_FAILURE;
    }
    free(hdr);
    free(req);
    free(resp);
    close(sockfd);
    return ECM_SUCCESS;
}

ecm_ctrl_t
do_sysoff(void)
{
    eridan_cmd_resp_t *resp;
    struct sockaddr_in  servaddr;
    int sockfd = connect_to_server(&servaddr);

    send_request(sockfd, &servaddr, ERIDAN_CMD_SYSOFF);
    printf("Sysoff message sent.\n");
    resp = get_response(sockfd, &servaddr);
    print_response(resp);

    if (resp == NULL) {
        return ECM_FAILURE;
    }
    free(resp);
    close(sockfd);
    return ECM_SUCCESS;
}

ecm_ctrl_t
do_startscp(void)
{
    eridan_cmd_resp_t *resp;
    struct sockaddr_in  servaddr;
    int sockfd = connect_to_server(&servaddr);
    eridan_cmd_hdr_t *hdr;
    eridan_cmd_req_t *req;

    get_request(ERIDAN_CMD_START_SCP, 6, &hdr, &req);
    hdr->length += 6 * EC_CHAR_STR_SIZE;
    req->num_args = 6;
    strcpy(req->cmd_args,                    "user1");
    strcpy(req->cmd_args+EC_CHAR_STR_SIZE,   "10.1.32.34");
    strcpy(req->cmd_args+2*EC_CHAR_STR_SIZE, "/opt/img1.gz");
    strcpy(req->cmd_args+3*EC_CHAR_STR_SIZE, "user2");
    strcpy(req->cmd_args+4*EC_CHAR_STR_SIZE, "10.1.32.90");
    strcpy(req->cmd_args+5*EC_CHAR_STR_SIZE, "/root/img2.gz");
    send_request_out(sockfd, &servaddr, hdr, req);

    printf("StartSCP message sent.\n");
    resp = get_response(sockfd, &servaddr);
    print_response(resp);

    if (resp == NULL) {
        free(hdr);
        free(req);
        close(sockfd);
        return ECM_FAILURE;
    }
    free(hdr);
    free(req);
    free(resp);
    close(sockfd);
    return ECM_SUCCESS;
}

ecm_ctrl_t
do_prepscp(void)
{
    eridan_cmd_resp_t *resp;
    struct sockaddr_in  servaddr;
    int sockfd = connect_to_server(&servaddr);

    send_request(sockfd, &servaddr, ERIDAN_CMD_PREP_SCP);
    printf("Hello message sent.\n");
    resp = get_response(sockfd, &servaddr);
    print_response(resp);

    free(resp);
    close(sockfd);
    return ECM_SUCCESS;
}

ecm_ctrl_t
do_resetnow(void)
{
    eridan_cmd_resp_t *resp;
    struct sockaddr_in  servaddr;
    int sockfd = connect_to_server(&servaddr);
    eridan_cmd_hdr_t *hdr;
    eridan_cmd_req_t *req;

    get_request(ERIDAN_CMD_RESET_NOW, 0, &hdr, &req);
    req->num_args = 0;
    send_request_out(sockfd, &servaddr, hdr, req);

    printf("Resetnow message sent.\n");
    resp = get_response(sockfd, &servaddr);
    print_response(resp);

    if (resp == NULL) {
        free(hdr);
        free(req);
        close(sockfd);
        return ECM_FAILURE;
    }
    free(hdr);
    free(req);
    free(resp);
    close(sockfd);
    return ECM_SUCCESS;
}

ecm_ctrl_t
do_resetdone(void)
{
    eridan_cmd_resp_t *resp;
    struct sockaddr_in  servaddr;
    int sockfd = connect_to_server(&servaddr);
    eridan_cmd_hdr_t *hdr;
    eridan_cmd_req_t *req;

    get_request(ERIDAN_CMD_SET_RXGAINS, 0, &hdr, &req);
    req->num_args = 0;
    send_request_out(sockfd, &servaddr, hdr, req);

    printf("Resetdone message sent.\n");
    resp = get_response(sockfd, &servaddr);
    print_response(resp);

    if (resp == NULL) {
        free(hdr);
        free(req);
        close(sockfd);
        return ECM_FAILURE;
    }
    free(hdr);
    free(req);
    free(resp);
    close(sockfd);
    return ECM_SUCCESS;
}

ecm_ctrl_t
do_sendupdates(void)
{
    eridan_cmd_resp_t *resp;
    struct sockaddr_in  servaddr;
    int sockfd = connect_to_server(&servaddr);
    eridan_cmd_hdr_t *hdr;
    eridan_cmd_req_t *req;

    get_request(ERIDAN_CMD_SEND_UPDATES, 2, &hdr, &req);
    hdr->length += 2 * EC_CHAR_STR_SIZE;
    req->num_args = 2;
    strcpy(req->cmd_args,                    "3453");
    strcpy(req->cmd_args+EC_CHAR_STR_SIZE,   "10.1.32.34");
    send_request_out(sockfd, &servaddr, hdr, req);

    printf("Sendupdates message sent.\n");
    resp = get_response(sockfd, &servaddr);
    print_response(resp);

    if (resp == NULL) {
        free(hdr);
        free(req);
        close(sockfd);
        return ECM_FAILURE;
    }
    free(hdr);
    free(req);
    free(resp);
    close(sockfd);
    return ECM_SUCCESS;
}

ecm_ctrl_t
do_checkupdates(void)
{
    eridan_cmd_resp_t *resp;
    struct sockaddr_in  servaddr;
    int sockfd = connect_to_server(&servaddr);
    eridan_cmd_hdr_t *hdr;
    eridan_cmd_req_t *req;

    get_request(ERIDAN_CMD_CHECK_UPDATES, 2, &hdr, &req);
    hdr->length += 2 * EC_CHAR_STR_SIZE;
    req->num_args = 2;
    strcpy(req->cmd_args,                    "3453");
    strcpy(req->cmd_args+EC_CHAR_STR_SIZE,   "10.1.32.34");
    send_request_out(sockfd, &servaddr, hdr, req);

    printf("Checkupdates message sent.\n");
    resp = get_response(sockfd, &servaddr);
    print_response(resp);

    if (resp == NULL) {
        free(hdr);
        free(req);
        close(sockfd);
        return ECM_FAILURE;
    }
    free(hdr);
    free(req);
    free(resp);
    close(sockfd);
    return ECM_SUCCESS;
}

ecm_ctrl_t
do_getversion(void)
{
    eridan_cmd_resp_t *resp;
    struct sockaddr_in  servaddr;
    int sockfd = connect_to_server(&servaddr);

    send_request(sockfd, &servaddr, ERIDAN_CMD_GET_VERSION);
    printf("Getversion message sent.\n");
    resp = get_response(sockfd, &servaddr);
    print_response(resp);

    if (resp == NULL) {
        close(sockfd);
        return ECM_FAILURE;
    }
    free(resp);
    close(sockfd);
    return ECM_SUCCESS;
}

typedef struct ecmctl_options_s {
    eridan_cmd_id_t cmdid;
    const char      *optname;
    const char      *description;
    const char      *help;
} ecmctl_options_t;

ecmctl_options_t ecmctl_options[] = {
    {.optname= "verbose",         .help="Run Verbose",                .description="Describe me", },
    {.optname= "help",            .help="Print Help",                 .description="Describe me", },
    {.optname= "sysinit",         .help="Initialize Eridan Radio",    .description="Describe me", .cmdid=ERIDAN_CMD_SYSINIT},
    {.optname= "getfreq",         .help="Get TX Frequency",           .description="Describe me", .cmdid=ERIDAN_CMD_GET_FREQ},
    {.optname= "getstats",        .help="Get Radio Stats",            .description="Describe me", .cmdid=ERIDAN_CMD_GET_STATS},
    {.optname= "setfreq",         .help="Set TX Frequency",           .description="Describe me", .cmdid=ERIDAN_CMD_SET_FREQ},
    {.optname= "getpower",        .help="Get TX Power",               .description="Describe me", .cmdid=ERIDAN_CMD_GET_PWR},
    {.optname= "setpower",        .help="Set TX Power",               .description="Describe me", .cmdid=ERIDAN_CMD_SET_PWR},
    {.optname= "getsamplerate",   .help="Get TX Samplerate",          .description="Describe me", .cmdid=ERIDAN_CMD_GET_SAMPLE_RATE},
    {.optname= "getrxfreq",       .help="Get RX Frequency",           .description="Describe me", .cmdid=ERIDAN_CMD_GET_RXFREQ},
    {.optname= "setrxfreq",       .help="Set RX Frequency",           .description="Describe me", .cmdid=ERIDAN_CMD_SET_RXFREQ},
    {.optname= "getrxsamplerate", .help="Get RX Samplerate",          .description="Describe me", .cmdid=ERIDAN_CMD_GET_RXSAMPLERATE},
    {.optname= "setrxsamplerate", .help="Set RX Samplerate",          .description="Describe me", .cmdid=ERIDAN_CMD_SET_RXSAMPLERATE},
    {.optname= "getrxgains",      .help="Get RX Gains",               .description="Describe me", .cmdid=ERIDAN_CMD_GET_RXGAINS},
    {.optname= "setrxgains",      .help="Set RX Gains",               .description="Describe me", .cmdid=ERIDAN_CMD_SET_RXGAINS},
    {.optname= "sysoff",          .help="Turnoff Eridan Radio",       .description="Describe me", .cmdid=ERIDAN_CMD_SYSOFF},
    {.optname= "startscp",        .help="Start SCP",                  .description="Describe me", .cmdid=ERIDAN_CMD_START_SCP},
    {.optname= "prepscp",         .help="Prepare for SCP",            .description="Describe me", .cmdid=ERIDAN_CMD_PREP_SCP},
    {.optname= "resetnow",        .help="Reset Eridan unit Now",      .description="Describe me", .cmdid=ERIDAN_CMD_RESET_NOW},
    {.optname= "resetdone",       .help="Reset Completed",            .description="Describe me", .cmdid=ERIDAN_CMD_RESET_DONE},
    {.optname= "sendupdates",     .help="Server to send updates to",  .description="Describe me", .cmdid=ERIDAN_CMD_SEND_UPDATES},
    {.optname= "checkupdates",    .help="Check update socket",        .description="Describe me", .cmdid=ERIDAN_CMD_CHECK_UPDATES},
    {.optname= "getversion",      .help="Get Eridan C plain version", .description="Describe me", .cmdid=ERIDAN_CMD_GET_VERSION},
};

#define NUM_ECM_OPTIONS (int)(sizeof(ecmctl_options)/sizeof(ecmctl_options_t))
#define BUFSIZE         (256)

void
Usage(void)
{
    printf("Ecmctl take the following options:\n");
    for (int i = 0; i < NUM_ECM_OPTIONS; i++) {
        printf("\t%-10s\t -\t %s\n", ecmctl_options[i].optname, ecmctl_options[i].help);
    }
    return;
}

ecm_ctrl_t
parse_args(int argc, char *argv[])
{
    int c;
    static int verbose_flag;

    /* Ref : https://www.gnu.org/software/libc/manual/html_node/Getopt-Long-Options.html */
    while (1)
    {
        static struct option long_options[] =
        {
            {"verbose",         no_argument,       &verbose_flag, 1},
            {"help",            no_argument,       0, 'h'},
            {"sysinit",         no_argument,       0, ERIDAN_CMD_SYSINIT},
            {"getfreq",         required_argument, 0, ERIDAN_CMD_GET_FREQ},
            {"getstats",        required_argument, 0, ERIDAN_CMD_GET_STATS},
            {"setfreq",         required_argument, 0, ERIDAN_CMD_SET_FREQ},
            {"getpower",        required_argument, 0, ERIDAN_CMD_GET_PWR},
            {"setpower",        required_argument, 0, ERIDAN_CMD_SET_PWR},
            {"getsamplerate",   required_argument, 0, ERIDAN_CMD_GET_SAMPLE_RATE},
            {"setsamplerate",   required_argument, 0, ERIDAN_CMD_SET_SAMPLE_RATE},
            {"getrxfreq",       required_argument, 0, ERIDAN_CMD_GET_RXFREQ},
            {"setrxfreq",       required_argument, 0, ERIDAN_CMD_SET_RXFREQ},
            {"getrxsamplerate", required_argument, 0, ERIDAN_CMD_GET_RXSAMPLERATE},
            {"setrxsamplerate", required_argument, 0, ERIDAN_CMD_SET_RXSAMPLERATE},
            {"getrxgains",      required_argument, 0, ERIDAN_CMD_GET_RXGAINS},
            {"setrxgains",      required_argument, 0, ERIDAN_CMD_SET_RXGAINS},
            {"sysoff",          no_argument,       0, ERIDAN_CMD_SYSOFF},
            {"startscp",        no_argument,       0, ERIDAN_CMD_START_SCP},
            {"prepscp",         no_argument,       0, ERIDAN_CMD_PREP_SCP},
            {"resetnow",        no_argument,       0, ERIDAN_CMD_RESET_NOW},
            {"resetdone",       no_argument,       0, ERIDAN_CMD_RESET_DONE},
            {"sendupdates",     no_argument,       0, ERIDAN_CMD_SEND_UPDATES},
            {"checkupdates",    no_argument,       0, ERIDAN_CMD_CHECK_UPDATES},
            {"getversion",      no_argument,       0, ERIDAN_CMD_GET_VERSION},
            {0, 0, 0, 0}
        };
        /* getopt_long stores the option index here. */
        int option_index = 0;
        char arg1[BUFSIZE];
        char arg2[BUFSIZE];

        c = getopt_long (argc, argv, "h",
                         long_options, &option_index);

        /* Detect the end of the options. */
        if (c == -1)
            break;

        switch ((eridan_cmd_id_t)c)
        {
        case ERIDAN_CMD_SYSINIT:
            puts ("sysinit\n");
            return do_sysinit();
            break;

        case ERIDAN_CMD_GET_FREQ:
            memset(arg1, 0, sizeof(arg1));
            memset(arg2, 0, sizeof(arg2));
            snprintf(arg1, BUFSIZE, "%s", optarg);
            printf("doing getfreq%s\n", optarg);
            return do_getfreq(arg1, arg2);
            break;

        case ERIDAN_CMD_GET_STATS:
            memset(arg1, 0, sizeof(arg1));
            memset(arg2, 0, sizeof(arg2));
            snprintf(arg1, BUFSIZE, "%s", optarg);
            printf("doing getstats%s\n", optarg);
            return do_getstats(arg1, arg2);
            break;       

        case ERIDAN_CMD_SET_FREQ:
            memset(arg1, 0, sizeof(arg1));
            memset(arg2, 0, sizeof(arg2));
            for (int i = 0; optind < argc && *argv[optind] != '-'; optind++) {
                snprintf(arg2, BUFSIZE, "%s", argv[optind]);
            }
            snprintf(arg1, BUFSIZE, "%s", optarg);
            printf("doing setfreq %s %s\n", arg1, arg2);
            return do_setfreq(arg1, arg2);
            break;

        case ERIDAN_CMD_GET_PWR:
            memset(arg1, 0, sizeof(arg1));
            memset(arg2, 0, sizeof(arg2));
            snprintf(arg1, BUFSIZE, "%s", optarg);
            printf("doing getpwr %s\n", optarg);
            return do_getpwr(arg1, arg2);
            break;

        case ERIDAN_CMD_SET_PWR:
            memset(arg1, 0, sizeof(arg1));
            memset(arg2, 0, sizeof(arg2));
            for (int i = 0; optind < argc && *argv[optind] != '-'; optind++) {
                snprintf(arg2, BUFSIZE, "%s", argv[optind]);
            }
            snprintf(arg1, BUFSIZE, "%s", optarg);
            printf("doing setpwr %s\n", optarg);
            return do_setpwr(arg1, arg2);
            break;

        case ERIDAN_CMD_GET_SAMPLE_RATE:
            memset(arg1, 0, sizeof(arg1));
            memset(arg2, 0, sizeof(arg2));
            snprintf(arg1, BUFSIZE, "%s", optarg);
            printf("doing getsamplerate %s\n"), optarg;
            return do_getsamplerate(arg1, arg2);
            break;

        case ERIDAN_CMD_SET_SAMPLE_RATE:
            memset(arg1, 0, sizeof(arg1));
            memset(arg2, 0, sizeof(arg2));
            for (int i = 0; optind < argc && *argv[optind] != '-'; optind++) {
                snprintf(arg2, BUFSIZE, "%s", argv[optind]);
            }
            snprintf(arg1, BUFSIZE, "%s", optarg);
            printf("doing setsamplerate %s\n", optarg);
            return do_setsamplerate(arg1, arg2);
            break;

        case ERIDAN_CMD_GET_RXFREQ:
            memset(arg1, 0, sizeof(arg1));
            memset(arg2, 0, sizeof(arg2));
            snprintf(arg1, BUFSIZE, "%s", optarg);
            printf("doing getrxfreq %s\n", optarg);
            return do_getrxfreq(arg1, arg2);
            break;

        case ERIDAN_CMD_SET_RXFREQ:
            memset(arg1, 0, sizeof(arg1));
            memset(arg2, 0, sizeof(arg2));
            for (int i = 0; optind < argc && *argv[optind] != '-'; optind++) {
                snprintf(arg2, BUFSIZE, "%s", argv[optind]);
            }
            snprintf(arg1, BUFSIZE, "%s", optarg);
            printf("doing setrxfreq %s\n", optarg);
            return do_setrxfreq(arg1, arg2);
            break;

        case ERIDAN_CMD_GET_RXSAMPLERATE:
            memset(arg1, BUFSIZE, sizeof(arg1));
            memset(arg2, 0, sizeof(arg2));
            snprintf(arg1, BUFSIZE, "%s", optarg);
            printf("doing getrxsamplerate %s\n", optarg);
            return do_getrxsamplerate(arg1, arg2);
            break;

        case ERIDAN_CMD_SET_RXSAMPLERATE:
            memset(arg1, BUFSIZE, sizeof(arg1));
            memset(arg2, 0, sizeof(arg2));
            for (int i = 0; optind < argc && *argv[optind] != '-'; optind++) {
                snprintf(arg2, BUFSIZE, "%s", argv[optind]);
            }
            snprintf(arg1, BUFSIZE, "%s", optarg);
            printf("doing setrxsamplerate %s\n", optarg);
            return do_setrxsamplerate(arg1, arg2);
            break;

        case ERIDAN_CMD_GET_RXGAINS:
            memset(arg1, BUFSIZE, sizeof(arg1));
            memset(arg2, 0, sizeof(arg2));
            snprintf(arg1, BUFSIZE, "%s", optarg);
            printf("doing getrxgains %s\n", optarg);
            return do_getrxgains(arg1, arg2);
            break;

        case ERIDAN_CMD_SET_RXGAINS:
            memset(arg1, BUFSIZE, sizeof(arg1));
            memset(arg2, 0, sizeof(arg2));
            for (int i = 0; optind < argc && *argv[optind] != '-'; optind++) {
                snprintf(arg2, BUFSIZE, "%s", argv[optind]);
            }
            snprintf(arg1, BUFSIZE, "%s", optarg);
            printf("doing setrxgains %s\n", optarg);
            return do_setrxgains(arg1, arg2);
            break;

        case ERIDAN_CMD_SYSOFF:
            printf("doing sysoff %s\n", optarg);
            return do_sysoff();
            break;

        case ERIDAN_CMD_START_SCP:
            printf("doing startscp %s\n", optarg);
            return do_startscp();
            break;

        case ERIDAN_CMD_PREP_SCP:
            printf("doing prepscp %s\n", optarg);
            return do_prepscp();
            break;

        case ERIDAN_CMD_RESET_NOW:
            printf("doing resetnow %s\n", optarg);
            return do_resetnow();
            break;

        case ERIDAN_CMD_RESET_DONE:
            printf("doing resetdone %s\n", optarg);
            return do_resetdone();
            break;

        case ERIDAN_CMD_SEND_UPDATES:
            printf("doing sendupdates %s\n", optarg);
            return do_sendupdates();
            break;

        case ERIDAN_CMD_CHECK_UPDATES:
            printf("doing checkupdates %s\n", optarg);
            return do_checkupdates();
            break;

        case ERIDAN_CMD_GET_VERSION:
            printf("doing getversion %s\n", optarg);
            return do_getversion();
            break;

        case 'h':
        case '?':
            Usage();
            return ECM_FAILURE;

        default:
            abort ();
        }
    }

    if (verbose_flag)
        puts ("verbose flag is set");

    /* Print any remaining command line arguments (not options). */
    if (optind < argc)
    {
        printf ("non-option ARGV-elements: ");
        while (optind < argc)
            printf ("%s ", argv[optind++]);
        putchar ('\n');
    }
    return ECM_SUCCESS;
}

int main(int argc, char *argv[])
{

    if (parse_args(argc, argv) == ECM_FAILURE) {
        printf("Command failed!\n");
        exit(EXIT_FAILURE);
    }

    return 0; 
}