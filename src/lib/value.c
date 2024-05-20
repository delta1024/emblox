#include "value.h"
#include "common.h"
#include "memory.h"
#include <stdio.h>

void lox_value_arrayinit(lox_value_array *array) {
    array->count = array->capacity = 0;
    array->entries                 = NULL;
}

void lox_value_arrayfree(lox_value_array *array, lox_memory *allocator) {
    lox_allocate(allocator, array->entries,
                 sizeof(lox_value) * array->capacity, 0);
    lox_value_arrayinit(array);
}
void lox_value_arraywrite(lox_value_array *array, lox_value val,
                          lox_memory *allocator) {
    if (array->count + 1 > array->capacity) {
        int ocap        = array->capacity;
        array->capacity = ocap < 8 ? 8 : ocap * 2;
        array->entries  = lox_allocate(allocator, array->entries,
                                       sizeof(lox_value) * ocap,
                                       sizeof(lox_value) * array->capacity);
    }
    array->entries[array->count++] = val;
}
void lox_value_print(lox_value val) {
    printf("%g", val);
}
