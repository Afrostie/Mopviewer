
/**
 * This class provides functionality for an ncurses interface.
 * useful weblink is useful http://hughm.cs.ukzn.ac.za/~murrellh/os/notes/ncurses.html
 */


#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <time.h>
#include <cmath>

#include <curses.h>
class nCurses {
private:
    int height;
    int width;
public:
    
    nCurses() {
        
    }
    ~nCurses() {
        
    }
    
    /**
     *start the ncurses session
     */
    void start(bool bnoecho, bool bcbreak, bool bkeypad, bool bnodelay) {
        initscr();
        if (bnoecho) {
            noecho();
        }
        if(bcbreak) {
            cbreak();
        }
        
        if (bkeypad) {
            keypad(stdscr, TRUE);
        }
        
        
        if (bnodelay){
            nodelay(stdscr, TRUE);
        }
    }
    /**
     *stop the ncurses session
     */
    void stop() {
        endwin();
    }
    
    /**
     * create a new window, with or without scrolling
     */
    WINDOW * makeNewWindow(int nlines, int ncols, int y0, int x0, bool bscroll) {
        WINDOW * win = newwin(nlines, ncols, y0, x0);
        if (bscroll) {
            scrollok(win, TRUE);
        }
        return win;
    }

 
	
     /**
     * resize window
     */
    int resizeWindow(WINDOW * win,int nlines, int ncols) {
        int r = wresize(win,nlines, ncols);
        return r;
    }   
	
    /**
     * get the console extent
     */
    void getHeightAndWidth(WINDOW *win,int *height, int *width){
        getmaxyx(win,*height,*width);
    }
	
    int getHeight() {
        return this->height;
    }
    int getWidth() {
        return this->width;
    }
    /*
        printw("Upper left corner         "); addch(ACS_ULCORNER); printw("  ACS_ULCORNER\n"); 
        printw("Lower left corner         "); addch(ACS_LLCORNER); printw("  ACS_LLCORNER\n");
        printw("Upper right corner        "); addch(ACS_URCORNER); printw("  ACS_URCORNER\n");
        printw("Lower right corner        "); addch(ACS_LRCORNER); printw("  ACS_LRCORNER\n");
        printw("Tee pointing right        "); addch(ACS_LTEE); printw("  ACS_LTEE\n");
        printw("Tee pointing left         "); addch(ACS_RTEE); printw("  ACS_RTEE\n");
        printw("Tee pointing up           "); addch(ACS_BTEE); printw("  ACS_BTEE\n");
        printw("Tee pointing down         "); addch(ACS_TTEE); printw("  ACS_TTEE\n");
        printw("Horizontal line           "); addch(ACS_HLINE); printw("  ACS_HLINE\n");
        printw("Vertical line             "); addch(ACS_VLINE); printw("  ACS_VLINE\n");
        printw("Large Plus or cross over  "); addch(ACS_PLUS); printw("  ACS_PLUS\n");
        printw("Scan Line 1               "); addch(ACS_S1); printw("  ACS_S1\n");
        printw("Scan Line 3               "); addch(ACS_S3); printw("  ACS_S3\n");
        printw("Scan Line 7               "); addch(ACS_S7); printw("  ACS_S7\n");
        printw("Scan Line 9               "); addch(ACS_S9); printw("  ACS_S9\n");
        printw("Diamond                   "); addch(ACS_DIAMOND); printw("  ACS_DIAMOND\n");
        printw("Checker board (stipple)   "); addch(ACS_CKBOARD); printw("  ACS_CKBOARD\n");
        printw("Degree Symbol             "); addch(ACS_DEGREE); printw("  ACS_DEGREE\n");
        printw("Plus/Minus Symbol         "); addch(ACS_PLMINUS); printw("  ACS_PLMINUS\n");
        printw("Bullet                    "); addch(ACS_BULLET); printw("  ACS_BULLET\n");
        printw("Arrow Pointing Left       "); addch(ACS_LARROW); printw("  ACS_LARROW\n");
        printw("Arrow Pointing Right      "); addch(ACS_RARROW); printw("  ACS_RARROW\n");
        printw("Arrow Pointing Down       "); addch(ACS_DARROW); printw("  ACS_DARROW\n");
        printw("Arrow Pointing Up         "); addch(ACS_UARROW); printw("  ACS_UARROW\n");
        printw("Board of squares          "); addch(ACS_BOARD); printw("  ACS_BOARD\n");
        printw("Lantern Symbol            "); addch(ACS_LANTERN); printw("  ACS_LANTERN\n");
        printw("Solid Square Block        "); addch(ACS_BLOCK); printw("  ACS_BLOCK\n");
        printw("Less/Equal sign           "); addch(ACS_LEQUAL); printw("  ACS_LEQUAL\n");
        printw("Greater/Equal sign        "); addch(ACS_GEQUAL); printw("  ACS_GEQUAL\n");
        printw("Pi                        "); addch(ACS_PI); printw("  ACS_PI\n");
        printw("Not equal                 "); addch(ACS_NEQUAL); printw("  ACS_NEQUAL\n");
        printw("UK pound sign             "); addch(ACS_STERLING); printw("  ACS_STERLING\n\n");	
	*/
	//windows
	WINDOW *  newWindow(int width, int height,int xloc, int yloc,short cp,short cf,short cb,bool bscroll, bool border) {
	WINDOW *  win;
	//this->defineForegroundBackgroundColour(cp,cf,cb);     
	win = newwin(xloc,yloc, width, height);
	this->colourWindow(win,COLOR_PAIR(cp));
	//this->removeFormatFromWindow(win,COLOR_PAIR(cp));
	//this->addFormatToWindow(win,COLOR_PAIR(cp));
	//addFormatToConsole(int attr)
	if (border) {
    this->addWindowBorder(win, '|', '|', ',', ',', '-', '-', '-', '-');
	}
        if (bscroll) {
            scrollok(win, TRUE);
        }
	//this->removeFormatFromWindow(win,COLOR_PAIR(cp));
	return win;
}


	// subwindows
	WINDOW *  subWindow(WINDOW * parent,int width, int height,int xloc, int yloc,short cp,bool bscroll, bool border) {
	WINDOW *  win;
	//this->defineForegroundBackgroundColour(0,7,0        
	win = subwin(parent,xloc,yloc, width, height);

	
	this->colourWindow(win,cp);
	this->removeFormatFromWindow(win,COLOR_PAIR(cp));
	this->addFormatToWindow(win,COLOR_PAIR (cp));
	//addFormatToConsole(int attr)
	if (border) {
    this->addWindowBorder(win, '|', '|', ',', ',', '-', '-', '-', '-');
	}
        if (bscroll) {
            scrollok(win, TRUE);
        }
	this->removeFormatFromWindow(win,COLOR_PAIR(cp));
	return win;
}
	
	
    /**
     * set the cursor position in the console
     */
    void setPosition(WINDOW *win, int y, int x){
        wmove(win, y, x);
    }
    
    
    /**
     * update or clear the whole console
     */
    void updatescr() {
        refresh();
    }
    
    
    /**
     * after changing the contents of a single window, update (refresh) it.
     * This function can also clear a window
     */
    void updateWindow(WINDOW *win) {
        wrefresh(win);
    }
    
    /**
     * delete a window
     */
    void delWindow(WINDOW *win) {
        delwin(win);
    }
    
    
    
    // character output
    
    /**
     * place character in console=
     */
    void addCharacterToScreen(chtype ch){
        
    }
    
    /**
     * place character in window specified at position x y
     */
    void addCharacterToWindowXY(WINDOW *win,chtype ch, int x, int y){
        mvwaddch(win, y, x, ch);
    }
    
    /**
     * place character in console (immediate)
     */
    void echpCharacterToScreen(chtype ch){
        echochar(ch);
    }
    
    /**
     * place character in console (immediate)
     */
    void echpCharacterToWindow(WINDOW *win,chtype ch){
        wechochar(win, ch);
    }
    
    
    /**
     * place string in console
     */
    void addStringinConsole(const char *str){
        addstr(str);
    }
    
    /**
     * place string in window
     */
    void addStringToWindow(WINDOW *win,const char *str){
        waddstr(win,str);
    }
    
    /**
     * place formatted string in console
     */
    void printFmtStringinConsole(const char *str){
        addstr(str);
    }
    
    /**
     * place formatted string in window
     */
    void printFmtStringToWindow(WINDOW *win,const char *str){
        wprintw(win,str);
    }
    
    // formatting
    
    
    /**
     * add formatting to console (will effect subsequent calls to addch, waddch, addstr and waddstr)
     */
    void addFormatToConsole(int attr){
        attron(attr);
    }
    
    /**
     * add formatting to window (will effect subsequent calls to  waddch, addstr and waddstr)
     */
    void addFormatToWindow(WINDOW *win, int attr){
        wattron(win,attr);
    }
    
    /**
     * remove specified formatting from console
     */
    void removeFormatFromConsole(int attr){
        attroff(attr);
    }
    
    /**
     * remove specified formatting from console
     */
    void removeFormatFromWindow(WINDOW *win, int attr){
        wattroff(win,attr);
    }
    
    /**
     * remove all formatting from console
     */
    void removeAllFormattingFromConsole(){
        standend();
    }
    
    /**
     * remove all formatting from window
     */
    void removeAllFormattingFromWindow(WINDOW *win){
        wstandend(win);
    }
    
    // colour
    /*
	    COLOR_BLACK   0
        COLOR_RED     1
        COLOR_GREEN   2
        COLOR_YELLOW  3
        COLOR_BLUE    4
        COLOR_MAGENTA 5
        COLOR_CYAN    6
        COLOR_WHITE   7
	*/
    
    /**
     * start using colour
     */
    void startColour(){
        start_color();
    }
    
    /**
     * create background and foreground colour pair, used when setting foreground and background colour, by redefining the first parameter n, which can then be used to set these colour attribute
     */
    int defineForegroundBackgroundColour(short n, short f, short b){
        init_pair(n, f,b);
        return n;
    }
    /**
     * create a colour, by redefining the first parameter n, which then represents new colour
     */
    int defineColour(short color, short r, short g, short b) {
        init_color(color,r,g,b);
        return color;
    }
    
    /**
     * add colour to window
     */
    void colourWindow(WINDOW *win, chtype ch) {
        wbkgd(win,ch);

    }
    // line graphics
    /*
     Here are some special characters which can be used for drawing routines as the chtype.
     
     ACS_BLOCK               solid square block
     ACS_BOARD               board of squares
     ACS_BTEE                bottom tee
     ACS_BULLET              bullet
     ACS_CKBOARD             checker board (stipple)
     ACS_DARROW              arrow pointing down
     ACS_DEGREE              degree symbol
     ACS_DIAMOND             diamond
     ACS_GEQUAL              greater-than-or-equal-to
     ACS_HLINE               horizontal line
     ACS_LANTERN             lantern symbol
     ACS_LARROW              arrow pointing left
     ACS_LEQUAL              less-than-or-equal-to
     ACS_LLCORNER            lower left-hand corner
     ACS_LRCORNER            lower right-hand corner
     ACS_LTEE                left tee
     ACS_NEQUAL              not-equal
     ACS_PI                  greek pi
     ACS_PLMINUS             plus/minus
     ACS_PLUS                plus
     ACS_RARROW              arrow pointing right
     ACS_RTEE                right tee
     ACS_S1                  scan line 1
     ACS_S3                  scan line 3
     ACS_S7                  scan line 7
     ACS_S9                  scan line 9
     ACS_STERLING            pound-sterling symbol
     ACS_TTEE                top tee
     ACS_UARROW              arrow pointing up
     ACS_ULCORNER            upper left-hand corner
     ACS_URCORNER            upper right-hand corner
     ACS_VLINE               vertical line
     */
    //calltype allows for different adding calls when drawing one of the above individually
    /*
     void addGraphic(bool callType,const chtype graphic) {
     if(callType) {
     addch(graphic);
     } else {
     addstr(graphic);
     }
     }
     */
    
    
    // borders
    
    
    /**
     * set a border for the console
     ls - left side,
     rs - right side,
     ts - top side,
     bs - bottom side,
     tl - top left-hand corner,
     tr - top right-hand corner,
     bl - bottom left-hand corner, and
     br - bottom right-hand corner.
     If any of these arguments is zero, then the corresponding default values (defined in curses.h) are used instead:
     ACS_VLINE,
     ACS_VLINE,
     ACS_HLINE,
     ACS_HLINE,
     ACS_ULCORNER,
     ACS_URCORNER,
     ACS_LLCORNER,
     ACS_LRCORNER.
     */
    void addBorder(chtype ls, chtype rs, chtype ts, chtype bs, chtype tl, chtype tr, chtype bl, chtype br) {
        border(ls,rs,ts,bs,tl,tr,bl,br);
    }
    
    
    /**
     * set a border for a window
     */
    void addWindowBorder(WINDOW *win, chtype ls, chtype rs, chtype ts, chtype bs, chtype tl, chtype tr, chtype bl, chtype br) {
        wborder(win,ls,rs,ts,bs,tl,tr,bl,br);
    }
    
    /**
     * draw a box in a window, Box is a shorthand for the following call: wborder(win, verch, verch, horch, horch, 0, 0, 0, 0).
     */
    void drawBox(WINDOW *win, chtype verch, chtype horch) {
        box(win,verch,horch);
    }
    
    
    
    /**
     * draw a border round a window
     */
    void addBorder(WINDOW *win, int a, int b,int c,int d,int e,int f, int g, int h) {
        wborder(win, a,b,c,d,e,f,g,h);
    }
    
    /**
     * draw a horizontal line in the console
     */
    void drawHorizontalLine(chtype ch, int n) {
        hline(ch,n);
    }
    
    /**
     * draw a vertical line in the console
     */
    void drawVerticalLine(chtype ch, int n) {
        vline(ch,n);
    }
    
    /**
     * draw a horizontal line in a window
     */
    void drawHorizontalLineWindow(WINDOW *win,chtype ch, int n) {
        whline(win,ch,n);
    }
    
    /**
     * draw a horizontal line in console from specified co-ordinates
     */
    void drawHorizontalLineWindowFromPos(int y, int x, chtype ch, int n) {
        mvhline(y,x,ch,n);
    }
    
    /**
     * draw a horizontal line in window from specified co-ordinates
     */
    void drawHorizontalLineWindowFromPos(WINDOW *win,int y, int x, chtype ch, int n) {
        mvwhline(win,y,x,ch,n);
    }
    
    
    /**
     * draw a vertical line in a window
     */
    void drawVerticalLineWindow(WINDOW *win,chtype ch, int n) {
        wvline(win,ch,n);
    }
    
    /**
     * draw a vertical line in console from specified co-ordinates
     */
    void drawVerticalLineWindowFromPos(int y, int x, chtype ch, int n) {
        mvvline(y,x,ch,n);
    }
    
    /**
     * draw a vertical line in window from specified co-ordinates
     */
    void drawVerticalLineWindowFromPos(WINDOW *win,int y, int x, chtype ch, int n) {
        mvwvline(win,y,x,ch,n);
    }
    
    
    
    
    // input
    /**
     * capture keypresses in console (no cursor movement)
     */
    char getChar(){
        return getch();
    }
    
    /**
     * capture keypresses in window (no cursor movement)
     */
    char getCharW(WINDOW *win){
        return wgetch(win);
    }
    
    /**
     * capture keypresses in console (with cursor movement)
     */
    char getCharM(int x, int y){
        return mvgetch(x,y);
    }
    
    /**
     * capture keypresses in window (with cursor movement)
     */
    char getCharWM(WINDOW *win,int x, int y){
        return wgetch(win);
    }
    
    
    
};