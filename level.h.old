#ifndef LEVEL_H
#define LEVEL_H

#include "level_events_def.h"

void process_EVENT_TYPE_PLANET(LEVEL_EVENT *event, char *event_data);
void process_EVENT_TYPE_NEBULA(LEVEL_EVENT *event, char *event_data);
void process_events(char *event_name, char *event_data, LEVEL_EVENT *event);
LEVEL_DATA load_level_data(const char * filename);
void init_level(LEVEL_DATA level_data);
void draw_level(LEVEL_DATA level_data);
void clear_level_data(LEVEL_DATA level_data);
void stop_event(LEVEL_EVENT *event);

#endif
