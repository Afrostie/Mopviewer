#ifndef ADVISOR_H_
#define ADVISOR_H_
/**
* This class contains static const members that are used to control behaviours in Moody
*/
class Advisor {
public:

	/**
	* Particle interaction setting - Setting for all normal particles (planets and such) - interact with all particles, including special ones, except for placemarks
	*/
	static const int interactALL = 1;

	/**
	* Particle interaction setting - Only interact with ordinary particles and collapsors (would be used for spacecraft)
	*/
	static const int interactEnvironmentOnly = 2;

	/**
	* Particle interaction setting - Setting to be used if the particle is not to interact with any other particles (acting as a fixed placemark)
	*/
	static const int interactNONE = 3;

	/**
	* Particle interaction setting - Setting to be used if the particle is not to interact with other particles of the same type (asteroids, spacecraft, things like that)
	*/
	static const int interactDifferentOnly = 4;

	/**
	* Particle interaction setting - This particle is acting as a collapsor
	*/
	static const int collapsor = 103;

	/**
	* Particle interaction setting - This particle is acting as a collapsor, and isn't being allowed to move
	*/
	static const int collapsorFixed = 104;

	/**
	* Particle interaction setting -  A placemark particle, not to be subject to movement at all
	*/
	static const int nonInteractive = 105;

	/**
	* Particle identity setting -  A Particle that needs no special treatment
	*/
	static const int ordinary = 106;

	/**
	* Particle identity setting -  this particle is a very small body, so small that it needn't be made to interact with others of the same type.
	*/
	static const int planetesimal = 108;
	/**
	* Particle identity setting -  this particle is an EA experiment chromosome
	*/
	static const int chromosome = 109;

	/**
	* Particle memory handling constants - retreive all stored data
	*/
	static const int ALL = 400;

	/**
	* Particle memory handling constants - retreive all stored data except for forces
	*/
	static const int OMIT_FORCES = 401;

	/**
	* Particle memory handling constants - retreive all stored data except for position
	*/
	static const int OMIT_VECTOR = 402;

	/**
	* tournament selection - lowest score required
	*/
	static const int LOWEST = 701;
	/**
	* tournament selection - highest score required
	*/
	static const int HIGHEST = 702;

	/**
	* sequence selected is first sequence (used during sequence mutation bounds checking)
	*/
	static const int Before_First_Sequence = -801;
	/**
	* sequence selected is last sequence (used during sequence mutation bounds checking)
	*/
	static const int After_Last_Sequence = -802;
	// these are used to control what gets read into and retreived from a particles memory

	static const int Spheres = 600;
	static const int Markers = 601;
	static const int Dots = 602;
};

#endif /* ADVISOR_H_ */
