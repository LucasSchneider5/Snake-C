#include <curses.h>
#include "worm.h"
#include "board_model.h"

extern void placeItem(int y, int x, chtype symbol, enum ColorPairs color_pair)
{
  move(y, x);
  attron(COLOR_PAIR(color_pair));
  addch(symbol);
  attroff(COLOR_PAIR(color_pair));
}

extern int getLastRow()
{
  return LINES-1;
}

extern int getLastCol()
{
  return COLS-1;
}
