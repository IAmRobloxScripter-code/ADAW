#define LISYTH_IMPLEMENTATION
#include "lisyth/lisyth.h"

int main() {
  process* build = create_process();
  set_compiler(build, "gcc");
  set_output(build, "main");
  add_file(build, "main.c");
  add_file(build, "interface.c");
  add_file(build, "$(pkg-config --cflags --libs notcurses)");
  add_flag(build, "Wall");
  add_flag(build, "Wextra");
  compile(build);

  return 0;
}
