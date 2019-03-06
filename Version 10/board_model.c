#include <string.h>
#include <curses.h>
#include "worm.h"
#include "board_model.h"
#include "messages.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

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
  int y;
  aboard->last_row = LINES - ROWS_RESERVED -1;
  aboard->last_col = COLS -1;
  if(aboard->last_col < MIN_NUMBER_OF_COLS -1 || aboard->last_row < MIN_NUMBER_OF_ROWS -1)
  {
    char buf[100];
    sprintf(buf, "Das Fenster ist zu klein: Wir brauchen %d x %d", MIN_NUMBER_OF_COLS, MIN_NUMBER_OF_ROWS + ROWS_RESERVED);
    showDialog(buf, "Bitte eine Taste druecken");
    return RES_FAILED;
  }
  aboard->cells = malloc(sizeof(enum BoardCodes*) * (LINES - ROWS_RESERVED));
  if(aboard->cells == NULL)
  {
    showDialog("Abbruch: Zu wenig Speicher", "Bitte eine Taste druecken");
    exit(RES_FAILED);
  }
  for(y = 0; y < LINES - ROWS_RESERVED; y++)
  {
    aboard->cells[y] = malloc(sizeof(enum BoardCodes) * COLS);
    if(aboard->cells[y] == NULL)
    {
      showDialog("Abbruch: Zu wenig Speicher", "Bitte eine Taste druecken");
      exit(RES_FAILED);
    }
  }
  return RES_OK;
}
enum ResCodes initializeLevelFromFile(struct board* aboard, const char* filename)
{
  int y;
  int x;
  int rownr;
  char buf[100];
  FILE* in;
  for(y = 0; y <= aboard->last_row; y++)
  {
    for(x = 0; x <= aboard->last_col; x++)
    {
      placeItem(aboard, y, x, BC_FREE_CELL, SYMBOL_FREE_CELL, COLP_FREE_CELL);
    }
  }
  aboard->food_items = 0;
  int bufsize = aboard->last_col + 3;
  char* buffer;
  if((buffer = malloc(sizeof(char) * bufsize)) == NULL)
  {
    sprintf(buf, "Kein Speicher mehr in initializeLevelFromFile\n");
    showDialog(buf, "Bitte eine Taste druecken");
    return RES_FAILED;
  }
  if((in = fopen(filename, "r")) == NULL)
  {
    sprintf(buf, "Kann Datei %s nicht oeffnen", filename);
    showDialog(buf, "Bitte eine Taste druecken");
    return RES_FAILED;
  }
  rownr = 0;
  while(rownr < aboard->last_row+1 && ! feof(in))
  {
    int len;
    if(fgets(buffer, bufsize, in) != NULL)
    {
      len = strlen(buffer);
      if(buffer[len - 1] != '\n')
      {
        buffer[len - 1] = '\0';
        while(fgetc(in) != '\n') {;}
      }
      else
      {
        buffer[len - 1] = '\0';
      }
    }
    else
      {
        if(!feof(in))
        {
          char buf[100];
          sprintf(buf, "Fehler beim Lesen von Zeile %d aus Datei %s", rownr + 1, filename);
          showDialog(buf, "Bitte eine Taste druecken");
          return RES_FAILED;
        }
        else
        {
          continue;
        }
      }
      len = strlen(buffer);
      for(x = 0; x <= aboard->last_col && x < len; x++)
      {
        switch(buffer[x])
        {
          case SYMBOL_BARRIER:
            placeItem(aboard, rownr, x, BC_BARRIER, SYMBOL_BARRIER, COLP_BARRIER);
            break;
          case SYMBOL_FOOD_1:
            placeItem(aboard, rownr, x, BC_FOOD_1, SYMBOL_FOOD_1, COLP_FOOD_1);
            aboard->food_items++;
            break;
          case SYMBOL_FOOD_2:
            placeItem(aboard, rownr, x, BC_FOOD_2, SYMBOL_FOOD_2, COLP_FOOD_2);
            aboard->food_items++;
            break;
          case SYMBOL_FOOD_3:
            placeItem(aboard, rownr, x, BC_FOOD_3, SYMBOL_FOOD_3, COLP_FOOD_3);
            aboard->food_items++;
            break;
        }
      }
      rownr ++;
    }
    free(buffer);
    for(x = 0; x <= aboard->last_col; x++)
    {
      move(aboard->last_row + 1, x);
      attron(COLOR_PAIR(COLP_BARRIER));
      addch(SYMBOL_BARRIER);
      attroff(COLOR_PAIR(COLP_BARRIER));
    }
  fclose(in);
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

void cleanupBoard(struct board* aboard)
{
  free(aboard->cells);
}
