#include <curses.h>
#include <iostream>
#include "nCurses.h"
#include <string>
#include <cstring>
#include "window.h"
#include "mopfile/MopFile.h"




class mopViewer {
  public:
  mopViewer(void);
  ~mopViewer(void);
  void autoPong();
  void manualPong();
  void textInput();
  void calculator();
  void showStats();
  void selectGame();
  void refreshAll();
  void redrawBox();
  void printOutput();
  MopFile * mopfile;
  MopState * mopstate;
};
