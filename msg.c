#include <string.h>

#include "msg.h"
#include "program.h"

int32_t msg_type(RequestMsg *msg)
{
    return msg->header.type;
}

uint32_t msg_size(RequestMsg *msg)
{
    return msg->header.size;
}

uint32_t msg_id(RequestMsg *msg)
{
    return msg->header.id;
}

Instruction *msg_data(RequestMsg *msg)
{
    return msg->data;
}

bool merge_program(RequestMsg *msg, Program *program)
{
    bool rv = program_resize(program, program_capacity(program) + msg_size(msg));
    if (!rv) {
        printf("Failed resize during program merge.\n");
        return false;
    }

    Instruction *src = msg_data(msg);
    Instruction *dst = program_data(program) + program_size(program);
    size_t src_size = msg_size(msg) * sizeof(Instruction);

    memcpy(dst, src, src_size);
    program->size += msg_size(msg);

    return true;
}

size_t split_program(Program *program, RequestMsg *msg)
{
    size_t size = program_size(program) > PAYLOAD_SIZE
        ? PAYLOAD_SIZE
        : program_size(program);

    memcpy(msg_data(msg), program_data(program), size);

    size_t size_new = program_size(program) - size;
    memmove(program_data(program),
            program_data(program) + size,
            size_new);

    program->size = size_new;

    msg->header = (RequestMsgH) {
        .type = MERGE,
        .size = size,
        .id = 0
    };

    return size;
}
