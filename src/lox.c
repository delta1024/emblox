#include <lox.h>
#include <stdio.h>
#include <stdlib.h>
#include "lox_common.h"

struct LoxState {
	int time;
};

void lox_print_version() {
	printf("%s", VERSION);
}

const char *lox_version_string() {
	return VERSION;
}
LoxState *lox_state_open() {
	LoxState *state = malloc(sizeof(LoxState));
	if (state == NULL) return NULL;
	state->time = 420;
	return state;
}
void lox_state_close(LoxState *state) {
	free(state);
}

int lox_state_time(LoxState *state) {
	return state->time;
}
