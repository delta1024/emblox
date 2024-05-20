#include "chunk.h"
#include "value.h"

void lox_chunk_init(lox_chunk *chunk) {
    chunk->capacity = chunk->count = 0;
    chunk->code = chunk->lines = NULL;
    lox_value_arrayinit(&chunk->constants);
}
void lox_chunk_writebyte(lox_chunk *chunk, uint8_t byte, uint8_t line,
                         lox_memory *allocator) {
    if (chunk->count + 1 > chunk->capacity) {
        size_t ocap     = chunk->capacity;
        chunk->capacity = ocap < 8 ? 8 : ocap * 2;
        chunk->code =
            allocator->f(allocator->ud, chunk->code, sizeof(uint8_t) * ocap,
                         sizeof(uint8_t) * chunk->capacity);
        chunk->lines =
            allocator->f(allocator->ud, chunk->lines, sizeof(uint8_t) * ocap,
                         sizeof(uint8_t) * chunk->capacity);
    }
    chunk->code[chunk->count]    = byte;
    chunk->lines[chunk->count++] = line;
}

int lox_chunk_writeconstant(lox_chunk *chunk, lox_value val,
                            lox_memory *allocator) {
    lox_value_arraywrite(&chunk->constants, val, allocator);
    return chunk->constants.count - 1;
}

void lox_chunk_free(lox_chunk *chunk, lox_memory *allocator) {
    allocator->f(allocator->ud, chunk->code, sizeof(uint8_t) * chunk->capacity,
                 0);
    allocator->f(allocator->ud, chunk->lines, sizeof(uint8_t) * chunk->capacity,
                 0);
    lox_value_arrayfree(&chunk->constants, allocator);
    lox_chunk_init(chunk);
}
