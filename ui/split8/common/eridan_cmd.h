#ifndef _ERIDAN_CMD_H_
#define _ERIDAN_CMD_H_

typedef struct eridan_cmd_hdr_s {
    uint16_t             cookie,
    uint8_t              version,
    uint8_t              type,
    uint16_t             length,
    uint16_t             reserved,
} eridan_cmd_hdr_t;

typedef struct eridan_node_addr_s {
    ip_addr_t            ip,
    uint16_t             port;
} eridan_node_addr_t;

typedef enum eridan_cmd_id_e {
    ERIDAN_CMD_SYSINIT = 0,
} eridan_cmd_id_t;

typedef struct eridan_cmd_req_s {
    eridan_node_addr_t  from,
    eridan_node_addr_t  to,
    uint16_t            reqid, 
    uint32_t            pre_shared_key
    eridan_cmd_id_t     cmdid,
    uint16_t            num_args,
    uint16_t            cmd_args[],
} eridan_cmd_req_t;

typedef struct eridan_cmd_resp_s {
    eridan_node_addr_t  from,
    eridan_node_addr_t  to,
    uint16_t            reqid,
    uint32_t            pre_shared_key
    eridan_cmd_id_t     cmdid,
    uint16_t            num_results,
    uint16_t            cmd_results[],
} eridan_cmd_resp_t;

#endif
