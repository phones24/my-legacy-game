#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    void** items;
    int size;
    int capacity;
} LIST;

inline LIST* list_create() {
    LIST* list = malloc(sizeof(LIST));
    if (!list) return NULL;

    list->capacity = 2;
    list->size = 0;
    list->items = malloc(list->capacity * sizeof(void*));

    if (!list->items) {
        free(list);
        return NULL;
    }

    return list;
}

inline void list_add(LIST* list, void* value) {
    if (!list) {
      return;
    }

    if (list->size == list->capacity) {
        int new_capacity = list->capacity * 2;
        void** new_items = realloc(list->items, new_capacity * sizeof(void*));

        if (!new_items) {
          return;
        }

        list->items = new_items;
        list->capacity = new_capacity;
    }

    list->items[list->size++] = value;
}

inline void* list_get(LIST* list, int index) {
    if (!list || index >= list->size) return NULL;
    return list->items[index];
}

inline void list_remove(LIST* list, int index) {
    if (!list || index >= list->size) {
      return;
    }

    for (int i = index; i < list->size - 1; i++) {
        list->items[i] = list->items[i + 1];
    }

    list->size--;
}

inline void list_free(LIST* list) {
    if (!list) {
      return;
    }

    free(list->items);
    free(list);
}
