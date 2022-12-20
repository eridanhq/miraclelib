// Client side implementation of UDP client-server model 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>
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

    return sockfd;
}

static inline void
print_response(eridan_cmd_resp_t *resp)
{
    printf("Req no: %x\n", resp->reqid);
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
    sendto(sockfd,  (const char *)req, sizeof(eridan_cmd_req_t),
                    MSG_CONFIRM,
                    (const struct sockaddr *) servaddr, sizeof(*servaddr));

    return ECM_SUCCESS;
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
    n = recvfrom(sockfd, hdr, sizeof(eridan_cmd_hdr_t),
                         MSG_WAITALL,
                         (struct sockaddr *) &servaddr, (unsigned int *)&len);
    printf("Recved %d bytes from server\n", n);
    //reply = (eridan_cmd_hdr_t *)buffer;
    resp = malloc(hdr->length);
    printf("Server : %x\n", hdr->cookie);
    printf("Size: %d\n", hdr->length);
    n = recvfrom(sockfd, (char *)resp, hdr->length+35, /* XXX */
                         MSG_WAITALL,
                         (struct sockaddr *) &servaddr, (unsigned int *)&len);
    //resp = (eridan_cmd_resp_t *)buffer;

    free(hdr);
    return resp;
}

void
do_sysinit(void)
{
    eridan_cmd_resp_t *resp;
    struct sockaddr_in  servaddr;
    int sockfd = connect_to_server(&servaddr);

    send_request(sockfd, &servaddr, ERIDAN_CMD_SYSINIT);
    printf("Sysinit message sent.\n");
    resp = get_response(sockfd, &servaddr);
    print_response(resp);

    free(resp);
    close(sockfd);
    return;
}


void
do_getfreq(void)
{
    eridan_cmd_resp_t *resp;
    struct sockaddr_in  servaddr;
    int sockfd = connect_to_server(&servaddr);
    eridan_cmd_hdr_t *hdr;
    eridan_cmd_req_t *req;

    get_request(ERIDAN_CMD_GET_FREQ, 1, &hdr, &req);
    req->num_args = 1;
    strcpy(req->cmd_args, "TRX1");
    send_request_out(sockfd, &servaddr, hdr, req);

    printf("Getfreq message sent.\n");
    resp = get_response(sockfd, &servaddr);
    print_response(resp);

    free(hdr);
    free(req);
    free(resp);
    close(sockfd);
    return;
}

void
do_getstats(void)
{
    eridan_cmd_resp_t *resp;
    struct sockaddr_in  servaddr;
    int sockfd = connect_to_server(&servaddr);
    eridan_cmd_hdr_t *hdr;
    eridan_cmd_req_t *req;

    get_request(ERIDAN_CMD_GET_STATS, 1, &hdr, &req);
    req->num_args = 1;
    strcpy(req->cmd_args, "TRX1");
    send_request_out(sockfd, &servaddr, hdr, req);

    printf("Getstats message sent.\n");
    resp = get_response(sockfd, &servaddr);
    print_response(resp);

    free(hdr);
    free(req);
    free(resp);
    close(sockfd);
    return;
}

void
do_setfreq(void)
{

    eridan_cmd_resp_t *resp;
    struct sockaddr_in  servaddr;
    int sockfd = connect_to_server(&servaddr);
    eridan_cmd_hdr_t *hdr;
    eridan_cmd_req_t *req;

    get_request(ERIDAN_CMD_SET_FREQ, 2, &hdr, &req);
    req->num_args = 2;
    strcpy(req->cmd_args, "TRX1");
    strcpy(req->cmd_args+EC_CHAR_STR_SIZE, "3.2e4");
    send_request_out(sockfd, &servaddr, hdr, req);

    printf("Setfreq message sent.\n");
    resp = get_response(sockfd, &servaddr);
    print_response(resp);

    free(hdr);
    free(req);
    close(sockfd);
    return;
}

void
do_getpwr(void)
{
    eridan_cmd_resp_t *resp;
    struct sockaddr_in  servaddr;
    int sockfd = connect_to_server(&servaddr);
    eridan_cmd_hdr_t *hdr;
    eridan_cmd_req_t *req;

    get_request(ERIDAN_CMD_GET_PWR, 1, &hdr, &req);
    req->num_args = 1;
    strcpy(req->cmd_args, "TRX1");
    send_request_out(sockfd, &servaddr, hdr, req);

    printf("Getpwr message sent.\n");
    resp = get_response(sockfd, &servaddr);
    print_response(resp);

    free(hdr);
    free(req);
    free(resp);
    close(sockfd);
    return;
}

void
do_setpwr(void)
{
    eridan_cmd_resp_t *resp;
    struct sockaddr_in  servaddr;
    int sockfd = connect_to_server(&servaddr);
    eridan_cmd_hdr_t *hdr;
    eridan_cmd_req_t *req;

    get_request(ERIDAN_CMD_SET_PWR, 2, &hdr, &req);
    req->num_args = 2;
    strcpy(req->cmd_args, "TRX1");
    strcpy(req->cmd_args+EC_CHAR_STR_SIZE, "1000");
    send_request_out(sockfd, &servaddr, hdr, req);

    printf("Setpwr message sent.\n");
    resp = get_response(sockfd, &servaddr);
    print_response(resp);

    free(hdr);
    free(req);
    free(resp);
    close(sockfd);
    return;
}

void
do_getsamplerate(void)
{
    eridan_cmd_resp_t *resp;
    struct sockaddr_in  servaddr;
    int sockfd = connect_to_server(&servaddr);
    eridan_cmd_hdr_t *hdr;
    eridan_cmd_req_t *req;

    get_request(ERIDAN_CMD_GET_SAMPLE_RATE, 1, &hdr, &req);
    req->num_args = 1;
    strcpy(req->cmd_args, "TRX1");
    send_request_out(sockfd, &servaddr, hdr, req);

    printf("Getsamplerate message sent.\n");
    resp = get_response(sockfd, &servaddr);
    print_response(resp);

    free(hdr);
    free(req);
    free(resp);
    close(sockfd);
    return;
}

void
do_setsamplerate(void)
{
    eridan_cmd_resp_t *resp;
    struct sockaddr_in  servaddr;
    int sockfd = connect_to_server(&servaddr);
    eridan_cmd_hdr_t *hdr;
    eridan_cmd_req_t *req;

    get_request(ERIDAN_CMD_SET_SAMPLE_RATE, 2, &hdr, &req);
    req->num_args = 2;
    strcpy(req->cmd_args, "TRX1");
    strcpy(req->cmd_args+EC_CHAR_STR_SIZE, "3.2e4");
    send_request_out(sockfd, &servaddr, hdr, req);

    printf("Setsamplerate message sent.\n");
    resp = get_response(sockfd, &servaddr);
    print_response(resp);

    free(hdr);
    free(req);
    free(resp);
    close(sockfd);
    return;
}

void
do_getrxfreq(void)
{
    eridan_cmd_resp_t *resp;
    struct sockaddr_in  servaddr;
    int sockfd = connect_to_server(&servaddr);
    eridan_cmd_hdr_t *hdr;
    eridan_cmd_req_t *req;

    get_request(ERIDAN_CMD_GET_RXFREQ, 1, &hdr, &req);
    req->num_args = 1;
    strcpy(req->cmd_args, "TRX1");
    send_request_out(sockfd, &servaddr, hdr, req);

    printf("Getrxfreq message sent.\n");
    resp = get_response(sockfd, &servaddr);
    print_response(resp);

    free(hdr);
    free(req);
    free(resp);
    close(sockfd);
    return;
}

void
do_setrxfreq(void)
{
    eridan_cmd_resp_t *resp;
    struct sockaddr_in  servaddr;
    int sockfd = connect_to_server(&servaddr);
    eridan_cmd_hdr_t *hdr;
    eridan_cmd_req_t *req;

    get_request(ERIDAN_CMD_SET_RXFREQ, 2, &hdr, &req);
    req->num_args = 2;
    strcpy(req->cmd_args, "TRX1");
    strcpy(req->cmd_args+EC_CHAR_STR_SIZE, "3.2e4");
    send_request_out(sockfd, &servaddr, hdr, req);

    printf("Setrxfreq message sent.\n");
    resp = get_response(sockfd, &servaddr);
    print_response(resp);

    free(hdr);
    free(req);
    free(resp);
    close(sockfd);
    return;
}

void
do_getrxsamplerate(void)
{
    eridan_cmd_resp_t *resp;
    struct sockaddr_in  servaddr;
    int sockfd = connect_to_server(&servaddr);
    eridan_cmd_hdr_t *hdr;
    eridan_cmd_req_t *req;

    get_request(ERIDAN_CMD_GET_RXSAMPLERATE, 1, &hdr, &req);
    req->num_args = 1;
    strcpy(req->cmd_args, "TRX1");
    send_request_out(sockfd, &servaddr, hdr, req);

    printf("Getrxsamplerate message sent.\n");
    resp = get_response(sockfd, &servaddr);
    print_response(resp);

    free(hdr);
    free(req);
    free(resp);
    close(sockfd);
    return;
}

void
do_setrxsamplerate(void)
{
    eridan_cmd_resp_t *resp;
    struct sockaddr_in  servaddr;
    int sockfd = connect_to_server(&servaddr);
    eridan_cmd_hdr_t *hdr;
    eridan_cmd_req_t *req;

    get_request(ERIDAN_CMD_SET_RXSAMPLERATE, 2, &hdr, &req);
    req->num_args = 2;
    strcpy(req->cmd_args, "TRX1");
    strcpy(req->cmd_args+EC_CHAR_STR_SIZE, "3.2e4");
    send_request_out(sockfd, &servaddr, hdr, req);

    printf("Setrxsamplerate message sent.\n");
    resp = get_response(sockfd, &servaddr);
    print_response(resp);

    free(hdr);
    free(req);
    free(resp);
    close(sockfd);
    return;
}

void
do_getrxgains(void)
{
    eridan_cmd_resp_t *resp;
    struct sockaddr_in  servaddr;
    int sockfd = connect_to_server(&servaddr);
    eridan_cmd_hdr_t *hdr;
    eridan_cmd_req_t *req;

    get_request(ERIDAN_CMD_GET_RXGAINS, 1, &hdr, &req);
    req->num_args = 1;
    strcpy(req->cmd_args, "TRX1");
    send_request_out(sockfd, &servaddr, hdr, req);

    printf("Getrxgains message sent.\n");
    resp = get_response(sockfd, &servaddr);
    print_response(resp);

    free(hdr);
    free(req);
    free(resp);
    close(sockfd);
    return;
}

void
do_setrxgains(void)
{
    eridan_cmd_resp_t *resp;
    struct sockaddr_in  servaddr;
    int sockfd = connect_to_server(&servaddr);
    eridan_cmd_hdr_t *hdr;
    eridan_cmd_req_t *req;

    get_request(ERIDAN_CMD_SET_RXGAINS, 2, &hdr, &req);
    req->num_args = 2;
    strcpy(req->cmd_args, "TRX1");
    strcpy(req->cmd_args+EC_CHAR_STR_SIZE, "3.2e4");
    send_request_out(sockfd, &servaddr, hdr, req);

    printf("Setrxgains message sent.\n");
    resp = get_response(sockfd, &servaddr);
    print_response(resp);

    free(hdr);
    free(req);
    free(resp);
    close(sockfd);
    return;
}

void
do_sysoff(void)
{
    eridan_cmd_resp_t *resp;
    struct sockaddr_in  servaddr;
    int sockfd = connect_to_server(&servaddr);

    send_request(sockfd, &servaddr, ERIDAN_CMD_SYSOFF);
    printf("Sysoff message sent.\n");
    resp = get_response(sockfd, &servaddr);
    print_response(resp);

    free(resp);
    close(sockfd);
    return;
}

void
do_startscp(void)
{
    eridan_cmd_resp_t *resp;
    struct sockaddr_in  servaddr;
    int sockfd = connect_to_server(&servaddr);

    send_request(sockfd, &servaddr, ERIDAN_CMD_START_SCP);
    printf("Hello message sent.\n");
    resp = get_response(sockfd, &servaddr);
    print_response(resp);

    free(resp);
    close(sockfd);
    return;
}

void
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
    return;
}

void
do_resetnow(void)
{
    eridan_cmd_resp_t *resp;
    struct sockaddr_in  servaddr;
    int sockfd = connect_to_server(&servaddr);

    send_request(sockfd, &servaddr, ERIDAN_CMD_RESET_NOW);
    printf("Hello message sent.\n");
    resp = get_response(sockfd, &servaddr);
    print_response(resp);

    free(resp);
    close(sockfd);
    return;
}

void
do_resetdone(void)
{
    eridan_cmd_resp_t *resp;
    struct sockaddr_in  servaddr;
    int sockfd = connect_to_server(&servaddr);

    send_request(sockfd, &servaddr, ERIDAN_CMD_RESET_DONE);
    printf("Hello message sent.\n");
    resp = get_response(sockfd, &servaddr);
    print_response(resp);

    free(resp);
    close(sockfd);
    return;
}

void
do_sendupdates(void)
{
    eridan_cmd_resp_t *resp;
    struct sockaddr_in  servaddr;
    int sockfd = connect_to_server(&servaddr);

    send_request(sockfd, &servaddr, ERIDAN_CMD_SEND_UPDATES);
    printf("Hello message sent.\n");
    resp = get_response(sockfd, &servaddr);
    print_response(resp);

    free(resp);
    close(sockfd);
    return;
}

void
do_checkupdates(void)
{
    eridan_cmd_resp_t *resp;
    struct sockaddr_in  servaddr;
    int sockfd = connect_to_server(&servaddr);

    send_request(sockfd, &servaddr, ERIDAN_CMD_CHECK_UPDATES);
    printf("Hello message sent.\n");
    resp = get_response(sockfd, &servaddr);
    print_response(resp);

    free(resp);
    close(sockfd);
    return;
}

void
do_getversion(void)
{
    eridan_cmd_resp_t *resp;
    struct sockaddr_in  servaddr;
    int sockfd = connect_to_server(&servaddr);

    send_request(sockfd, &servaddr, ERIDAN_CMD_GET_VERSION);
    printf("Hello message sent.\n");
    resp = get_response(sockfd, &servaddr);
    print_response(resp);

    free(resp);
    close(sockfd);
    return;
}

typedef struct ecmctl_options_s {
    char            *optname;
    eridan_cmd_id_t cmdid;
    char            *description;
    char            *help;
} ecmctl_options_t;

ecmctl_options_t ecmctl_options[] = {
    {.optname= "verbose",         .help="Run Verbose",                .description="Describe me", },
    {.optname= "help",            .help="Print Help",                 .description="Describe me", },
    {.optname= "sysinit",         .help="Initialize Eridan Radio",    .description="Describe me", .cmdid=ERIDAN_CMD_SYSINIT},
    {.optname= "getfreq",         .help="Get TX Frequency",           .description="Describe me", .cmdid=ERIDAN_CMD_GET_FREQ},
    {.optname= "getstats",        .help="Get Radio Stats",            .description="Describe me", .cmdid=ERIDAN_CMD_GET_STATS},
    {.optname= "setfreq",         .help="Set TX Frequency",           .description="Describe me", .cmdid=ERIDAN_CMD_SET_FREQ},
    {.optname= "getpwr",          .help="Get TX Power",               .description="Describe me", .cmdid=ERIDAN_CMD_GET_PWR},
    {.optname= "setpwr",          .help="Set TX Power",               .description="Describe me", .cmdid=ERIDAN_CMD_SET_PWR},
    {.optname= "getsamplerate",   .help="Get TX Samplerate",          .description="Describe me", .cmdid=ERIDAN_CMD_GET_SAMPLE_RATE},
    {.optname= "setsamplerate",   .help="Set TX Samplerate",          .description="Describe me", .cmdid=ERIDAN_CMD_SET_SAMPLE_RATE},
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

#define NUM_ECM_OPTIONS (sizeof(ecmctl_options)/sizeof(ecmctl_options_t))

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
            {"getfreq",         no_argument,       0, ERIDAN_CMD_GET_FREQ},
            {"getstats",        no_argument,       0, ERIDAN_CMD_GET_STATS},
            {"setfreq",         required_argument, 0, ERIDAN_CMD_SET_FREQ},
            {"getpwr",          no_argument,       0, ERIDAN_CMD_GET_PWR},
            {"setpwr",          no_argument,       0, ERIDAN_CMD_SET_PWR},
            {"getsamplerate",   no_argument,       0, ERIDAN_CMD_GET_SAMPLE_RATE},
            {"setsamplerate",   no_argument,       0, ERIDAN_CMD_SET_SAMPLE_RATE},
            {"getrxfreq",       no_argument,       0, ERIDAN_CMD_GET_RXFREQ},
            {"setrxfreq",       no_argument,       0, ERIDAN_CMD_SET_RXFREQ},
            {"getrxsamplerate", no_argument,       0, ERIDAN_CMD_GET_RXSAMPLERATE},
            {"setrxsamplerate", no_argument,       0, ERIDAN_CMD_SET_RXSAMPLERATE},
            {"getrxgains",      no_argument,       0, ERIDAN_CMD_GET_RXGAINS},
            {"setrxgains",      no_argument,       0, ERIDAN_CMD_SET_RXGAINS},
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

        c = getopt_long (argc, argv, "h",
                         long_options, &option_index);

        /* Detect the end of the options. */
        if (c == -1)
            break;

        switch ((eridan_cmd_id_t)c)
        {
        case ERIDAN_CMD_SYSINIT:
            puts ("sysinit\n");
            do_sysinit();
            break;

        case ERIDAN_CMD_GET_FREQ:
            puts("doing getfreq\n");
            do_getfreq();
            break;

        case ERIDAN_CMD_GET_STATS:
            puts("doing getstats\n");
            do_getstats();
            break;       

        case ERIDAN_CMD_SET_FREQ:
            puts("doing setfreq\n");
            do_setfreq();
            break;

        case ERIDAN_CMD_GET_PWR:
            puts("doing getpwr\n");
            do_getpwr();
            break;

        case ERIDAN_CMD_SET_PWR:
            puts("doing setpwr\n");
            do_setpwr();
            break;

        case ERIDAN_CMD_GET_SAMPLE_RATE:
            puts("doing getsamplerate\n");
            do_getsamplerate();
            break;

        case ERIDAN_CMD_SET_SAMPLE_RATE:
            puts("doing setsamplerate\n");
            do_setsamplerate();
            break;

        case ERIDAN_CMD_GET_RXFREQ:
            puts("doing getrxfreq\n");
            do_getrxfreq();
            break;

        case ERIDAN_CMD_SET_RXFREQ:
            puts("doing setrxfreq\n");
            do_setrxfreq();
            break;

        case ERIDAN_CMD_GET_RXSAMPLERATE:
            puts("doing getrxsamplerate\n");
            do_getrxsamplerate();
            break;

        case ERIDAN_CMD_SET_RXSAMPLERATE:
            puts("doing setrxsamplerate\n");
            do_setrxsamplerate();
            break;

        case ERIDAN_CMD_GET_RXGAINS:
            puts("doing getrxgains\n");
            do_getrxgains();
            break;

        case ERIDAN_CMD_SET_RXGAINS:
            puts("doing setrxgains\n");
            do_setrxgains();
            break;

        case ERIDAN_CMD_SYSOFF:
            puts("doing sysoff\n");
            do_sysoff();
            break;

        case ERIDAN_CMD_START_SCP:
            puts("doing startscp\n");
            do_startscp();
            break;

        case ERIDAN_CMD_PREP_SCP:
            puts("doing prepscp\n");
            do_prepscp();
            break;

        case ERIDAN_CMD_RESET_NOW:
            puts("doing resetnow\n");
            do_resetnow();
            break;

        case ERIDAN_CMD_RESET_DONE:
            puts("doing resetdone\n");
            do_resetdone();
            break;

        case ERIDAN_CMD_SEND_UPDATES:
            puts("doing sendupdates\n");
            do_sendupdates();
            break;

        case ERIDAN_CMD_CHECK_UPDATES:
            puts("doing checkupdates\n");
            do_checkupdates();
            break;

        case ERIDAN_CMD_GET_VERSION:
            puts("doing getversion\n");
            do_getversion();
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
        printf("Arguments unknown!\n");
        exit(EXIT_FAILURE);
    }

    return 0; 
}