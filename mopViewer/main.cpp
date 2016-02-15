#include <curses.h>
#include <iostream>
#include "mopViewer.h"
#include <string>



int main() {
        mopViewer mopViewers;
        std::string fileName;
        int skipCount;
        std::cout << "Enter the MopFile Name (No need to use extension/location: ";
        std::cin >> fileName;
        std::cout << std::endl << "Enter Required Skip Count: ";
        std::cin >> skipCount;
        mopViewers.selectGame(fileName, skipCount);
        return 0;
}
