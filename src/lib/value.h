#ifndef _LOX_VALUE_H
#define _LOX_VALUE_H

#include "lox/value.h"
#include "memory.h"
typedef lox_number_t lox_value;

typedef struct lox_value_array {
    int count;
    int capacity;
    lox_value *entries;
} lox_value_array;

void lox_value_array_init(lox_value_array *array);
void lox_value_array_write(lox_value_array *array, lox_value value,
                           struct memory_tracker *tracker);
void lox_value_array_free(lox_value_array *array,
                          struct memory_tracker *tracker);

void lox_value_print(lox_value val);

#endif // !_LOX_VALUE_H
