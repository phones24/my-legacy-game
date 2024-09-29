#pragma once

#include "base_object.h"
#include "graphics_def.h"

typedef struct {
  BASE_OBJECT base;
} COL_OBJECT;

extern COL_OBJECT* collision_objects[100];
extern int collision_objects_count;

void add_object_to_collision_list(COL_OBJECT *object);
void remove_object_from_collision_list(COL_OBJECT *entity);

