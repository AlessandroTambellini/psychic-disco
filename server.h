#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <assert.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "el.h"

// PAYLOAD_SIZE should be a value greater than sizeof(Instruction)
// defined in program.h which, as of now, is 16 bytes
#define PAYLOAD_SIZE 2 * 16

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
} RequestHeader;

typedef struct {
    RequestHeader header;
    int8_t payload[PAYLOAD_SIZE];
} Request;

typedef struct {
    int32_t status;
    uint32_t size;
} ResponseHeader;

typedef struct {
    ResponseHeader header;
    int8_t payload[PAYLOAD_SIZE];
} Response;

bool handle_connection(Conn *conn);
bool handle_request(Conn *conn);
bool handle_merge(Conn *conn, Request *req, Response *res);
bool handle_exec(Conn *conn, Request *req, Response *res);
bool handle_reset(Conn *conn, Request *req, Response *res);
bool handle_get(Conn *conn, Request *req, Response *res);
bool handle_delete(Conn *conn, Request *req, Response *res);
bool handle_response(Conn *conn);
