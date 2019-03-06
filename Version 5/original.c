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

//Enumerationen
enum ResCodes
{
  RES_OK,
  RES_FAILED,
};
enum ColorPairs
{
COLP_USER_WORM = 1,
};
enum GameStates
{
WORM_GAME_ONGOING,
WORM_OUT_OF_BOUNDS,
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
int theworm_headpos_y;
int theworm_headpos_x;
int theworm_dx;
int theworm_dy;
int theworm_wcolor; 
void initializeColors();
void readUserInput(enum GameStates* agame_state);
int doLevel();
void initializeCursesApplication(); 
void cleanupCursesApp(void);
void placeItem(int y, int x, chtype symbol, enum ColorPairs color_pair);
int getLastRow();
int getLastCol();
int initializeWorm(int headpos_y, int headpos_x, enum WormHeading dir, int color);
void showWorm();
void moveWorm(enum GameStates* agame_state);
void setWormHeading(enum WormHeading dir);
void initializeColors() 
{
    start_color();
    init_pair(COLP_USER_WORM, COLOR_GREEN, COLOR_BLACK);
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
int doLevel() 
{
    enum GameStates* agame_state;
    enum ResCodes res_code; 
    bool end_level_loop;   
    int bottomLeft_y, bottomLeft_x;

    agame_state = WORM_GAME_ONGOING;
    bottomLeft_y =  getLastRow();
    bottomLeft_x =  0;
    res_code = initializeWorm(bottomLeft_y, bottomLeft_x , WORM_RIGHT, COLP_USER_WORM);
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
        moveWorm(agame_state);
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
int initializeWorm(int headpos_y, int headpos_x, enum WormHeading dir, int color) 
{
    theworm_headpos_y = headpos_y;
    theworm_headpos_x = headpos_x;
    //Richtung (Wurm)
    void setWormHeading(enum WormHeading dir)
{
}
    theworm_wcolor = color;
    return RES_OK;
}

//Wurm
void showWorm(enum ColorPairs color_pair ) 
{
    placeItem(
            theworm_headpos_y,
            theworm_headpos_x,
            SYMBOL_WORM_INNER_ELEMENT,theworm_wcolor);
}

//Wurm (Bewegung)
void moveWorm(enum GameStates* agame_state) 
{
    theworm_headpos_y += theworm_dy;
    theworm_headpos_x += theworm_dx;
    if (theworm_headpos_x < 0) 
    {
        *agame_state = WORM_OUT_OF_BOUNDS;
    } 
    else if (theworm_headpos_x > getLastCol() ) 
    { 
        *agame_state = WORM_OUT_OF_BOUNDS;
    } 
    else if (theworm_headpos_y < 0) 
    {  
        *agame_state = WORM_OUT_OF_BOUNDS;
    } 
    else if (theworm_headpos_y > getLastRow() ) 
    {
        *agame_state = WORM_OUT_OF_BOUNDS;
    }
    else 
    {
    }
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
    enum ResCodes res_code;
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
        res_code = doLevel();
        cleanupCursesApp();
    }
    return 221;
}
