#include "state.h" // IWYU pragma: keep
#include "chunk.h"
#include "memory.h"
#include "vm.h"
#include <lox.h>
#include <stdio.h>
lox_state *lox_state_open() {
    struct memory_tracker tracker;
    lox_state *state = lox_malloc(sizeof(lox_state), &tracker);
    if (state == NULL)
        return NULL;
    state->memory = tracker;
    lox_vm_init(&state->vm);
    lox_chunk_init(&state->chunk);
    state->last_error = LOX_ERROR_OK;
    return state;
}

void lox_state_close(lox_state *state) {
    struct memory_tracker tracker;
    tracker = state->memory;
    lox_vm_free(&state->vm, &tracker);
    lox_chunk_free(&state->chunk, &tracker);
    lox_free(state, sizeof(lox_state), &tracker);
}

void lox_state_printerr(lox_state *state) {
    switch (state->last_error) {
    case LOX_ERROR_STACK_OVERFLOW:
        lox_vm_reporterror(&state->vm, "stack overflow");
        break;
    case LOX_ERROR_OK:
    case LOX_ERROR_INTERPRET:
    case LOX_ERROR_COMPILE:
        break;
    }
}
