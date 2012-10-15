#include <stdlib.h>

#include <ncurses.h>

int main()
{   
    initscr();

    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLUE);
    
    int rows, cols;
    getmaxyx(stdscr, rows, cols);

    WINDOW *pad = newpad(1000, cols);
    wbkgd(pad, COLOR_PAIR(1));

    FILE *gpl = fopen("COPYING", "r");
    char *line = NULL;
    size_t read = 0;
    
    if (gpl != NULL) {
        while (getline(&line, &read, gpl) != -1) {
            wprintw(pad, line);
            free(line);
            line = NULL;
            read = 0;
        }
        fclose(gpl);
   }

    wmove(pad, rows-1, cols-1);
    // prefresh(*pad, pminrow, pmincol, sminrow, smincol, smaxrow, smaxcol);
    prefresh(pad, 0, 0, 2, 4, rows - 3 , cols - 5);
    wtimeout(pad, 20);

    int ch = ERR;
    while(ch == ERR) {
        ch = wgetch(pad);

        switch(ch) {
            case KEY_UP:
            case KEY_DOWN:
            case KEY_LEFT:
            case KEY_RIGHT:
                break;
            default:
                ch = ERR;
        }
    }
    endwin();

    return 0;
}
