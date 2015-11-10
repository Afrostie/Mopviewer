#pragma once
/**
* This class represents a particle in Moody.
* Particles are responsible for their own movement, so contain internal integration methods.
* Particles can also be used as spacecraft and/or subjects of evolutionary computation experiments.
*/

#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <time.h>
#include <cmath>
#include "../etc/Advisor.h"
#include "../etc/Numerics.h"
#include "../mopfile/MopState.h"
#include "memory/MemoryEntry.h"
#include "memory/MemoryContent.h"
#include "struct/ParticleStruct.h"
#include "history/ParticleHistory.h"
#include "spacecraft/spacecraft.h"
#include "json/json.h"

class Particle {
private:
    /**
    * this particles mass
    */
    double mass;

    /**
    * stored copy of this particles mass
    */
    double stored_mass;

    /**
    * the radius for this particle
    */
    double radius;

    /**
    * The position of this particle along the X axis
    */
    double x;

    /**
    * The position of this particle along the Y axis
    */
    double y;

    /**
    * The position of this particle along the Z axis
    */
    double z;

    /**
    * The velocity of this particle along the X axis
    */
    double xd;

    /**
    * The velocity of this particle along the Y axis
    */
    double yd;

    /**
    * The velocity of this particle along the Z axis
    */
    double zd;

    /**
    * The force acting on this particle along the X axis
    */
    double xf;

    /**
    * The force acting on this particle along the Y axis
    */
    double yf;

    /**
    * The force acting on this particle along the Z axis
    */
    double zf;

    /**
    * X Axis location member that is read by other particles during integration
    */
    double x_ext;

    /**
    * Y Axis location member that is read by other particles during integration
    */
    double y_ext;

    /**
    * Z Axis location member that is read by other particles during integration
    */
    double z_ext;

    /**
    * The stored initial position of this particle along the X axis
    */
    double stored_x;

    /**
    * The stored initial position of this particle along the Y axis
    */
    double stored_y;

    /**
    * The stored initial position of this particle along the Z axis
    */
    double stored_z;

    /**
    * The stored initial velocity of this particle along the X axis
    */
    double stored_xd;

    /**
    * The stored initial velocity of this particle along the Y axis
    */
    double stored_yd;

    /**
    * The stored initial velocity of this particle along the Z axis
    */
    double stored_zd;

    /**
    * the red portion of this particles colour
    */
    int red;

    /**
    * the green portion of this particles colour
    */
    int green;

    /**
    * the blue portion of this particles colour
    */
    int blue;

    /**
    * the std::string name of this particle
    */
    std::string name;

    /**
    *  What types of particle this particle is allowed to interact with.
    */
    int interactionPermission;

    /**
    * what type this particle is - (collapsor/normal particle/placemark/spacecraft)
    */
    int identity;

    /**
    *  what size this particle will have when displayed in a visualisation
    */
    int visualRepresentation;

    /**
    * the current score of this particle (when an EA is being performed)
    */
    double score;

    /**
    * determines whether this particle is set up to be a spacecraft or not
    */
    bool isSpacecraft;

    /**
    * tells us whether the spacecraft burn sequence manager has been allocated
    */
    bool burnsManagerActive;

    /**
    * the percentage of this particles total mass that is structural content
    */
    double structurePercentage;

    /**
    * the percentage of this particles total mass that is payload content
    */
    double payloadPercentage;

    /**
    * the percentage of this particles total mass that is fuel content
    */
    double fuelPercentage;

    /**
    * the specific impulse value in use
    */
    double specificImpulse;

    /**
    * the percentage of this particles total mass that is structural content
    */
    double stored_structurePercentage;

    /**
    * the percentage of this particles total mass that is payload content
    */
    double stored_payloadPercentage;

    /**
    * the percentage of this particles total mass that is fuel content
    */
    double stored_fuelPercentage;

    /**
    * the specific impulse value in use
    */
    double stored_specificImpulse;

    /**
    * this particles memory (used during integration)
    */
    MemoryEntry * memory;

    /**
    * how many entries are stored in memory
    */
    int memorySize;

    /**
    * this particles history
    */
    ParticleHistory *history;

    /**
    * what is the overall size of the history storage array
    */
    int historySize;

    /**
    * what index are we currently at in the history
    */
    int currentHistoryIndex;

    /**
    * rotation variable storage (used when placing/moving particles on the surface of a sphere)
    */
    double curlyThing;

    /**
    * other rotation variable storage (used when placing/moving particles on the surface of a sphere)
    */
    double theta;

    /**
    * counter used to track the time till the next history entry needs to be stored
    */
    int historyCounter;

    /**
    * interval at which (in steps) a new history entry is to be stored
    */
    int historyInterval;

    /**
    * bool that lets us know if the history store exists
    */
    int historyExists;

    /**
    * Burn sequence manager, used if this particle is set up as a spacecraft
    */
    BurnSequenceManager *burnsManager;

    /**
    * find F.
    */
    double gravitationalForceCalc( double m1,  double m2, double distance, double G) {
        return G * (m1 * m2) / distance;
    }

    /**
    * Mathematical functions and operations
    */
    Numerics maths;

    /**
     * distance between slices
     */
    double sliceHeight;
    /**
     * distance between measuiring points along a single line on a slice
     */
    double gapWidth;
    /**
     * distance between measuiring points between a single line on a slice
     */
    double gapLength;

    /**
    * create this particles required history array. Do not call directly, use createHistory instead.
    */
    void buildHistoryStorage(int historySize) {
        if (historySize >0) {
            if (historyExists) {
                std::cerr  << "> Particle history has already been created. Invalid operation prevented - exiting" << std::endl;
                exit(0);
            }
            this->history = new ParticleHistory[historySize];
            this->historySize = historySize;
            this->historyExists = true;
            this->currentHistoryIndex = 0;
        }  else {
            std::cerr  << "> Particle history size must be >0. Invalid operation prevented - exiting" << std::endl;
            exit(0);
        }
    }

public:
    Particle();
    ~Particle();


    /**
     *  setup for gravMap
     *
     */
    void setupGravMeasuringParticle(double sliceHeight, double gapWidth, double gapLength) {
        this->sliceHeight = sliceHeight;
        this->gapWidth = gapWidth;
        this->gapLength = gapLength;
    }

    /**
     *  gravmap motion for measuring particle along scanline
     * perameter defines up or down (1 = positive, 0 = negative)
     */
    void moveParticleAlongScanlineOnce(int direction) {
        if (1) {
            this->y+=this->gapLength;
        }
        if (0) {
            this->y-=this->gapLength;
        }
    }

    /**
     *  gravmap motion for measuring particle to next scanline, resetting to origin
     * perameter defines up or down (1 = positive, 0 = negative)
     */
    void moveParticleToNextScanlineGravMap(int direction) {
        this->y = 0;
        if(1) {
            this->y+=this->gapWidth;
        }
        if (0) {
            this->y-=this->gapWidth;
        }

    }
    /**
     *  gravmap motion for measuring particle to next slice, resetting to origin on x and y if positive. If negative move off origin along x by one gaplength to avoid double measures of origin
     * perameter defines up or down (1 = positive, 0 = negative)
     */
    void moveParticletoNextSliceGravMap(int direction) {
        this->x=0;
        this->y=0;
        if(1) {
            this->z+=this->sliceHeight;
        }

        if(0) {
            this->z-=this->sliceHeight;
            this->x-=this->gapLength;
        }
    }
    /**
     *  gravmap motion reset to origin in all parameters.
     */
    void resetToOriginGravMap() {
        this->x=0;
        this->y=0;
        this->z=0;
    }



    void turnIntoPlacemarker () {
        this->isSpacecraft = false;
        this->name = "placemarker";
        this->identity = Advisor::nonInteractive;
        this->interactionPermission = Advisor::interactNONE;
        this->visualRepresentation = 1;
        this->mass = 0;
        this->radius = 0;
        this->x = 0;
        this->y = 0;
        this->z = 0;
        this->xd = 0;
        this->yd = 0;
        this->zd = 0;
        this->red = 0;
        this->green = 0;
        this->blue = 0;
        this->stored_x = 0;
        this->stored_xd = 0;
        this->stored_y = 0;
        this->stored_yd = 0;
        this->stored_z = 0;
        this->stored_zd = 0;
        this->score = 0;
        this->x_ext = 0;
        this->y_ext = 0;
        this->z_ext = 0;
    }

    /**
    * Make a placemark particle with the specified location (x,y,z) and colour (red,green,blue)
    */
    void turnIntoPlacemarker(double x, double y, double z, int red, int green, int blue) {
        this->isSpacecraft = false;
        this->name = "placemarker";
        this->identity = Advisor::nonInteractive;
        this->interactionPermission = Advisor::interactNONE;
        this->visualRepresentation = 1;
        this->mass = 0;
        this->radius = 0;
        this->x = x;
        this->y = y;
        this->z = z;
        this->xd = 0;
        this->yd = 0;
        this->zd = 0;
        this->red = red;
        this->green = green;
        this->blue = blue;
        this->stored_x = 0;
        this->stored_xd = 0;
        this->stored_y = 0;
        this->stored_yd = 0;
        this->stored_z = 0;
        this->stored_zd = 0;
        this->score = 0;
        this->x_ext = 0;
        this->y_ext = 0;
        this->z_ext = 0;
    }

    /**
    * make a placemark particle with the same location and colour as an existing particle.
    */
    void turnIntoPlacemarker(Particle &other) {
        ParticleStruct tmp;
        tmp.isSpacecraft = false;
        tmp.name = "placemarker";
        tmp.visualRepresentation = 1;
        tmp.mass = 0;
        tmp.radius = 0;
        tmp.identity = Advisor::nonInteractive;
        tmp.interactionPermission = Advisor::interactNONE;
        tmp.x = other.x;
        tmp.y = other.y;
        tmp.z = other.z;
        tmp.xd = 0;
        tmp.yd = 0;
        tmp.zd = 0;
        tmp.red = other.red;
        tmp.green = other.green;
        tmp.blue = other.blue;
        tmp.score = 0;
        tmp.x_ext = 0;
        tmp.y_ext = 0;
        tmp.z_ext = 0;
    }
    /**
    * copy the content of another particle into this one. Note that particle memory and history are never copied
    */
    void fillFromExistingParticle (Particle & other) {
        if (this != &other) // protect against invalid self-assignment
        {
            if (this->burnsManagerActive) {
                delete this->burnsManager;
            }
            this->name = other.name;
            this->mass = other.mass;
            this->radius = other.radius;
            this->x = other.x;
            this->y = other.y;
            this->z = other.z;
            this->x_ext = other.x;
            this->y_ext = other.y;
            this->z_ext = other.z;
            this->curlyThing = other.curlyThing;
            this->theta = other.theta;
            this->xd = other.xd;
            this->yd = other.yd;
            this->zd = other.zd;
            this->visualRepresentation = other.visualRepresentation;
            this->identity = other.identity;
            this->interactionPermission = other.interactionPermission;
            this->red = other.red;
            this->green = other.green;
            this->blue = other.blue;
            this->score = other.score;
            this->isSpacecraft = other.isSpacecraft;
            if (this->isSpacecraft) {
                this->fuelPercentage = other.fuelPercentage;
                this->payloadPercentage = other.payloadPercentage;
                this->structurePercentage = other.structurePercentage;
                this->specificImpulse = other.specificImpulse;
                this->burnsManagerActive = other.burnsManagerActive;
                if (this->burnsManagerActive) {
                    this->burnsManager = new BurnSequenceManager();
                    this->burnsManager->fillFromExistingBurnSequenceManager(*other.burnsManager);
                }
            }
        }
    }

    bool operator == ( const Particle & other) {
        // not all members need to be compared
        if (this->name != other.name) {
            return false;
        }
        if (this->mass != other.mass) {
            return false;
        }
        if (this->radius != other.radius) {
            return false;
        }
        if (this->x != other.x) {
            return false;
        }
        if (this->y != other.y) {
            return false;
        }
        if (this->z != other.z) {
            return false;
        }
        if (this->xd != other.xd) {
            return false;
        }
        if (this->yd != other.yd) {
            return false;
        }
        if (this->zd != other.zd) {
            return false;
        }
        return true;
    }

    bool operator  != ( const Particle & other) {
        return !(*this==other);
    }

    /**
    *  export as xml for use as part of a new particles.xml file.
    *  this differs from overloaded '<<', as it doesn't include the burn sequence managers content.
    */
    std::string exportAsXMLForParticlesFile() {
        std::stringstream out;
        out <<"  <particle>" <<std::endl;
        out <<"   <interactionPermission>";
        if (this->interactionPermission == Advisor::interactALL) {
            out <<"interactALL";
        }
        if (this->interactionPermission == Advisor::interactNONE) {
            out <<"interactNONE";
        }
        if (this->interactionPermission == Advisor::interactEnvironmentOnly ) {
            out <<"interactEnvironmentOnly";
        }
        if (this->interactionPermission == Advisor::interactDifferentOnly) {
            out <<"interactDifferentOnly";
        }
        out <<"</interactionPermission>" <<std::endl;
        out <<"   <identity>";
        if (this->identity == Advisor::collapsor) {
            out <<"collapsor";
        }

        if (this->identity == Advisor::collapsorFixed) {
            out <<"collapsorFixed";
        }
        if (this->identity == Advisor::nonInteractive) {
            out <<"nonInteractive";
        }
        if (this->identity == Advisor::ordinary) {
            out <<"ordinary";
        }
        if (this->identity == Advisor::planetesimal) {
            out <<"planetesimal";
        }
        if (this->identity == Advisor::chromosome) {
            out <<"chromosome";
        }
        out <<"</identity>" <<std::endl;
        out <<"   <name>"<< this->name<<"</name>" <<std::endl;
        out <<"   <visualSize>"<<this->visualRepresentation <<"</visualSize>" <<std::endl;
        out <<"   <rgb>" <<std::endl;
        out <<"    <red>"<<this->red <<"</red>" <<std::endl;
        out <<"    <green>"<<this->green <<"</green>" <<std::endl;
        out <<"    <blue>"<<this->blue <<"</blue>" <<std::endl;
        out <<"   </rgb>" <<std::endl;
        out <<"   <mass>"<<std::scientific<< this->mass<<"</mass>" <<std::endl;
        out <<"   <radius>"<<std::scientific<< this->radius <<"</radius>" <<std::endl;
        out <<"   <vector>" <<std::endl;
        out <<"    <X>"<<std::scientific<<this->x <<"</X>" <<std::endl;
        out <<"    <Y>"<<std::scientific<<this->y <<"</Y>" <<std::endl;
        out <<"    <Z>"<<std::scientific<<this->z <<"</Z>" <<std::endl;
        out <<"    <XD>"<<std::scientific<<this->xd <<"</XD>" <<std::endl;
        out <<"    <YD>"<<std::scientific<<this->yd <<"</YD>" <<std::endl;
        out <<"    <ZD>"<<std::scientific<<this->zd <<"</ZD>" <<std::endl;
        out <<"   </vector>"<<std::endl;
        out <<"  </particle>" <<std::endl;
        return out.str();
    }

    /**
    *  reset so the particle begins at memory entry 0 again.
    * There is no need to actually empty the memory blocks
    */
    void resetMemory() {
        this->memorySize = 0;
    }

    /**
    * get the name of this particle (a std::string)
    */
    std::string getName() {
        return this->name;
    }
    /**
    * get the mass of this particle
    */
    double getMass() {
        return this->mass;
    }

    /**
    * get the BurnSequenceManager.
    * causes Moody to exit if you try to access a burn sequence manager for a particle
    * that is not set up as a spacecraft
    */
    BurnSequenceManager & getBurnSequenceManager() {
        if (!this->isSpacecraft) {
            std::cerr  << "> Attempt to access burn sequence manager for particle not set up as a spacecraft. Invalid operation prevented - exiting" << std::endl;
            exit(0);
        }
        return *this->burnsManager;
    }

    /**
    * initialise the burn sequence manager
    */
    void initialiseBurnSequenceManager(int num,int  maxBurns, int maxSequenceSize, int states, int numberOfStepsPerState) {
        if (!this->isSpacecraft) {
            std::cerr  << "> Attempt to access burn sequence manager for particle not set up as a spacecraft. Invalid operation prevented - exiting" << std::endl;
            exit(0);
        }
        this->burnsManagerActive = true;
        this->burnsManager = new BurnSequenceManager();
        this->burnsManager->buildBurnSequences(num,maxBurns,maxSequenceSize,states,numberOfStepsPerState);
        this->burnsManagerActive =  true;
    }

    /**
    * rebuild the burn sequence timing tables for this particles burn sequence manager
    */
    void rebuildBurnSequenceTimingTables(int steps) {
        if (this->burnsManagerActive) {
            this->burnsManager->rebuildBurnSequenceTimingTables(steps);
        }
    }

    /**
    * make stored copies of spacecraft related members that will change when used and need to be reset
    */
    void storeSpacecraftRelatedMembers() {
        this->stored_mass = this->mass;
        this->stored_fuelPercentage = this->fuelPercentage;
        this->stored_payloadPercentage = this->payloadPercentage;
        this->stored_structurePercentage = this->structurePercentage;
        this->stored_specificImpulse = this->specificImpulse;
    }

    /**
    * restore spacecraft related members that have changed when used and need to be reset
    */
    void restoreSpacecraftRelatedMembers() {
        this->stored_mass = this->mass;
        this->stored_fuelPercentage = this->fuelPercentage;
        this->stored_payloadPercentage = this->payloadPercentage;
        this->stored_structurePercentage = this->structurePercentage;
        this->stored_specificImpulse = this->specificImpulse;
    }

    /**
    * set/update the score currently allocated to this particle
    */
    void setScore(double score) {
        this->score = score;
    }
    /**
    * get the score currently allocated to this particle
    */
    double getScore() {
        return this->score;
    }

    /**
    * get whether or not this particle has been set up as a spacecraft
    */
    bool getIsSpacecraft() {
        return this->isSpacecraft;
    }

    /**
    *  fill all the private members of this particle using a particle structs content.
    * this option exists so a particles internal state can be extracted, changed and re-inserted.
    * It's not currently used, but may be useful for EA experiments
    */
    void fillFromExistingParticleStruct(ParticleStruct & p);

    /**
    * create the block of memory this particle is going to use.
    * This needs to be done in advance, because creating memory storage
    * as needed is far too costly
    */
    void createMemory( int size) {
        this->memory = new MemoryEntry[size];
    }
    /**
    *  get the number of vectors currently stored in memory
    */
    int tellMeMemorySize() {
        return this->memorySize;
    }

    /**
    * set this particles position and/or velocity to be the first one stored in this particles memory
    */
    void revertToFirstStoredVector( int option);

    /**
    * set this particles position and/or velocity to the indicated one stored in this particles memory
    */
    void revertToVector( int index, int option);

    /**
    * set this particles position and/or velocity to the one most recently stored in this particles memory
    */
    void revertToLastStoredVector( int option);

    /**
    * Euler method interaction between this particle and another
    */
    void interactWithSpecified(Particle & that,  double G);

    /**
    * Euler method interaction between this particle and all others
    */
    void interactWithAll(Particle * allParticles,  int numParticles,  double G,  int myIndex);

    /**
    * place the current private state of this particle into a particleStruct
    */
    ParticleStruct exportContentAsParticleStruct();

    /**
    * place the current private state of this particle into a MopItem, ready to be recorded in a mop file
    */
    MopItem exportAsMopItem() {
        MopItem mi;
        mi.red =this->red;
        mi.green =this->green;
        mi.blue =this->blue;
        mi.interactionPermission =this->interactionPermission;
        mi.identity =this->identity;
        mi.mass =this->mass;
        mi.visualRepresentation =this->visualRepresentation;
        strcpy(mi.name,this->name.c_str());
        mi.radius =this->radius;
        mi.x =this->x;
        mi.xd =this->xd;
        mi.y =this->y;
        mi.yd =this->yd;
        mi.z =this->z;
        mi.zd =this->zd;
        return mi;
    }

    /**
    *  find the distance between this particle and another
    */
    double findDistance(Particle &that) {
        double xdist(this->x - that.x_ext);
        double ydist(this->y - that.y_ext);
        double zdist(this->z - that.z_ext);
        double xt(maths.sqr(xdist));
        double yt(maths.sqr(ydist));
        double zt(maths.sqr(zdist));
        double distancesqr(xt+yt+zt);
        return sqrt(distancesqr);
    }

    /**
    *  store the current vector in memory.
    *  the particles block of memory must already have been assigned
    */
    int addCurrentVectorToMemory() {
        this->memory[this->memorySize].x = this->x;
        this->memory[this->memorySize].xd = this->xd;
        this->memory[this->memorySize].xf = this->xf;
        this->memory[this->memorySize].y = this->y;
        this->memory[this->memorySize].yd = this->yd;
        this->memory[this->memorySize].yf = this->yf;
        this->memory[this->memorySize].z = this->z;
        this->memory[this->memorySize].zd = this->zd;
        this->memory[this->memorySize].zf = this->zf;
        this->memorySize++;
        return 1;
    }

    /**
    * Record the starting position of this particle, to be used for later resets
    */
    void storeCurrentPosition() {
        this->stored_x = this->x;
        this->stored_y = this->y;
        this->stored_z = this->z;
    }

    /**
    * Record the starting velocity of this particle, to be used for later resets
    */
    void storeCurrentVelocity() {
        this->stored_xd = this->xd;
        this->stored_yd = this->yd;
        this->stored_zd = this->zd;
    }

    /**
    * Record the starting velocity of this particle, to be used for later resets
    */
    void storeCurrentStateVector() {
        this->storeCurrentPosition();
        this->storeCurrentVelocity();
    }
    /**
    * Reset this particles position using previously stored values
    */
    void resetToStoredPosition() {
        this->x = this->stored_x;
        this->y = this->stored_y;
        this->z = this->stored_z;
    }

    /**
    * Reset this particles velocity using previously stored values
    */
    void resetToStoredVelocity() {
        this->xd = this->stored_xd;
        this->yd = this->stored_yd;
        this->zd = this->stored_zd;
    }
    /**
    * Reset this particles positon and velocity using previously stored values
    */
    void resetToStoredStateVector() {
        this->resetToStoredPosition();
        this->resetToStoredVelocity();
    }
    /**
    * inertia calculation
    */
    void accountForInertia() {
        if ((this->interactionPermission!=Advisor::interactNONE)&&(this->identity!=Advisor::collapsorFixed)) {
            this->xf = this->xf/this->mass;
            this->yf = this->yf/this->mass;
            this->zf = this->zf/this->mass;
        }
    }
    /**
    * reset force accumulators. Not needed in integration, this exists for non integration based uses of Moody, such as gravitational mapping.
    */
    void resetForceAccumulators() {
        this->xf = 0;
        this->yf = 0;
        this->zf = 0;
    }

    /**
    * Move a distance specified by using Eulers method.
    * The assumption here is that this will be used as part of a more complex integrator, not on its own.
    */
    void moveParticle( int travelTime);

    /**
    * Performs the last stage of RK4 integration, producing the final position of the particle
    */
    void finaliseStep( int travelTime);

    /**
    * During integration, the x/y/z values of a particle change. However, since the
    * state of a particle at the start of a first order move, at least in terms of position,
    * needs to be known by other particles that also want to move relative to it,
    * we need to make a copy of the starting position that the other particles can read.
    */
    void setExternalReferenceVector() {
        this->x_ext = this->x;
        this->y_ext = this->y;
        this->z_ext = this->z;
    }

    /**
    * kick this particles velocity using the existing force
    */
    void kick_noforceCalc(int stepsize) {
        if ((this->identity!=Advisor::nonInteractive)&&(this->identity!=Advisor::collapsorFixed)) {
            this->xd += this->xf*stepsize;
            this->yd += this->yf*stepsize;
            this->zd += this->zf*stepsize;
        }
    }

    /**
    * Symplectic method - drifts this particle across a step
    */
    void drift (int stepsize) {
        if ((this->identity!=Advisor::nonInteractive)&&(this->identity!=Advisor::collapsorFixed)) {
            this->x +=(stepsize*this->xd);
            this->y +=(stepsize*this->yd);
            this->z +=(stepsize*this->zd);
        }
    }

    /**
    * Convert available fuel to thrust using this particles specific impulse value
    * This method impements Konstantin Tsiolkovsky's rocket equation
    * The result is correct for field free space.
    * FuelMass is the mass that will be removed as a result of this burn.
    * that mass is not removed by this method.
    */
    double convertFuelToThrust (double fuelMass) {
        double remaining_mass;
        double thrust;
        double stored_ln;
        double exhaust_velocity;
        exhaust_velocity = this->specificImpulse *9.81;
        remaining_mass = this->mass - fuelMass;
        stored_ln = log (this->mass / remaining_mass);
        thrust = exhaust_velocity * stored_ln;
        return thrust;
    }

    /**
    *  set up this particle as a spacecraft, fuel structure and paylod must equal unity
    */
    void makeSpacecraft (SpacecraftInitialiser sci);

    /**
    * Apply a burn event
    * If there was no burn event due can be found by the burn sequence manager but this was called, the BurnEventFire object used will be zero filled and do nothing.
    */
    void ExecuteBurn (int sequence, int state, int step) {
        BurnEventFire fired(this->burnsManager->getCurrentBurnEvent(sequence, state,step));
        double fuel((fired.fuelPct*this->fuelPercentage)*this->mass);
        // convert this fuel into thrust
        double thrust(this->convertFuelToThrust(fuel));
        // apply the available thrust according to the axis allocation percentages
        this->xd += fired.xMinus*thrust;
        this->xd += fired.xPlus*thrust;
        this->yd += fired.yMinus*thrust;
        this->yd += fired.yPlus*thrust;
        this->zd += fired.zMinus*thrust;
        this->zd += fired.zPlus*thrust;
        //revise the fuel percentage, add it to the structure percentage
        this->fuelPercentage-=fired.fuelPct*this->fuelPercentage;
        // add it to the structure percentage
        this->structurePercentage=fired.fuelPct*this->fuelPercentage;
        // change the mass
        this->mass -=fuel;
    }

    /**
    * Place this particle in a random orbital location around another particle, with, or without, an associated velocity
    */
    void generateOrbitalPosition(Particle * allParticles, int numParticles, int focusParticle, double altitude, bool equaliseVelocities,bool addEscapeVelocity, double escapeVelocity);

    /**
    * This does the same as generateOrbitalPosition, but does not touch velocity
    */
    void placeParticleRandomlyOnSphere(Particle * allParticles, int numParticles, int focusParticle, double altitude) {
        double orbit_tmp;
        double curlyThing,theta;
        if ((focusParticle>-1)&&(focusParticle<numParticles)) {
            // place this particle in the same location as the particle we want to be orbitting
            this->x = allParticles[focusParticle].x;
            this->y = allParticles[focusParticle].y;
            this->z = allParticles[focusParticle].z;
            // The orbital height needs to have the radius of the orbitted particle added
            orbit_tmp = altitude+allParticles[focusParticle].radius;
            // Pi is presented as an int here, but the result is divided, returning the result to its proper form
            curlyThing = ((double)(rand() % (2*3141592)))/100;
            theta = ((float)(rand() % 3141592))/100;
            this->x += orbit_tmp*(cos(curlyThing)*sin(theta));
            this->y += orbit_tmp*(sin(curlyThing)*sin(theta));
            this->z += orbit_tmp*(cos(theta));
            this->curlyThing = curlyThing;
            this->theta = theta;
        }
    }




    /**
    *  mutation operator - mutate location for this particle
    */
    void mutateLocation() {
        int select(rand() %3);
        switch(select) {
        case 0:
            this->x+=maths.generateGaussian();
            break;
        case 1:
            this->y+=maths.generateGaussian();
            break;
        case 2:
            this->z+=maths.generateGaussian();
            break;
        }
    }

    /**
    *  mutation operator - mutate velocity for this particle
    */
    void mutateVelocity() {
        int select(rand() %3);
        switch(select) {
        case 0:
            this->xd+=maths.generateGaussian();
            break;
        case 1:
            this->yd+=maths.generateGaussian();
            break;
        case 2:
            this->zd+=maths.generateGaussian();
            break;
        }
    }

    /**
    *  mutation operator - mutate mass for this particle
    */
    void mutateMass() {
        this->mass+=(this->mass*maths.generateGaussian());
    }

    /**
    *  mutation operator - mutate fuel percentage for this particle
    * has no effect if this particle isn't set up as a spacecraft
    * changes to fuel mass are taken from or given to payload
    */
    void mutateFuelPercentage() {
        if (this->isSpacecraft) {
            double change = this->fuelPercentage*(sqrt(maths.sqr(maths.generateGaussian())));
            int choice = rand() %10;
            if (choice<5) {
                this->fuelPercentage+=change;
                this->payloadPercentage-=change;
            } else {
                this->fuelPercentage-=change;
                this->payloadPercentage+=change;
            }
        }
    }

    /**
    *  mutation operator - mutate payload for this particle
    * has no effect if this particle isn't set up as a spacecraft
    * changes to payload mass are taken from or given to fuel
    */
    void mutatePayloadPercentage() {
        if (this->isSpacecraft) {
            double change = this->payloadPercentage*(sqrt(maths.sqr(maths.generateGaussian())));
            int choice = rand() %10;
            if (choice<5) {
                this->fuelPercentage-=change;
                this->payloadPercentage+=change;
            } else {
                this->fuelPercentage+=change;
                this->payloadPercentage-=change;
            }
        }
    }

    /**
    *  mutation operator - mutate structural mass (what's left after payload and fuel mass)  for this particle
    * has no effect if this particle isn't set up as a spacecraft.
    * changes are taken from or given to fuel and payload mass equally
    */
    void mutateStructurePercentage() {
        if (this->isSpacecraft) {
            double change = this->structurePercentage*(sqrt(maths.sqr(maths.generateGaussian())));
            int choice = rand() %10;
            if (choice<5) {
                this->structurePercentage+=change;
                this->fuelPercentage-=change/2;
                this->payloadPercentage-=change/2;
            } else {
                this->structurePercentage-=change;
                this->fuelPercentage+=change/2;
                this->payloadPercentage+=change/2;
            }
        }
    }

    /**
    *  mutation operator - assuming this particle has been placed on the surface of a sphere, alter the angles by some small random amount and recalculate position. Does not alter velocity/direction of travel. Must already have been placed on a sphere
    * parameters:
    * the central point of the sphere as x,y,z
    * radius of the sphere
    */
    void  mutatePositionOnSphere(double radius) {
        double newTheta = this->maths.generateGaussian()*this->theta;
        double newCurlyThing = this->maths.generateGaussian()*this->curlyThing;
        this->curlyThing += newCurlyThing;
        this->theta +=newTheta;
        this->x += radius*(cos(this->curlyThing)*sin(this->theta));
        this->y += radius*(sin(this->curlyThing)*sin(this->theta));
        this->z += radius*(cos(this->theta));
    }
    /**
    *  mutation operator (second version) - assuming this particle has been placed on the surface of a sphere, alter the angles by some small random amount and recalculate position. Does not alter velocity/direction of travel. Must already have been placed on a sphere
    * parameters:
    * the environment Set
    * the index of the particle this is to be centred on
    * radius of the sphere
    */
    void mutatePositionOnSphere(Particle *allParticles,int focusParticle,double radius) {
        this->x = allParticles[focusParticle].x;
        this->y = allParticles[focusParticle].y;
        this->z = allParticles[focusParticle].z;
        double newTheta = this->maths.generateGaussian()*this->theta;
        double newCurlyThing = this->maths.generateGaussian()*this->curlyThing;
        this->curlyThing += newCurlyThing;
        this->theta += newTheta;
        this->x +=(radius*(cos(this->curlyThing)*sin(this->theta)));
        this->y += (radius*(sin(this->curlyThing)*sin(this->theta)));
        this->z +=(radius*(cos(this->theta)));
    }

    /**
    *  mutation operator - assuming this particle has been placed on the surface of a sphere, such as is done in
    * the method generateOrbitalPosition, alter the angles by some small random amount, recalculate position and direction of travel.
    * parameters:
    * the central point of the sphere as x,y,z
    * radius of the sphere
    * true will cause the direction of travel to be altered as well, false will just move the particle
    * if velocity is to be altered,  baseXd and such are used to provide an initial velocity to which the angled changes will be appended
    * (not currently correct I think)
    */
    void mutatePositionAndVelocityOnSphere(double x, double y, double z, double radius, double velocity, double baseXd, double baseYd, double baseZd) {
        double x_out,y_out,z_out;
        double newTheta = this->maths.generateGaussian()*this->theta;
        double newCurlyThing = this->maths.generateGaussian()*this->curlyThing;
        this->curlyThing += newCurlyThing;
        this->theta +=newTheta;
        this->x += cos(this->curlyThing)*sin(this->theta);
        this->y += sin(this->curlyThing)*sin(this->theta);
        this->z += cos(this->theta);
        x_out=(velocity*(cos(newCurlyThing)*sin(newTheta)));
        y_out=(velocity*(sin(newCurlyThing)*sin(newTheta)));
        z_out=(velocity*(cos(newTheta)));
        this->xd =  baseXd + x_out;
        this->yd =  baseYd + y_out;
        this->zd =  baseZd + z_out;
    }

    /**
    * reset this particle history array ready for reuse. Does not resize history
    */
    void resetHistory() {
        if (historyExists) {
            this->currentHistoryIndex = 0;
            for (int x(0); x<this->historySize; x++) {
                this->history[x].blank();
            }
            this->currentHistoryIndex = 0;
            this->historyCounter = 0;
        }  else {
            std::cerr  << "> clearHistory called - Particle history not currently in use. Invalid operation prevented - exiting" << std::endl;
            exit(0);
        }
    }

    /**
    * delete this particle's history array completelly. Use this if you want to de-allocate the history array,  and use resetHistory if you just want to clear it.
    */
    void deleteHistory() {
        if (historyExists) {
            delete [] this->history;
            this->historyExists = false;
            this->historySize = 0;
            this->historyInterval = 0;
            this->currentHistoryIndex = 0;
            this->historyCounter = 0;
        }  else {
            std::cerr  << "> deleteHistory called - Particle history not currently in use. Invalid operation prevented - exiting" << std::endl;
            exit(0);
        }
    }
    /**
    * store the current state vector in this particles history. All storage attempts once the history is filled are ignored
    */
    void AddCurrentStateVectorToHistory() {
        if (historyExists) {
            if (this->currentHistoryIndex<this->historySize) {
                this->history[this->currentHistoryIndex].x = this->x;
                this->history[this->currentHistoryIndex].y = this->y;
                this->history[this->currentHistoryIndex].z = this->z;
                this->history[this->currentHistoryIndex].xd = this->xd;
                this->history[this->currentHistoryIndex].yd = this->yd;
                this->history[this->currentHistoryIndex].zd = this->zd;
                this->currentHistoryIndex++;
            }
        }  else {
            std::cerr  << "> AddCurrentStateVectorToHistory called - Particle history not currently in use. Invalid operation prevented - exiting" << std::endl;
            exit(0);
        }
    }

    /**
    * store the current state vector in this particles history. All storage attempts once the history is filled are ignored
    * parameters are the number of states, the number of states per step, and the number of times per state you want to record a history entry.
    * Unless you are planning to record the histories of a select few particles, call createHistoryStorage() in Capsule, which will build history storage for
    * all particles in the environment set
    */
    void createHistory(int states, int steps, int frequency) {
        if (!historyExists) {
            div_t size = div((states*steps),frequency);
            this->historySize = size.quot;
            this->buildHistoryStorage(this->historySize);
            this->historyInterval = frequency;
            this->historyCounter = 0;
            this->currentHistoryIndex = 0;
        }  else {
            std::cerr  << "> createHistory called - Particle history already in use. Invalid operation prevented - exiting" << std::endl;
            exit(0);
        }
    }

    /**
    * record a history entry if it's time to do so. For this to work it has to be called once for every step of an iterating model
    * unless you are specifically recording the history of a select set of particles, you should use the doHistory() implemented in
    * Capsule (well, CapSuleBase), which calls this for all particles in the environment set.
    */
    void doHistory() {
        if (historyExists) {
            if (this->historyCounter>=historyInterval) {
                this->AddCurrentStateVectorToHistory();
                this->historyCounter=0;
            } else {
                this->historyCounter++;
            }
        }  else {
            std::cerr  << "> doHistory called - Particle history not in use. Invalid operation prevented - exiting" << std::endl;
            exit(0);
        }
    }

    /**
    *  export this particles history. Fails if history not in use
    */
    ParticleHistory & getHistory() {
        return *(this->history);
    }

    /*
    *  get one entry of this particles history
    */
    ParticleHistory & getHistoryEntry(int index) {
        return this->history[index];
    }

    /**
    *  get the overall size of history array
    */
    const int getHistoryStorageArraySize() {
        return this->historySize;
    }

    /**
    *  get the amount of the history array which is currently filled
    */
    const int getStoredHistoryCount() {
        return this->currentHistoryIndex;
    }

    /**
    *  debug method, print the current state vector to stdout
    */
    void printStateVector(int indent) {
        for (int x(0); x<indent; x++) {
            std::cout << " ";
        }
        std::cout <<"Name: "<< this->name << std::endl;

        for (int x(0); x<indent; x++) {
            std::cout << " ";
        }
        std::cout <<"Mass: "<< this->mass << std::endl;
        for (int x(0); x<indent; x++) {
            std::cout << " ";
        }
        std::cout <<"x: "<<this->x << std::endl;
        for (int x(0); x<indent; x++) {
            std::cout << " ";
        }
        std::cout <<"y: "<<this->y<< std::endl;
        for (int x(0); x<indent; x++) {
            std::cout << " ";
        }
        std::cout <<"z: "<<this->z << std::endl;
        for (int x(0); x<indent; x++) {
            std::cout << " ";
        }
        std::cout <<"xd: "<<this->xd << std::endl;
        for (int x(0); x<indent; x++) {
            std::cout << " ";
        }
        std::cout <<"yd: "<<this->yd << std::endl;
        for (int x(0); x<indent; x++) {
            std::cout << " ";
        }
        std::cout <<"zd: "<<this->zd<<std::endl;
        for (int x(0); x<indent; x++) {
            std::cout << " ";
        }
        std::cout <<"xf: "<<this->xf<<std::endl;
        for (int x(0); x<indent; x++) {
            std::cout << " ";
        }
        std::cout <<"yf: "<< this->yf<<std::endl;
        for (int x(0); x<indent; x++) {
            std::cout << " ";
        }
        std::cout <<"zf: "<<this->zf<<std::endl;
    }


    /**
    *  Symmplectic Integration kick method
    */
    void kick(Particle * allParticles,const int numParticles, const double G, const int myIndex, int stepsize) {
        this->xf = 0;
        this->yf = 0;
        this->zf = 0;
        bool allowed = true;
        // we only integrate if the particle in questiod is allowed to move
        // ones that aren't are non interactives (placemarkers) and fixed collapsors
        if ((this->identity!=Advisor::nonInteractive)&&(this->identity!=Advisor::collapsorFixed)) {
            // get the dv for movement
            for (int i(0); i<numParticles; i++) {
                allowed = true;
                if (i!=myIndex) { // no self comparisons
                    // establish whether I am only allowed to interact with different particles
                    if (this->interactionPermission==Advisor::interactDifferentOnly) {
                        // is this particle the same as me?
                        if (this->identity==allParticles[i].identity) {
                            // the same, I can't interact
                            allowed = false;
                        }
                    } else if(allParticles[i].interactionPermission == Advisor::interactNONE) {
                        allowed = false;
                    } else if (this->interactionPermission==Advisor::interactEnvironmentOnly) {
                        //we get here  in this particle has been set so as to not be allowed to interact with other chromosomes
                        // so, is the other particle also a chromosome that I can't interact with?
                        if (allParticles[i].identity == Advisor::chromosome) {
                            // yes
                            allowed = false;
                        }
                    }

                    if (allowed) {
                        // ok, it's interactive with respect to us, and if we got this far we are meant to be effected by its f, so find it.
                        double xdist(this->x - allParticles[i].x_ext);
                        double ydist(this->y - allParticles[i].y_ext);
                        double zdist(this->z - allParticles[i].z_ext);
                        double xt(maths.sqr(xdist));
                        double yt(maths.sqr(ydist));
                        double zt(maths.sqr(zdist));
                        double distancesqr(xt+yt+zt);
                        double distance(sqrt(distancesqr));
                        double f(G * (this->mass * allParticles[i].mass) / distancesqr);
                        double xpart(xdist/2);
                        double newDist(this->x + xpart);
                        if ((maths.sqr(newDist - allParticles[i].x_ext))< xt) {
                            this->xf += f*xdist/distance;
                        } else {
                            this->xf -= f*xdist/distance;
                        }
                        double ypart(ydist/2);
                        newDist = this->y + ypart;
                        if ((maths.sqr(newDist - allParticles[i].y_ext))< yt) {
                            this->yf += f*ydist/distance;
                        } else {
                            this->yf -= f*ydist/distance;
                        }
                        double zpart(zdist/2);
                        newDist = this->z + zpart;
                        if ((maths.sqr(newDist - allParticles[i].z_ext))< zt) {
                            this->zf += f*zdist/distance;
                        } else {
                            this->zf -= f*zdist/distance;
                        }
                    }
                }
            }
            if ((this->interactionPermission!=Advisor::interactNONE)&&(this->identity!=Advisor::collapsorFixed)) {
                this->accountForInertia();
                this->xd += this->xf*stepsize;
                this->yd += this->yf*stepsize;
                this->zd += this->zf*stepsize;
            }
        }
    }
   Json::Value exportAsJsonObject() {
        Json::Value tmp(Json::objectValue);
        tmp["name"] = this->name;
	tmp["mass"] = this->mass;
	tmp["radius"] = this->radius;
	tmp["x"] = this->x;
	tmp["y"] = this->y;
	tmp["z"] = this->z;
	tmp["xd"] = this->xd;
	tmp["yd"] = this->yd;
	tmp["zd"] = this->zd;
	tmp["red"] = this->red;
	tmp["green"] = this->green;
	tmp["blue"] = this->blue;
	tmp["nteractionPermission"] = this->interactionPermission;
	tmp["identity"] = this->identity;
	tmp["visualRepresentation"] = this->visualRepresentation;
        tmp["isSpacecraft"] = this->isSpacecraft;
	// more members exist, but are not yet required
        return tmp;
    }

   void importFromJsonObject(const Json::Value input) {
     this->name = input["name"].asString();
     this->mass = input["mass"].asDouble();
     this->radius = input["radius"].asDouble();
     this->x = input["x"].asDouble();
     this->y = input["y"].asDouble();
     this->z = input["z"].asDouble();
     this->xd = input["xd"].asDouble();
     this->yd = input["yd"].asDouble();
     this->zd = input["zd"].asDouble();
     this->red = input["red"].asInt();
     this->green = input["green"].asInt();
     this->blue = input["blue"].asInt();
     this->interactionPermission = input["nteractionPermission"].asInt();
     this->identity = input["identity"].asInt();
     this->visualRepresentation = input["visualRepresentation"].asInt();
     this->isSpacecraft = input["isSpacecraft"].asBool();

   }
};
