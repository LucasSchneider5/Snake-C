#include <curses.h>                              
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include "prep.h"
#include "worm.h"
#include "worm_model.h"
#include "board_model.h"

void initializeColors();
void readUserInput(enum GameStates* agame_state);
enum ResCodes doLevel();

void initializeColors() 
{
    start_color();
    init_pair(COLP_USER_WORM, COLOR_GREEN, COLOR_BLACK);
    init_pair(COLP_FREE_CELL, COLOR_BLACK, COLOR_BLACK);
}

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
        //nodelay(stdscr, false);
        res_code = doLevel();
        cleanupCursesApp();
    }
    return 221;
}

