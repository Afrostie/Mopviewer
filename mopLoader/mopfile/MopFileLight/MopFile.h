#pragma once
/*e
   *This class manages the MOP files used by Moody
 */
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include "MopState.h"
#include "MopItem.h"
#include "../../etc/Fragment.h"
#include "../../particle/normal/Particle.h"
#include <zlib.h>
class MopFile {
private:

        /**
         * the output file stream
         */
        std::ofstream outFileStream;

        /**
         * the input file steam
         */
        std::ifstream inFileStream;

        /**
         * the mop file name to be used
         */
        std::string filename;

        /**
         * Unspecified error flag
         *
         */

        int error;

        /**
         * get a single character from the input file stream
         */
        bool consumeChar(char &ch) {
                if (!this->inFileStream.eof()) {
                        ch = this->inFileStream.get();
                        return true;
                } else {
                        return false;
                }
        }
        //Checks what the next value to be read is, used for checking for end of particles
        char consumeCharPeek(char &ch) {
                if (!this->inFileStream.eof()) {
                        ch = this->inFileStream.peek();
                        return ch;
                } else {
                        return '$';
                }
        }
        //Utility function to ignore characters for a certain length
        bool consumeCharIgnore(char &ch, int length) {
                if (!this->inFileStream.eof()) {
                        this->inFileStream.ignore(length, '$');
                        return true;
                } else {
                        return false;
                }
        }

        /**
         * read a MopState from the mop file and store it in a fragment
         * uses a std::string as an intermediate structure.
         *
         */
        bool readMopStateFromFileAsText(Fragment &input, int skip) {
                std::string tmp;
                std::string tmp2;
                char ch;
                bool result = false;
                int count = 0;
                int particleLength;
                //Start by adding number of particles to string
                while(this->consumeCharPeek(ch) != '|') {
                        result = this->consumeChar(ch);
                        //std::cout << "Current Result is: " << ch << std::endl;
                        tmp.push_back(ch);
                }
                do {
                        result = this->consumeChar(ch);
                        if (!result) return false;
                        if(ch=='[') {
                                this->error = 1;
                                return false;
                        }
                        //Check for start of a particles
                        else if(ch == '|') {
                                //Check whether to skip a paticle
                                if(count < skip) {
                                        //Convert string to int and ignore characters to particle length
                                        particleLength = this->getMopItemLength();
                                        result = this->consumeCharIgnore(ch, particleLength);
                                        result = this->consumeChar(ch);
                                        count++;
                                        //std::cout << "> " << particleLength << std::endl;
                                }
                                else{
                                        //Same as above but read rather than ignore
                                        particleLength = this->getMopItemLength();
                                        //std::cout << "> " << particleLength << std::endl;
                                        for(int i = 0; i < particleLength+1; i++) {
                                                result = this->consumeChar(ch);
                                                tmp.push_back(ch);
                                        }
                                        count = 0;
                                }
                        }
                        else
                                tmp.push_back(ch);

                } while (ch!='$');
                //Output contents of read string for debugging
                std::cout << tmp;
                input.fill(tmp);
                std::cout << "> Finished Reading State" << std::endl;
                return true;
        }

        //Not currently used
        /*std::string loadSingleMopItem(int len) {
                std::string tmp;
                char ch;
                bool result = false;
                int i;
                result = this->consumeChar(ch); // will be '|'
                for (i=0; i<len; i++) {
                        result = this->consumeChar(ch);
                        tmp.push_back(ch);
                }
                return tmp;
        }*/


        int getMopItemLength() {
                std::string tmp;
                char ch;
                int len;
                while(this->consumeCharPeek(ch) != '|') {
                        this->consumeChar(ch);
                        tmp.push_back(ch);
                }
                len = atoi(tmp.c_str());
                //This could do with changing, without this if it ignores the last particle
                //It seems to skip past the end state indicator, causing seg fault
                return len-1;
        }


        /**
         * build a MopState from a fragment
         *
         */
        MopState  * buildMopStateFromFragment(Fragment &source, int skip) {
                MopState *ms = new MopState();
                Fragment worker;
                Fragment thing;
                int numParticles;
                int pos(0);
                pos = worker.fill(source,pos,'|');
                numParticles = worker.toInt();
                numParticles = round(numParticles/(skip+1));
                //std::cout << "> Particle count " << numParticles << std::endl;
                for (int x(0); x<numParticles; x++) {
                        MopItem mi;
                        pos++;
                        pos = thing.fill(source,pos,',');
                        mi.id = thing.toInt();
                        //  std::cout << "> particle id: " << mi.id << std::endl;

                        mi.id = thing.toInt();
                        pos++;
                        pos = thing.fill(source,pos,',');
                        mi.visualRepresentation = thing.toInt();
                        //std::cout << ">  mi.visualRepresentation " <<  mi.visualRepresentation << std::endl;

                        pos++;
                        pos = thing.fill(source,pos,',');
                        mi.red = thing.toInt();
                        pos++;
                        pos = thing.fill(source,pos,',');
                        mi.green = thing.toInt();
                        pos++;
                        pos = thing.fill(source,pos,',');
                        mi.blue = thing.toInt();
                        pos++;
                        pos = thing.fill(source,pos,',');
                        mi.x  = thing.toFloat();
                        //std::cout << "> x: " << mi.x << std::endl;
                        //if(x==2) {
                        //exit(0);
                        //  }
                        //std::cout << "> extracted string " << thing.str() << std::endl;
                        //std::cout << "X value: " << mi.x << std::endl;
                        pos++;
                        pos = thing.fill(source,pos,',');
                        mi.y = thing.toFloat();
                        pos++;
                        pos = thing.fill(source,pos,',');
                        mi.z  = thing.toFloat();
                        ms->addMopItem(mi);
                }
                std::cout << "> converted the String to a MopState" << std::endl;
                return ms;
        }

        /**
         * if the filename given has no *.mop extension, add one.
         */
        std::string setMopFileExtension(std::string fn) {
                std::string temp;
                if(!(fn.substr(fn.find_last_of(".") + 1) == "mop")) {
                        temp = fn.append(".mop");
                }
                return temp;
        }

        /**
         * close the mopfile reader
         */
        void closeMopfileReader() {
                this->inFileStream.close();
        }

public:
        MopFile() {
                this->error = 0;
        }
        ~MopFile() {
        }
        // no copy constructor for this class, there's no need to copy it anyway.

        /**
         * retrieve the filename
         */
        std::string getFilename() {
                return this->filename;
        }

        int maxStates;
        int countStates;

        /**
         * read a single state from a mopfile (mopfile must already be open)
         */
        MopState * readState(int skipCount) {
                MopState * result = new MopState();
                //int itemCount;
                try {
                        // start by reading in the next state from the file as a whole string
                        Fragment initial;
                        //std::cerr <<" > Reading the state as a Text String  " << std::endl;
                        bool worked = this->readMopStateFromFileAsText(initial, skipCount);
                        //std::cout <<initial.c_str()<<std::endl;

                        if (!worked) {
                                return NULL;
                        }

                        // now convert that to a MopState object
                        //std::cerr <<" > starting string to mopstate conversion" << std::endl;
                        result = this->buildMopStateFromFragment(initial, skipCount);
                        //std::cerr <<" >finishing string to mopstate conversion" << std::endl;
                        if (!result) {
                                return NULL;
                        }
                        if (this->error==1) {
                                std::cerr <<"Error reading state from Mopfile" << std::endl;
                                exit(0);
                        }
                } catch(...) {
                        std::cerr <<"Error reading state from Mopfile" << std::endl;
                        exit(0);
                }
                return result;
        }

        /*
         * read a single state from the mop file, cycling back to the start of the file
         * if the end is reached
         */
        MopState * readCyclingState(int skipCount) {
                // get the next state from the mopfile
                MopState *incoming = this->readState(skipCount);
              /* if(this->countStates < this->maxStates){
                    this->countStates++;
                    std::cout << "Count States: " << countStates << std::endl;
                    return incoming;
                }
                else{
                        this->countStates = 1;
                        std::cout << "Count States: " << countStates << std::endl;
                        this->resetFile();
                        incoming = this->readState(skipCount);
                        return incoming;
                }*/
                if (incoming == NULL) {
                        this->resetFile();

                        incoming = this->readState(skipCount);
                }
                return incoming;
                
        }

        /**
         * This method sets the filename Mopfile will use. This varient prepends the path
         */
        void setFilename (std::string path,std::string fn){
                this->filename = path.append(this->setMopFileExtension(fn));
        };



        /**
         * This writes the current set of particles to the mopfile.
         * and sends it to be written to the specified file.
         * The data are stored as follows:
         * 1: An integer declaring how many particles are in the state
         * 2: A set of MopItems in text form, each holding the data for a particle
         * Note: the trailing comma is required, as reading the elements back correctly requires it
         */

        void writeState(Particle *localSet, int environmentSetSize) {
                std::stringstream tmp;
                std::stringstream write;
                MopItem mi;
                for (int x(0); x<environmentSetSize; x++) {

                        mi = localSet[x].exportAsMopItem();
                        tmp << mi.id << ",";
                        tmp << mi.visualRepresentation << ",";
                        tmp << mi.red << ",";
                        tmp << mi.green << ",";
                        tmp << mi.blue << ",";
                        tmp << mi.x << ",";
                        tmp << mi.y << ",";
                        tmp << mi.z << ",";
                        write<< environmentSetSize;

                        write << "|";
                        write << tmp.str().length()+1;
                        write << "|";
                        write << tmp.str();
                        tmp.str("");
                }

                write<< "$";
                std::string str = write.str();
                this->outFileStream.open(this->filename.c_str(),std::ios::app);
                this->outFileStream << str;
                this->outFileStream.close();
        }

        /**
         * close the mopfile writer
         */
        void closeMopfileWriter() {
                this->outFileStream.close();
        }
        /**
         * This method sets the filename Mopfile
         * This method sets the filename Mopfile will use. This varient does not prepend the path,
         * which assumes the path is already in the provided string, as would be the case for a file open dialog.
         * The mop extention is also not added.
         */
        void setFilename (std::string fn){
                this->filename = fn;
        };

         void setMaxStates (int max){
            this->maxStates = max;
            this->countStates = 0;
        };

        /**
         * open the mopfile for reading
         */
        void openMopfileReader() {
                this->closeMopfileReader();
                try {
                        this->inFileStream.open(this->filename.c_str(),std::ios_base::in);
                        this->inFileStream.seekg (0, std::ios_base::beg);
                } catch(...) {
                        std::cerr << "error: cannot open mop file" << std::endl;
                }
        }

        /**
         * reset the mop file back to the start of the file
         */
        void resetFile() {
            std::cout << "Resetting File: " << std::endl;
                this->closeMopfileReader();
                this->inFileStream.clear();
                this->openMopfileReader();
                this->inFileStream.seekg (0, std::ios_base::beg);
        }

};
