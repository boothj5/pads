#include <stdlib.h>
#include <ncurses.h>

#define PAD_SIZE 200

int main()
{   
    initscr();
    cbreak();

    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLUE);
    
    int rows, cols;
    int prow = 0;
    getmaxyx(stdscr, rows, cols);

    WINDOW *pad = newpad(PAD_SIZE, cols);
    wbkgd(pad, COLOR_PAIR(1));
    keypad(pad, TRUE);

    FILE *gpl = fopen("COPYING", "r");
    char *line = NULL;
    size_t read = 0;
    int line_num = 0;
    
    if (gpl != NULL) {
        while ((getline(&line, &read, gpl) != -1) && 
                (line_num < PAD_SIZE)) {
            wprintw(pad, "%d: %s", line_num, line);
            free(line);
            line = NULL;
            read = 0;
            line_num++;
        }
        fclose(gpl);
   }

    // prefresh(*pad, pminrow, pmincol, sminrow, smincol, smaxrow, smaxcol);
    prefresh(pad, prow, 0, 2, 4, rows - 3 , cols - 5);
    wtimeout(pad, -1);

    while(TRUE) {
        int ch = wgetch(pad);

        switch(ch) {
            case KEY_UP:
                prow--;
                if (prow < 0) {
                    prow = 0;
                }
                prefresh(pad, prow, 0, 2, 4, rows - 3 , cols - 5);
                break;
            case KEY_DOWN:
                prow++;
                if (prow > PAD_SIZE) {
                    prow = PAD_SIZE;
                }
                prefresh(pad, prow, 0, 2, 4, rows - 3 , cols - 5);
                break;
            default:
                break;
        }
    }
    endwin();

    return 0;
}
