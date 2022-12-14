#ifndef _ERIDAN_CMD_H_
#define _ERIDAN_CMD_H_

#define EC_MAGIC_COOKIE  0x45435246
#define EC_SERVER_PORT   9400
#define EC_VERSION       0x01
#define EC_TYPE_REQ      0xA5
#define EC_TYPE_RESP     0xB6
#define EC_CHAR_STR_SIZE 64

typedef struct eridan_cmd_hdr_s {
    uint32_t             cookie;
    uint8_t              version;
    uint8_t              type;
    uint32_t             length;
    uint16_t             reserved;
} eridan_cmd_hdr_t;

typedef struct eridan_node_addr_s {
    uint32_t             ip;
    uint16_t             port;
} eridan_node_addr_t;

typedef enum eridan_cmd_id_e {
    ERIDAN_CMD_SYSINIT          = 0,
    ERIDAN_CMD_GET_FREQ         = 1,
    ERIDAN_CMD_GET_STATS        = 2,
    ERIDAN_CMD_SET_FREQ         = 3,
    ERIDAN_CMD_GET_PWR          = 4,
    ERIDAN_CMD_SET_PWR          = 5,
    ERIDAN_CMD_GET_SAMPLE_RATE  = 6,
    ERIDAN_CMD_SET_SAMPLE_RATE  = 7,
    ERIDAN_CMD_GET_RXFREQ       = 8,
    ERIDAN_CMD_SET_RXFREQ       = 9,
    ERIDAN_CMD_GET_RXSAMPLERATE = 10,
    ERIDAN_CMD_SET_RXSAMPLERATE = 11,
    ERIDAN_CMD_GET_RXGAINS      = 12,
    ERIDAN_CMD_SET_RXGAINS      = 13,
    ERIDAN_CMD_SYSOFF           = 14,
    ERIDAN_CMD_START_SCP        = 15,
    ERIDAN_CMD_PREP_SCP         = 16,
    ERIDAN_CMD_RESET_NOW        = 17,
    ERIDAN_CMD_RESET_DONE       = 18,
    ERIDAN_CMD_SEND_UPDATES     = 19,
    ERIDAN_CMD_CHECK_UPDATES    = 20,
    ERIDAN_CMD_GET_VERSION      = 21,
} eridan_cmd_id_t;

typedef struct eridan_cmd_req_s {
    eridan_node_addr_t  from;
    eridan_node_addr_t  to;
    uint16_t            reqid;
    uint32_t            pre_shared_key;
    eridan_cmd_id_t     cmdid;
    uint16_t            num_args;
    char                cmd_args[];
} eridan_cmd_req_t;

typedef struct eridan_cmd_resp_s {
    eridan_node_addr_t  from;
    eridan_node_addr_t  to;
    uint16_t            reqid;
    uint32_t            pre_shared_key;
    eridan_cmd_id_t     cmdid;
    uint16_t            num_results;
    char                cmd_results[];
} eridan_cmd_resp_t;

typedef enum ecm_ctrl_ {
    ECM_FAILURE = -1,
    ECM_SUCCESS
} ecm_ctrl_t;

#endif