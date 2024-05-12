#ifndef LOX_ERROR_H
#define LOX_ERROR_H
// IWYU pragma: private, include <lox.h>
typedef enum lox_error_t {

    LOX_ERROR_OK,
    LOX_ERROR_COMPILE,
    LOX_ERROR_INTERPRET,
    LOX_ERROR_STACK_OVERFLOW,
} lox_error_t;

#endif // !LOX_ERROR_H
