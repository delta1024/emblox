#ifndef LOX_AUX_H
#define LOX_AUX_H
// IWYU pragma: private, include <lox.h>

#include "error.h"
#include "state.h"
/**
 * @file lox/aux.h
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
#endif // !LOX_AUX_H
