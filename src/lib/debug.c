#include "debug.h"
#include "chunk.h"
#include <stdint.h>
#include <stdio.h>
static const char *op_str[] = {
    [OP_RETURN] = "OP_RETURN",     [OP_CONSTANT] = "OP_CONSTANT",
    [OP_ADD] = "OP_ADD",           [OP_SUBTRACT] = "OP_SUBTRACT",
    [OP_MULTIPLY] = "OP_MULTIPLY", [OP_DIVIDE] = "OP_DIVIDE",
};

void lox_debug_dissasemble_chunk(lox_chunk *chunk, const char *name) {
    printf("== %s ==\n", name);
    for (int offset = 0; offset < chunk->count;) {
        offset = lox_debug_dissasemble_instruction(chunk, offset);
    }
}

static int simple_instruction(const char *name, int offset);
static int constant_instruction(const char *name, lox_chunk *chunk, int offset);
int lox_debug_dissasemble_instruction(lox_chunk *chunk, int offset) {
    printf("%04d ", offset);
    if (offset > 0 && chunk->lines[offset] == chunk->lines[offset - 1]) {
        printf("   | ");
    } else {
        printf("%4d ", chunk->lines[offset]);
    }

    uint8_t instruction = chunk->code[offset];
    switch (instruction) {
    case OP_CONSTANT:
        return constant_instruction(op_str[instruction], chunk, offset);
    case OP_RETURN:
    case OP_ADD:
    case OP_SUBTRACT:
    case OP_MULTIPLY:
    case OP_DIVIDE:
        return simple_instruction(op_str[instruction], offset);
    default:
        printf("Unknown opcode %d\n", instruction);
        return offset + 1;
    }
}
static int simple_instruction(const char *name, int offset) {
    printf("%s\n", name);
    return offset + 1;
}
static int constant_instruction(const char *name, lox_chunk *chunk,
                                int offset) {
    uint8_t constant = chunk->code[offset + 1];
    printf("%-16s %4d '", name, constant);
    lox_value_print(chunk->constants.entries[constant]);
    printf("'\n");
    return offset + 2;
}
