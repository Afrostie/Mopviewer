#include <iostream>
#include <curses.h>
#include <thread>
#include <chrono>
#include "nCurses.h"
#include <string>
#include <cstring>
#include "window.h"
#include "mopfile/mophandling.h"
#include "mopfile/MopFile.h"

class newGame {
	public:
	newGame(void);
	~newGame(void);
	void autoPong();
	void manualPong();
	void textInput();
	void calculator();
	void showStats();
	void selectGame();
};
