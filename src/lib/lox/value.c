#include "value.h"
#include "state.h" // IWYU pragma: keep
#include "vm.h"
#include <lox.h>
void lox_printvalue(lox_state *state, int span) {
    lox_value val = lox_vm_peekvalue(&state->vm, span);
    lox_value_print(val);
}
