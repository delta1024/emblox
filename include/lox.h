#ifndef LOX_H
#define LOX_H

typedef struct LoxState LoxState; 

LoxState *lox_state_open();
void lox_state_close(LoxState *state);
void lox_print_version();
const char *lox_version_string();
#endif // !LOX_H
