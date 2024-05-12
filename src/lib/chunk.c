#include "chunk.h"
#include "memory.h"
#include <stddef.h>
#include <stdint.h>

void lox_chunk_init( lox_chunk *chunk) {
    chunk->code = chunk->lines = NULL;
    chunk->count = chunk->capacity = 0;
}

void lox_chunk_free(lox_chunk *chunk, struct memory_tracker *tracker) {
    lox_free(chunk->code, arraysize(uint8_t, chunk->capacity), tracker);
    lox_free(chunk->lines, arraysize(uint8_t, chunk->capacity), tracker);
    lox_chunk_init(chunk);
}

void lox_chunk_write_byte(lox_chunk *chunk, uint8_t byte, uint8_t line, struct memory_tracker *tracker) {
    if (chunk->count + 1 > chunk->capacity) {
        size_t old_cap = chunk->capacity;
        chunk->capacity = old_cap < 8 ? 8 : old_cap * 2;
        chunk->code =
            lox_reallocarray(chunk->code, chunk->capacity,
                             old_cap, sizeof(uint8_t), tracker );
        chunk->lines =
            lox_reallocarray(chunk->lines, chunk->capacity,
                             old_cap, sizeof(uint8_t), tracker);
    }
    chunk->lines[chunk->count] = line;
    chunk->code[chunk->count++] = byte;
}
