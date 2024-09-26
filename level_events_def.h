#ifndef LEVEL_DEF_H
#define LEVEL_DEF_H

typedef enum  {
  EVENT_TYPE_PLANET,
  EVENT_TYPE_NEBULA,
  EVENT_TYPE_ENEMY1,
  EVENT_TYPE_UNKNOWN = 255
} LEVEL_EVENT_TYPE;

typedef enum  {
  EVENT_STATUS_NOT_STARTED,
  EVENT_STATUS_ACTIVE,
  EVENT_STATUS_FINISHED,
} LEVEL_EVENT_STATUS;

typedef struct {
  int x;
  float y;
  int sprite_num;
  int speed;
} LEVEL_EVENT__PLANET;

typedef struct {
  int x;
  float y;
  int sprite_num;
  int speed;
} LEVEL_EVENT__ENEMY1;

typedef struct {
  int x;
  float y;
  int sprite_num;
} LEVEL_EVENT__NEBULA;

typedef struct {
  int id;
  LEVEL_EVENT_TYPE type;
  unsigned long clock;
  LEVEL_EVENT_STATUS status;
  void *event_data;
} LEVEL_EVENT;

typedef struct {
  LEVEL_EVENT *events;
  int count;
} LEVEL_DATA;

#endif
