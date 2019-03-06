#include <curses.h>
#include "worm.h"
#include "board_model.h"
#include "messages.h"

void placeItem(struct board* aboard, int y, int x, enum BoardCodes board_code, chtype symbol, enum ColorPairs color_pair)
{
  move(y, x);
  attron(COLOR_PAIR(color_pair));
  addch(symbol);
  attroff(COLOR_PAIR(color_pair));
  aboard->cells[y][x] = board_code; 
}

int getLastRowOnBoard(struct board* aboard)
{
  return aboard->last_row;
}

int getLastColOnBoard(struct board* aboard)
{
  return aboard->last_col;
}

int getNumberOfFoodItems(struct board* aboard)
{
  return aboard->food_items;
}

enum BoardCodes getContentAt(struct board* aboard, struct pos position)
{
  return aboard->cells[position.y][position.x];
}

void setNumberOfFoodItems(struct board* aboard, int n)
{
  n = aboard->food_items;
}

void decrementNumberOfFoodItems(struct board* aboard)
{
  aboard->food_items = aboard->food_items -1; 
}

enum ResCodes initializeBoard(struct board* aboard)
{
  if(COLS < MIN_NUMBER_OF_COLS || LINES < MIN_NUMBER_OF_ROWS + ROWS_RESERVED)
  {
    char buf[100];
    sprintf(buf,"Das Fenster ist zu klein: wir brauchen %d x %d", MIN_NUMBER_OF_COLS, MIN_NUMBER_OF_ROWS + ROWS_RESERVED);
    showDialog(buf,"Bitte eine Taste druecken");
    return RES_FAILED;
  }
  aboard->last_row = MIN_NUMBER_OF_ROWS -1;
  aboard->last_col = MIN_NUMBER_OF_COLS -1;
  return RES_OK;
}

enum ResCodes initializeLevel(struct board* aboard)
{
  int x;
  int y;
  for(y = 0; y <= aboard->last_row; y++)
  {
    for(x = 0; x <= aboard->last_col; x++)
    {
      placeItem(aboard, y, x, BC_FREE_CELL, SYMBOL_FREE_CELL, COLP_FREE_CELL);
    }
  }
  y = aboard->last_row +1;
  for(x = 0; x <= aboard->last_col; x++)
  {
    move(y, x);
    attron(COLOR_PAIR(COLP_BARRIER));
    addch(SYMBOL_BARRIER);
    attroff(COLOR_PAIR(COLP_BARRIER));
  }
  for(y = 0; y <= aboard->last_row; y++)
  {
    placeItem(aboard, y, aboard->last_col, BC_BARRIER, SYMBOL_BARRIER, COLP_BARRIER);
  }
  for(y = 10; y <= 20; y++)
  {
    x = 10;
    placeItem(aboard, y, x, BC_BARRIER, SYMBOL_BARRIER, COLP_BARRIER);
  }
  for(y = 5; y <= 15; y++)
  {
    x = 50;
    placeItem(aboard, y, x, BC_BARRIER, SYMBOL_BARRIER, COLP_BARRIER);
  }
  placeItem(aboard, 10, 48, BC_FOOD_1, SYMBOL_FOOD_1, COLP_FOOD_1);
  placeItem(aboard, 17, 29, BC_FOOD_1, SYMBOL_FOOD_1, COLP_FOOD_1);
  placeItem(aboard, 13, 45, BC_FOOD_2, SYMBOL_FOOD_2, COLP_FOOD_2);
  placeItem(aboard, 22, 22, BC_FOOD_2, SYMBOL_FOOD_2, COLP_FOOD_2);
  placeItem(aboard, 15, 56, BC_FOOD_2, SYMBOL_FOOD_2, COLP_FOOD_2);
  placeItem(aboard, 9, 68, BC_FOOD_2, SYMBOL_FOOD_2, COLP_FOOD_2);
  placeItem(aboard, 8, 27, BC_FOOD_3, SYMBOL_FOOD_3, COLP_FOOD_3);
  placeItem(aboard, 20, 28, BC_FOOD_3, SYMBOL_FOOD_3, COLP_FOOD_3);
  placeItem(aboard, 4, 57, BC_FOOD_3, SYMBOL_FOOD_3, COLP_FOOD_3);
  placeItem(aboard, 6, 4, BC_FOOD_3, SYMBOL_FOOD_3, COLP_FOOD_3);
  aboard->food_items = 10;
return RES_OK;
}

