#include "common.h"
#include "state.h" // IWYU pragma: keep
#include <lox.h>
#include <stdio.h>
#include <stdlib.h>

void lox_print_version() {
    printf("%s", LOX_VERSION);
}

const char *lox_version_string() {
    return LOX_VERSION;
}

LoxState *lox_state_open() {
    LoxState *state = malloc(sizeof(LoxState));
    if (state == NULL)
        return NULL;
    state->time = 420;
    return state;
}

void lox_state_close(LoxState *state) {
    free(state);
}
