#ifndef LOX_ERROR_H
#define LOX_ERROR_H
// IWYU pragma: private, include <lox.h>

/**
 * @file lox/error.h
 * @brief Lox error handling variables
 * @ingroup Lox
 * @{
 */

/** Generic return codes */
typedef enum lox_error_t {
    LOX_ERROR_OK = 0,              /**< No error */

    LOX_ERROR_COMPILE        = -1, /**< Error occured during compile step. */
    LOX_ERROR_INTERPRET      = -2, /**< Error occured during interpret step. */
    LOX_ERROR_STACK_OVERFLOW = -3, /**< Runtume vm stack overflowed */
} lox_error_t;

#endif // !LOX_ERROR_H
