#include <curses.h>
#include "prep.h"

extern void initializeCursesApplication()
{
  initscr();
  noecho();
  cbreak();
  nonl();
  keypad(stdscr, TRUE);
  curs_set(0);
  nodelay(stdscr, TRUE);
}

extern void cleanupCursesApp(void)
{
  standend();
  refresh();
  curs_set(1);
  endwin();
}
