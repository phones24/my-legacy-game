#pragma once

#include "base_object.h"
#include "list.h"
#include "graphics_def.h"

typedef struct {
  BASE_OBJECT base;
} COL_OBJECT;

extern LIST *collision_objects;

void init_collision_list();
void add_object_to_collision_list(COL_OBJECT *object, ON_HIT_FN on_hit);
void remove_object_from_collision_list(COL_OBJECT *entity);
void clear_collision_list();

