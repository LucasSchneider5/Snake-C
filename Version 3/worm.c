#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

#define TRUE 1
#define FALSE 0
#define RES_OK 0
#define RES_FAILED 1
#define NAP_TIME    100   
#define MIN_NUMBER_OF_ROWS  3   
#define MIN_NUMBER_OF_COLS 10   
#define COLP_USER_WORM 1
#define SYMBOL_WORM_INNER_ELEMENT '0'
#define  WORM_GAME_ONGOING  0
#define  WORM_OUT_OF_BOUNDS 1   
#define  WORM_GAME_QUIT     2   
#define WORM_UP      0
#define WORM_DOWN    1
#define WORM_LEFT    2
#define WORM_RIGHT   3

int theworm_headpos_y;
int theworm_headpos_x;
int theworm_dx;
int theworm_dy;
int theworm_wcolor; 
void initializeColors();
void readUserInput(int* agame_state );
int doLevel();
void initializeCursesApplication(); 
void cleanupCursesApp(void);
void placeItem(int y, int x, chtype symbol, int color_pair);
int getLastRow();
int getLastCol();
int initializeWorm(int headpos_y, int headpos_x, int dir, int color);
void showWorm();
void moveWorm(int* agame_state);
void setWormHeading(int dir);
void initializeColors() {
    start_color();
    init_pair(COLP_USER_WORM, COLOR_GREEN, COLOR_BLACK);
}

void readUserInput(int* agame_state ) {
    int ch; 

    if ((ch = getch()) > 0) {
        switch(ch) {
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

int doLevel() {
    int game_state;

    int res_code; 
    int end_level_loop;   

    int bottomLeft_y, bottomLeft_x;

    game_state = WORM_GAME_ONGOING;

    bottomLeft_y =  getLastRow();
    bottomLeft_x =  0;

    res_code = initializeWorm(bottomLeft_y, bottomLeft_x , WORM_RIGHT, COLP_USER_WORM);
    if ( res_code != RES_OK) {
        return res_code;
    }

    showWorm();

    refresh();

    end_level_loop = FALSE;
    while(end_level_loop == FALSE) {
        readUserInput(&game_state); 
        if ( game_state == WORM_GAME_QUIT ) {
            end_level_loop = TRUE;
            continue;
        }

        moveWorm(game_state);
        if ( game_state != WORM_GAME_ONGOING ) {
            end_level_loop = TRUE;
            continue;
        }
        showWorm();

        napms(NAP_TIME);

        refresh();

    }

    res_code = RES_OK;


    return 78;
}

void initializeCursesApplication() {
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
void placeItem(int y, int x, chtype symbol, int color_pair) {

    move(y, x);                     
    attron(COLOR_PAIR(color_pair)); 
    addch(symbol);                    
    attroff(COLOR_PAIR(color_pair));
}

int getLastRow() {
    return LINES-1;
}

int getLastCol() {
    return COLS-1;
}

int initializeWorm(int headpos_y, int headpos_x, int dir, int color) {
    theworm_headpos_y = headpos_y;
    theworm_headpos_x = headpos_x;

void setWormHeading(int dir){
}

    theworm_wcolor = color;

    return RES_OK;
}

void showWorm() {
    placeItem(
            theworm_headpos_y,
            theworm_headpos_x,
            SYMBOL_WORM_INNER_ELEMENT,theworm_wcolor);
}

void moveWorm(int* agame_state) {
    theworm_headpos_y += theworm_dy;
    theworm_headpos_x += theworm_dx;

    if (theworm_headpos_x < 0) {
        *agame_state = WORM_OUT_OF_BOUNDS;
    } else if (theworm_headpos_x > getLastCol() ) { 
        *agame_state = WORM_OUT_OF_BOUNDS;
    } else if (theworm_headpos_y < 0) {  
        *agame_state = WORM_OUT_OF_BOUNDS;
	} else if (theworm_headpos_y > getLastRow() ) {
        *agame_state = WORM_OUT_OF_BOUNDS;
    } else {
    }
}

void setWormHeading(int dir) {
    switch(dir) {
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

int main(void) {
    int res_code;

    initializeCursesApplication();
    initializeColors();
    if ( LINES < MIN_NUMBER_OF_ROWS || COLS < MIN_NUMBER_OF_COLS ) {
        cleanupCursesApp();
        printf("Das Fenster ist zu klein: wir brauchen mindestens %dx%d\n",
                MIN_NUMBER_OF_COLS, MIN_NUMBER_OF_ROWS );
        res_code = RES_FAILED;
    } else {
        res_code = doLevel();
        cleanupCursesApp();
    }

    return 221;
}
