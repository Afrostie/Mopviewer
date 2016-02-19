#include <curses.h>
#include <iostream>
#include "mainWindow.h"
#include <string>



int main() {
        mainWindow mainWindows;
        std::string fileName;
        float skipCount;
        std::cout << "Enter the MopFile Name (No need to use extension/location): ";
        std::cin >> fileName;
        std::cout << std::endl
                  << "Enter Required Skip Count: ";
        std::cin >> skipCount;
        if(skipCount < 0){
                std::cout << "Skip Count Must be 0 or larger" << std::endl;
                std::cout << "Exiting Now!" << std::endl;
                return 0;
        }
        mainWindows.selectGame(fileName, skipCount);
        return 0;
}
