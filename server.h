#ifndef SERVER_H
#define SERVER_H

#include "el.h"

// PAYLOAD_SIZE should be a value greater than sizeof(Instruction)
// defined in program.h which, as of now, is 16 bytes
#define PAYLOAD_SIZE (2 * sizeof(Instruction))

typedef enum {
    MERGE,
    INSERT,
    EXEC,
    RESET,
    GET,
    DELETE,
    INSP,
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
bool handle_insert(Conn *conn, Request *req, Response *res);
bool handle_exec(Conn *conn, Request *req, Response *res);
bool handle_reset(Conn *conn, Request *req, Response *res);
bool handle_get(Conn *conn, Request *req, Response *res);
bool handle_delete(Conn *conn, Request *req, Response *res);
bool handle_insp(Conn *conn, Request *req, Response *res);
bool handle_response(Conn *conn);
void start_server();

#endif
