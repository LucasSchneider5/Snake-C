#include <curses.h>                            //Benötigten Bibliotheken
#include <string.h>

#define RES_OK 0                               //Definitionen
#define RES_FAILED 1

void initializeCursesApplication() {           //Curse Einstellungen aufrufen
   
initscr();

    noecho(); 
    cbreak(); 
    nonl();   
    keypad(stdscr, TRUE);
    curs_set(0);    
  
    nodelay(stdscr, TRUE);
}

void cleanupCursesApp(void)                    //Curse Einstellungen beenden
{
    standend();   
    refresh();  
    curs_set(1);
    endwin();   
}

int main(void) {                               //Hauptfunktion
    int res_code;       
    char* message_template = "Das Fenster hat xxx Zeilen und yyy Spalten";
    int msg_len;
    int min_rows = 3;

  
initscr(); 
    msg_len = strlen(message_template);                 

                                               //Falls das Fenster zu klein ist
    if ( LINES < min_rows  || COLS < msg_len   ) {           

cbreak();
      
        printf("Das Fenster ist zu klein: wir brauchen mindestens %dx%d\n", msg_len, min_rows );

        res_code = RES_FAILED;
    } else {                                   //Wenn die Größe passt
      
        int mid_row = LINES/2;                 //Definition des anfangspunktes der Nachticht
        int start_col = COLS/2 - (msg_len)/2;

        mvaddch(LINES-1, COLS-1, 'D');         //Buchstaben in den Ecken
        mvaddch(LINES-1, 0, 'C');
        mvaddch(0, COLS-1, 'B');
        mvaddch(0, 0, 'A');
                                               //Ausgabe der Nachricht
        mvprintw(mid_row, start_col, "Das Fenster hat %3d Zeilen und %3d Spalten", LINES, COLS);                                                      
        refresh();                             //Endeinstellungen
        getch();
        nodelay(stdscr, TRUE);
        keypad(stdscr, TRUE);
        endwin();
        res_code = RES_OK;
        
        
    }
    return res_code;
}
