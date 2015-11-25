#include <curses.h>
#include <iostream>
#include "mopViewer.h"
#include <string>
#include <cstring>
#include "mopfile/MopFile.h"

int main() {
  mopViewer mopViewers;
  mopViewers.selectGame();
  return 0;
}
