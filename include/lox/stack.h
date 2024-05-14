#ifndef LOX_STACK_H
#define LOX_STACK_H
// IWYU pragma: private, include <lox.h>
#include "state.h"
#include "value.h"
/** 
 * @file lox/stack.h 
 * @brief lox_stack Lox stack manipulation functions
 * @defgroup lox_stack Lox stack manipulation functions
 * @ingroup Lox
 * @{
 */

/** Pushes an integer onto the stack. */
void lox_pushnumber(lox_state *state, lox_number_t value);

/**
 * Pops an integer off the stack.
 * @returns an integer or 0 if value is not a number.
 */
lox_number_t lox_popnumber(lox_state *state);
/**
 * Retrieves the value at span - 1 from the top of the stack.
 * @returns an integer or 0 if value in not a number
*/
lox_number_t lox_peeknumber(lox_state *state, int span);
/** Retrievs the type of the value at \a span - 1 from the top of the stack. */
lox_value_t lox_peektype(lox_state *state, int span);
/** @} */
#endif // !LOX_STACK_H
