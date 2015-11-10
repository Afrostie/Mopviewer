#include "Particle.h"

Particle::Particle() {
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
    this->burnsManagerActive = false;
    this->structurePercentage = 0;
    this->payloadPercentage = 0;
    this->fuelPercentage = 0;
    this->specificImpulse = 0;
    this->score = 0;
    this->historySize = 0;
    this->currentHistoryIndex = 0;
    this->historyExists = false;
    this->historyCounter = 0;
    this->historyInterval = 0;
    this->curlyThing = 0;
    this->theta = 0;
}

Particle::~Particle() {
    // clean up allocated arrays
    if (this->memorySize!=0) {
        delete[] this->memory;
    }
    if (this->historySize!=0) {
        delete[] this->history;
    }
    if (this->burnsManagerActive) {
        delete this->burnsManager;
    }
}

/**
*  put the particle vector back to the first recorded vector
*/
void Particle::revertToFirstStoredVector(const int option) {
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
*  put the particle vector back to the last one that was stored
* what parts of the stored memory are recovered is specified by the
// option parameter
*/
void Particle::revertToLastStoredVector(const int option) {
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
* put the particle vector back to the specified stored vector
*/
void Particle::revertToVector(const int index,const int option) {
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
*
* This interaction is for self only. That is if this objects vector won't
* be changed as a result of this particular interaction, it's not calculated.
* Note that we query the other particles '_ext' stores of its position,
* therefore this must be set for all particles prior to this interaction,
* via a call to setexternalReferenceVector()
*
*/
void Particle::interactWithSpecified(Particle & that, const double G) {
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
        } else if (this->interactionPermission==Advisor::interactEnvironmentOnly) {
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
            double xpart(xdist/2);
            double newDist(this->x + xpart);
            if ((maths.sqr(newDist - that.x_ext))< xt) {
                this->xf += f*xdist/distance;
            } else {
                this->xf -= f*xdist/distance;
            }
            double ypart(ydist/2);
            newDist = this->y + ypart;
            if ((maths.sqr(newDist - that.y_ext))< yt) {
                this->yf += f*ydist/distance;
            } else {
                this->yf -= f*ydist/distance;
            }
            double zpart(zdist/2);
            newDist = this->z + zpart;
            if ((maths.sqr(newDist - that.z_ext))< zt) {
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
void Particle::interactWithAll(Particle * allParticles,const int numParticles, const double G, const int myIndex) {
    // clear the accumulators that we are about to use
    this->xf = 0;
    this->yf = 0;
    this->zf = 0;
    // get the dv for movement
    for (int i(0); i<numParticles; i++) {
        if (i!=myIndex) { // no self comparisons
            this->interactWithSpecified(allParticles[i],G);
        }
    }
}

/**
* used to export the content of a Particle into a ParticleStruct.
*/
ParticleStruct Particle::exportContentAsParticleStruct() {
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
    tmp.score = this->score;
    tmp.isSpacecraft = this->isSpacecraft;
    if (tmp.isSpacecraft == true) {
        tmp.burnsManagerActive = this->burnsManagerActive;
        tmp.structurePercentage = this->structurePercentage;
        tmp.payloadPercentage = this->payloadPercentage;
        tmp.fuelPercentage = this->fuelPercentage;
        tmp.specificImpulse = this->specificImpulse;
    }
    return tmp;
}

/**
*  fill all the private members of this particle using a particleStructs content.
* this option exists so a particles internal state can be extracted, changed and re-inserted.
*/
void Particle::fillFromExistingParticleStruct(ParticleStruct & p) {
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
    this->score = p.score;
    this->curlyThing = p.curlyThing;
    this->theta = p.theta;
    this->isSpacecraft = p.isSpacecraft;
    if (this->isSpacecraft == true) {
        this->structurePercentage = p.structurePercentage;
        this->payloadPercentage = p.payloadPercentage;
        this->fuelPercentage = p.fuelPercentage;
        this->specificImpulse = p.specificImpulse;
        // instantiate the burn sequence manager, or reset an existing one.
        if (this->burnsManager==NULL) {
            this->burnsManager = new BurnSequenceManager();
        } else {
            this->burnsManager->clear();
            this->burnsManager = new BurnSequenceManager();
        }
    }
}

/**
* For placing a spacecraft in a random orbital location around an environment particle
* This version concerns itself only with orbital position by default,
* with booleans enabling further modifications.
* 'Orbitting' in this context means 'positioned relative to'.
* Velocity of the spacecraft in orbit is updated to match that of
* the particle it is orbitting only if this is requested in
* the parameter 'equaliseVelocities'.
* This matching is not adjusted to take into account velocity differences due to
* orbital altitude, since the aim of this method is not to put satellites
* into stable orbits, it is meant to enable their placement above atmosphere,
* thereby removing the need to include the hydronamical calculations
* that would be required if a launch took place from the surface of a planet.
*
* If you are modelling surface launch from a body without atmosphere, set altitude to zero
* and do not add an escape velocity.
*
* Escape velocity is only set if the parameter 'addEscapeVelocity'
* is set to true. If it is, then an escape velocity
* must be provided, otherwise this parameter can be ignored as a
* default value is supplied.
* Escape velocity should be the velocity that a spacecraft would have
* after slowing from initial launch and reaching its starting altitude.
* Therefore you should not use surface escape velocity. Instead use low orbit escape velocity.
* For example: At Earths surface, required escape velocity is 11200 meters per second,
* but you can't go that fast in atmosphere, so the actual escape velocity is 10900 meters per second
* from low earth orbit, which is a lot easier to acheive.
* Thus, in terms of modelling it is simpler to assume the surface launch to reach low orbit
* has already been completed.
* When escape velocity is set, it is applied so the spacecraft particle is moving
* directly away from the orbitted particle. This may not always be required, but anything else
* results in the spacecraft failing to escape unless further thrust is added.
* if you want a different initial launch direction, leave the escape velocity
* off and add your own velocities to the spacecraft after positioning
*/
void Particle::generateOrbitalPosition(Particle * allParticles, int numParticles, int focusParticle, double altitude, bool equaliseVelocities,bool addEscapeVelocity, double escapeVelocity) {
    double orbit_tmp,x_out,y_out,z_out;
    double curlyThing,theta;
    if ((focusParticle>-1)&&(focusParticle<numParticles)) {
        // zero the initial velocities
        this->xd = 0;
        this->yd = 0;
        this->zd = 0;
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
        // are velocities to be equalised?
        if (equaliseVelocities) {
            this->xd = allParticles[focusParticle].xd;
            this->yd = allParticles[focusParticle].yd;
            this->zd = allParticles[focusParticle].zd;
        }
        // is escape velocity to be added?
        if (addEscapeVelocity) {
            if (escapeVelocity!=0.0) {
                x_out=(escapeVelocity*(cos(curlyThing)*sin(theta)));
                y_out=(escapeVelocity*(sin(curlyThing)*sin(theta)));
                z_out=(escapeVelocity*(cos(theta)));
                this->xd += x_out;
                this->yd += y_out;
                this->zd += z_out;
            }
        }
    }
}

/**
*  set up this particle as a spacecraft, fuel structure and paylod must equal unity
*/
void Particle::makeSpacecraft (SpacecraftInitialiser sci) {
    if(sci.structurePercentage+sci.payloadPercentage+sci.fuelPercentage!=1) {
        std::cerr << "spacecraft mass distribution is incorrect: " << sci.structurePercentage+sci.payloadPercentage+sci.fuelPercentage<< std::endl;
        exit(0);
    }
    this->isSpacecraft = true;
    this->red = sci.red;
    this->blue = sci.blue;
    this->green = sci.green;
    this->structurePercentage = sci.structurePercentage;
    this->payloadPercentage = sci.payloadPercentage;
    this->fuelPercentage = sci.fuelPercentage;
    this->mass = sci.mass;
    this->radius = sci.radius;
    this->name = sci.name;
    this->identity = sci.identity;
    this->specificImpulse = sci.specificImpulse;
    this->interactionPermission = sci.interactionPermission;
    this->visualRepresentation = sci.visualRepresentation;
    this->burnsManagerActive = false;
}

/**
* Performs the last stage of RK4 integration, producing the final position of the particle
*/
void Particle::finaliseStep(const int travelTime) {
    // move this particle according to the forces acting on it
    if ((this->identity!=Advisor::collapsorFixed)&&(this->identity!=Advisor::nonInteractive)) {
        double motionx((this->memory[0].xf+this->memory[3].xf+(2*this->memory[1].xf)+(2*this->memory[2].xf))/6);
        double motiony((this->memory[0].yf+this->memory[3].yf+(2*this->memory[1].yf)+(2*this->memory[2].yf))/6);
        double motionz((this->memory[0].zf+this->memory[3].zf+(2*this->memory[1].zf)+(2*this->memory[2].zf))/6);
        for (int j(0); j<travelTime; j++) {
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
* Move a distance specified by using Eulers method.
* The assumption here is that this will be used as part of a more complex integrator, not on its own.
*/
void Particle::moveParticle(const int travelTime) {
    // move this particle according to the forces acting on it
    if ((this->identity!=Advisor::collapsorFixed)&&(this->identity!=Advisor::nonInteractive)) {
        for (int j(0); j<travelTime; j++) {
            this->xd+=this->xf;
            this->x+=xd;
            this->yd+=this->yf;
            this->y+=yd;
            this->zd+=this->zf;
            this->z+=zd;
        }
    }
}
