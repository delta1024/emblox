#ifndef _LOX_CHUNK_H
#define _LOX_CHUNK_H

#include "memory.h"
#include "value.h"
#include <stdint.h>

typedef enum lox_opcode_t {
    OP_RETURN,
    OP_CONSTANT,
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_DIVIDE,
} lox_opcode_t;

typedef struct lox_chunk {
    int count;
    int capacity;
    uint8_t *code;
    uint8_t *lines;
    lox_value_array constants;
} lox_chunk;

void lox_chunk_init(lox_chunk *chunk);
void lox_chunk_free(lox_chunk *chunk, struct memory_tracker *tracker);
void lox_chunk_write_byte(lox_chunk *chunk, uint8_t byte, uint8_t line,
                          struct memory_tracker *tracker);
int lox_chunk_write_constant(lox_chunk *chunk, lox_value value,
                             struct memory_tracker *tracker);

#endif // !_LOX_CHUNK_H
