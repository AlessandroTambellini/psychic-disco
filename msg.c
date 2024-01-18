#include <string.h>

#include "msg.h"
#include "program.h"

uint32_t msg_size(RequestMsg *msg)
{
    return msg->header.size;
}

Instruction *msg_data(RequestMsg *msg)
{
    return msg->data;
}

bool merge_program(RequestMsg *msg, Program *program)
{
    return program_merge(program, msg_data(msg), msg_size(msg));
}

size_t split_program(Program *program, RequestMsg *msg)
{
    size_t size = program_size(program) > PAYLOAD_SIZE
        ? PAYLOAD_SIZE
        : program_size(program);

    if (size) {
        program_split(program, msg_data(msg), size);

        msg->header = (RequestMsgH) {
            .type = MERGE,
            .size = size,
        };
    }

    return size;
}
