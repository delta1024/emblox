#include "value.h"
#include "memory.h"
#include <stdio.h>

void lox_value_print(lox_value val) {
    printf("%g", val);
}

void lox_value_array_init(lox_value_array *array) {
    array->count = array->capacity = 0;
    array->entries = NULL;
}

void lox_value_array_write(lox_value_array *array, lox_value value, struct memory_tracker *tracker) {
    if (array->count + 1 > array->capacity) {
        int old_cap = array->capacity;
        array->capacity = old_cap < 8 ? 8 : old_cap * 2;
        array->entries = lox_reallocarray(array->entries, array->capacity, old_cap, sizeof(lox_value), tracker);
    }
    array->entries[array->count++] = value;

}
void lox_value_array_free(lox_value_array *array, struct memory_tracker *tracker) {
    lox_free(array->entries, arraysize(lox_value, array->capacity), tracker);
    lox_value_array_init(array);
}
