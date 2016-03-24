#include <curses.h>
#include <iostream>
#include "mopViewer/viewer/mainWindow.h"
#include <string>



int main() {
        mainWindow mainWindows;
        std::string fileName;
        float skipCount;


        std::cout << " _ __ ___   ___  _ __/\\   /(_) _____      _____ _ __ " << std::endl;    
        std::cout << "| '_ ` _ \\ / _ \\| '_ \\ \\ / / |/ _ \\ \\ /\\ / / _ \\ '__|" << std::endl;
        std::cout << "| | | | | | (_) | |_) \\ V /| |  __/\\ V  V /  __/ |   " << std::endl;
        std::cout << "|_| |_| |_|\\___/| .__/ \\_/ |_|\\___| \\_/\\_/ \\___|_|   " << std::endl;
        std::cout << "                |_|                                  " << std::endl;

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
