#pragma once
#include <notcurses/notcurses.h>

typedef struct notcurses_options notcurses_options;
typedef struct notcurses notcurses;

notcurses* start_window();
void stop_window(notcurses*);
char get_key(notcurses*);
