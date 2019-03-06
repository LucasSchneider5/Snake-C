#include <curses.h>
#include "worm.h"
#include "board_model.h"
#include "worm_model.h"

int theworm_maxindex;
int theworm_headindex;
int theworm_wormpos_x[WORM_LENGTH];
int theworm_wormpos_y[WORM_LENGTH];
int theworm_dx;
int theworm_dy;
enum ColorPairs theworm_wcolor;

extern enum ResCodes initializeWorm(int len_max, int headpos_x, int headpos_y, enum WormHeading dir, enum ColorPairs color)
{
  int i;
  theworm_maxindex = len_max -1;
  theworm_headindex = 0;
  for(i = 0; i <= theworm_maxindex; i++)
  {
  theworm_wormpos_x[i] = UNUSED_POS_ELEM;
  theworm_wormpos_y[i] = UNUSED_POS_ELEM;
  }
  theworm_wormpos_x[theworm_headindex] = getLastRow();
  theworm_wormpos_y[theworm_headindex] = 0;
  setWormHeading(dir);
  theworm_wcolor = color;
  return RES_OK;
}

extern void showWorm(enum ColorPairs color_pair)
{
  placeItem(theworm_wormpos_x[theworm_headindex], theworm_wormpos_y[theworm_headindex], SYMBOL_WORM_INNER_ELEMENT, theworm_wcolor);
}

extern void cleanWormTail()
{
  int tailindex;
  tailindex = (theworm_headindex + 1) % WORM_LENGTH;
  if(theworm_wormpos_x[tailindex] >= theworm_maxindex)
      {
      placeItem(theworm_wormpos_x[tailindex], theworm_wormpos_y[tailindex], SYMBOL_FREE_CELL, COLP_FREE_CELL);
      }
}

extern void moveWorm(enum GameStates *agame_state)
{
  int headpos_x;
  int headpos_y;
    headpos_x = theworm_wormpos_x[theworm_headindex] + theworm_dy;
    headpos_y = theworm_wormpos_y[theworm_headindex] + theworm_dx;
      if(headpos_y < 0)
      {
        *agame_state = WORM_OUT_OF_BOUNDS;
      }
      else if(headpos_y > getLastCol())
      {
        *agame_state = WORM_OUT_OF_BOUNDS;
      }
      else if(headpos_x < 0)
      {
        *agame_state = WORM_OUT_OF_BOUNDS;
      }
      else if(headpos_x > getLastRow())
      {
        *agame_state = WORM_OUT_OF_BOUNDS;
      }
      else
      {
        if(isInUseByWorm(headpos_y, headpos_x))
        {
          *agame_state = WORM_CROSSING;
        }
      }
      if(*agame_state == WORM_GAME_ONGOING)
      {
        int i;
        theworm_headindex = i;
        if(i <= theworm_maxindex)
        {
          i += 1;
        }
        else if(i == theworm_maxindex)
        {
          i = 0;
        }
        theworm_wormpos_x[theworm_headindex] = headpos_x;
        theworm_wormpos_y[theworm_headindex] = headpos_y;
      }
}

extern bool isInUseByWorm(int new_headpos_y, int new_headpos_x)
{
  int i;
  bool collision = false;
  i = theworm_headindex;
  do
  {
    if(theworm_wormpos_y[i] == new_headpos_y && theworm_wormpos_x[i] == new_headpos_x)
    {
      collision = true;
      break;
    }
    if(i == 0)
    {
      i = theworm_maxindex;
    }
    else
    {
      i --;
    }
  }
  while(i != theworm_headindex && theworm_wormpos_x[i] != UNUSED_POS_ELEM);
  return collision;
}

extern void setWormHeading(enum WormHeading dir)
{
  switch(dir)
  {
    case WORM_UP :
      theworm_dx = 0;
      theworm_dy = -1;
      break;
    case WORM_DOWN :
      theworm_dx = 0;
      theworm_dy = +1;
      break;
    case WORM_LEFT :
      theworm_dx = -1;
      theworm_dy = 0;
      break;
    case WORM_RIGHT :
      theworm_dx = +1;
      theworm_dy = 0;
      break;
  }
}









