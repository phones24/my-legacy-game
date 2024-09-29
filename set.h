#pragma once

#include <stdio.h>
#include <stdlib.h>

#define INITIAL_CAPACITY 10

typedef struct {
    int* data;
    int size;
    int capacity;
} SET;

SET* set_create();
void set_add(SET* set, int value);
int set_has(SET* set, int value);
void set_delete(SET* set, int value);
void set_free(SET* set);

SET* set_create() {
    SET* set = (SET*)malloc(sizeof(SET));
    set->data = (int*)malloc(INITIAL_CAPACITY * sizeof(int));
    set->size = 0;
    set->capacity = INITIAL_CAPACITY;
    return set;
}

void set_resize(SET* set, int new_capacity) {
    set->data = (int*)realloc(set->data, new_capacity * sizeof(int));
    set->capacity = new_capacity;
}

void set_add(SET* set, int value) {
    if (set_has(set, value)) {
      return;
    }

    if (set->size == set->capacity) {
        set_resize(set, set->capacity * 2);
    }

    set->data[set->size++] = value;
}

int set_has(SET* set, int value) {
    for (int i = 0; i < set->size; i++) {
        if (set->data[i] == value) {
            return 1;
        }
    }
    return 0;
}

void set_delete(SET* set, int value) {
    for (int i = 0; i < set->size; i++) {
        if (set->data[i] == value) {
            for (int j = i; j < set->size - 1; j++) {
                set->data[j] = set->data[j + 1];
            }
            set->size--;
            return;
        }
    }
}

void set_clear(SET* set) {
    set->size = 0;
}

void set_free(SET* set) {
    free(set->data);
    free(set);
}
