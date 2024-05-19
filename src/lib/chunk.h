#ifndef _LOX_CHUNK_
#define _LOX_CHUNK_
#include "common.h" // IWYU pragma: keep
#include "memory.h"
#include "value.h"

typedef enum op_code_t {
    OP_RETURN,
    OP_CONSTANT,
} op_code_t;

typedef struct lox_chunk {
    int capacity;
    int count;
    uint8_t *code;
    uint8_t *lines;
    lox_value_array constants;
} lox_chunk;

void lox_chunk_init(lox_chunk *chunk);
void lox_chunk_free(lox_chunk *chunk, lox_memory *allocator);
void lox_chunk_writebyte(lox_chunk *chunk, uint8_t byte, uint8_t line,
                         lox_memory *allocator);
int lox_chunk_writeconstant(lox_chunk *chunk, value val, lox_memory *allocator);
#endif // !_LOX_CHUNK_
