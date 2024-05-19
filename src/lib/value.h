#ifndef _LOX_VALUE_H
#define _LOX_VALUE_H
#include "memory.h"
#include <lox.h>
typedef lox_number value;

typedef struct lox_value_array {
    int count;
    int capacity;
    value *entries;
} lox_value_array;

void lox_value_array_init(lox_value_array *array);
void lox_value_array_free(lox_value_array *array, lox_memory *allocator);
void lox_value_array_write(lox_value_array *array, value val,
                           lox_memory *allocator);
void lox_print_value(value val);
#endif // !_LOX_VALUE_H
