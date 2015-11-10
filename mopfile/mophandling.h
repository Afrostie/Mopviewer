/**
* Author      : Dr Carey Pridgeon
* Copyright   : Dr Carey Pridgeon 2009
* Licence     : Licensed under the Apache License, Version 2.0 (the "License");
*             : you may not use this file except in compliance with the License.
*             : You may obtain a copy of the License at
*             : http://www.apache.org/licenses/LICENSE-2.0
*             :
*             : Unless required by applicable law or agreed to in writing,
*             : software distributed under the License is distributed on
*             : an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
*             : either express or implied. See the License for the specific
*             : language governing permissions and limitations under the License.
*/
#ifndef MOPHANDLING_H
#define MOPHANDLING_H
//#include <QString>
#include <math.h>
#include "MopFile.h"
#include <string>
#include <iostream>

class MopHandling
{
private:
    MopFile *localMop;
    MopState *displayState;
    MopState *loadedState;
    bool haveLocalMop;
    bool firstState;
    bool haveLoadedState;
    bool haveDisplayState;
    int scaler;
    int originalScaler;
    int stateCount;
    int displayWidth;
    int displayHeight;
    MopItem tmp;


    double m1,m2,m3;
    double maxDistanceFromOrigin;
    inline double sqr( double x) const {
        return ((x)*(x));
    }
public:
    bool ready;
    bool mopActive;
	std::string fileName;
	static MopHandling &shared_instance() {static MopHandling MopHandling1(1); return MopHandling1;}
    MopHandling( int scaler) {
						 this->stateCount = 0;
        this->firstState = true;
        this->haveDisplayState = false;
        this->haveLoadedState = false;
        this->haveLocalMop = false;
        this->mopActive = false;
        this->scaler = scaler;
        this->maxDistanceFromOrigin = 0;
        this->ready = false;
		createMopFile("10000_days.mop");
	}

    ~MopHandling() {
        delete this->localMop;
        delete this->loadedState;
        delete this->displayState;
    }


    void createMopFile(std::string filename) {
      std::cout <<"Step 1" << std::endl;
       if (filename.empty()==false) {
            if (this->haveLocalMop ==true) {
                delete this->localMop;
                this->haveLocalMop=false;
                this->localMop = new MopFile();
                this->haveLocalMop = true;
                this->localMop->setFilename(filename);
                this->localMop->openMopfileReader();
                this->loadNextState();
                this->stateCount = 0;
            } else {
                this->haveLocalMop=false;
                this->localMop = new MopFile();
                this->haveLocalMop = true;
                this->localMop->setFilename(filename);
                this->localMop->openMopfileReader();
                this->loadNextState();
                this->stateCount = 0;
            }
        }
    }
    // prepare loaded and display states for use

    void loadNextState() {
      std::cout << "Step 1" << std::endl;
        if (this->haveDisplayState) {
            delete this->displayState;
            this->haveDisplayState = false;
        }
        if (this->haveLoadedState) {
            delete this->loadedState;
            this->haveLoadedState = false;
        }
        std::cout <<"Step 2" << std::endl;
        // get the next state from the mopfile
        this->loadedState = this->localMop->readState();
        if (this->loadedState == NULL) {
            this->localMop->resetFile();
            this->loadedState = this->localMop->readState();
            this->stateCount = 0;
        }
        std::cout <<"Step 3" << std::endl;
        // prepare the display state by scaling it's members
        this->displayState = new MopState();
        // is this the very first state? If so, establish the correct initial scaler
        if (this->firstState) {
            this->firstState = false;
            // find out which particle is furthest out, on which axis
            double xv,yv,zv;
            double max = 0;
            for (int j(0); j<this->loadedState->getItemCount(); j++) {
                xv = sqrt(sqr(this->loadedState->getMopItem(j).x));
                yv = sqrt(sqr(this->loadedState->getMopItem(j).y));
                zv = sqrt(sqr(this->loadedState->getMopItem(j).z));
                if (xv>max) max = xv;
                if (yv >max) max = yv;
                if (zv>max) max = zv;
            }
            this->maxDistanceFromOrigin = max;
            this->originalScaler = this->scaler;

        }
        std::cout <<"Step 4" << std::endl;
        for (int x(0); x<this->loadedState->getItemCount(); x++) {
            m1 = (sqrt(sqr(this->loadedState->getMopItem(x).x))/(this->maxDistanceFromOrigin+(0.5*this->maxDistanceFromOrigin)));
            m2 = (sqrt(sqr(this->loadedState->getMopItem(x).y))/(this->maxDistanceFromOrigin+(0.5*this->maxDistanceFromOrigin)));
            m3 = (sqrt(sqr(this->loadedState->getMopItem(x).z))/(this->maxDistanceFromOrigin+(0.5*this->maxDistanceFromOrigin)));
            if (this->loadedState->getMopItem(x).x>0) {
                tmp.x = m1*this->scaler;
            } else {
                tmp.x = m1*(-this->scaler);
            }

            if (this->loadedState->getMopItem(x).y>0) {
                tmp.y = m2*this->scaler;
            } else {
                tmp.y = m2*(-this->scaler);
            }

            if (this->loadedState->getMopItem(x).z>0) {
                tmp.z = m3*this->scaler;
            } else {
                tmp.z = m3*(-this->scaler);
            }
            // now colour
            tmp.red = this->loadedState->getMopItem(x).red;
            tmp.green = this->loadedState->getMopItem(x).green;
            tmp.blue = this->loadedState->getMopItem(x).blue;
            this->displayState->addMopItem(tmp);
        }
        std::cout <<"Step 5" << std::endl;
        // ready for state to be displayed
        this->stateCount++;
        this->haveDisplayState = true;
        this->haveLoadedState = true;
        this->ready = true;
    }

    void rescale() {
        if (this->haveLoadedState) {
            if (this->haveDisplayState) {
                delete this->displayState;
                this->haveDisplayState = false;
            }
            this->displayState = new MopState();
            for (int x(0); x<this->loadedState->getItemCount(); x++) {
                m1 = (sqrt(sqr(this->loadedState->getMopItem(x).x))/(this->maxDistanceFromOrigin+(0.5*this->maxDistanceFromOrigin)));
                m2 = (sqrt(sqr(this->loadedState->getMopItem(x).y))/(this->maxDistanceFromOrigin+(0.5*this->maxDistanceFromOrigin)));
                m3 = (sqrt(sqr(this->loadedState->getMopItem(x).z))/(this->maxDistanceFromOrigin+(0.5*this->maxDistanceFromOrigin)));
                if (this->loadedState->getMopItem(x).x>0) {
                    tmp.x = m1*this->scaler;
                } else {
                    tmp.x = m1*(-this->scaler);
                }

                if (this->loadedState->getMopItem(x).y>0) {
                    tmp.y = m2*this->scaler;
                } else {
                    tmp.y = m2*(-this->scaler);
                }

                if (this->loadedState->getMopItem(x).z>0) {
                    tmp.z = m3*this->scaler;
                } else {
                    tmp.z = m3*(-this->scaler);
                }
                this->displayState->addMopItem(tmp);
            }
            this->haveDisplayState = true;
        }

    }

    void deleteMopFile() {
        delete this->localMop;
        this->haveLocalMop = false;
        this->firstState = true;
        this->scaler = this->originalScaler;
    }

    void restartPlayback() {
        if (this->haveLoadedState) {
            delete this->loadedState;
            this->localMop->resetFile();
            this->loadedState = this->localMop->readState();
            this->stateCount = 0;
        }
    }

    MopFile * getMopFile() {
        return this->localMop;
    }

    MopState * getDisplayState() {
        return this->displayState;
    }

    MopState * getLoadedState() {
        return this->loadedState;
    }

    int getScaler() {
        return this->scaler;
    }

    void setScaler(int scaler) {
        this->scaler = this->originalScaler*scaler;
        this->rescale();
    }
    int getOriginalScaler() {
        return this->originalScaler;
    }
    int getNumParticles() {
        return this->loadedState->getItemCount();
    }

    int getStateCount() {
        return this->stateCount;
    }
};

#endif // MOPHANDLING_H
