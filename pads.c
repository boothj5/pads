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

    wprintw(pad, "Rows: %d, Cols: %d", rows, cols);
    wmove(pad, rows-1, cols-1);
    // prefresh(*pad, pminrow, pmincol, sminrow, smincol, smaxrow, smaxcol);
    prefresh(pad, 0, 0, 2, 4, rows - 3 , cols - 5);
    wgetch(pad);
    
    endwin();

    return 0;
}
