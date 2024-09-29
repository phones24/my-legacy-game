#include "collision.h"

COL_OBJECT* collision_objects[100];
int collision_objects_count = 0;

void add_object_to_collision_list(COL_OBJECT *object) {
  if (collision_objects_count >= 100) {
    return;
  }

  collision_objects[collision_objects_count++] = object;
}

void remove_object_from_collision_list(COL_OBJECT *entity) {
  for (int i = 0; i < collision_objects_count; i++) {
    if (collision_objects[i]->base.id == entity->base.id) {
      collision_objects[i] = collision_objects[collision_objects_count - 1];
      collision_objects_count--;
      return;
    }
  }
}
