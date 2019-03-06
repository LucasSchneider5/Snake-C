#ifndef _BOARD_MODEL_H
#define _BOARD_MODEL_H

#include <curses.h>
#include "worm.h"

struct pos
{
  int y;
  int x;
};

extern void placeItem(int y, int x, chtype symbol, enum ColorPairs color_pair);
extern int getLastRow();
extern int getLastCol();

#endif
