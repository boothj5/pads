#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>

// Pad rows are numbered 0 to PAD_SIZE-1
#define PAD_SIZE 200

// number of rows and cols on the screen
// numbered 0 to rows-1 and 0 to cols-1
static int rows;
static int cols;

// current pad row
static int prow;

// number of visible rows of pad
static int visible_pad_height;

static WINDOW *pad;

static void init_ncurses(void);
static void init_pad(void);
static void read_file(void);
static void handle_input(void);
static void handle_exit(void);
static void pad_draw(int pad_row);

int main()
{   
    atexit(handle_exit);
    init_ncurses();
    init_pad();
    
    read_file();

    // prefresh(*pad, pminrow, pmincol, sminrow, smincol, smaxrow, smaxcol);
    pad_draw(0);
    wtimeout(pad, -1);

    while(TRUE) {
        handle_input();
    }

    endwin();

    return 0;
}

static void 
init_ncurses(void)
{
    initscr();
    cbreak();
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLUE);
    getmaxyx(stdscr, rows, cols);
}

static void
init_pad(void)
{
    pad = newpad(PAD_SIZE, cols);
    wbkgd(pad, COLOR_PAIR(1));
    keypad(pad, TRUE);
    prow = 0;
}

static void
read_file(void)
{
    FILE *gpl = fopen("frankenstein.txt", "r");
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
}

static void
handle_input(void)
{
    int ch = wgetch(pad);

    switch(ch) {
        case KEY_UP:
            prow--;
            if (prow < 0) {
                prow = 0;
            }
            pad_draw(prow);
            break;
        case KEY_DOWN:
            prow++;
            if (prow > PAD_SIZE) {
                prow = PAD_SIZE;
            }
            pad_draw(prow);
            break;
        default:
            break;
    }
}

static void
handle_exit(void)
{
    endwin();
    printf("\nQuit successfully.\n\n");
}

static void
pad_draw(int pad_row)
{
    prefresh(pad, 
        // starting row and column of pad
        pad_row, 0,
        // screen coords of pad
        2, 4, rows - 3 , cols - 5);
}

