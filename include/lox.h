#ifndef LOX_H
#define LOX_H
// IWYU pragma: begin_export
#include "lox/state.h"
#include "lox/error.h"
// IWYU pragma: end_export
/*! @file lox.h 
 \defgroup lox lox
 @{
*/
/*!
  Parses the contents of the file at \a path and executs the resulting function 
  @returns \ref lox_error_t or \ref LOX_ERROR_OK
 */
lox_error_t lox_dofile(lox_state *state, const char *path);

/*!
 Parses @a source and executes the resulting function.
 @returns  LOX_ERROR_OK or  \ref lox_error_t
*/
lox_error_t lox_dostring(lox_state *state, const char *source);
/*!
prints lox version and exits
*/
void lox_print_version();
/*!
@returns version string
*/
const char *lox_version_string();
/**@}*/
#endif // !LOX_H
