#ifndef _LOX_CHUNK_H
#define _LOX_CHUNK_H

#include "memory.h"
#include <stdint.h>
typedef enum lox_opcode_t {
    OP_RETURN,
} lox_opcode_t;

typedef struct lox_chunk {
    struct memory_tracker *tracker;
    int count;
    int capacity;
    uint8_t *code;
    uint8_t *lines;
} lox_chunk;

void lox_chunk_init(struct memory_tracker *tracker, lox_chunk *chunk);
void lox_chunk_free(lox_chunk *chunk);
void lox_chunk_write_byte(lox_chunk *chunk, uint8_t byte, uint8_t line);

#endif // !_LOX_CHUNK_H
