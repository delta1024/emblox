#include "value.h"
#include "common.h"
#include "memory.h"
#include <stdio.h>

void lox_value_array_init(lox_value_array *array) {
    array->count = array->capacity = 0;
    array->entries                 = NULL;
}

void lox_value_array_free(lox_value_array *array, lox_memory *allocator) {
    allocator->f(allocator->ud, array->entries, sizeof(value) * array->capacity,
                 0);
    lox_value_array_init(array);
}
void lox_value_array_write(lox_value_array *array, value val,
                           lox_memory *allocator) {
    if (array->count + 1 > array->capacity) {
        int ocap        = array->capacity;
        array->capacity = ocap < 8 ? 8 : ocap * 2;
        array->entries =
            allocator->f(allocator->ud, array->entries, sizeof(value) * ocap,
                         sizeof(value) * array->capacity);
    }
    array->entries[array->count++] = val;
}
void lox_print_value(value val) {
    printf("%g", val);
}
