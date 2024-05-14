#ifndef LOX_H
#define LOX_H
// IWYU pragma: begin_export
#include "lox/error.h"
#include "lox/stack.h"
#include "lox/state.h"
#include "lox/value.h"
// IWYU pragma: end_export

/** 
 * @file lox.h 
 * @brief A toy programing language
 * @defgroup Lox API for the lox library
 * @{
 */


/**
 * Parses the contents of the file at path and executes the resulting function 
 * @param state An active lox state
 * @param path The reletive path to the file to open
 * @returns \ref lox_error_t or \ref LOX_ERROR_OK
 */
lox_error_t lox_dofile(lox_state *state, const char *path);

/**
 * Parses @a source and executes the resulting function.
 * @returns \ref lox_error_t or \ref LOX_ERROR_OK
*/
lox_error_t lox_dostring(lox_state *state, const char *source);
/** Prints lox version and exits */
void lox_print_version();
/** Get a the program version as a string */
const char *lox_version_string();

/** @} */
#endif // !LOX_H
