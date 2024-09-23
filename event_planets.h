#ifndef EVENT_PLANETS_H
#define EVENT_PLANETS_H

SPRITE3 planets_sprite;

void init_event__planet(LEVEL_EVENT *event) {
  planets_sprite = load_sprite("planets");
}

void start_event__planet(LEVEL_EVENT *event) {
  LEVEL_EVENT__PLANET *cur_evevnt = (LEVEL_EVENT__PLANET *)event->event_data;

  cur_evevnt->y = 0 - planets_sprite.height[cur_evevnt->sprite_num];
}

void draw_event__planet(LEVEL_EVENT *event) {
  LEVEL_EVENT__PLANET *cur_event = (LEVEL_EVENT__PLANET *)event->event_data;

  if (cur_event->y > SCREEN_HEIGHT) {
    event->status = EVENT_STATUS_FINISHED;
  }

  draw_sprite(planets_sprite, cur_event->sprite_num, cur_event->x, cur_event->y);

  cur_event->y += cur_event->speed;
}

void clear_event__planet(LEVEL_EVENT *event) {
  clear_sprite(planets_sprite);
}

#endif
