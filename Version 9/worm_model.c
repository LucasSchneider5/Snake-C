#include <curses.h>
#include "worm.h"
#include "board_model.h"
#include "worm_model.h"

struct pos getWormHeadPos(struct worm* aworm)
{
  return aworm->wormpos[aworm->headindex];
}

extern enum ResCodes initializeWorm(struct worm* aworm, int len_max, int len_cur, struct pos headpos, enum WormHeading dir, enum ColorPairs color)
{
  int i;
  aworm->maxindex = len_max -1;
  aworm->cur_lastindex = len_cur -1;
  aworm->headindex = 0;
  for(i = 0; i <= aworm->maxindex; i++)
  {
  aworm->wormpos[i].y = UNUSED_POS_ELEM;
  aworm->wormpos[i].x = UNUSED_POS_ELEM;
  }
  aworm->wormpos[aworm->headindex] = headpos;
  setWormHeading(aworm, dir);
  aworm->wcolor = color;
  return RES_OK;
}

extern void showWorm(struct board* aboard, struct worm* aworm)
{
  int tailindex = (aworm->headindex +1) % (aworm->cur_lastindex +1);
  int i = aworm->headindex;
  do
  {
    char n;
    if(i == aworm->headindex)
    {
      n = SYMBOL_WORM_HEAD_ELEMENT;
    }
    else if(i == tailindex)
    {
      n = SYMBOL_WORM_TAIL_ELEMENT;
    }
    else 
    {
      n = SYMBOL_WORM_INNER_ELEMENT;
    }
    placeItem(aboard, aworm->wormpos[i].y, aworm->wormpos[i].x, BC_USED_BY_WORM, n, aworm->wcolor);
    if(i == 0)
    {
      i = aworm->cur_lastindex;
    }
    else
    {
      i --;
    }
  }
  while(aworm->headindex != i && aworm->wormpos[i].x != UNUSED_POS_ELEM);
}

extern void cleanWormTail(struct board* aboard, struct worm* aworm)
{
  aworm->tailindex = (aworm->headindex + 1) % (aworm->cur_lastindex +1);
  if(aworm->wormpos[aworm->tailindex].y != UNUSED_POS_ELEM)
      {
      placeItem(aboard, aworm->wormpos[aworm->tailindex].y, aworm->wormpos[aworm->tailindex].x, BC_FREE_CELL, SYMBOL_FREE_CELL, COLP_FREE_CELL);
      }
}
extern void moveWorm(struct board* aboard, struct worm* aworm, enum GameStates *agame_state)
{
    struct pos headpos;
      headpos = aworm->wormpos[aworm->headindex];
      headpos.x += aworm->dx;
      headpos.y += aworm->dy;

      if(headpos.x < 0)
      {
        *agame_state = WORM_OUT_OF_BOUNDS;
      }
      else if(headpos.x > getLastColOnBoard(aboard))
      {
        *agame_state = WORM_OUT_OF_BOUNDS;
      }
      else if(headpos.y < 0)
      {
        *agame_state = WORM_OUT_OF_BOUNDS;
      }
      else if(headpos.y > getLastRowOnBoard(aboard))
      {
        *agame_state = WORM_OUT_OF_BOUNDS;
      }
      else
      {
        switch(getContentAt(aboard, headpos))
        {
          case BC_FOOD_1:
            *agame_state = WORM_GAME_ONGOING;
            growWorm(aworm, BONUS_1);
            decrementNumberOfFoodItems(aboard);
            break;
          case BC_FOOD_2:
            *agame_state = WORM_GAME_ONGOING;
            growWorm(aworm, BONUS_2);
            decrementNumberOfFoodItems(aboard);
            break;
          case BC_FOOD_3:
            *agame_state = WORM_GAME_ONGOING;
            growWorm(aworm, BONUS_3);
            decrementNumberOfFoodItems(aboard);
            break;
          case BC_BARRIER:
            *agame_state = WORM_CRASH;
            break;
          case BC_USED_BY_WORM:
            *agame_state = WORM_CROSSING;
            break;
          default:
            {;}
        }
      }
      if(*agame_state == WORM_GAME_ONGOING)
      {
       aworm->headindex++;
       if(aworm->headindex > aworm->cur_lastindex)
       {
         aworm->headindex = 0;
       }
       aworm->wormpos[aworm->headindex] = headpos;
      }
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

extern int getWormLength(struct worm* aworm)
{
  return aworm->cur_lastindex;
}

extern void growWorm(struct worm* aworm, enum Boni growth)
{
  if(aworm->cur_lastindex + growth <= aworm->maxindex)
  {
    aworm->cur_lastindex += growth;
  }
  else
  {
    aworm->cur_lastindex = aworm->maxindex;
  }
}









