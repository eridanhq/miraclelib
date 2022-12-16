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
    printf("Command was: %d\n", resp->cmdid);
    printf("Response: %s\n", resp->cmd_results);

    return;
}

ecm_ctrl_t
send_request(int sockfd, struct sockaddr_in *servaddr, eridan_cmd_id_t cmdid)
{
    eridan_cmd_hdr_t *hdr;
    eridan_cmd_req_t *req;

    hdr = malloc(sizeof(eridan_cmd_hdr_t));
    memset(hdr, 0, sizeof(eridan_cmd_hdr_t));
    hdr->cookie  = EC_MAGIC_COOKIE;
    hdr->version = EC_VERSION;
    hdr->length  = sizeof(eridan_cmd_req_t);
    hdr->type    = EC_TYPE_REQ;
    req   = malloc(sizeof(eridan_cmd_req_t));
    memset(req, 0, sizeof(eridan_cmd_req_t));
    req->reqid   = 0x3434;
    req->cmdid   = cmdid;

    sendto(sockfd,  (const char *)hdr, sizeof(eridan_cmd_hdr_t),
                    MSG_CONFIRM,
                    (const struct sockaddr *) servaddr, sizeof(*servaddr));
    sendto(sockfd,  (const char *)req, sizeof(eridan_cmd_req_t),
                    MSG_CONFIRM,
                    (const struct sockaddr *) servaddr, sizeof(*servaddr));

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
    printf("Hello message sent.\n");
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

    send_request(sockfd, &servaddr, ERIDAN_CMD_GET_FREQ);
    printf("Hello message sent.\n");
    resp = get_response(sockfd, &servaddr);
    print_response(resp);

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

    send_request(sockfd, &servaddr, ERIDAN_CMD_GET_STATS);
    printf("Hello message sent.\n");
    resp = get_response(sockfd, &servaddr);
    print_response(resp);

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

    send_request(sockfd, &servaddr, ERIDAN_CMD_SET_FREQ);
    printf("Hello message sent.\n");
    resp = get_response(sockfd, &servaddr);
    print_response(resp);

    free(resp);
    close(sockfd);
    return;
}

void
do_getpwr(void)
{
    eridan_cmd_resp_t *resp;
    struct sockaddr_in  servaddr;
    int sockfd = connect_to_server(&servaddr);

    send_request(sockfd, &servaddr, ERIDAN_CMD_GET_PWR);
    printf("Hello message sent.\n");
    resp = get_response(sockfd, &servaddr);
    print_response(resp);

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

    send_request(sockfd, &servaddr, ERIDAN_CMD_SET_PWR);
    printf("Hello message sent.\n");
    resp = get_response(sockfd, &servaddr);
    print_response(resp);

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

    send_request(sockfd, &servaddr, ERIDAN_CMD_GET_SAMPLE_RATE);
    printf("Hello message sent.\n");
    resp = get_response(sockfd, &servaddr);
    print_response(resp);

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

    send_request(sockfd, &servaddr, ERIDAN_CMD_SET_SAMPLE_RATE);
    printf("Hello message sent.\n");
    resp = get_response(sockfd, &servaddr);
    print_response(resp);

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

    send_request(sockfd, &servaddr, ERIDAN_CMD_GET_RXFREQ);
    printf("Hello message sent.\n");
    resp = get_response(sockfd, &servaddr);
    print_response(resp);

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

    send_request(sockfd, &servaddr, ERIDAN_CMD_SET_RXFREQ);
    printf("Hello message sent.\n");
    resp = get_response(sockfd, &servaddr);
    print_response(resp);

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

    send_request(sockfd, &servaddr, ERIDAN_CMD_GET_RXSAMPLERATE);
    printf("Hello message sent.\n");
    resp = get_response(sockfd, &servaddr);
    print_response(resp);

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

    send_request(sockfd, &servaddr, ERIDAN_CMD_SET_RXSAMPLERATE);
    printf("Hello message sent.\n");
    resp = get_response(sockfd, &servaddr);
    print_response(resp);

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

    send_request(sockfd, &servaddr, ERIDAN_CMD_GET_RXGAINS);
    printf("Hello message sent.\n");
    resp = get_response(sockfd, &servaddr);
    print_response(resp);

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

    send_request(sockfd, &servaddr, ERIDAN_CMD_SET_RXFREQ);
    printf("Hello message sent.\n");
    resp = get_response(sockfd, &servaddr);
    print_response(resp);

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
    printf("Hello message sent.\n");
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

void
Usage(void)
{
    printf("Print all long options here\n");
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