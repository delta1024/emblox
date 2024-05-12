#ifndef LOX_H
#define LOX_H
// IWYU pragma: begin_export
#include "lox/state.h"
#include "lox/error.h"
// IWYU pragma: end_export

lox_error_t lox_dofile(lox_state *state, const char *path);
lox_error_t lox_dostring(lox_state *state, const char *source);
void lox_print_version();
const char *lox_version_string();
#endif // !LOX_H
