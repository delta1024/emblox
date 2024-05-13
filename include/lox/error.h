#ifndef LOX_ERROR_H
#define LOX_ERROR_H
/*! \file lox/error.h */
// IWYU pragma: private, include <lox.h>

/** 
 An error type returned by lox
*/
typedef enum lox_error_t {
    /** 
     \n 
     */
    LOX_ERROR_OK, 
    /**
     Error occured during compile step.
    */
    LOX_ERROR_COMPILE, 
    /**
     Error occured during interpret step.
    */
    LOX_ERROR_INTERPRET, 
    /**
     Runtume vm stack overflowed
    */
    LOX_ERROR_STACK_OVERFLOW,
} lox_error_t;

#endif // !LOX_ERROR_H
