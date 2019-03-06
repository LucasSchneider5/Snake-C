//Bibliotheken
#include <curses.h>                              
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

//Definitionen
#define NAP_TIME    100   
#define MIN_NUMBER_OF_ROWS  3   
#define MIN_NUMBER_OF_COLS 10   
#define SYMBOL_WORM_INNER_ELEMENT '0'
#define WORM_LENGTH 20
#define UNUSED_POS_ELEM -1
#define SYMBOL_FREE_CELL ' '

//Enumerationen
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
enum WormHeading
{
WORM_UP,
WORM_DOWN,
WORM_LEFT,
WORM_RIGHT,
};

//Deklarationen
int theworm_wormpos_y[WORM_LENGTH];
int theworm_wormpos_x[WORM_LENGTH];
int theworm_maxindex;
int theworm_headindex;
int theworm_dx;
int theworm_dy;
enum ColorPairs theworm_wcolor; 
void initializeColors();
void readUserInput(enum GameStates* agame_state);
enum ResCodes doLevel();
void initializeCursesApplication(); 
void cleanupCursesApp(void);
void placeItem(int y, int x, chtype symbol, enum ColorPairs color_pair);
int getLastRow();
int getLastCol();
enum ResCodes initializeWorm(int len_max, int headpos_y, int headpos_x, enum WormHeading dir, enum ColorPairs color);
void showWorm();
void cleanWormTail();
void moveWorm(enum GameStates* agame_state);
bool isInUseByWorm(int new_headpos_y, int new_headpos_x);
void setWormHeading(enum WormHeading dir);

//Initialisierung der Farben
void initializeColors() 
{
    start_color();
    init_pair(COLP_USER_WORM, COLOR_GREEN, COLOR_BLACK);
    init_pair(COLP_FREE_CELL, COLOR_BLACK, COLOR_BLACK);
}

//Tasteneingaben
void readUserInput(enum GameStates* agame_state) 
{
    int ch; 
    if ((ch = getch()) > 0) {
        switch(ch) 
        {
            case 'q' :   
                *agame_state = WORM_GAME_QUIT;
                break;
            case KEY_UP :
                setWormHeading(WORM_UP);
                break;
            case KEY_DOWN :
                setWormHeading(WORM_DOWN);
                break;
            case KEY_LEFT :
                setWormHeading(WORM_LEFT);
                break;
            case KEY_RIGHT :
                setWormHeading(WORM_RIGHT);
                break;
            case 's' :
                nodelay(stdscr, FALSE);  
                break;
            case ' ' :
                nodelay(stdscr, TRUE);
                break;
        }
    }
    return;
}

//Spielstand
enum ResCodes doLevel() 
{
    enum GameStates agame_state;
    enum ResCodes res_code; 
    bool end_level_loop;   
    int bottomLeft_y, bottomLeft_x;

    agame_state = WORM_GAME_ONGOING;
    bottomLeft_y =  getLastRow();
    bottomLeft_x =  0;
    res_code = initializeWorm(WORM_LENGTH, bottomLeft_y, bottomLeft_x , WORM_RIGHT, COLP_USER_WORM);
    if ( res_code != RES_OK) 
    {
        return res_code;
    }
    showWorm();
    refresh();
    end_level_loop = false;
    while(!end_level_loop) 
    {
        readUserInput(&agame_state); 
        if (agame_state == WORM_GAME_QUIT) 
        {
            end_level_loop = true;
            continue;
        }
        cleanWormTail();
        moveWorm(&agame_state);
        if (agame_state != WORM_GAME_ONGOING) 
        {
            end_level_loop = true;
            continue;
        }
        showWorm();
        napms(NAP_TIME);
        refresh();
    }
    res_code = RES_OK;
    return 78;
}

//NCURSES
void initializeCursesApplication() 
{
    initscr();
    noecho(); 
    cbreak(); 
    nonl();   
    keypad(stdscr, TRUE);
    curs_set(0);         
    nodelay(stdscr, TRUE); 
}

void cleanupCursesApp(void)
{
    standend();  
    refresh();   
    curs_set(1); 
    endwin();    
}

//Wurm
void placeItem(int y, int x, chtype symbol, enum ColorPairs color_pair) 
{
    move(y, x);                     
    attron(COLOR_PAIR(color_pair)); 
    addch(symbol);                    
    attroff(COLOR_PAIR(color_pair));
}

//Spielfeld
int getLastRow() 
{
    return LINES-1;
}

//Spielfeld
int getLastCol() 
{
    return COLS-1;
}

//Wurm
enum ResCodes initializeWorm(int len_max, int headpos_y, int headpos_x, enum WormHeading dir, enum ColorPairs color) 
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
    
    //Richtung (Wurm)
    setWormHeading(dir);
    theworm_wcolor = color;
    return RES_OK;
}

//Wurm
void showWorm(enum ColorPairs color_pair) 
{
    placeItem(theworm_wormpos_x[theworm_headindex], theworm_wormpos_y[theworm_headindex], SYMBOL_WORM_INNER_ELEMENT,theworm_wcolor);
}

void cleanWormTail()
{
    int tailindex;
    tailindex = (theworm_headindex + 1) % WORM_LENGTH;
    if(theworm_wormpos_x[tailindex] >= theworm_maxindex)     
    {
      placeItem(theworm_wormpos_x[tailindex], theworm_wormpos_y[tailindex], SYMBOL_FREE_CELL, COLP_FREE_CELL);           
    }
}

//Wurm (Bewegung)
void moveWorm(enum GameStates *agame_state) 
{
  int headpos_x;
  int headpos_y;
   headpos_x = theworm_wormpos_x[theworm_headindex] + theworm_dy;       
   headpos_y = theworm_wormpos_y[theworm_headindex] + theworm_dx;          
    if (headpos_y < 0)
    {
        *agame_state = WORM_OUT_OF_BOUNDS;
    } 
    else if (headpos_y > getLastCol() ) 
    { 
        *agame_state = WORM_OUT_OF_BOUNDS;
    } 
    else if (headpos_x < 0)
    {
        *agame_state = WORM_OUT_OF_BOUNDS;
    } 
    else if (headpos_x > getLastRow() )
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
     /* if(theworm_headindex == WORM_LENGTH)
      {
        theworm_headindex = 0;
      }
      else
      {
        theworm_headindex += 1;
      }
      theworm_wormpos_x[theworm_headindex] = headpos_x;
      theworm_wormpos_y[theworm_headindex] = headpos_y;*/
     int n;
     theworm_headindex = n;
     if(n <= theworm_maxindex)
      {
      n += 1;
      }
      else if(n == theworm_maxindex)
      {
      n = 0;
      }
      theworm_wormpos_x[theworm_headindex] = headpos_x;       
      theworm_wormpos_y[theworm_headindex] = headpos_y;
    }
}


bool isInUseByWorm(int new_headpos_y, int new_headpos_x)
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

//Wurm (Richtung)
void setWormHeading(enum WormHeading dir)
{
    switch(dir) 
    {
        case WORM_UP :
            theworm_dx=0;
            theworm_dy=-1;
            break;
        case WORM_DOWN :
            theworm_dx=0;
            theworm_dy=+1;
            break;
        case WORM_LEFT :
            theworm_dx=-1;
            theworm_dy=0;
            break;
        case WORM_RIGHT :
            theworm_dx=+1;
            theworm_dy=0;
            break;
    }
} 

//Fehlercode (Terminal zu klein)
int main(void)
    {
    int res_code;
    initializeCursesApplication();
    initializeColors();
    if ( LINES < MIN_NUMBER_OF_ROWS || COLS < MIN_NUMBER_OF_COLS )
    {
        cleanupCursesApp();
        printf("Das Fenster ist zu klein: wir brauchen mindestens %dx%d\n",
                MIN_NUMBER_OF_COLS, MIN_NUMBER_OF_ROWS );
        res_code = RES_FAILED;
    }
    else
    {
    //  nodelay(stdscr, false);
        res_code = doLevel();
        cleanupCursesApp();
    }
    return 221;
}

