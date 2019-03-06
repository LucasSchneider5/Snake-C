#ifndef _WORM_MODEL_H
#define _WORM_MODEL_H

#include <stdbool.h>
#include "worm.h"
#include "board_model.h"

struct worm
{
  int maxindex;
  int headindex;
  int tailindex;
  struct pos wormpos[WORM_LENGTH];
  int dx;
  int dy;
  enum ColorPairs wcolor;
};

enum WormHeading
{
  WORM_UP,
  WORM_DOWN,
  WORM_LEFT,
  WORM_RIGHT,
};

extern enum ResCodes initializeWorm(struct worm* aworm, int len_max, struct pos headpos, enum WormHeading dir, enum ColorPairs color);
extern void showWorm(struct worm* aworm);
extern void CleanWormTail(struct worm* aworm);
extern void moveWorm(struct worm* aworm, enum GameStates* agame_state);
extern bool isInUseByWorm(struct worm* aworm, struct pos new_headpos);
extern void setWormHeading(struct worm* aworm, enum WormHeading dir);
extern struct pos getWormHeadPos(struct worm* aworm);

#endif
