#include <stdio.h>
#include <stdlib.h>
#include <mem.h>

#include "event_planets.h"
#include "event_nebula.h"

#define MAX_CURRENT_EVENTS 5

int active_event_count = 0;
int next_event_num = 0;
LEVEL_EVENT active_events[100];
LEVEL_EVENT *current_events[MAX_CURRENT_EVENTS];

void process_EVENT_TYPE_PLANET(LEVEL_EVENT *event, char *event_data) {
  LEVEL_EVENT__PLANET *planet = (LEVEL_EVENT__PLANET *)malloc(sizeof(LEVEL_EVENT__PLANET));

  if(sscanf(event_data, "%d %d %d", &planet->sprite_num, &planet->x, &planet->speed) != 3) {
    printf("Error: Invalid planet event data\n");
    exit(1);
  }

  event->type = EVENT_TYPE_PLANET;
  event->event_data = (void *)planet;
}

void process_EVENT_TYPE_NEBULA(LEVEL_EVENT *event, char *event_data) {
  LEVEL_EVENT__NEBULA *nebula = (LEVEL_EVENT__NEBULA *)malloc(sizeof(LEVEL_EVENT__NEBULA));

  if(sscanf(event_data, "%d %d", &nebula->sprite_num, &nebula->x) != 2) {
    printf("Error: Invalid nebula event data\n");
    exit(1);
  }

  event->type = EVENT_TYPE_NEBULA;
  event->event_data = (void *)nebula;
}

void process_events(char *event_name, char *event_data, LEVEL_EVENT *event) {
  if(strcmp(event_name, "PLANET") == 0) {
    process_EVENT_TYPE_PLANET(event, event_data);
  }

  if(strcmp(event_name, "NEBULA") == 0) {
    process_EVENT_TYPE_NEBULA(event, event_data);
  }
}

LEVEL_EVENT read_event(char *line) {
  char event_name[20];
  char event_data[200];
  char* endptr;
  LEVEL_EVENT event;

  event.type = EVENT_TYPE_UNKNOWN;
  event.status = EVENT_STATUS_NOT_STARTED;

  char* token = strtok(line, " ");
  event.clock = strtoul(token, &endptr, 10);

  if (*endptr != '\0') {
    printf("Conversion failed, invalid character: %c\n", *endptr);
    exit(1);
  }

  token = strtok(NULL, " ");
  strcpy(event_name, token);

  char* remaining_data = strtok(NULL, "\n");
  if (remaining_data != NULL) {
    strcpy(event_data, remaining_data);
  }

  process_events(event_name, event_data, &event);

  return event;
}

void alloc_events_memory(LEVEL_DATA *level_data) {
  if(level_data->count == 0) {
    level_data->events = (LEVEL_EVENT *)malloc(sizeof(LEVEL_EVENT));
  } else {
    level_data->events = (LEVEL_EVENT *)realloc(level_data->events, (level_data->count + 1) * sizeof(LEVEL_EVENT));
  }

  if (level_data->events == NULL) {
    printf("Error: Could not allocate memory for level events\n");
    exit(1);
  }
}

LEVEL_DATA load_level_data(const char * filename) {
  FILE *file = fopen(filename, "r");

  if (!file) {
    printf("Error: Could not open level file %s\n", filename);
    exit(1);
  }

  LEVEL_DATA level_data;
  level_data.count = 0;
  level_data.events = NULL;

  char line[100];
  int event_id = 0;
  while (fgets(line, sizeof(line), file)) {
    if (line[0] == '#' || line[0] == '\n') {
      continue;
    }

    LEVEL_EVENT event = read_event(line);

    event.id = event_id++;

    if (event.type == EVENT_TYPE_UNKNOWN) {
      continue;
    }

    alloc_events_memory(&level_data);

    level_data.events[level_data.count] = event;
    level_data.count++;
  }

  fclose(file);

  return level_data;
}

void init_event(LEVEL_EVENT *event) {
  switch (event->type) {
    case EVENT_TYPE_PLANET:
      init_event__planet(event);
      break;
    case EVENT_TYPE_NEBULA:
      init_event__nebula(event);
      break;
    default:
      break;
  }
}

void draw_event(LEVEL_EVENT *event) {
  switch (event->type) {
    case EVENT_TYPE_PLANET:
      draw_event__planet(event);
      break;
    case EVENT_TYPE_NEBULA:
      draw_event__nebula(event);
      break;
    default:
      break;
  }
}

void start_event(LEVEL_EVENT *event) {
  switch (event->type) {
    case EVENT_TYPE_PLANET:
      start_event__planet(event);
      break;
    case EVENT_TYPE_NEBULA:
      start_event__nebula(event);
      break;
    default:
      break;
  }
}

void clear_event(LEVEL_EVENT *event) {
  switch (event->type) {
    case EVENT_TYPE_PLANET:
      clear_event__planet(event);
      break;
    case EVENT_TYPE_NEBULA:
      clear_event__nebula(event);
      break;
    default:
      break;
  }
}

void init_level(LEVEL_DATA level_data) {
  for (int i = 0; i < level_data.count; i++) {
    init_event(&level_data.events[i]);
  }
}

void check_active_events(LEVEL_DATA level_data) {
  for (int i = next_event_num; i < level_data.count && i < next_event_num + MAX_CURRENT_EVENTS; i++) {
    LEVEL_EVENT *event = &level_data.events[i];

    if (event->status != EVENT_STATUS_NOT_STARTED) {
      continue;
    }

    if (game_clock_ms >= event->clock) {
      event->status = EVENT_STATUS_ACTIVE;
      next_event_num = i + 1;
      current_events[active_event_count++] = event;

      start_event(event);
    }
  }
}

void clear_active_events() {
  for (int i = 0; i < active_event_count; i++) {
    if (current_events[i]->status == EVENT_STATUS_FINISHED) {
      current_events[i] = current_events[active_event_count - 1];
      active_event_count--;
      break;
    }
  }
}

void stop_event(LEVEL_EVENT *event) {
  event->status = EVENT_STATUS_FINISHED;
}

void draw_level(LEVEL_DATA level_data) {
  check_active_events(level_data);

  for (int i = 0; i < active_event_count; i++) {
    LEVEL_EVENT *event = current_events[i];
    draw_event(event);
  }

  clear_active_events();
}

void clear_level_data(LEVEL_DATA level_data) {
  for (int i = 0; i < level_data.count; i++) {
    clear_event(&level_data.events[i]);

    if (level_data.events[i].event_data != NULL) {
      free(level_data.events[i].event_data);
    }
  }

  free(level_data.events);
}


