#pragma once
#include <string>
#include <sstream>
#include "etc/Timing.h"
#include "etc/Numerics.h"
#include "etc/Annealer.h"
#include "etc/nCurses.h"
#include "particle/Particle.h"
#include "tinyxml/ticpp.h"
#include "mopfile/MopFile.h"
//#include "cuda.h"

/**
* The base class for Capsules.
* This class contains all Capsule functionality which does not require inclusion of the external parallelisation libraries
* that Moody supports.
*/
class CapsuleBase
{
protected:
    /**
    * the mop file name to use
    */
    std::string MopName;

    /**
    * the current step size
    */
    int stepSize;

    /**
    * the current number of steps in a state
    */
    int numSteps;

    /**
    * the current number of states to be recorded
    */
    int numStates;

    /**
    * the number of environment particles
    */
    int environmentSetSize;

    /**
    * the number of environment particles there were before the set was extended to allow for the inclusion of population particles
    */
    int originalEnvironmentSetSize;

    /**
    * the number of particles in the experiment population (for evolutionary computation)
    */
    int populationSetSize;

    /**
    * the number of particles in the intermediate set
    */
    int intermediateSetSize;
    /**
    * the current capsule version
    */
    float version;

    /**
     * the gravitational constant in use
     */
    float gravitationalConstant;

    /**
    * the project folder location
    */
    std::string projectFolder;

    /**
    * the configuration data folder location
    */
    std::string cfgFolder;

    /**
    * the particle data folder location
    */
    std::string particleFolder;

    /**
    * the result data folder location
    */
    std::string resultFolder;

    /**
    * the array of particle classes for the environment
    */
    Particle *environmentSet;

    /**
    * the array of particle classes for the EA population
    */
    Particle *populationSet;

    /**
    * the array of particle classes that is used as an intermediate between the envirnment and population sets
    * when mutations are performed on particles they should usually be in this array.
    */
    Particle *intermediateSet;

    /**
    * boolean that says whether the intermediate set exists or not
    */
    bool intermediateSetExists;

    /**
    * boolean that says whether the environment set exists or not
    */
    bool environmentSetExists;

    /**
    * boolean that says whether the population set exists or not
    */
    bool populationSetExists;

    /**
    * An Annealer, to be used when simulated annealing is in use.
    */
    Annealer *cooler;
    /**
    * bool to tell whether the annealer has already been allocated
    */
    bool annealerActive;
    /**
     * slice scan line currently being processed
     */
    int currentScanLine;

    /**
     * slice currently being processed
     */
    int currentSlice;
    /**
     * mapping particle index
     */
    int gravMapParticleIndex;
    /*
     * mapping result storage
     */
    MopItem **gravMapMopList;
    /*
     * number of slices/scanlines/gaps and distances twixt each
     */
    int halfSliceCount;
    float sliceHeight;
    int totalSliceCount;
    int halfScanLineCount;
    int halfScanLinePointCount;
    int totalScanLinePointCount;
    float scanLineLength;
    float scanLineGapLength;
    float scanLineGapWidth;
    int totalScanLines;
    float zExtentFomCentre;
    float yExtentFomCentre;
    float xTop;
    float xBottom;
    float yTop;
    float yBottom;
    float zTop;
    float zBottom;
    
    /**
     * load in the settings for grav map version -
     */
    virtual void loadSettingsGravMap( std::string fileName) {
        try {
            ticpp::Document doc(fileName.c_str());
            doc.LoadFile();
            ticpp::Element* enVars = doc.FirstChildElement("root");
            // project name
            this->MopName = enVars->FirstChildElement("MopName")->GetText();
            // project version
            this->version = atof(enVars->FirstChildElement("version")->GetText().c_str());
            // gravitational measure
            this->gravitationalConstant = atof(enVars->FirstChildElement("G")->GetText().c_str());
            this->zExtentFomCentre = atof(enVars->FirstChildElement("zExtentFomCentre")->GetText().c_str());
            this->zTop = this->zExtentFomCentre;
            this->zBottom = -(this->zExtentFomCentre);
            
            this->yExtentFomCentre = atof(enVars->FirstChildElement("yExtentFomCentre")->GetText().c_str());
            this->yTop = this->yExtentFomCentre;
            this->yBottom = -(this->yExtentFomCentre);
            
            this->scanLineGapWidth = atof(enVars->FirstChildElement("scanLineGapWidth")->GetText().c_str());
            
            this->scanLineLength = atof(enVars->FirstChildElement("scanLineLength")->GetText().c_str());
            
            this->halfScanLinePointCount = atoi(enVars->FirstChildElement("halfScanLinePointCount")->GetText().c_str());
            
            this->scanLineGapLength = scanLineLength/halfScanLinePointCount;
            
            this->totalScanLinePointCount = 2*this->halfScanLinePointCount;
            
            this->halfScanLineCount = atoi(enVars->FirstChildElement("halfScanLineCount")->GetText().c_str());
            this->totalScanLines = 2*halfScanLineCount;
            
            
            // half the number of slices to be taken (because we always start in the centre)
            this->halfSliceCount = atoi(enVars->FirstChildElement("halfSliceCount")->GetText().c_str());
            this->totalSliceCount = 2*this->halfSliceCount;
            // the distance between slices
            this->sliceHeight = atof(enVars->FirstChildElement("sliceHeight")->GetText().c_str());
        }
        catch(ticpp::Exception& ex) {
            //Report the error
            std::cout << ex.m_details;
        }
    }
    
    /**
    * load in the settings for this project -  used internally by Moody, you don't need to call this directly
    */
    virtual void loadSettings( std::string fileName) {
        try {
            ticpp::Document doc(fileName.c_str());
            doc.LoadFile();
            ticpp::Element* enVars = doc.FirstChildElement("root");
            // project name
            this->MopName = enVars->FirstChildElement("MopName")->GetText();
            // project version
            this->version = atof(enVars->FirstChildElement("version")->GetText().c_str());
            // gravitational ant
            this->gravitationalConstant = atof(enVars->FirstChildElement("G")->GetText().c_str());
            // number of states required
            this->numStates = atoi(enVars->FirstChildElement("states")->GetText().c_str());
            // number of steps in a single state
            this->numSteps = atoi(enVars->FirstChildElement("steps")->GetText().c_str());
            // the size, in seconds of a single step
            this->stepSize = atoi(enVars->FirstChildElement("stepSize")->GetText().c_str());
        }
        catch(ticpp::Exception& ex) {
            //Report the error
            std::cout << ex.m_details;
        }
    }

    /**
    * Load the particles for the current experiment from file - used internally by Moody, you don't need to call this directly
    */
    virtual void loadParticles( std::string fileName) {
        try {
            ticpp::Document doc(fileName.c_str());
            doc.LoadFile();
            ticpp::Element* root = doc.FirstChildElement("root");
            ticpp::Element* child;
            // iterate through and collect up all the particles to be found
            for(child = root->FirstChildElement("particle"); child; child = child->NextSiblingElement(false)) {
                ParticleStruct content;
                // setting that controls how this particle interacts with other particles
                // name string size check
                content.name = child->FirstChildElement("name")->GetText().c_str();
                if(content.name.length()>50 ) {
                    std::cerr  << "> Particle Name too long, maximum of 50 characters allowed. Invalid operation prevented - exiting" << std::endl;
                    exit(0);
                }
                std::string irep = child->FirstChildElement("interactionPermission")->GetText();
                if (irep == "interactALL") {
                    content.interactionPermission = Advisor::interactALL;
                }
                if (irep == "interactNONE") {
                    content.interactionPermission = Advisor::interactNONE;
                }
                if (irep == "interactEnvironmentOnly") {
                    content.interactionPermission = Advisor::interactEnvironmentOnly;
                }
                if (irep == "interactDifferentOnly") {
                    content.interactionPermission = Advisor::interactDifferentOnly;
                }
                //setting that determines what class of particle this is, which effects its interactions.
                std::string srep = child->FirstChildElement("identity")->GetText();
                if (srep == "collapsor") {
                    content.identity = Advisor::collapsor;
                }
                if (srep == "collapsorFixed") {
                    content.identity = Advisor::collapsorFixed;
                }
                if (srep == "nonInteractive") {
                    content.identity = Advisor::nonInteractive;
                }
                if (srep == "ordinary") {
                    content.identity = Advisor::ordinary;
                }
                if (srep == "chromosome") {
                    content.identity = Advisor::chromosome;
                }
                if (srep == "planetesimal") {
                    content.identity = Advisor::planetesimal;
                }
                content.visualRepresentation = atoi(child->FirstChildElement("visualSize")->GetText().c_str());
                // particle colouration is set here and can be altered to reflect
                // some change of state for a particle.
                // The resulting rgb values are used by the viewer
                content.red = atoi(child->FirstChildElement("rgb")->FirstChildElement("red")->GetText().c_str());
                content.green = atoi(child->FirstChildElement("rgb")->FirstChildElement("green")->GetText().c_str());
                content.blue = atoi(child->FirstChildElement("rgb")->FirstChildElement("blue")->GetText().c_str());
                // physical info
                content.radius = atof(child->FirstChildElement("radius")->GetText().c_str());
                content.mass = atof(child->FirstChildElement("mass")->GetText().c_str());
                // position/velocity
                content.x = atof(child->FirstChildElement("vector")->FirstChildElement("X")->GetText().c_str());
                content.y = atof(child->FirstChildElement("vector")->FirstChildElement("Y")->GetText().c_str());
                content.z = atof(child->FirstChildElement("vector")->FirstChildElement("Z")->GetText().c_str());
                content.xd = atof(child->FirstChildElement("vector")->FirstChildElement("XD")->GetText().c_str());
                content.yd = atof(child->FirstChildElement("vector")->FirstChildElement("YD")->GetText().c_str());
                content.zd = atof(child->FirstChildElement("vector")->FirstChildElement("ZD")->GetText().c_str());
                content.stored_x = 0;
                content.stored_xd = 0;
                content.stored_y = 0;
                content.stored_yd = 0;
                content.stored_z = 0;
                content.stored_zd = 0;
                content.score = 0;
                // spacecraft specific aditions to Particle
                content.isSpacecraft = false;
                content.structurePercentage = 0;
                content.payloadPercentage = 0;
                content.fuelPercentage = 0;
                content.specificImpulse = 0;
                this->addToSet(content,environment);
            }
        }
        catch(ticpp::Exception& ex) {
            //Report the error
            std::cout << ex.m_details;
        }
    }

public:
    Timing timer;
    MopFile *mop;

    CapsuleBase(void) {
        this->mop = new MopFile;
        this->environmentSetSize = 0;
        this->populationSetSize = 0;
        this->intermediateSetSize = 0;
        this->annealerActive = false;
        this->populationSetExists = false;
        this->environmentSetExists = false;
        this->intermediateSetExists = false;
    }

    virtual ~CapsuleBase(void) {
        delete this->mop;
        if (this->environmentSetExists) {
            delete [] this->environmentSet;
        }
        if (this->populationSetExists) {
            delete [] this->populationSet;
        }
        if (this->intermediateSetExists) {
            delete [] this->intermediateSet;
        }
        if (this->annealerActive) {
            delete this->cooler;
        }
    }


 
    /**
    * Mathematical functions and operations. Here it's public, whereas other Moody classes use it privatelly.
    */
    Numerics maths;
    /**
    * prepare the annealer for use in Stochastic Hill Climbing.
    * it only needs to be allocated once, all other calls to this method will just reset it.
    * as the annealer uses such a tiny amount of memory and gets deleted when the capsule is
    * itself deleted, there is no explicit delete method.
    */
    virtual void prepareAnnealer( int runningTime,  float haltingPct = 0) {
        if (this->annealerActive) {
            this->cooler->initialise(runningTime,haltingPct);
        } else {
            this->cooler = new Annealer();
            this->cooler->initialise(runningTime,haltingPct);
            this->annealerActive = true;
        }
    }

    /**
    * How many particles are currently in the model? (includes non interactive particles)
    */
    int getEnvironmentSetSize()  {
        return this->environmentSetSize;
    }

    /**
    * How many particles are there in the environment set which haven't been put there in order to make room for fitness testing chromosomes
    */
    int getOriginalEnvironmentSetSize()  {
        return this->originalEnvironmentSetSize;
    }
    /**
    * How many particles are currently in the experimentation population?
    */
    int getPopulationSetSize()  {
        return this->populationSetSize;
    }

    /**
    * How many particles are currently in the intermediate set?
    */
    int getIntermediateSetSize()  {
        return this->intermediateSetSize;
    }

    /**
    * tells you the current mop file name in use
    */
    std::string getMopName()  {
        return this->MopName;
    }

    /**
    * Returns a pointer to the environment set. used if a particle needs access to the entire particle set for some comparison operations,
    * such as generating an orbital position. Generally it's better to retreive particles individually
    */
    Particle * getEnvironmentSet()  {
        if (this->environmentSetExists) {
            if (this->environmentSetSize!=0) {
                return this->environmentSet;
            } else {
                std::cerr  << "> Particle environment set array not yet created. Invalid operation prevented - exiting" << std::endl;
                exit(0);
            }
        } else {
            std::cerr  << "> Attempt to retreive the environment set before it has been created. Invalid operation prevented - exiting" << std::endl;
            exit(0);
        }
    }
    /**
    * Returns a specified member of the environment set. Causes an exit if the array index is out of bounds.
    */
    Particle &getEnvironmentSetMember( int index)  {
        if (this->environmentSetExists) {
            if ((index>=0)&&(index<this->environmentSetSize)) {
                return this->environmentSet[index];
            } else {
                std::cerr  << "> Particle environment set array index out of range. Invalid operation prevented - exiting" << std::endl;
                exit(0);
            }
        } else {
            std::cerr  << "> Attempt to retreive a member of the environment set before it has been created. Invalid operation prevented - exiting" << std::endl;
            exit(0);
        }
    }

    /**
    * Returns a specified member of the population set. Causes an exit if the array index is out of bounds.
    */
    Particle &getPopulationSetMember( int index)  {
        if (this->populationSetExists) {
            if ((index>=0)&&(index<this->populationSetSize)) {
                return this->populationSet[index];
            } else {
                std::cerr  << "> Particle population set array index out of range. Invalid operation prevented - exiting" << std::endl;
                exit(0);
            }
        } else {
            std::cerr  << "> Attempt to retreive a member of the population set before it has been created. Invalid operation prevented - exiting" << std::endl;
            exit(0);
        }
    }

    /**
    * Returns a specified member of the intermediate set. Causes an exit if the array index is out of bounds.
    */
    Particle &getIntermediateSetMember( int index)  {
        if (this->intermediateSetExists) {
            if ((index>=0)&&(index<this->intermediateSetSize)) {
                return this->intermediateSet[index];
            } else {
                std::cerr  << "> Particle intermediate set array index out of range. Invalid operation prevented - exiting" << std::endl;
                exit(0);
            }
        } else {
            std::cerr  << "> Attempt to retreive a member of the intermediate set before it has been created. Invalid operation prevented - exiting" << std::endl;
            exit(0);
        }
    }

    /**
    * This method is used when adding a particle to the set stored in this Capsule.
    * Either appends to the end of the array, or creates the array if it doesn't exist yet.
    * Updates record of particle array length
    * which set to add to is determined by the option parameter
     * environment 1
     * population 2
    */
    void addToSet (Particle &p, int option) {
        if (option == environment) {
            int newSize(this->environmentSetSize);
            if (newSize==0) {
                // we need to start as a dynamic array, not a pointer for delete to work properly,
                // so we need to create the first array element and initialise it from the one that was
                // passed as a pointer;
                this->environmentSet =  new Particle[1];
                this->environmentSet[0].fillFromExistingParticle(p);
                this->environmentSetSize = 1;
                this->environmentSetExists = true;
            } else {
                // If we got here, then the aray has been created, so we can safely extend it using pointers
                // to objects without upsetting delete []
                newSize++;
                Particle * pT = new Particle[newSize];
                for (int x(0); x<newSize-1; x++) {
                    pT[x].fillFromExistingParticle(this->environmentSet[x]);
                }
                pT[newSize-1].fillFromExistingParticle(p);
                delete [] this->environmentSet;
                this->environmentSet= pT;
                pT = NULL;
                this->environmentSetSize = newSize;
            }
        }
        if (option == population) {
            int newSize(this->populationSetSize);
            if (newSize==0) {
                // we need to start as a dynamic array, not a pointer for delete to work properly,
                // so we need to create the first array element and initialise it from the one that was
                // passed as a pointer;
                this->populationSet =  new Particle[1];
                this->populationSet[0].fillFromExistingParticle(p);
                this->populationSetSize = 1;
                this->populationSetExists = true;
            } else {
                // If we got here, then the array has been created, so we can safely extend it using pointers
                // to objects without upsetting delete []
                newSize++;
                Particle * pT = new Particle[newSize];
                for (int x(0); x<newSize-1; x++) {
                    pT[x].fillFromExistingParticle(this->populationSet[x]);
                }
                pT[newSize-1].fillFromExistingParticle(p);
                delete [] this->populationSet;
                this->populationSet= pT;
                pT = NULL;
                this->populationSetSize = newSize;
            }

        }
    };

    /**
    * This method is used when adding a particle to the environment set using a ParticleStruct
    * Either appends to the end of the array, or creates the array if it doesn't exist yet.
    * Updates record of particle array length
     * which set to add to is determined by the option parameter
     * environment 1
     * population 2
     */
    void addToSet (ParticleStruct & ps, int option) {
        if (option == environment) {
            int newSize(this->environmentSetSize);
            if (newSize==0) {
                // we need to start as a dynamic array, not a pointer for delete to work properly,
                // so we need to create the first array element and initialise it from the one that was
                // passed as a pointer;
                this->environmentSet =  new Particle[1];
                this->environmentSet[0].fillFromExistingParticleStruct(ps);
                this->environmentSetSize = 1;
                this->environmentSetExists = true;
            } else {
                // If we got here, then the aray has been created, so we can safely extend it using pointers
                // to objects without upsetting delete []
                newSize++;
                Particle * pT = new Particle[newSize];
                for (int x(0); x<newSize-1; x++) {
                    pT[x].fillFromExistingParticle(this->environmentSet[x]);
                }
                pT[newSize-1].fillFromExistingParticleStruct(ps);
                delete [] this->environmentSet;
                this->environmentSet= pT;
                pT = NULL;
                this->environmentSetSize = newSize;
            }
        }
        if (option == population) {
            int newSize(this->populationSetSize);
            if (newSize==0) {
                // we need to start as a dynamic array, not a pointer for delete to work properly,
                // so we need to create the first array element and initialise it from the one that was
                // passed as a pointer;
                this->populationSet =  new Particle[1];
                this->populationSet[0].fillFromExistingParticleStruct(ps);
                this->populationSetSize = 1;
                this->populationSetExists = true;
            } else {
                // If we got here, then the aray has been created, so we can safely extend it using pointers
                // to objects without upsetting delete []
                newSize++;
                Particle * pT = new Particle[newSize];
                for (int x(0); x<newSize-1; x++) {
                    pT[x].fillFromExistingParticle(this->populationSet[x]);
                }
                pT[newSize-1].fillFromExistingParticleStruct(ps);
                delete [] this->populationSet;
                this->populationSet= pT;
                pT = NULL;
                this->populationSetSize = newSize;
            }

        }
    };

    /**
    * This method is used when removing a paticle from the set stored in this Capsule.
    * Updates record of particle array length
     * which set to remove to is determined by the option parameter
     * environment 1
     * population 2
     */
    void deleteFromSet ( int index, int option) {
        if (option == environment) {
            int y(0);
            if (this->environmentSetExists) {
                if ((index<0)||(index>this->environmentSetSize-1)) {
                    std::cerr  << "> Environment set index supplied is out of bounds. Invalid operation prevented - exiting" << std::endl;
                    exit(0);
                }
                int x;
                int newSize = this->environmentSetSize-1;
                if (newSize >0) {
                    Particle * pT = new Particle[newSize];
                    for (x=0; x<newSize+1; x++) {
                        if (x!=index) {
                            pT[y].fillFromExistingParticle(this->environmentSet[x]);
                            y++;
                        }
                    }
                    delete [] this->environmentSet;
                    this->environmentSet= pT;
                    pT = NULL;
                    this->environmentSetSize-=1;
                    if (this->environmentSetSize ==0) {
                        this->environmentSetExists = false;
                    }
                }
            } else {
                std::cerr  << "> Attempt to delete a member of the population set before it has been created. Invalid operation prevented - exiting" << std::endl;
                exit(0);
            }
        }
        if (option == population) {
            int y(0);
            if (this->populationSetExists) {
                if ((index<0)||(index>this->populationSetSize-1)) {
                    std::cerr  << "> Population set index supplied is out of bounds. Invalid operation prevented - exiting" << std::endl;
                    exit(0);
                }
                int x;
                int newSize = this->populationSetSize-1;
                if (newSize >0) {
                    Particle * pT = new Particle[newSize];
                    for (x=0; x<newSize+1; x++) {
                        if (x!=index) {
                            pT[y].fillFromExistingParticle(this->populationSet[x]);
                            y++;
                        }
                    }
                    delete [] this->populationSet;
                    this->populationSet= pT;
                    pT = NULL;
                    this->populationSetSize-=1;
                }
            } else {
                std::cerr  << "> Attempt to delete a member of the population set before it has been created. Invalid operation prevented - exiting" << std::endl;
                exit(0);
            }
        }
    };

    /**
    * This method is used to add extra array entries to the environment set. These entries are then available for use
    * by chromosomes moved (copied) over from the intermediate set.
    * These extra entries should be filled with proper particles before the environment set is actually used
    */
    void extendEnvironmentSetWithBlanks (int size) {
        if (this->environmentSetExists) {
            if ((size>0)&&(this->environmentSetSize>0)) {
                int newSize = this->environmentSetSize+size;
                Particle * tmp = new Particle[newSize];
                for (int x = 0; x<this->environmentSetSize; x++) {
                    tmp[x].fillFromExistingParticle(this->environmentSet[x]);
                }
                // possibly don't actually need to fill these as placemarkers, might remove this
                for (int y = this->environmentSetSize; y<newSize; y++) {
                    tmp[y].turnIntoPlacemarker();
                }
                delete [] this->environmentSet;
                this->environmentSet = tmp;
                tmp = NULL;
                this->environmentSetSize = newSize;
            } else {
                std::cerr  << "> number of extra entries to add to the environment set cannot be zero or negative, or environment set not yet created. Invalid operation prevented - exiting" << std::endl;
                exit(0);
            }
        } else {
            std::cerr  << "> Attempt to extend the environment set before it has been created. Invalid operation prevented - exiting" << std::endl;
            exit(0);
        }
    };

    /**
    * This method is used to record how many particles there are in the environment set before we
    * add blank entries for use during EA experiments
    */
    void recordOriginalEnvironmentSize () {
        this->originalEnvironmentSetSize = this->environmentSetSize;
    };

    /**
     * Returns the number of states currently set
     */
    int getNumStates() {
        return this->numStates;
    }
    /**
     * Returns the number of states currently set
     */
    float getVersion() {
        return this->version;
    }

    /**
     * Returns the number of steps that occur in each state
     */
    int getNumSteps() {
        return this->numSteps;
    }

    /**
     * Returns the size of a single step
     */
    int getStepSize() {
        return this->stepSize;
    }

    /**
     * Returns the current value for G
     */
    float getGravitationalConstant() {
        return this->gravitationalConstant;
    }


    /**
     * returns the absolute path to the result folder
     */
    std::string getResultFolder()  {
        return this->resultFolder;
    }

    /**
     * possibly obvious what this method does...
     * returns pointless int to stop python wrapping getting confused, blessit...
     */
    int setRandomSeed(int seed) {
        srand (seed);
        return 1;
    }

    /**
    * This takes the setting from another capsule, replacing those currently present
    */
    void TakeSettingsFromAnotherCapsule( CapsuleBase &other) {
        this->MopName = other.MopName;
        this->stepSize = other.stepSize;
        this->numSteps = other.numSteps;
        this->numStates = other.numStates;
        this->version = other.version;
        this->gravitationalConstant = other.gravitationalConstant;
        this->projectFolder = other.projectFolder;
        this->cfgFolder = other.cfgFolder;
        this->particleFolder = other.particleFolder;
        this->resultFolder = other.resultFolder;
        this->environmentSetSize = other.environmentSetSize;
    }

    /**
    * This takes the particles from another capsule, replacing those currently present
    */
    void TakeParticlesFromAnotherCapsule( CapsuleBase &other) {
        if (this->environmentSetSize!=0) {
            this->environmentSetSize = 0;
            delete this->environmentSet;
        }
        for (int x(0); x<other.environmentSetSize; x++) {
            this->addToSet(other.environmentSet[x],environment);
        }
        if (this->populationSetSize!=0) {
            this->populationSetSize = 0;
            delete this->populationSet;
        }
        for (int y(0); y<other.populationSetSize; y++) {
            this->addToSet(other.environmentSet[y],population);
        }
    }
    /**
    * Load a project from the specified folder. Also prepares and records the absolute paths
    * for the particle, cfg and result folders
    */
    virtual void loadProject(std::string projectFolder) {
        std::string settingsFileName;
        std::string particleFileName;
        // the project is held in the folder path specified as a parameter for this class, so we load all
        // project content relative to this path
        // need to set up the filenames we'll be using (need better way to do this)
        //store this in the setting, as it will need to be known by other capsules
        // check for end /
        char trailing(projectFolder[projectFolder.length()-1]);
        if ((trailing!='/')&&(trailing!='\\')) {
            projectFolder = projectFolder.append("/");
        }
        // store out the location, and set the other required folder locations relative to this
        this->projectFolder = projectFolder;
        this->particleFolder = projectFolder;
        this->cfgFolder = projectFolder;
        this->resultFolder = projectFolder;
        this->particleFolder = this->particleFolder.append("particles/");
        this->cfgFolder      = this->cfgFolder.append("cfg/");
        this->resultFolder   = this->resultFolder.append("result/");
        settingsFileName     = this->cfgFolder.append("cfg.xml");
        particleFileName     = this->particleFolder.append("particles.xml");
        try {
            this->loadSettings(settingsFileName);
        }
        catch (char *e) {
            std::cout <<"> Failed to load the settings file: "<< *e <<"  Invalid operation prevented - exiting" << std::endl;
            exit(0);
        }
        try {
            this->loadParticles(particleFileName);
			            std::cout << particleFileName << std::endl;

        }
        catch (char *e) {
            std::cout << "> Failed to load the particles file: "<< *e  <<"  Invalid operation prevented - exiting" << std::endl;
            exit(0);
        }
    }

    /**
     * Load a project from the specified folder. Also prepares and records the absolute paths
     * for the particle, cfg and result folders
     */
    virtual void loadProjectGravMap(std::string projectFolder) {
        std::string settingsFileName;
        std::string particleFileName;
        // the project is held in the folder path specified as a parameter for this class, so we load all
        // project content relative to this path
        // need to set up the filenames we'll be using (need better way to do this)
        //store this in the setting, as it will need to be known by other capsules
        // check for end /
        char trailing(projectFolder[projectFolder.length()-1]);
        if ((trailing!='/')&&(trailing!='\\')) {
            projectFolder = projectFolder.append("/");
        }
        // store out the location, and set the other required folder locations relative to this
        this->projectFolder = projectFolder;
        this->particleFolder = projectFolder;
        this->cfgFolder = projectFolder;
        this->resultFolder = projectFolder;
        this->particleFolder = this->particleFolder.append("particles/");
        this->cfgFolder      = this->cfgFolder.append("cfg/");
        this->resultFolder   = this->resultFolder.append("result/");
        settingsFileName     = this->cfgFolder.append("cfg.xml");
        particleFileName     = this->particleFolder.append("particles.xml");
        try {
            this->loadSettingsGravMap(settingsFileName);
        }
        
        catch (char *e) {
            std::cout <<"> Failed to load the gravitational mapping settings file: "<< *e <<"  Invalid operation prevented - exiting" << std::endl;
            exit(0);
        }
        try {
            this->loadParticles(particleFileName);
        }
        catch (char *e) {
            std::cout << "> Failed to load the particles file: "<< *e  <<"  Invalid operation prevented - exiting" << std::endl;
            exit(0);
        }
    }

    /**
    * Export the particle set currently held by this Capsule into a particles.xml file.
    */
    void exportParticlesToXMLFile(std::string fn) {
        std::ofstream outFileStream;
        std::string filename;
        filename = this->getResultFolder().append(fn);
        try {
            outFileStream.open(filename.c_str(),std::ios::out);
            outFileStream << "<?xml version=\"1.0\"?><root xsi:noNamespaceSchemaLocation = \"EnvironmentSet.xsd\" xmlns:xsi = \"http://www.w3.org/2001/XMLSchema-instance\">" << std::endl;
            for (int i(0); i<this->getEnvironmentSetSize(); i++) {
                outFileStream <<  this->getEnvironmentSetMember(i).exportAsXMLForParticlesFile();
            }
            outFileStream << "</root>" << std::endl;
            outFileStream.close();
        } catch(...) {
            std::cerr << "error: cannot write new particles.xml file. Invalid operation prevented - exiting" << std::endl;
            exit(0);
        }
    }

    /**
    *  Tournament Selection, works for lowest or highest value
    */
    virtual int tournamentSelection (int num, int direction) {
        int selected;
        int rSelect;
        rSelect = rand () % this->populationSetSize;
        selected = rSelect;
        float score = this->getPopulationSetMember(rSelect).getScore();
        if (direction == Advisor::LOWEST) {
            for (int x = 1; x<num; x++) {
                rSelect = rand () % this->populationSetSize;
                if (this->getPopulationSetMember(rSelect).getScore()<score) {
                    score = this->getPopulationSetMember(rSelect).getScore();
                    selected = rSelect;
                }
            }
            return selected;
        } else if (direction == Advisor::HIGHEST) {
            for (int x = 1; x<num; x++) {
                rSelect = rand () % this->populationSetSize;
                if (this->getPopulationSetMember(rSelect).getScore()>score) {
                    score = this->getPopulationSetMember(rSelect).getScore();
                    selected = rSelect;
                }
            }
            return selected;
        }
        std::cerr <<"something went wrong during tournament selection. Invalid operation prevented - exiting"<< std::endl;
        return -1;
    }

    /**
    *  find least fit member of the population by stored score, works for lowest or highest score.
    */
    virtual int findLeastFit (int direction) {
        int selected;
        selected = 0;
        float score = this->getPopulationSetMember(0).getScore();
        if (direction == Advisor::LOWEST) {
            for (int x = 1; x<this->populationSetSize; x++) {
                if (this->getPopulationSetMember(x).getScore()<score) {
                    score = this->getPopulationSetMember(x).getScore();
                    selected = x;
                }
            }
            return selected;
        } else if (direction == Advisor::HIGHEST) {
            for (int i = 1; i<this->populationSetSize; i++) {
                if (this->getPopulationSetMember(i).getScore()>score) {
                    score = this->getPopulationSetMember(i).getScore();
                    selected = i;
                }
            }
            return selected;
        }
        std::cerr <<"something went wrong when looking for least fit particle. Invalid operation prevented - exiting"<< std::endl;
        return -1;
    }

    /**
    *  Fitness as a measure of proximity to target, to be used while model is iterating
    */
    virtual float fitnessProximitySingleTarget (int subject, int target) {
        if (this->environmentSet!=NULL) {
            return this->getEnvironmentSetMember(subject).findDistance(this->getEnvironmentSetMember(target));
        }
        return 0;
    }

    /**
    * Fitness as a measure of proximity to target, to  be used after model has finished a run.
    * requires that history be recorded for subject and target.
    */
    virtual float fitnessProximitySingleTargetUsingHistory (int subject,int target) {
        float * record = new float[this->getEnvironmentSetMember(subject).getStoredHistoryCount()];
        for (int i = 0; i<this->getEnvironmentSetMember(subject).getStoredHistoryCount(); i++) {
            record[i] = this->maths.calculateDistance(this->getEnvironmentSetMember(subject).getHistoryEntry(i).x,this->getEnvironmentSetMember(subject).getHistoryEntry(i).y,this->getEnvironmentSetMember(subject).getHistoryEntry(i).z,this->getEnvironmentSetMember(target).getHistoryEntry(i).x,this->getEnvironmentSetMember(target).getHistoryEntry(i).y,this->getEnvironmentSetMember(target).getHistoryEntry(i).z);
        }
        //now iterate through the record and find the lowest value
        float lowest= record[0];
        for(int x=1; x<this->getEnvironmentSetMember(subject).getStoredHistoryCount(); x++) {
            if(record[x]<lowest) {
                lowest=record[x];
            }
        }
        delete [] record;
        return lowest;
    }

    /**
    * Fitness as a measure of time in relative proximity to target, to  be used after model has finished a run.
    * requires that history be recorded for subject and target.
    * A duration of one will make this perform exactly like 'fitnessProximitySingleTargetUsingHistory'
    */

    virtual float fitnessProximityAndDurationSingleTargetUsingHistory (int subject,int target , int duration) {
        double * record = new double[this->getEnvironmentSetMember(subject).getStoredHistoryCount()-duration];
        double accum;
        int pos(0);
        for (int i = 0; i<this->getEnvironmentSetMember(subject).getStoredHistoryCount(); i++) {
            accum = 0;
            for (int j=0; j<duration; j++) {
                accum +=this->maths.calculateDistance(this->getEnvironmentSetMember(subject).getHistoryEntry(i).x,this->getEnvironmentSetMember(subject).getHistoryEntry(i).y,this->getEnvironmentSetMember(subject).getHistoryEntry(i).z,this->getEnvironmentSetMember(target).getHistoryEntry(i).x,this->getEnvironmentSetMember(target).getHistoryEntry(i).y,this->getEnvironmentSetMember(target).getHistoryEntry(i).z);
            }
            pos++;
            record[i] = accum;
        }
        //now iterate through the record and find the lowest value
        double lowest= record[0];
        for(int x=1; x<this->getEnvironmentSetMember(subject).getStoredHistoryCount(); x++) {
            if(record[x]<lowest) {
                lowest=record[x];
            }
        }
        delete [] record;
        return lowest;
    }

    /**
    * Allocates the block of memory required by each particle. How many
    * memory entries are created is specified by the parameter memorySize
    */
    void createParticleMemory(int memorySize) {
        for (int i(0); i<this->environmentSetSize; i++) {
            this->environmentSet[i].createMemory(memorySize);
        }
    }

    /**
    * Unlike the population of environment sets, the intermediate set is of fixed size
    * particles are moved in and out of currently existing positions.
    * therefore the intermediate set must be created first and
    * filled with 'blank' particles. Placemarkers are used initially.
    */
    void createIntermediateSet (int size) {
        if(size > 0) {
            this->intermediateSet =  new Particle[size];
            for (int x(0); x<size; x++) {
                this->intermediateSet[x].turnIntoPlacemarker();
            }
            this->intermediateSetSize = size;
        } else {
            std::cerr  << "> Intermediate set size must be greater than zero. Invalid operation prevented - exiting" << std::endl;
        }
    }

    /**
    *  move a particle from the population set to the intermediate set - copies content only, no particle deleted.
    * particle history and memory content are not copied if present
    */
    void moveParticleFromPopulationToIntermediateSet (int subject, int target) {
        if((subject<0)||(subject>=this->populationSetSize)) {
            std::cerr  << "> population subject index is out of bounds. Invalid operation prevented - exiting" << std::endl;
            exit(0);
        }
        if((target<0)||(target>=this->intermediateSetSize)) {
            std::cerr  << "> intermediate target index is out of bounds. Invalid operation prevented - exiting" << std::endl;
            exit(0);
        }
        this->intermediateSet[target].fillFromExistingParticle(this->populationSet[subject]);
    }

    /**
    *  move a particle from the intermediate set to the population set
    * particle history and memory content are not copied if present
    */
    void moveParticleFromIntermediateToPopulationSet (int subject, int target) {
        if((subject<0)||(subject>=this->intermediateSetSize)) {
            std::cerr  << "> intermediate subject index is out of bounds. Invalid operation prevented - exiting" << std::endl;
            exit(0);
        }
        if((target<0)||(target>=this->populationSetSize)) {
            std::cerr  << "> population target index is out of bounds. Invalid operation prevented - exiting" << std::endl;
            exit(0);
        }
        this->populationSet[target].fillFromExistingParticle(this->populationSet[subject]);
    }

    /**
    *  move a particle from the intermediate set to the environment - copies content only, no particle deleted.
    * particle history and memory are not copied if present
    */
    void moveParticleFromIntermediateToEnvironmentSet (int subject, int target) {
        if((subject<0)||(subject>=this->intermediateSetSize)) {
            std::cerr  << "> intermediate subject index is out of bounds. Invalid operation prevented - exiting" << std::endl;
            exit(0);
        }
        if((target<0)||(target>=this->environmentSetSize)) {
            std::cerr  << "> environment target index is out of bounds. Invalid operation prevented - exiting" << std::endl;
            exit(0);
        }
        if (this->getOriginalEnvironmentSetSize()==0) {
            std::cerr  << "> original environment set size is zero, did you forget to record it before extending it to accomodate chromosomes from the intermediate set? Invalid operation prevented - exiting" << std::endl;
            exit(0);


        }
        this->environmentSet[target].fillFromExistingParticle(this->intermediateSet[subject]);
    }

    /**
    *  mop  file writer wrapper, using this means you don't need to supply the method parameters normally required.
    */
    void writeCurrentStateToMopFile(int choice) {
        if(choice ==Advisor::smallerMop) {
            this->mop->writeStatelight(this->environmentSet,this->getEnvironmentSetSize());
        }
        if(choice ==Advisor::standardMop) {
        this->mop->writeState(this->environmentSet,this->getEnvironmentSetSize());
        }
        
    }

    /**
    *  Traverse the environment particle set and check for then apply any burns due at the state/step passed as parameters
    */
    void processSpacecraft(int state, int step) {
        int burnSequenceIndex;
        for (int k(0); k<this->environmentSetSize; k++) {
            if (this->environmentSet[k].getIsSpacecraft()) {
                burnSequenceIndex = this->environmentSet[k].getBurnSequenceManager().scanBurnTables(state,step);
                if (burnSequenceIndex!=-1) {
                    // retreive the burn and fire it
                    this->environmentSet[k].ExecuteBurn(burnSequenceIndex,state,step);
                }
            }
        }
    }

    /**
    * Record the state vectors of all particles in the environment population.
    * this can then be recalled to restore iterated particles to their initial starting velocity and position
    */
    void recordCurrentStateVectorsForEnvironmentSet() {
        for (int l=0; l< this->environmentSetSize; l++) {
            this->environmentSet[l].storeCurrentStateVector();
        }
    }

    /**
    * Revert  all particles in the environment population to previously stored state vectors.
    */
    void ResetToRecordedStateVectorsForEnvironmentSet() {
        for (int l=0; l< this->environmentSetSize; l++) {
            this->environmentSet[l].resetToStoredStateVector();
        }
    }

    /**
    * create a particle history for each particle in the environment set.
    *
    */
    void createHistoryStorage(int frequency) {
        for (int x(0); x<this->environmentSetSize; x++) {
            this->environmentSet[x].createHistory(this->numStates,this->numSteps,frequency);
        }
    }

    /**
    * Record a history entry for each particle in the environment if it is time to do so.
    * For this to work it must be called once per step during the course of an experiment.
    * Also, it has to have been set up correctly, lest you over-run the array.
    */
    void doHistory() {
        for (int x(0); x<this->environmentSetSize; x++) {
            this->environmentSet[x].doHistory();
        }
    }

    /**
    * Reset the history for every particle in the environment set
    */
    void resetHistory() {
        for (int x(0); x<this->environmentSetSize; x++) {
            this->environmentSet[x].resetHistory();
        }
    }

    /**
    * Delete the history for every particle in the environment set
    */
    void deleteHistory() {
        for (int x(0); x<this->environmentSetSize; x++) {
            this->environmentSet[x].deleteHistory();
        }
    }

    /**
    *  get the amount of the history array which is currently filled
    * Note: This will ONLY work if two conditions have been met
    * 1: That CabsuleBases doHistory is being used to record history for all particles
    * 2: That all particle histories have been set up with a call to CapsuleBases createHistoryStorage method
    */
    const int getStoredHistoryCount() {
        return this->environmentSet[0].getStoredHistoryCount();
    }

    /**
    * create the mop file that will be used to store the time series output from Moody
    */
    void createMopFile(std::string location,std::string fn) {
        this->mop = new MopFile;
        this->mop->setFilename(location,fn);
    }
    
    /**
     *
     */
    float getSliceHeight() {
        return this->sliceHeight;
    }
    
    /**
     *
     */
    int getHalfSliceCount() {
        return this->halfSliceCount;
    }
    
    /**
     *
     */
    int getTotalSliceCount() {
        return this->totalSliceCount;
    }
    /**
     *
     */
    int getHalfScanLineCount() {
        return this->halfScanLineCount;
    }
    /**
     *
     */
    int getTotalScanLines() {
        return this->totalScanLines;
    }
    /**
     *
     */
    float getScanLineGapLength() {
        return this->scanLineGapLength;
    }
    
    /**
     *
     */
    float getScanLineGapWidth() {
        return this->scanLineGapWidth;
    }
    
    /**
     *
     */
    void setGravMapParticleIndex(int gravMapParticleIndex) {
        this->gravMapParticleIndex = gravMapParticleIndex;
    }
    
    /**
     *
     */
    int getGravMapParticleIndex() {
        return gravMapParticleIndex;
    }
    
    float getXTop() {
        return this->xTop;
    }
    
    float getXBottom() {
        return this->xBottom;
    }
    
    float getYTop() {
        return this->yTop;
    }
    
    float getYBottom() {
        return this->yBottom;
    }
    
    float getZTop() {
        return this->zTop;
    }
    
    float getZBottom() {
        return this->zBottom;
    }
    
    Particle &getGravMapParticle()  {
        return this->environmentSet[this->gravMapParticleIndex];
    }
    
    void makeMopStorageArray() {
        this->gravMapMopList = new MopItem*[this->totalScanLines];
        for (int x(0);x<this->totalScanLines;x++) {
            this->gravMapMopList[x] = new MopItem[this->totalScanLinePointCount];
        }
    }
    
    void addToGavMapStorageArray(int x, int y, MopItem tmp) {
        this->gravMapMopList[x][y].fillFromExistingMopItem(tmp);
    }

    void killMopStorageArray() {
        for (int x(0);x<this->totalScanLines;x++) {
            delete[] this->gravMapMopList[x];
        }
        delete[] this->gravMapMopList;
    }
    
    void writeSliceToMopFile(){
        for (int x(0);x<this->totalScanLines;x++) {
            for (int y(0);y<this->totalScanLinePointCount;y++) {
                this->mop->writeMopItemToSmallerMapFile(this->gravMapMopList[x][y]);
            }
        }
    }
    
    void writeToMopStorageArray(MopItem mi) {
        this->gravMapMopList[mi.scanLine][mi.scanLinePos].fillFromExistingMopItem(mi);
    }

    /**
    *  abstract method
    */
    virtual void iterateRK4() = 0;

    /**
    *  abstract method
    */
    virtual void iterateMidPoint() = 0;

    /**
    *  abstract method
    */
    virtual void iterateSym() = 0;
 
    /**
     *  abstract method
     */
    //virtual void gravMapTakePointMeasure() = 0;
    
};
