#pragma once
/**
	* This class holds a single entry in a particles history
	* history means the positions and velocities recorded for a particle during a given run
	*/
class ParticleHistory {
public:
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
    * clear this memory entry's content
    */
    void blank() {
        this->x = 0;
        this->y = 0;
        this->z = 0;
        this->xd = 0;
        this->yd = 0;
        this->zd = 0;
    }
};