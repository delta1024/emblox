#ifndef LOX_STATE_H
#define LOX_STATE_H
// IWYU pragma: private, include <lox.h>

/** 
 * @file lox/state.h
 * @brief Lox state managment functions
 * @defgroup lox_state Lox state managment functions
 * @ingroup Lox
 * @{
 */

/**
 * An opaque structure representing a handle
 * to an active @ref lox_state.
 */
typedef struct lox_state lox_state;

/**
 * Opens a new @ref lox_state
 * @return owner must call lox_state_close() to free state memory
 */
lox_state *lox_state_open();
/** Disposes of any memory allocated in a state */
void lox_state_close(lox_state *state);
/** Prints the last error encountered by a state. */
void lox_state_printerr(lox_state *state);
/** @} */
#endif // !LOX_STATE_H
