#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_CAPACITY 10

typedef struct {
    void* data;
    int size;
    int capacity;
    size_t elem_size;
} ARRAY;

ARRAY* array_create(size_t elem_size) {
    ARRAY* array = (ARRAY*)malloc(sizeof(ARRAY));
    array->data = malloc(INITIAL_CAPACITY * elem_size);
    array->size = 0;
    array->capacity = INITIAL_CAPACITY;
    array->elem_size = elem_size;
    return array;
}

void array_resize(ARRAY* array, int new_capacity) {
    array->data = realloc(array->data, new_capacity * array->elem_size);
    array->capacity = new_capacity;
}

void array_add(ARRAY* array, const void* value) {
    if (array->size == array->capacity) {
        array_resize(array, array->capacity * 2);
    }
    memcpy((char*)array->data + array->size * array->elem_size, value, array->elem_size);
    array->size++;
}

void* array_get(ARRAY* array, int index) {
    if (index < 0 || index >= array->size) {
        return NULL;
    }
    return (char*)array->data + index * array->elem_size;
}

void array_remove(ARRAY* array, int index) {
    if (index < 0 || index >= array->size) {
        return;
    }
    memmove((char*)array->data + index * array->elem_size,
            (char*)array->data + (index + 1) * array->elem_size,
            (array->size - index - 1) * array->elem_size);
    array->size--;
}

void array_clear(ARRAY* array) {
    array->size = 0;
}

void array_free(ARRAY* array) {
    free(array->data);
    free(array);
}
