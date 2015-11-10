/*
* MopState.cpp
*
*  Created on: 8 Oct 2009
*      Author: Carey
*/

#include "MopState.h"

MopState::MopState() {
    this->itemCount = 0;
}

MopState::~MopState() {
    delete [] this->content;
}

/**
* add a new item to the state being reconstructed from a mopfile
*/
void MopState::addMopItem(MopItem current)
{   int x;
    MopItem * temp;
    if (this->itemCount==0) {
        this->content = new MopItem[1];
        this->content[0].fillFromExistingMopItem(current);
        this->itemCount = 1;
    }
    else {
        this->itemCount++;
        temp = new MopItem[this->itemCount];
        for(x = 0; x <this->itemCount-1; x++) {
            temp[x].fillFromExistingMopItem(this->content[x]);
        }
        temp[this->itemCount-1].fillFromExistingMopItem(current);
        delete [] this->content;
        this->content = temp;
        temp = NULL;
    }
}
