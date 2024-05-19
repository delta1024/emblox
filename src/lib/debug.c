#include "debug.h"
#include "chunk.h"
#include "value.h"
#include <stdio.h>
const char *strs[] = {
    [OP_RETURN]   = "OP_RETURN",
    [OP_CONSTANT] = "OP_CONSTANT",
};
void lox_debug_dissasemble_chunk(lox_chunk *chunk, const char *name) {
    printf("== %s ==\n", name);
    for (int i = 0; i < chunk->count;) {
        i = lox_debug_dissasemble_instruction(chunk, i);
    }
}
int constant_instruction(const char *name, lox_chunk *chunk, int offset) {
    uint8_t constant = chunk->code[offset + 1];
    printf("%-16s %4d '", name, constant);
    lox_print_value(chunk->constants.entries[constant]);
    printf("'\n");
    return offset + 2;
}
int simple_instruction(const char *name, int offset) {
    printf("%s\n", name);
    return offset + 1;
}
int lox_debug_dissasemble_instruction(lox_chunk *chunk, int offset) {
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
        return simple_instruction(strs[byte], offset);
    }
}
