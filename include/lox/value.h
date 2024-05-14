#ifndef LOX_VALUE_H
#define LOX_VALUE_H
// IWYU pragma: private, include <lox.h>

#include "state.h"

/** 
 * @file lox/value.h 
 * @brief Lox value manipulation functions
 * @defgroup lox_value Lox value manipulation functions
 * @ingroup Lox
 * @{
 */

/** A type representing the underling type of a lox value. */
typedef enum lox_value_t {
    /** Value represents a number. */
    LOX_VALUE_NUMBER,
} lox_value_t;

/** A number as represented by lox */
typedef double lox_number_t;

/** Prints the value at span. */
void lox_printvalue(lox_state *state, int span);
/** @} */
#endif // !LOX_VALUE_H
