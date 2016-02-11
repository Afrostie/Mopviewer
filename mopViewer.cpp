#include "mopViewer.h"

mopViewer::mopViewer() {
}
mopViewer::~mopViewer() {
}

void mopViewer::refreshAll(WINDOW* window1, WINDOW* window2){
        refresh();
        wrefresh(window1);
        wrefresh(window2);
}

int mopViewer::printOutput(MopState * mopstate, int currentRow, int currentitem, WINDOW* window){
        mvwprintw(window, currentRow,1, "Particle Number: %d", currentitem);
        currentRow++;
        mvwprintw(window, currentRow,1, "Particle X: %f", mopstate->getMopItem(currentitem).x);
        currentRow++;
        mvwprintw(window, currentRow,1, "Particle Y: %f", mopstate->getMopItem(currentitem).y);
        currentRow++;
        mvwprintw(window, currentRow,1, "Particle Z: %f", mopstate->getMopItem(currentitem).z);
        currentRow++;
        mvwprintw(window, currentRow,1, "Particle Red: %d", mopstate->getMopItem(currentitem).red);
        currentRow++;
        mvwprintw(window, currentRow,1, "Particle Green: %d", mopstate->getMopItem(currentitem).green);
        currentRow++;
        mvwprintw(window, currentRow,1, "Particle Blue: %d", mopstate->getMopItem(currentitem).blue);
        currentRow+=2;;
        return currentRow;
}



void mopViewer::showStats() {
  int skip = 4;
        mopfile = new MopFile();
        mopfile->setFilename("Testing_Project_10000.mop");
        mopfile->openMopfileReader();
        mopstate = new MopState();
        mopstate = mopfile->readCyclingState(skip);
        std::cout << "Item Count: " << mopstate->getItemCount() << std::endl;
        for (int i = 0; i < mopstate->getItemCount(); i++) {
                std::cout << "Particle "<< i << " X value: " << mopstate->getMopItem(i).x << std::endl;
                std::cout << "Particle "<< i << " Y value: " << mopstate->getMopItem(i).y << std::endl;
                std::cout << "Particle "<< i << " Z value: " << mopstate->getMopItem(i).z << std::endl;
        }
        int x, y, x2, y2, x3, y3;
        nCurses nCurse;
        //TODO: Switch to a single init function
        // nCurse.start(true, true, true, false);
        initscr();
        raw();
        nCurse.startColour();
        getmaxyx(stdscr, x, y);
        init_pair(1, COLOR_WHITE, COLOR_BLACK);
        init_pair(2, COLOR_BLACK, COLOR_WHITE);
        wbkgd(stdscr, COLOR_PAIR(1));
        wattron(stdscr, COLOR_PAIR(1));
        nCurse.drawBox(stdscr, ACS_VLINE, ACS_HLINE);

        WINDOW *window1 = newwin(x-4, (y/2)-5, 2, 2);
        wbkgd(window1, COLOR_PAIR(2));
        nCurse.drawBox(window1, ACS_VLINE, ACS_HLINE);

        WINDOW *window2 = newwin(x-4, (y/2)-5, 2, (y/2)+2);
        wbkgd(window2, COLOR_PAIR(2));
        nCurse.drawBox(window2, ACS_VLINE, ACS_HLINE);

        wattron(stdscr, A_BOLD);
        mvwprintw(stdscr, 1, (y/2)-10, "Welcome to Mopviewer");

        getmaxyx(window1, x2, y2);
        getmaxyx(window2, x3, y3);

        //Get the output for sizes of windows, helps me understand the sizing
        std::cout << "Max X: " << x << std::endl;
        std::cout << "Max Y: " << y << std::endl;

        std::cout << "Max X1: " << x2 << std::endl;
        std::cout << "Max Y1: " << y2 << std::endl;

        std::cout << "Max X2: " << x3 << std::endl;
        std::cout << "Max Y2: " << y3 << std::endl;

        //Calculate the total amount of items per column, relative to screen size
        //Output the size for debugging etc
        float maxitems = (x2-2)/8;
        std::cout << "Max amount of items per column: " << maxitems << std::endl;

        //Display the information to
        //Setup a couple of variables to keep track of the loaded items and rows
        int currentRow = 1;
        int currentRow2 = 1;
        int currentitem = 0;
        //Cycles through and prints each item , maxitems*2 as it prints to both columns
        for (int i = 0; i < (2*maxitems); i++) {
                //If the current row is greater than the the max amount of items, print to 2nd column
                if((currentRow >= (8*maxitems))) {
                        currentRow2 = mopViewer::printOutput(mopstate, currentRow2,currentitem, window2);
                        currentitem++;
                }
                //Else print to 1st column
                else if(currentitem >= 0) {
                        currentRow = mopViewer::printOutput(mopstate, currentRow,currentitem, window1);
                        currentitem++;
                }
        }
        //Refresh all windows to update the display
        mopViewer::refreshAll(window1, window2);

        //Pretty much copy and pasting the same chunk of code, small changes
        //According to which button is pressed, really horrible/long atm
        int ch;
        while((ch = getch()) != GLFW_KEY_2)
        { switch(ch) {
          case KEY_RIGHT:
                  currentRow = 1;
                  currentRow2 = 1;
                  for (int i = 0; i < (2*maxitems); i++) {
                          if((currentRow >= (8*maxitems)) && (currentitem <= mopstate->getItemCount())) {
                                  currentRow2 = mopViewer::printOutput(mopstate, currentRow2,currentitem, window2);
                                  currentitem++;

                          }
                          else if (currentitem <= mopstate->getItemCount()) {
                                  currentRow = mopViewer::printOutput(mopstate, currentRow,currentitem, window1);
                                  currentitem++;
                          }
                  }
                  mopViewer::refreshAll(window1, window2);
                  //std::cout << "Loaded Next Items" << std::endl;
                  break;

          case KEY_LEFT:
                  //Need to clear otherwise if the number is shorter than previous, ends will show
                  wclear(window1);
                  wclear(window2);
                  currentRow = 1;
                  currentRow2 = 1;
                  //When going back, reduce the currentitem to make sure right ones are showed
                  currentitem = currentitem - ((maxitems*2)*2);
                  for (int i = 0; i < (2*maxitems); i++) {
                          if((currentRow >= (8*maxitems)) && (currentitem <= mopstate->getItemCount())) {
                                  currentRow2 = mopViewer::printOutput(mopstate, currentRow2,currentitem, window2);
                                  currentitem++;
                          }
                          else if (currentitem <= mopstate->getItemCount() && (currentitem <= mopstate->getItemCount()) && currentitem >= 0) {
                                  currentRow = mopViewer::printOutput(mopstate, currentRow,currentitem, window1);
                                  currentitem++;
                          }
                          else{
                                  currentitem = 0;
                                  currentRow = mopViewer::printOutput(mopstate, currentRow,currentitem, window1);
                                  currentitem++;
                          }
                  }

                  //TODO: Need to either create a function tovredraw and refresh or find a different method of clearing
                  //Redraw boxes around windows as clear deletes them
                  nCurse.drawBox(window1, ACS_VLINE, ACS_HLINE);
                  nCurse.drawBox(window2, ACS_VLINE, ACS_HLINE);

                  mopViewer::refreshAll(window1, window2);
                  //std::cout << "Loaded Previous Items" << std::endl;
                  break;

          case KEY_UP:
                  mopstate = mopfile->readCyclingState(skip);
                  currentRow = 1;
                  currentRow2 = 1;
                  currentitem = currentitem - (maxitems*2);
                  for (int i = 0; i < (2*maxitems); i++) {
                          if((currentRow >= (8*maxitems)) && (currentitem <= mopstate->getItemCount())) {
                                  currentRow2 = mopViewer::printOutput(mopstate, currentRow2,currentitem, window2);
                                  currentitem++;

                          }
                          else if (currentitem <= mopstate->getItemCount()) {
                                  currentRow = mopViewer::printOutput(mopstate, currentRow,currentitem, window1);
                                  currentitem++;
                          }
                  }
                  mopViewer::refreshAll(window1, window2);
                  //std::cout << "Loaded Next State" << std::endl;
                  break;

          case KEY_DOWN:
                  //TODO: Find a better way to reset the file
                  //Reopen the mopfile to start from begining
                  mopfile = new MopFile();
                  mopfile->setFilename("Testing_Project.mop");
                  mopfile->openMopfileReader();
                  mopstate = new MopState();
                  mopstate = mopfile->readCyclingState(skip);
                  currentitem = currentitem - (maxitems*2);
                  currentRow = 1;
                  currentRow2 = 1;
                  for (int i = 0; i < (2*maxitems); i++) {
                          if((currentRow >= (8*maxitems)) && (currentitem <= mopstate->getItemCount())) {
                                  currentRow2 = mopViewer::printOutput(mopstate, currentRow2,currentitem, window2);
                                  currentitem++;

                          }
                          else if (currentitem <= mopstate->getItemCount()) {
                                  currentRow = mopViewer::printOutput(mopstate, currentRow,currentitem, window1);
                                  currentitem++;
                          }
                  }
                  mopViewer::refreshAll(window1, window2);
                  //std::cout << "Reset State" << std::endl;
                  break;
          }}
        //When finished end the session and return to the main menu screen
        endwin();
        mopViewer::selectGame();
}



/*
   TODO: Create helper functions. These include:
    Clear then refresh all screens
    Draw box for window
    Properly comment/organise the code
 */

void mopViewer::selectGame() {
        clear();
        int mx = 0, my = 0, mx2 = 0, my2 = 0;
        char welcome[] = "Enter 1 to View Stats and 2 to Open MopViewer: ";
        char exitMessage[] = "Enter anything else to exit";
        // Create inital screen. Better to replace
        // with the single start function from moody
        WINDOW *myWindow = initscr();
        raw();
        cbreak();
        keypad(myWindow, TRUE);
        //   noecho();
        // Enable use of colour on screen
        start_color();
        // Create two colour pairs for the different screens
        init_pair(1, COLOR_BLACK, COLOR_CYAN);
        init_pair(2, COLOR_BLACK, COLOR_RED);
        init_pair(3, COLOR_WHITE, COLOR_BLACK);
        // Set the colour scheme of window 1 to pair 1
        wattron(myWindow, COLOR_PAIR(1));
        // Set background of main window to colour scheme 1
        wbkgd(myWindow, COLOR_PAIR(1));
        // Create a border for the main console screen
        box(myWindow, ACS_BULLET, ACS_BULLET);
        // Make a new window that is a sub window of
        // main window and add a border to it
        getmaxyx(myWindow, mx, my);
        WINDOW *myWindow2 = subwin(myWindow, mx-5, my-5, 3, 3);
        box(myWindow2, ACS_VLINE, ACS_HLINE);
        // Get the max size of both windows for use later on
        getmaxyx(myWindow, mx, my);
        getmaxyx(myWindow2, mx2, my2);
        // Set background and colour shceme of child window
        wattron(myWindow2, COLOR_PAIR(2));
        wbkgd(myWindow2, COLOR_PAIR(2));
        // Add the title string to the main window
        mvaddstr(1, (my2/2)-10, "Welcome to MopViewer!");
        // Print string to second window at the x,y co-ordinates specificed
        mvwprintw(myWindow2, mx2/2, ((my2-std::strlen(welcome))/2), "%s", welcome);
        mvwprintw(myWindow2, (mx2/2)+10, ((my2-std::strlen(exitMessage))/2), "%s", exitMessage);
        wmove(myWindow, (mx2/2)+5, ((my2/2)+std::strlen(welcome)));
        // Refresh main window otherwise background colour won't show for some reason
        wrefresh(myWindow);
        char inputString[1];
        // Get the input into inputString
        wgetstr(myWindow2, inputString);
        mopViewer mopViewers;
        testWindow newWindow;
        if (inputString[0] == '1') {
                clear();
                refresh();
                endwin();
                mopViewers.showStats();
        } else if (inputString[0] == '2') {
                clear();
                refresh();
                endwin();
                newWindow.init();
                mopViewers.selectGame();
        }
        endwin();
}
