#include <curses.h>
#include <iostream>
#include "nCurses.h"
#include <string>
#include <cstring>
#include "window.h"
#include "../mopLoader/mopfile/MopFile.h"




class mopViewer {
public:
mopViewer(void);
~mopViewer(void);
void showStats(std::string fileName, int skipCount);
void selectGame(std::string fileName, int skipCount);
void refreshAll(WINDOW * window1, WINDOW * window2);
int printOutput(MopState * mopstate, int currentrow, int currentitem, WINDOW* window);
MopFile * mopfile;
MopState * mopstate;
};
