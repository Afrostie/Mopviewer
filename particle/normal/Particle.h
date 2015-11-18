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
#include "../../etc/Advisor.h"
#include "../../etc/Numerics.h"
#include "../../etc/Fragment.h"
#include "../memory/MemoryEntry.h"
#include "../memory/MemoryContent.h"
#include "../struct/ParticleStruct.h"
#ifdef MOPFILE
#include "../../mopfile/MopFile/MopItem.h"
#endif


#ifdef MOPFILELIGHT
#include "../../mopfile/MopFileLight/MopItem.h"
#endif
#include "../json/json.h"


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
	int id;
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
	* rotation variable storage (used when placing/moving particles on the surface of a sphere)
	*/
	double curlyThing;

	/**
	* other rotation variable storage (used when placing/moving particles on the surface of a sphere)
	*/
	double theta;


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



public:
Particle() {
	this->memorySize=0;
	this->x = 0;
	this->y = 0;
	this->z = 0;
	this->xd = 0;
	this->yd = 0;
	this->zd = 0;
	this->x_ext = 0;
	this->y_ext = 0;
	this->z_ext = 0;
	this->xf = 0;
	this->yf = 0;
	this->zf = 0;
	// spacecraft specific aditions to Particle
	this->isSpacecraft = false;
	this->structurePercentage = 0;
	this->payloadPercentage = 0;
	this->fuelPercentage = 0;
	this->specificImpulse = 0;
	this->score = 0;
	this->curlyThing = 0;
	this->theta = 0;
}
~Particle() {
	// clean up allocated arrays
	if (this->memorySize!=0) {
		delete[] this->memory;
	}

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
	* copy the content of another particle into this one. Note that particle memory is never copied
	*/
	void fillFromExistingParticle (Particle & other) {
		if (this != &other) // protect against invalid self-assignment
		{
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
		if (this->interactionPermission == Advisor::interactParticlesOnly) {
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
	*  kill memory
	*/
	void deleteMemory() {
	  if (this->memorySize!=0) {
		  delete[] this->memory;
	  }
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
*  fill all the private members of this particle using a particleStructs content.
* this option exists so a particles internal state can be extracted, changed and re-inserted.
*/
void fillFromExistingParticleStruct(ParticleStruct & p) {
	this->red = p.red;
	this->green = p.green;
	this->blue = p.blue;
	this->interactionPermission = p.interactionPermission;
	this->identity = p.identity;
	this->mass = p.mass;
	this->visualRepresentation = p.visualRepresentation;
	this->name = p.name;
	this->radius = p.radius;
	this->x = p.x;
	this->xd = p.xd;
	this->y = p.y;
	this->yd = p.yd;
	this->z = p.z;
	this->zd = p.zd;
	this->stored_x = p.stored_x;
	this->stored_y = p.stored_y;
	this->stored_z = p.stored_z;
	this->stored_xd = p.stored_xd;
	this->stored_yd = p.stored_yd;
	this->stored_zd = p.stored_zd;
	this->curlyThing = p.curlyThing;
	this->theta = p.theta;

}

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
*  put the particle vector back to the first recorded vector
*/
void revertToFirstStoredVector(const int option) {
	if (this->memorySize>=1) {
		switch(option) {
		case Advisor::OMIT_FORCES:
			this->x = this->memory[0].x;
			this->xd = this->memory[0].xd;
			this->y = this->memory[0].y;
			this->yd = this->memory[0].yd;
			this->z = this->memory[0].z;
			this->zd = this->memory[0].zd;
			break;
		case Advisor::OMIT_VECTOR:
			this->xf = this->memory[0].xf;
			this->yf = this->memory[0].yf;
			this->zf = this->memory[0].zf;
			break;
		case Advisor::ALL:
			this->x = this->memory[0].x;
			this->xd = this->memory[0].xd;
			this->xf = this->memory[0].xf;
			this->y = this->memory[0].y;
			this->yd = this->memory[0].yd;
			this->yf = this->memory[0].yf;
			this->z = this->memory[0].z;
			this->zd = this->memory[0].zd;
			this->zf = this->memory[0].zf;
			break;
		default:
			std::cerr << "attempt to access particle meemory without access type being specified correctly- error of the fatal kind - exiting :(" << std::endl;
			exit(0);
		}
	}
}

/**
* put the particle vector back to the specified stored vector
*/
void revertToVector(const int index,const int option) {
	if (this->memorySize!=0) {
		if ((index>=0)&&(index<this->memorySize)) {
			switch(option) {
			case Advisor::OMIT_FORCES:
				this->x = this->memory[index].x;
				this->xd = this->memory[index].xd;
				this->y = this->memory[index].y;
				this->yd = this->memory[index].yd;
				this->z = this->memory[index].z;
				this->zd = this->memory[index].zd;
				break;
			case Advisor::ALL:
				this->x = this->memory[index].x;
				this->xd = this->memory[index].xd;
				this->xf = this->memory[index].xf;
				this->y = this->memory[index].y;
				this->yd = this->memory[index].yd;
				this->yf = this->memory[index].yf;
				this->z = this->memory[index].z;
				this->zd = this->memory[index].zd;
				this->zf = this->memory[index].zf;
				break;
			case Advisor::OMIT_VECTOR:
				this->xf = this->memory[index].xf;
				this->yf = this->memory[index].yf;
				this->zf = this->memory[index].zf;
				break;
			default:
				std::cout << "attempt to access particle memory without access type being specified correctly- error of the fatal kind - exiting :(" << std::endl;
				exit(index);
			}
		}
	}
}
/**
*  put the particle vector back to the last one that was stored
* what parts of the stored memory are recovered is specified by the
// option parameter
*/
void revertToLastStoredVector(const int option) {
	if (this->memorySize>=1) {
		switch(option) {
		case Advisor::OMIT_FORCES:
			this->x = this->memory[this->memorySize-1].x;
			this->xd = this->memory[this->memorySize-1].xd;
			this->y = this->memory[this->memorySize-1].y;
			this->yd = this->memory[this->memorySize-1].yd;
			this->z = this->memory[this->memorySize-1].z;
			this->zd = this->memory[this->memorySize-1].zd;
			break;
		case Advisor::ALL:
			this->x = this->memory[this->memorySize-1].x;
			this->xd = this->memory[this->memorySize-1].xd;
			this->xf = this->memory[this->memorySize-1].xf;
			this->y = this->memory[this->memorySize-1].y;
			this->yd = this->memory[this->memorySize-1].yd;
			this->yf = this->memory[this->memorySize-1].yf;
			this->z = this->memory[this->memorySize-1].z;
			this->zd = this->memory[this->memorySize-1].zd;
			this->zf = this->memory[this->memorySize-1].zf;
			break;
		case Advisor::OMIT_VECTOR:
			this->xf = this->memory[this->memorySize-1].xf;
			this->yf = this->memory[this->memorySize-1].yf;
			this->zf = this->memory[this->memorySize-1].zf;
			break;
		default:
			std::cout << "attempt to access particle memory without access type being specified correctly- error of the fatal kind - exiting :(" << std::endl;
			exit(0);
		}
	}
}
	/**
	* Euler method interaction between this particle and another
	*/
    void interactWithSpecified(Particle & that, const double G) {
        if ((this->identity!=Advisor::nonInteractive)&&(this->identity!=Advisor::collapsorFixed)) {
            bool allowed = true;
            if (this == &that) {
                allowed = false;
            }
            // establish whether I am only allowed to interact with different particles
            if (this->interactionPermission==Advisor::interactDifferentOnly) {
                // is this particle the same as me?
                if (this->identity==that.identity) {
                    // the same, I can't interact
                    allowed = false;
                }
            } else if(that.interactionPermission == Advisor::interactNONE) {
                allowed = false;
            } else if (this->interactionPermission==Advisor::interactParticlesOnly) {
                //we get here  in this particle has been set so as to not be allowed to interact with other chromosomes
                // so, is the other particle also a chromosome that I can't interact with?
                if (that.identity == Advisor::chromosome) {
                    allowed = false;
                }
            }
            if (allowed) {
                double xdist(this->x - that.x_ext);
                double ydist(this->y - that.y_ext);
                double zdist(this->z - that.z_ext);
                double xt(maths.sqr(xdist));
                double yt(maths.sqr(ydist));
                double zt(maths.sqr(zdist));
                double distancesqr(xt+yt+zt);
                double distance(sqrt(distancesqr));
                double f(G * (this->mass * that.mass) / distancesqr);

                //double xpart(xdist/2);
                //double newDist(this->x + (xdist/2));
                //if ((maths.sqr(newDist - that.x_ext))< xt) {
                if ( (maths.sqr( ( this->x + (xdist/2) ) - that.x_ext) ) < xt ) {

                    this->xf += f*xdist/distance;
                } else {
                    this->xf -= f*xdist/distance;
                }

                //double ypart(ydist/2);
                //newDist = this->y + (ydist/2);
                if ( (maths.sqr( ( this->y + (ydist/2) ) - that.y_ext) ) < yt ) {
                    this->yf += f*ydist/distance;
                } else {
                    this->yf -= f*ydist/distance;
                }

                //double zpart(zdist/2);
                //newDist = this->z + (zdist/2);
                //if ((maths.sqr(newDist - that.z_ext))< zt) {
                if ( (maths.sqr( ( this->z + (zdist/2) ) - that.z_ext) ) < zt) {
                    this->zf += f*zdist/distance;
                } else {
                    this->zf -= f*zdist/distance;
                }
            }
        }
    }


/**
* calculate the interactions between this particle and all other particles
*/
void interactWithAll(Particle * allParticles,const int numParticles, const double G, const int myIndex) {
	// clear the accumulators that we are about to use
	this->xf = 0;
	this->yf = 0;
	this->zf = 0;
	// get the dv for movement
	for (int i(0);i<numParticles;i++) {
		if (i!=myIndex) { // no self comparisons
			this->interactWithSpecified(allParticles[i],G);
		}
	}
}
/**
* used to export the content of a Particle into a ParticleStruct.
*/
ParticleStruct exportContentAsParticleStruct() {
	ParticleStruct tmp;
	tmp.x = this->x;
	tmp.y = this->y;
	tmp.z = this->z;
	tmp.xd = this->xd;
	tmp.yd = this->yd;
	tmp.zd = this->zd;
	tmp.stored_x = this->stored_x;
	tmp.stored_y = this->stored_y;
	tmp.stored_z = this->stored_z;
	tmp.stored_xd = this->stored_xd;
	tmp.stored_yd = this->stored_yd;
	tmp.stored_zd = this->stored_zd;
	tmp.stored_fuelPercentage = this->stored_fuelPercentage;
	tmp.stored_payloadPercentage = this->stored_payloadPercentage;
	tmp.stored_structurePercentage = this->stored_structurePercentage;
	tmp.stored_specificImpulse = this->stored_specificImpulse;
	tmp.name = this->name;
	tmp.mass = this->mass;
	tmp.radius = this->radius;
	tmp.curlyThing = this->curlyThing;
	tmp.theta = this->theta;
	tmp.visualRepresentation = this->visualRepresentation;
	tmp.identity = this->identity;
	tmp.interactionPermission = this->interactionPermission;
	tmp.red = this->red;
	tmp.green = this->green;
	tmp.blue = this->blue;
	return tmp;
}

#ifdef MOPFILE
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
#endif

#ifdef MOPFILELIGHT
    /**
     * place the current private state of this particle into a MopItem, ready to be recorded in a mop file
     */
    MopItem exportAsMopItem() {
        MopItem mi;
        mi.red =this->red;
        mi.green =this->green;
        mi.blue =this->blue;
        mi.id =this->id;
        mi.visualRepresentation =this->visualRepresentation;
        mi.x =this->x;
        mi.y =this->y;
        mi.z =this->z;
        return mi;
    }
#endif



	/**
	*  find the distance between this particle and another
	*/
	double findDistance(Particle &that) {
		return sqrt(maths.sqr(this->x - that.x_ext)+maths.sqr(this->y - that.y_ext)+maths.sqr(this->z - that.z_ext));
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
void moveParticle(const int travelTime) {
	// move this particle according to the forces acting on it
	if ((this->identity!=Advisor::collapsorFixed)&&(this->identity!=Advisor::nonInteractive)) {
		for (int j(0);j<travelTime;j++) {
			this->xd+=this->xf;
			this->x+=xd;
			this->yd+=this->yf;
			this->y+=yd;
			this->zd+=this->zf;
			this->z+=zd;
		}
	}
}

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
	*  debug method, print the current state vector to stdout
	*/
	void printStateVector(int indent) {
		for (int x(0);x<indent;x++) {
			std::cout << " ";
		}
		std::cout <<"Name: "<< this->name << std::endl;

		for (int x(0);x<indent;x++) {
			std::cout << " ";
		}
		std::cout <<"Mass: "<< this->mass << std::endl;
		for (int x(0);x<indent;x++) {
			std::cout << " ";
		}
		std::cout <<"x: "<<this->x << std::endl;
		for (int x(0);x<indent;x++) {
			std::cout << " ";
		}
		std::cout <<"y: "<<this->y<< std::endl;
		for (int x(0);x<indent;x++) {
			std::cout << " ";
		}
		std::cout <<"z: "<<this->z << std::endl;
		for (int x(0);x<indent;x++) {
			std::cout << " ";
		}
		std::cout <<"xd: "<<this->xd << std::endl;
		for (int x(0);x<indent;x++) {
			std::cout << " ";
		}
		std::cout <<"yd: "<<this->yd << std::endl;
		for (int x(0);x<indent;x++) {
			std::cout << " ";
		}
		std::cout <<"zd: "<<this->zd<<std::endl;
		for (int x(0);x<indent;x++) {
			std::cout << " ";
		}
		std::cout <<"xf: "<<this->xf<<std::endl;
		for (int x(0);x<indent;x++) {
			std::cout << " ";
		}
		std::cout <<"yf: "<< this->yf<<std::endl;
		for (int x(0);x<indent;x++) {
			std::cout << " ";
		}
		std::cout <<"zf: "<<this->zf<<std::endl;
	}
/**
* Performs the last stage of RK4 integration, producing the final position of the particle
*/
void finaliseStep(const int travelTime) {
	// move this particle according to the forces acting on it
	if ((this->identity!=Advisor::collapsorFixed)&&(this->identity!=Advisor::nonInteractive)) {
		double motionx((this->memory[0].xf+this->memory[3].xf+(2*this->memory[1].xf)+(2*this->memory[2].xf))/6);
		double motiony((this->memory[0].yf+this->memory[3].yf+(2*this->memory[1].yf)+(2*this->memory[2].yf))/6);
		double motionz((this->memory[0].zf+this->memory[3].zf+(2*this->memory[1].zf)+(2*this->memory[2].zf))/6);
		for (int j(0);j<travelTime;j++) {
			this->xd+=motionx;
			this->x+=xd;
			this->yd+=motiony;
			this->y+=yd;
			this->zd+=motionz;
			this->z+=zd;
		}
	}
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
			for (int i(0);i<numParticles;i++) {
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
					} else if (this->interactionPermission==Advisor::interactParticlesOnly) {
						//we get here  in this particle has been set so as to not be allowed to interact with other chromosomes
						// so, is the other particle also a chromosome that I can't interact with?
						if (allParticles[i].identity == Advisor::chromosome) {
							// yes
							allowed = false;
						}
					}

					if (allowed) {
                        double xdist(this->x - allParticles[i].x_ext);
                        double ydist(this->y - allParticles[i].y_ext);
                        double zdist(this->z - allParticles[i].z_ext);
                        double xt(maths.sqr(xdist));
                        double yt(maths.sqr(ydist));
                        double zt(maths.sqr(zdist));
                        double distancesqr(xt+yt+zt);
                        double distance(sqrt(distancesqr));
                        double f(G * (this->mass * allParticles[i].mass) / distancesqr);

                        //double xpart(xdist/2);
                        //double newDist(this->x + (xdist/2));
                        //if ((maths.sqr(newDist - allParticles[i].x_ext))< xt) {
                        if ( (maths.sqr( ( this->x + (xdist/2) ) - allParticles[i].x_ext) ) < xt ) {

                            this->xf += f*xdist/distance;
                        } else {
                            this->xf -= f*xdist/distance;
                        }

                        //double ypart(ydist/2);
                        //newDist = this->y + (ydist/2);
                        if ( (maths.sqr( ( this->y + (ydist/2) ) - allParticles[i].y_ext) ) < yt) {
                            this->yf += f*ydist/distance;
                        } else {
                            this->yf -= f*ydist/distance;
                        }

                        if ( (maths.sqr( ( this->z + (zdist/2) ) - allParticles[i].z_ext) ) < zt ) {
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
    const char * exportAsCharArray() {
            std::stringstream tmp;
            tmp << this->name << "[";
            tmp << this->name << ",";
            tmp << this->mass << ",";
            tmp << this->radius << ",";
            tmp << this->visualRepresentation << ",";
            tmp << this->interactionPermission << ",";
            tmp << this->identity << ",";
            tmp << this->red << ",";
            tmp << this->green << ",";
            tmp << this->blue << ",";
            tmp << this->x << ",";
            tmp << this->y << ",";
            tmp << this->z << ",";
            tmp << this->xd << ",";
            tmp << this->yd << ",";
            tmp << this->zd << ",";
            tmp << this->xf << ",";
            tmp << this->yf << ",";
            tmp << this->zf;
            tmp << "]";
            std::string str = tmp.str();
            return str.c_str();
    }


    /**
     * fill this particle from a char array
     *
     */
    void fillFromCharArray(const char *str) {
        Fragment thing, worker;
        int pos2(0);
            //pos++;
	    std::string tmp(str);
        worker.fill(tmp, tmp.size());
            pos2 = 0;
            thing.fill(worker,'[',pos2);
	    if (thing.size()<50) {
		    for (int x(1);x<thing.size();x++) {
			this->name[x] = thing.at(x);
		    }
	    } else {
		std::cerr  << "> Particle Name too long, possible Buffer Overflow prevented - exiting. (FillFromCharArray)" << std::endl;
		exit(0);
	    }
            //std::cout << "> particle name: " << this->name << std::endl;
            thing.fill(worker,',',pos2);
            this->mass = thing.toFloat();
            //std::cout << "> particle mass: " << this->mass << std::endl;
            thing.fill(worker,',',pos2);
            this->radius = thing.toFloat();
            //std::cout << "> particle radius: " << this->radius << std::endl;
            thing.fill(worker,',',pos2);
            this->visualRepresentation = thing.toInt();
            thing.fill(worker,',',pos2);
            this->interactionPermission = thing.toInt();
            thing.fill(worker,',',pos2);
            this->identity = thing.toInt();
            thing.fill(worker,',',pos2);
            this->red = thing.toInt();
            thing.fill(worker,',',pos2);
            this->green = thing.toInt();
            thing.fill(worker,',',pos2);
            this->blue = thing.toInt();
            thing.fill(worker,',',pos2);
            this->x  = thing.toFloat();
            thing.fill(worker,',',pos2);
            this->y = thing.toFloat();
            thing.fill(worker,',',pos2);
            this->z  = thing.toFloat();
            thing.fill(worker,',',pos2);
            this->xd = thing.toFloat();
            thing.fill(worker,',',pos2);
            this->yd = thing.toFloat();
            thing.fill(worker,',',pos2);
            this->zd = thing.toFloat();
            thing.fill(worker,',',pos2);
            this->xf = thing.toFloat();
            thing.fill(worker,',',pos2);
            this->yf  = thing.toFloat();
            thing.fill(worker,',',pos2);
            this->zf = thing.toFloat();
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
        }

};
