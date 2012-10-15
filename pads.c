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
    // prefresh(*pad, pminrow, pmincol, sminrow, smincol, smaxrow, smaxcol);
    prefresh(pad, 0, 0, 2, 2, 10, 10);
    wmove(pad, rows-1, 0);
    wgetch(pad);
    
    endwin();

    return 0;
}
