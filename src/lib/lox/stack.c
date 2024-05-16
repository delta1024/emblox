#include "state.h" // IWYU pragma: keep
#include "vm.h"
#include <lox.h>
lox_error_t lox_pushnumber(lox_state *state, lox_number_t value) {
    lox_error_t result = lox_vm_pushvalue(&state->vm, value);
    if (result != LOX_ERROR_OK)
        state->last_error = result;
}
lox_number_t lox_popnumber(lox_state *state) {
    return lox_vm_popvalue(&state->vm);
}
lox_number_t lox_peeknumber(lox_state *state, int span) {
    return lox_vm_peekvalue(&state->vm, span);
}
lox_value_t lox_peektype(lox_state *state, int span) {
    return LOX_VALUE_NUMBER;
}
