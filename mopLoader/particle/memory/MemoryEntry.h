#pragma once
/**
* A memory entry for a particle
*/
class MemoryEntry
{
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
	*  is the content of this memory block valid? (memory resets do not actually empty memory blocks, that's a waste of time)
	*/
	bool exists;
	MemoryEntry(void)
	{
	}

	~MemoryEntry(void)
	{
	}
};