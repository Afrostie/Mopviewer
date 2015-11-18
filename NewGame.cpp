#include "NewGame.h"

newGame::newGame(){

}
newGame::~newGame(){

}
void newGame::autoPong(){
	nCurses nCurse;
	int x = 0, y = 0, max_y = 0, max_x = 0, next_x = 0, direction = 1, speed;
	int counter = 0;
		std::cout << "Enter a speed from 1-100 (higher = slower): ";
		std::cin >> speed;
		//Create the inital screen
	nCurse.start(true,false,true,false);
		//Disable the cursor
		curs_set(FALSE);

		while(counter < 1000){
		//Get screen size
		getmaxyx(stdscr, max_y, max_x);
		//Empty screen then print the o to first place
		clear ();
		mvprintw(y,x,"o");
		//Update the screen with the text then wait
		refresh();
		std::this_thread::sleep_for(std::chrono::milliseconds(speed));
		next_x = x + direction;
		counter++;
		//Check bounds then update direction when bounds mets
		if(next_x >= max_x || next_x < 0){
			direction *= -1;
		} else{
			x+= direction;
		}
	}
	endwin();
	newGame newGames;
	newGames.selectGame();
}

void newGame::manualPong(){
	int test;
	int x = 0, y = 0, ch = KEY_F(1);
std::cout << "Use arrow keys to move, 1 to clear screen and 2 to exit: ";
std::cin >> test;
clear();
refresh();
	//Start curses session and disable mouse
	nCurses nCurse;
	nCurse.start(true,false,true,false);
	curs_set(FALSE);

	//Check for when the F2 key is pressed. Move the dot each time the corresponding key has been press
	while((ch = getch()) != GLFW_KEY_2)
	{	switch(ch)
		{	case KEY_LEFT:
			//	clear ();
				mvprintw(y,x,"o");
				refresh();
				x--;
				break;
			case KEY_RIGHT:
				//clear ();
				mvprintw(y,x,"o");
				refresh();
				x++;
				break;
			case KEY_UP:
				//clear ();
				mvprintw(y,x,"o");
				refresh();
				y--;
				break;
			case KEY_DOWN:
			//	clear ();
				mvprintw(y,x,"o");
				refresh();
				y++;
				break;
			case GLFW_KEY_1:
				clear();
				refresh();
				break;
		}
	}
	clear();
	refresh();
	endwin();
	newGame newGames;
	newGames.selectGame();
}

void newGame::textInput(){
	int mx = 0, my = 0, mx2 = 0, my2 = 0;
	char welcome[] = "Enter Anything: ";
	//Create inital screen. Better to replace with the single start function from moody
		WINDOW *myWindow = initscr();
		raw();
		cbreak();
		keypad(myWindow, TRUE);
	//	noecho();
		//Enable use of colour on screen
		start_color();
		//Create two colour pairs for the different screens
		init_pair(1, COLOR_BLACK, COLOR_CYAN);
		init_pair(2,COLOR_BLACK, COLOR_RED);
		init_pair(3,COLOR_WHITE,COLOR_BLACK);
		//Set the colour scheme of window 1 to pair 1
		wattron(myWindow, COLOR_PAIR(1));
		//Set background of main window to colour scheme 1
		wbkgd(myWindow, COLOR_PAIR(1));
		//Create a border for the main console screen
		box(myWindow, ACS_BULLET, ACS_BULLET);
		//Make a new window that is a sub window of main window and add a border to it
		getmaxyx(myWindow, mx, my);
		WINDOW *myWindow2 = subwin(myWindow,mx-5,my-5,3,3);
		box(myWindow2, ACS_VLINE, ACS_HLINE);
		//Get the max size of both windows for use later on
		getmaxyx(myWindow, mx, my);
		getmaxyx(myWindow2, mx2, my2);
		//Set background and colour shceme of child window
		wattron(myWindow2, COLOR_PAIR(2));
		wbkgd(myWindow2, COLOR_PAIR(2));
		//Add the title string to the main window
		mvaddstr(1, (my2/2)-10,"Welcome to MopViewer!");
		//Print string to second window at the x,y co-ordinates specificed
		mvwprintw(myWindow2, mx2/2, ((my2-std::strlen(welcome))/2), "%s", welcome);
		wmove(myWindow, (mx2/2)+5,((my2/2)+std::strlen(welcome)));
		//Refresh main window otherwise background colour won't show for some reason
		wrefresh(myWindow);
		char inputString[2];
		//Get the input into inputString
		wgetstr(myWindow2,inputString);
		//Output the string to the second window and refresh to display it
		mvwprintw(myWindow2,(mx2/2)+1, ((my2-std::strlen(welcome))/2), "You entered: %s", inputString);
		wrefresh(myWindow2);
		//Invert colours at the end because i can
		wbkgd(myWindow, COLOR_PAIR(3));
		refresh();

		//Wait for any input and then end
			endwin();
			newGame newGames;
			newGames.selectGame();
}

void newGame::calculator(){
	newGame newGames;
	int x,y, value1, value2, ch, currentRow = 1;
	nCurses nCurse;
	//nCurse.start(true,true,true,false);
	WINDOW *stdscr = initscr();
	raw();
	nCurse.startColour();
	getmaxyx(stdscr, x,y);
	init_pair(1,COLOR_WHITE, COLOR_BLACK);
	init_pair(2,COLOR_BLACK,COLOR_WHITE);
	wbkgd(stdscr,COLOR_PAIR(1));
	wattron(stdscr,COLOR_PAIR(1));
	refresh();
	nCurse.drawBox(stdscr, ACS_VLINE, ACS_HLINE);


	mvprintw(1, (y/2)-5, "Calculator");
	refresh();
	WINDOW *mainWindow =subwin(stdscr, (x/1.5)+4,y/1.5,x/6,y/6);
	keypad(mainWindow, TRUE);
//	WINDOW *mainWindow =nCurse.makeNewWindow(3,3,1,1,true);
	nCurse.drawBox(mainWindow, ACS_VLINE, ACS_HLINE);
	wattron(mainWindow,COLOR_PAIR(2));
	wbkgd(mainWindow, COLOR_PAIR(2));

	int x2,y2;
	char test[3] = {'1', '+', '4'};
	getmaxyx(mainWindow,x2,y2);
	while(test[1] != 0){
		mvwprintw(mainWindow, currentRow, (y2/2)-17, "Enter your input: ");
		wmove(mainWindow,currentRow, (y/2)-30);
		wrefresh(mainWindow);

		wgetstr(mainWindow,test);
		wrefresh(mainWindow);
		currentRow++;
 		float result;

		if(test[1] == '+'){
			value1 = test[0] - '0';
			value2 = test[2] - '0';
			result = value1 + value2;
			mvwprintw(mainWindow, currentRow, ((y2/2)-17), "Answer is:  %f", result);
			currentRow++;
			wrefresh(mainWindow);
		}

		else if(test[1] == '-'){
			value1 = test[0] - '0';
			value2 = test[2] - '0';
			result = value1 - value2;
			mvwprintw(mainWindow, currentRow, ((y2/2)-17), "Answer is:  %f", result);
			currentRow++;
			wrefresh(mainWindow);
		}
		else if(test[1] == '*'){
			value1 = test[0] - '0';
			value2 = test[2] - '0';
			result = value1 * value2;
			mvwprintw(mainWindow, currentRow, ((y2/2)-17), "Answer is:  %f", result);
			currentRow++;
			wrefresh(mainWindow);
		}
		else if(test[1] == '/'){
			value1 = test[0] - '0';
			value2 = test[2] - '0';
			result = value1 / value2;
			mvwprintw(mainWindow, currentRow, ((y2/2)-17), "Answer is:  %f", result);
			currentRow++;
			wrefresh(mainWindow);
		}
		else if(test[1] == '0'){
			nCurse.stop();
			clear();
			refresh();
		newGames.selectGame();
		}
	}

	nCurse.stop();
//	newGame newGames;
	newGames.selectGame();
}


void newGame::showStats(){

	int x, y, x2,y2,x3,y3;
	nCurses nCurse;
	//nCurse.start(true,true,true,false);
	initscr();
	raw();
	nCurse.startColour();
	getmaxyx(stdscr, x,y);
	init_pair(1,COLOR_WHITE, COLOR_BLACK);
	init_pair(2,COLOR_BLACK,COLOR_WHITE);
	wbkgd(stdscr,COLOR_PAIR(1));
	wattron(stdscr,COLOR_PAIR(1));
	nCurse.drawBox(stdscr, ACS_VLINE, ACS_HLINE);

	WINDOW *window1=newwin(x-4,(y/2)-5,2,2);
	wbkgd(window1,COLOR_PAIR(2));
	nCurse.drawBox(window1, ACS_VLINE, ACS_HLINE);

	WINDOW *window2=newwin(x-4,(y/2)-5,2,(y/2)+2);
	wbkgd(window2,COLOR_PAIR(2));
	nCurse.drawBox(window2, ACS_VLINE, ACS_HLINE);

	wattron(stdscr,A_BOLD);
	mvwprintw(stdscr, 1, (y/2)-10, "Welcome to Mopviewer");


	getmaxyx(window1, x2,y2);
	getmaxyx(window2, x3,y3);

int rowcounter = 0;

	refresh();
	wrefresh(window1);
	wrefresh(window2);




		getch();
		endwin();
}
















void newGame::selectGame(){
	int mx = 0, my = 0, mx2 = 0, my2 = 0;
	char welcome[] = "Enter 1 to Watch, 2 to Draw, 3 for Text Input, 4 for Calculator, 5 to open MopViewer: ";
	char exitMessage[] = "Enter anything else to exit";
	//Create inital screen. Better to replace with the single start function from moody
		WINDOW *myWindow = initscr();
		raw();
		cbreak();
		keypad(myWindow, TRUE);
	//	noecho();
		//Enable use of colour on screen
		start_color();
		//Create two colour pairs for the different screens
		init_pair(1, COLOR_BLACK, COLOR_CYAN);
		init_pair(2,COLOR_BLACK, COLOR_RED);
		init_pair(3,COLOR_WHITE,COLOR_BLACK);
		//Set the colour scheme of window 1 to pair 1
		wattron(myWindow, COLOR_PAIR(1));
		//Set background of main window to colour scheme 1
		wbkgd(myWindow, COLOR_PAIR(1));
		//Create a border for the main console screen
		box(myWindow, ACS_BULLET, ACS_BULLET);
		//Make a new window that is a sub window of main window and add a border to it
		getmaxyx(myWindow, mx, my);
		WINDOW *myWindow2 = subwin(myWindow,mx-5,my-5,3,3);
		box(myWindow2, ACS_VLINE, ACS_HLINE);
		//Get the max size of both windows for use later on
		getmaxyx(myWindow, mx, my);
		getmaxyx(myWindow2, mx2, my2);
		//Set background and colour shceme of child window
		wattron(myWindow2, COLOR_PAIR(2));
		wbkgd(myWindow2, COLOR_PAIR(2));
		//Add the title string to the main window
		mvaddstr(1, (my2/2)-10,"Welcome to MopViewer!");
		//Print string to second window at the x,y co-ordinates specificed
		mvwprintw(myWindow2, mx2/2, ((my2-std::strlen(welcome))/2), "%s", welcome);
		mvwprintw(myWindow2, (mx2/2)+10, ((my2-std::strlen(exitMessage))/2), "%s", exitMessage);
		wmove(myWindow, (mx2/2)+5,((my2/2)+std::strlen(welcome)));
		//Refresh main window otherwise background colour won't show for some reason
		wrefresh(myWindow);
		char inputString[1];
		//Get the input into inputString
		wgetstr(myWindow2,inputString);
	newGame newGames;
		testWindow newWindow;
	if(inputString[0] == '1'){
		clear();
		refresh();
		endwin();
		newGames.autoPong();
	}
	else if (inputString[0] == '2'){
		clear();
		refresh();
		endwin();
		newGames.manualPong();
	}
	else if (inputString[0] == '3'){
		clear();
		refresh();
		endwin();
		newGames.textInput();
	}

	else if (inputString[0] == '4'){
		clear();
		refresh();
		endwin();
		newGames.calculator();
	}

	else if(inputString[0] == '5'){
		clear();
		refresh();
		endwin();
		newWindow.init();
		newGames.selectGame();
	}

	else if(inputString[0] == '6'){
		clear();
		refresh();
		endwin();
		newGames.showStats();
	}
	else
		endwin();
}
