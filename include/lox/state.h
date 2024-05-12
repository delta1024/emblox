#ifndef LOX_STATE_H
#define LOX_STATE_H

// IWYU pragma: private, include <lox.h>
typedef struct lox_state lox_state;
lox_state *lox_state_open();
void lox_state_close(lox_state *state);
void lox_state_printerr(lox_state *state);
void lox_state_printerr(lox_state *state);

#endif // !LOX_STATE_H
