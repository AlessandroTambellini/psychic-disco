#ifndef MSG_H
#define MSG_H

#include "program.h"

typedef enum {
    MERGE,
    EXEC,
    RESET,
} RequestMsgType;

typedef struct {
    int32_t type; // enum RequestMsgType
    uint32_t size;
    uint32_t id;
} RequestMsgH;

// struct RequestMsg
// +-----------------+
// |     type(32)    |
// +-----------------+
// |     size(32)    |
// +-----------------+
// |      id(32)     |
// +-----------------+
// |  i[0].code(32)  |
// +-----------------+
// |       ...       |
// +-----------------+
// |  v[0].arg2(32)  |
// +-----------------+
// |  v[n].code(32)  |
// +-----------------+
// |       ...       |
// +-----------------+
// |  v[n].arg2(32)  |
// +-----------------+

#define PAYLOAD_SIZE 4096

typedef struct {
    RequestMsgH header;
    Instruction data[PAYLOAD_SIZE];
} RequestMsg;

// struct ResultMsg
// +-----------------+
// |     type(32)    |
// +-----------------+
// |     ret(32)     |
// +-----------------+

typedef struct {
    int32_t type; // enum RequestMsgType
    uint32_t id;
    int32_t ret;
} ResultMsg;

int32_t msg_type(RequestMsg *msg);
uint32_t msg_size(RequestMsg *msg);
uint32_t msg_id(RequestMsg *msg);
Instruction *msg_data(RequestMsg *msg);
bool msg_merge_program(RequestMsg *msg, Program *program);
bool msg_from_program(RequestMsg **msg, Program *program);
bool msg_to_program(RequestMsg *msg, Program *program);

#endif
