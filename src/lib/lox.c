#include "common.h"
#include "chunk.h"
#include "state.h" // IWYU pragma: keep
#include <lox.h>
#include <stdio.h>

void lox_print_version() {
    printf("%s", LOX_VERSION);
}

const char *lox_version_string() {
    return LOX_VERSION;
}
lox_error_t lox_dostring(lox_state *state, const char *source) {
    lox_chunk_write_byte(&state->chunk, OP_RETURN, 0);
    state->vm.cur_chunk = &state->chunk;
    state->vm.ip = state->chunk.code;

    lox_error_t result = lox_vm_interpret(&state->vm);
    if (result != LOX_ERROR_OK) {
        state->last_error = result;
        return LOX_ERROR_INTERPRET;
    }
    return LOX_ERROR_OK;
}
lox_error_t lox_dofile(lox_state *state, const char *path) {
    FILE *file = fopen(path, "r");
    if (file == NULL) {
        perror("could not open file");
        return LOX_ERROR_COMPILE;
    }

    lox_error_t result = LOX_ERROR_OK;

    fseek(file, 0, SEEK_END);
    size_t f_len = ftell(file);
    rewind(file);

    char *buffer = lox_malloc(&state->memory, sizeof(char) * (f_len + 1));
    if (buffer == NULL) {
        perror("could not allocate memory for buffer");
        result = LOX_ERROR_COMPILE;
        goto defer;
    }

    int read = fread(buffer, sizeof(char), f_len, file);
    if (read != f_len) {
        perror("could not read file");
        result = LOX_ERROR_COMPILE;
        goto buf_defer;
    }
    buffer[read] = '\0';

    result = lox_dostring(state, buffer);
buf_defer:
    lox_free(&state->memory, buffer, sizeof(char) * (f_len + 1));
defer:
    fclose(file);
    return result;
}
