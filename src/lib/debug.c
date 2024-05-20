#include "debug.h"
#include "chunk.h"
#include "value.h"
#include <stdio.h>
const char *strs[] = {
    [OP_RETURN] = "OP_RETURN",     [OP_CONSTANT] = "OP_CONSTANT",
    [OP_ADD] = "OP_ADD",           [OP_SUBTRACT] = "OP_SUBTRACT",
    [OP_MULTIPLY] = "OP_MULTIPLY", [OP_DIVIDE] = "OP_DIVIDE",
};
void lox_debug_dissasemblechunk(lox_chunk *chunk, const char *name) {
    printf("== %s ==\n", name);
    for (int i = 0; i < chunk->count;) {
        i = lox_debug_dissasembleinstruction(chunk, i);
    }
}
int constant_instruction(const char *name, lox_chunk *chunk, int offset) {
    uint8_t constant = chunk->code[offset + 1];
    printf("%-16s %4d '", name, constant);
    lox_value_print(chunk->constants.entries[constant]);
    printf("'\n");
    return offset + 2;
}
int simple_instruction(const char *name, int offset) {
    printf("%s\n", name);
    return offset + 1;
}
int lox_debug_dissasembleinstruction(lox_chunk *chunk, int offset) {
    printf("%04d ", offset);
    if (offset > 0 && chunk->lines[offset] == chunk->lines[offset - 1]) {
        printf("   | ");
    } else {
        printf("%4d ", chunk->lines[offset]);
    }
    uint8_t byte = chunk->code[offset];
    switch ((op_code_t)byte) {
    case OP_CONSTANT:
        return constant_instruction(strs[byte], chunk, offset);
    case OP_RETURN:
    case OP_ADD:
    case OP_SUBTRACT:
    case OP_MULTIPLY:
    case OP_DIVIDE:
        return simple_instruction(strs[byte], offset);
    default:
        fprintf(stderr, "unknown opcode: %d\n", byte);
        return offset + 1;
    }
}
