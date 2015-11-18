#pragma once
#include <string>
/**
* This struct is used when manipulating/initialising the contents of a particle
*/
struct ParticleStruct {
	/**
	* the id of this particle
	*/
	int id;
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
	* rotation variable storage (used when placing/moving particles on the surface of a sphere)
	*/
	double curlyThing;

	/**
	* other rotation variable storage (used when placing/moving particles on the surface of a sphere)
	*/
	double theta;	

};
