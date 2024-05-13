#ifndef LOX_VALUE_H
#define LOX_VALUE_H
#include "state.h"
// IWYU pragma: private, include <lox.h>

/*! \file lox/value.h */
/**
 *   \defgroup lox_value lox_value
 *   \ingroup lox
 */
/// @{

/** A type representing the underling type of a lox value. */
typedef enum lox_value_t {
    /** \n */
    LOX_VALUE_NUMBER,
} lox_value_t;

/*! A number as represented by lox */
typedef double lox_number_t;

/*!
 * Prints the value at \a span - 1 from the stack top to stdout.
 */
void lox_printvalue(lox_state *state, int span);
/// @}
#endif // !LOX_VALUE_H
