/**
 *       This file simply asks for the Mopfile and then loads mopViewer
 */

#ifdef _WIN32
#include <nCurses\curses.h>
#else
#include <curses.h>
#endif
#include <iostream>
#include "src/Viewer/mainWindow.h"
#include <string>

int main() {
        mainWindow mainWindows;
        std::string fileName;
        float skipCount;

        //Display the title in ASCII art, because why not?
        std::cout << " _ __ ___   ___  _ __/\\   /(_) _____      _____ _ __ " << std::endl;
        std::cout << "| '_ ` _ \\ / _ \\| '_ \\ \\ / / |/ _ \\ \\ /\\ / / _ \\ '__|" << std::endl;
        std::cout << "| | | | | | (_) | |_) \\ V /| |  __/\\ V  V /  __/ |   " << std::endl;
        std::cout << "|_| |_| |_|\\___/| .__/ \\_/ |_|\\___| \\_/\\_/ \\___|_|   " << std::endl;
        std::cout << "                |_|                                  " << std::endl;


        #ifdef MOPFILELIGHT
        std::cout << "mopViewer was compiled for MopFileLight" << std::endl;
        #endif
        #ifdef MOPFILE
        std::cout << "mopViewer was compiled for MopFileLight" << std::endl;
        #endif
        //Get the file name and skip count
        std::cout << "Enter the MopFile Name (No need to use extension/location): ";
        std::cin >> fileName;
        std::cout << std::endl
                  << "Enter Required Skip Count: ";
        std::cin >> skipCount;
        //Check the skipcount is in the required range
        if((skipCount < 0) || (skipCount > 10)){
                std::cout << "Skip Count must be between 0 and 10" << std::endl;
                std::cout << "Exiting Now!" << std::endl;
                return 0;
        }
        mainWindows.selectGame(fileName, skipCount);
        return 0;
}
