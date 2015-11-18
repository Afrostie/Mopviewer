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
#ifdef SERIALCLASSIC
#include "../../particle/serialclassic/Particle.h"
#else
#include "../../particle/normal/Particle.h"
#endif
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
     * read a MopState from the mop file and store it in a fragment
     * uses a std::string as an intermediate structure.
     *
     */
    bool readMopStateFromFileAsText(Fragment &input) {
        std::string tmp;
		
        char ch;
        bool result = false;
        do {
            result = this->consumeChar(ch);
            if (!result) return false;
            if(ch=='[') {
                this->error = 1;
                return false;
            }
            tmp.push_back(ch);
        } while ((ch!='$')&&(ch!='@'));
		std::string tmp2= decompress(tmp);
        input.fill(tmp2, tmp2.length());
        return true;
    }

    /**
     * build a MopState from a fragment
     *
     */
    MopState  * buildMopStateFromFragment(Fragment &source) {
        MopState *ms = new MopState();
        Fragment worker;
        Fragment thing;
        int numParticles;
        int pos(0);
        int pos2(0);
        worker.fill(source,'|',pos);
        numParticles = worker.toInt();
        //std::cout << "> Particle count " << numParticles << std::endl;
        for (int x(0); x<numParticles; x++) {
            MopItem mi;
            //pos++;
            worker.fill(source,'|',pos);
            //std::cout << "> extracted string " << worker.getString() << std::endl;
            pos2 = 0;
            thing.fill(worker,',',pos2);
            strcpy(mi.name, thing.c_str());
            //std::cout << "> particle name: " << mi.name << std::endl;
            thing.fill(worker,',',pos2);
            mi.mass = thing.toFloat();
            //std::cout << "> particle mass: " << mi.mass << std::endl;
            thing.fill(worker,',',pos2);
            mi.radius = thing.toFloat();
            //std::cout << "> particle radius: " << mi.radius << std::endl;
            thing.fill(worker,',',pos2);
            mi.visualRepresentation = thing.toInt();
            thing.fill(worker,',',pos2);
            mi.interactionPermission = thing.toInt();
            thing.fill(worker,',',pos2);
            mi.identity = thing.toInt();
            thing.fill(worker,',',pos2);
            mi.red = thing.toInt();
            thing.fill(worker,',',pos2);
            mi.green = thing.toInt();
            thing.fill(worker,',',pos2);
            mi.blue = thing.toInt();
            thing.fill(worker,',',pos2);
            mi.x  = thing.toFloat();
            thing.fill(worker,',',pos2);
            mi.y = thing.toFloat();
            thing.fill(worker,',',pos2);
            mi.z  = thing.toFloat();
            thing.fill(worker,',',pos2);
            mi.xd = thing.toFloat();
            thing.fill(worker,',',pos2);
            mi.yd = thing.toFloat();
            thing.fill(worker,',',pos2);
            mi.zd = thing.toFloat();
            thing.fill(worker,',',pos2);
            mi.xf = thing.toFloat();
            thing.fill(worker,',',pos2);
            mi.yf  = thing.toFloat();
            thing.fill(worker,',',pos2);
            mi.zf = thing.toFloat();
            ms->addMopItem(mi);
        }
        //std::cout << "> converted the String to a MopState" << std::endl;
        return ms;
        return NULL;
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

    /**
     * read a single state from a mopfile (mopfile must already be open)
     */
    MopState * readState() {
        MopState * result = new MopState();
        //int itemCount;
        try {
            // start by reading in the next state from the file as a whole string
            Fragment initial;
            //std::cerr <<" > Reading the state as a Text String  " << std::endl;
            bool worked = this->readMopStateFromFileAsText(initial);
            if (!worked) {
                return NULL;
            }

            // now convert that to a MopState object
            //std::cerr <<" > starting string to mopstate conversion" << std::endl;
 
            result = this->buildMopStateFromFragment(initial);

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


    /**
     * read a single state from the mop file, cycling back to the start of the file
     * if the end is reached
     */
    MopState * readCyclingState() {
        // get the next state from the mopfile
        MopState *incoming = this->readState();
        if (incoming == NULL) {
            this->resetFile();
            incoming = this->readState();
        }
        return incoming;
    } 

    /**
     * This method sets the filename Mopfile will use. This varient prepends the path
     */
    void setFilename (std::string path,std::string fn) {
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
	 /*
	    	void writeState(Particle *localSet, int environmentSetSize) {
        std::stringstream tmp;
        MopItem mi;

        tmp << environmentSetSize;
        for (int x(0); x<environmentSetSize; x++) {
            tmp << "|";
            mi = localSet[x].exportAsMopItem();
            tmp << mi.name << ",";
            tmp << mi.mass << ",";
            tmp << mi.radius << ",";
            tmp << mi.visualRepresentation << ",";
            tmp << mi.interactionPermission << ",";
            tmp << mi.identity << ",";
            tmp << mi.red << ",";
            tmp << mi.green << ",";
            tmp << mi.blue << ",";
            tmp << mi.x << ",";
            tmp << mi.y << ",";
            tmp << mi.z << ",";
            tmp << mi.xd << ",";
            tmp << mi.yd << ",";
            tmp << mi.zd << ",";
            tmp << mi.xf << ",";
            tmp << mi.yf << ",";
            tmp << mi.zf << ",";
        }
        std::string str = this->compress(tmp.str());
        this->outFileStream.open(this->filename.c_str(),std::ios::app);
        this->outFileStream << "@";
		this->outFileStream << str;
		this->outFileStream << "$";
        this->outFileStream.close();
    } 
	 
	 */
    	void writeState(Particle *localSet, int environmentSetSize) {
        std::stringstream tmp;
        MopItem mi;

        tmp << environmentSetSize;
        for (int x(0); x<environmentSetSize; x++) {
            tmp << "|";
            mi = localSet[x].exportAsMopItem();
            tmp << mi.name << ",";
            tmp << mi.mass << ",";
            tmp << mi.radius << ",";
            tmp << mi.visualRepresentation << ",";
            tmp << mi.interactionPermission << ",";
            tmp << mi.identity << ",";
            tmp << mi.red << ",";
            tmp << mi.green << ",";
            tmp << mi.blue << ",";
            tmp << mi.x << ",";
            tmp << mi.y << ",";
            tmp << mi.z << ",";
            tmp << mi.xd << ",";
            tmp << mi.yd << ",";
            tmp << mi.zd << ",";
            tmp << mi.xf << ",";
            tmp << mi.yf << ",";
            tmp << mi.zf << ",";
        }
        tmp << "$";
        std::string str = tmp.str();
        this->outFileStream.open(this->filename.c_str(),std::ios::app);
        this->outFileStream << str;
        this->outFileStream.close();
    }
    

   char *  compress (std::string str, int *size) {
        char * zLibInString = new char[str.length()];
        char * zLibOutString = new char[str.length()];

        int g;
        int h = str.length();
        for (g=0;g<h;g++) {
            zLibOutString[g] ='\0';
        }
        strcpy(zLibInString,str.c_str());
        // zlib struct
        z_stream defstream;
        defstream.zalloc = Z_NULL;
        defstream.zfree = Z_NULL;
        defstream.opaque = Z_NULL;
        defstream.avail_in = (uInt)strlen(zLibInString)+1; // size of input, string + terminator
        defstream.next_in = (Bytef *)zLibInString; // input char array
        defstream.avail_out = (uInt)sizeof(zLibOutString); // size of output
        defstream.next_out = (Bytef *)zLibOutString; // output char array
        // the actual compression work.
        deflateInit(&defstream, Z_BEST_COMPRESSION);
        deflate(&defstream, Z_FINISH);
        deflateEnd(&defstream);
        size= (int*) strlen(zLibOutString);
        return zLibOutString;  
    }



     char * decompress (char * zLibInString, int len) {
         
        char * zLibOutString = new char[len];
        int g;

        for (g=0;g<len;g++) {
            zLibOutString[g] ='\0';
        }

        // zlib struct
        z_stream infstream;
        z_stream defstream;

        infstream.zalloc = Z_NULL;
        infstream.zfree = Z_NULL;
        infstream.opaque = Z_NULL;
        infstream.avail_in = (uInt)((char*)defstream.next_out - zLibInString); // size of input
        infstream.next_in = (Bytef *)zLibInString; // input char array
        infstream.avail_out = (uInt)sizeof(zLibOutString); // size of output
        infstream.next_out = (Bytef *)zLibOutString; // output char array
        // the actual DE-compression work.
        inflateInit(&infstream);
        inflate(&infstream, Z_NO_FLUSH);
        inflateEnd(&infstream);
        return zLibOutString;
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
    void setFilename (std::string fn) {
        this->filename = fn;
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
        this->closeMopfileReader();
        this->inFileStream.clear();
        this->openMopfileReader();
        this->inFileStream.seekg (0, std::ios_base::beg);
    }
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
/**
 * zlib compress and decompress functions:
 * Copyright 2007 Timo Bingmann <tb@panthema.net>
 * Distributed under the Boost Software License, Version 1.0.
 * (See http://www.boost.org/LICENSE_1_0.txt)
 */
std::string compress (const std::string& str,
                            int compressionlevel = Z_BEST_COMPRESSION)
{
    z_stream zs;                        // z_stream is zlib's control structure
    memset(&zs, 0, sizeof(zs));

    deflateInit(&zs, compressionlevel);

    zs.next_in = (Bytef*)str.data();
    zs.avail_in = str.size();           // set the z_stream's input

    int ret;
    char outbuffer[32768];
    std::string outstring;

    // retrieve the compressed bytes blockwise
    do {
        zs.next_out = reinterpret_cast<Bytef*>(outbuffer);
        zs.avail_out = sizeof(outbuffer);

        ret = deflate(&zs, Z_FINISH);

        if (outstring.size() < zs.total_out) {
            // append the block to the output string
            outstring.append(outbuffer,
                             zs.total_out - outstring.size());
        }
    } while (ret == Z_OK);

    deflateEnd(&zs);


    return outstring;
}

/** Decompress an STL string using zlib and return the original data. */
std::string decompress(const std::string& str)
{
    z_stream zs;                        // z_stream is zlib's control structure
    memset(&zs, 0, sizeof(zs));

    inflateInit(&zs);

    zs.next_in = (Bytef*)str.data();
    zs.avail_in = str.size();

    int ret;
    char outbuffer[32768];
    std::string outstring;

    // get the decompressed bytes blockwise using repeated calls to inflate
    do {
        zs.next_out = reinterpret_cast<Bytef*>(outbuffer);
        zs.avail_out = sizeof(outbuffer);

        ret = inflate(&zs, 0);

        if (outstring.size() < zs.total_out) {
            outstring.append(outbuffer,
                             zs.total_out - outstring.size());
        }

    } while (ret == Z_OK);

    inflateEnd(&zs);

    return outstring;
}





};