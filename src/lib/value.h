#ifndef _LOX_VALUE_H
#define _LOX_VALUE_H
#include "memory.h"
#include <lox.h>
typedef lox_number lox_value;

typedef struct lox_value_array {
    int count;
    int capacity;
    lox_value *entries;
} lox_value_array;

void lox_value_arrayinit(lox_value_array *array);
void lox_value_arrayfree(lox_value_array *array, lox_memory *allocator);
void lox_value_arraywrite(lox_value_array *array, lox_value val,
                          lox_memory *allocator);
void lox_value_print(lox_value val);
#endif // !_LOX_VALUE_H
