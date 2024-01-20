#ifndef MSG_H
#define MSG_H

#include "program.h"

#define PAYLOAD_SIZE 2

#define MIN(a, b) a < b ? a : b

typedef enum {
    MERGE,
    EXEC,
    RESET,
    GET,
    DELETE,
} MsgType;

typedef struct {
    int32_t type; // enum MsgType
    uint32_t size;
    uint32_t start;
} RequestHeader;

typedef struct {
    RequestHeader header;
    Instruction data[PAYLOAD_SIZE];
} Request;

typedef struct {
    int32_t type; // enum MsgType
    int32_t ret;
    uint32_t size;
} ResponseHeader;

typedef struct {
    ResponseHeader header;
    Instruction data[PAYLOAD_SIZE];
} Response;

#endif
