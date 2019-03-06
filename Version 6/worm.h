#ifndef _WORM_H
#define _WORM_H

#define NAP_TIME 100
#define MIN_NUMBER_OF_ROWS 3
#define MIN_NUMBER_OF_COLS 10
#define WORM_LENGTH 20
#define UNUSED_POS_ELEM -1
#define SYMBOL_FREE_CELL ' '
#define SYMBOL_WORM_INNER_ELEMENT '0'

enum ResCodes
{
  RES_OK,
  RES_FAILED,
};

enum ColorPairs
{
  COLP_USER_WORM = 1,
  COLP_FREE_CELL,
};

enum GameStates
{
  WORM_GAME_ONGOING,
  WORM_OUT_OF_BOUNDS,
  WORM_CROSSING,
  WORM_GAME_QUIT,
};

#endif
