#include "interface.h"

notcurses *start_window() {
  notcurses_options options = {
      .flags = NCOPTION_SUPPRESS_BANNERS,
  };

  notcurses *window = notcurses_init(&options, NULL);
  return window;
}

void stop_window(notcurses *window) { notcurses_stop(window); }

char get_key(notcurses* window) {
  ncinput nc_input;
  return notcurses_get(window, NULL, &nc_input);
}
