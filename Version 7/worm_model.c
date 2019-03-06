#include <curses.h>
#include "worm.h"
#include "board_model.h"
#include "worm_model.h"

struct pos getWormHeadPos(struct worm* aworm)
{
  return aworm->wormpos[aworm->headindex];
}

extern enum ResCodes initializeWorm(struct worm* aworm, int len_max, struct pos headpos, enum WormHeading dir, enum ColorPairs color)
{
  int i;
  aworm->maxindex = len_max -1;
  aworm->headindex = 0;
  for(i = 0; i <= aworm->maxindex; i++)
  {
  aworm->wormpos[i].y = UNUSED_POS_ELEM;
  aworm->wormpos[i].x = UNUSED_POS_ELEM;
  }
  aworm->wormpos[aworm->headindex].x = getLastRow();
  aworm->wormpos[aworm->headindex].y = 0;
  setWormHeading(aworm, dir);
  aworm->wcolor = color;
  return RES_OK;
}

extern void showWorm(struct worm* aworm)
{
  placeItem(aworm->wormpos[aworm->headindex].x, aworm->wormpos[aworm->headindex].y, SYMBOL_WORM_INNER_ELEMENT, aworm->wcolor);
}

extern void cleanWormTail(struct worm* aworm)
{
  aworm->tailindex = (aworm->headindex + 1) % WORM_LENGTH;
  if(aworm->wormpos[aworm->tailindex].x >= aworm->maxindex)
      {
      placeItem(aworm->wormpos[aworm->tailindex].x, aworm->wormpos[aworm->tailindex].y, SYMBOL_FREE_CELL, COLP_FREE_CELL);
      }
}
extern void moveWorm(struct worm* aworm, enum GameStates *agame_state)
{
    struct pos headpos;
      headpos = aworm->wormpos[aworm->headindex];
      headpos.x += aworm->dy;
      headpos.y += aworm->dx;

      if(headpos.y < 0)
      {
        *agame_state = WORM_OUT_OF_BOUNDS;
      }
      else if(headpos.y > getLastCol())
      {
        *agame_state = WORM_OUT_OF_BOUNDS;
      }
      else if(headpos.x < 0)
      {
        *agame_state = WORM_OUT_OF_BOUNDS;
      }
      else if(headpos.x > getLastRow())
      {
        *agame_state = WORM_OUT_OF_BOUNDS;
      }
      else
      {
        if(isInUseByWorm(aworm, headpos))
        {
          *agame_state = WORM_CROSSING;
        }
      }
      if(*agame_state == WORM_GAME_ONGOING)
      {
        aworm->headindex++;
        if(aworm->headindex > aworm->maxindex)
        {
          aworm->headindex = 0;
        }
        aworm->wormpos[aworm->headindex] = headpos;
      }
}

extern bool isInUseByWorm(struct worm* aworm, struct pos new_headpos)
{
  int i;
  bool collision = false;
  i = aworm->headindex;
  do
  {
    if(aworm->wormpos[i].y == new_headpos.y && aworm->wormpos[i].x == new_headpos.x)
    {
      collision = true;
      break;
    }
    if(i == 0)
    {
      i = aworm->maxindex;
    }
    else
    {
      i --;
    }
  }
  while(i != aworm->headindex && aworm->wormpos[i].x != UNUSED_POS_ELEM);
  return collision;
}

extern void setWormHeading(struct worm* aworm, enum WormHeading dir)
{
  switch(dir)
  {
    case WORM_UP :
      aworm->dx = 0;
      aworm->dy = -1;
      break;
    case WORM_DOWN :
      aworm->dx = 0;
      aworm->dy = +1;
      break;
    case WORM_LEFT :
      aworm->dx = -1;
      aworm->dy = 0;
      break;
    case WORM_RIGHT :
      aworm->dx = +1;
      aworm->dy = 0;
      break;
  }
}









