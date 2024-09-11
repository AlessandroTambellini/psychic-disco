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
// TODO: handle cases where PAYLOAD_SIZE is not a multiple of
// sizeof(Instruction)
#define PAYLOAD_SIZE (2 * sizeof(Instruction))

typedef enum {
    MERGE,
    EXEC,
    RESET,
    GET,
    DELETE,
} Method;

typedef struct {
    int32_t type; // enum Method
    uint32_t size;
} RequestHeader;

typedef struct {
    RequestHeader header;
    uint8_t payload[PAYLOAD_SIZE];
} Request;

typedef enum {
    SUCCESS,
    FAILURE,
    UNKNOWN_METHOD,
} Status;

typedef struct {
    int32_t status; // enum Status
    uint32_t size;
} ResponseHeader;

typedef struct {
    ResponseHeader header;
    uint8_t payload[PAYLOAD_SIZE];
} Response;

bool handle_connection(Conn *conn);
bool handle_request(Conn *conn);
bool handle_merge(Conn *conn, Request *req, Response *res);
bool handle_exec(Conn *conn, Request *req, Response *res);
bool handle_reset(Conn *conn, Request *req, Response *res);
bool handle_get(Conn *conn, Request *req, Response *res);
bool handle_delete(Conn *conn, Request *req, Response *res);
bool handle_response(Conn *conn);
