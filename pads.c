#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>

// file to read
#define FILENAME "frankenstein.txt"

// Pad rows are numbered 0 to PAD_SIZE-1
#define PAD_ROWS 200

// number of rows and cols on the screen
// numbered 0 to rows-1 and 0 to cols-1
static int rows;
static int cols;

// current pad row
static int current_pad_row;

// visible pad rows
static int visible_pad_rows;

static WINDOW *pad;

static void init_screen(void);
static void init_pad(void);
static void read_file(void);
static void handle_key(void);
static void handle_exit(void);
static void pad_draw(int pad_row);

int main()
{   
    atexit(handle_exit);
    init_screen();
    init_pad();
    
    read_file();

    pad_draw(0);
    wtimeout(pad, -1);

    while(TRUE) {
        handle_key();
    }

    endwin();

    return 0;
}

static void 
init_screen(void)
{
    initscr();
    cbreak();
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLUE);
    getmaxyx(stdscr, rows, cols);
    visible_pad_rows = rows - 4;
    wprintw(stdscr, "Press UP, DOWN, PAGE UP or PAGE DOWN:");
    refresh();
}

static void
init_pad(void)
{
    pad = newpad(PAD_ROWS, cols);
    wbkgd(pad, COLOR_PAIR(1));
    keypad(pad, TRUE);
    current_pad_row = 0;
}

static void
read_file(void)
{
    FILE *gpl = fopen(FILENAME, "r");
    char *line = NULL;
    size_t read = 0;
    int line_num = 0;
    
    if (gpl != NULL) {
        while ((getline(&line, &read, gpl) != -1) && 
                (line_num < PAD_ROWS)) {
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
handle_key(void)
{
    int ch = wgetch(pad);

    switch(ch) {
        case KEY_NPAGE: // page down
            current_pad_row += visible_pad_rows;
            if (current_pad_row + visible_pad_rows > PAD_ROWS) {
                current_pad_row = PAD_ROWS - visible_pad_rows;
            }
            break;
        case KEY_PPAGE: // page up
            current_pad_row -= visible_pad_rows;
            if (current_pad_row < 0) {
                current_pad_row = 0;
            }
            break;
        case KEY_UP:
            current_pad_row--;
            if (current_pad_row < 0) {
                current_pad_row = 0;
            }
            break;
        case KEY_DOWN:
            current_pad_row++;
            if (current_pad_row + visible_pad_rows > PAD_ROWS) {
                current_pad_row = PAD_ROWS - visible_pad_rows;
            }
            break;
        default:
            break;
    }

    if (current_pad_row + visible_pad_rows > PAD_ROWS) {
        current_pad_row = PAD_ROWS - visible_pad_rows;
    } else if (current_pad_row < 0) {
        current_pad_row = 0;
    }
    
    pad_draw(current_pad_row);
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
    // prefresh(*pad, pminrow, pmincol, sminrow, smincol, smaxrow, smaxcol);
    prefresh(pad, pad_row, 0, 2, 0, rows - 3 , cols - 1);
}

