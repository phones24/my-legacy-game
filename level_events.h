#pragma once

typedef enum  {
  EVENT_TYPE_PLANET,
  EVENT_TYPE_NEBULA,
  EVENT_TYPE_ENEMY1,
  EVENT_TYPE_ROCK,
  EVENT_TYPE_UNKNOWN = 255
} LEVEL_EVENT_TYPE;

typedef enum  {
  EVENT_STATUS_NOT_STARTED,
  EVENT_STATUS_ACTIVE,
  EVENT_STATUS_FINISHED,
} LEVEL_EVENT_STATUS;

typedef struct {
  int id;
  LEVEL_EVENT_TYPE type;
  LEVEL_EVENT_STATUS status;
  unsigned long clock;
  int event_data[10];
  int event_data_size;
} LEVEL_EVENT;

typedef struct {
  LEVEL_EVENT events[100];
  int count;
} LEVEL_DATA;

extern LEVEL_DATA level_inline;
