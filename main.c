#include "interface.h"

int main() {
  notcurses* window = start_window();
  if (!window) return 1;
  
  while (1) {
    char key = get_key(window);
    if (key == 'q') break;
    if (key < 0) break;
  }

  return 0;
}  
