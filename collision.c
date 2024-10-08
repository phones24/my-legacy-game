#include "collision.h"
#include "list.h"

LIST *collision_objects;
static unsigned long last_id = 0;

void init_collision_list() {
  collision_objects = list_create(sizeof(COL_OBJECT));
}

void add_object_to_collision_list(COL_OBJECT *object, ON_HIT_FN on_hit) {
  if (collision_objects->size >= 100) {
    return;
  }

  object->base.collision_id = last_id++;
  object->base.on_hit = on_hit;

  list_add(collision_objects, object);
}

void remove_object_from_collision_list(COL_OBJECT *entity) {
  for (int i = 0; i < collision_objects->size; i++) {
    COL_OBJECT* object = list_get(collision_objects, i);
    if (object->base.collision_id == entity->base.collision_id) {
      list_remove(collision_objects, i);
      return;
    }
  }
}

void clear_collision_list() {
  list_free(collision_objects);
}
