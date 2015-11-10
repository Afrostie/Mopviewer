#pragma once
/**
	* Very simple annealer for use in Stochastic Hill Climbing. Provides a simple linear rate of cooling.
	* To use, initialise before beginning the iterations of the hill climber, then make one call to reduce()
	* once for every subsequent iteration. If an annealing jump is produced reduce() will return true, if not, it
	* will return false;
	*/
#include "Numerics.h"
class Annealer
{
private:

	/**
	* the amount by which a single decrement reduces the temperature
	*/
	double coolingTerm;

	/**
	* at what point in the experiment, in terms of iterations, does the annealer need to reach haltingTemperature
	*/
	int haltingPoint;

	/**
	* the current temperature
	*/
	double temperatureNow;

	/**
	* total number of iterations still to do
	*/

	int iterationsRemaining;

	/**
	* the number of iterations that may pass before a temperature decrement takes place
	*/
	int decrementInterval;

	/**
	* the current value of the counter
	*/
	int counterValue;

	/**
	* the current value of the counter
	*/
	double randPick;

	/**
	* Mathematical functions and operations
	*/
	Numerics maths;

public:

	Annealer(void)
	{
	}

	virtual ~Annealer(void)
	{
	}
	Annealer(const Annealer& a) {
		this->coolingTerm = a.coolingTerm;
		this->counterValue = a.counterValue;
		this->decrementInterval = a.decrementInterval;
		this->haltingPoint = a.haltingPoint;
		this->temperatureNow = a.temperatureNow;
	}

	Annealer & operator = (const Annealer & other) {
		if (this != &other) // protect against invalid self-assignment
		{
			this->coolingTerm = other.coolingTerm;
			this->counterValue = other.counterValue;
			this->decrementInterval = other.decrementInterval;
			this->haltingPoint = other.haltingPoint;
			this->temperatureNow = other.temperatureNow;
		}
		return *this;
	}
	friend std::ostream &operator << (std::ostream &out,Annealer  a) {
		out <<"  <annealer>" <<std::endl;
		out <<"		<coolingTerm>"<<a.coolingTerm<<"</coolingTerm>" <<std::endl;
		out <<"		<counterValue>"<<a.counterValue<<"</counterValue>" <<std::endl;
		out <<"		<decrementInterval>"<<a.decrementInterval<<"</decrementInterval>" <<std::endl;
		out <<"		<haltingPoint>"<<a.haltingPoint<<"</haltingPoint>" <<std::endl;
		out <<"		<temperatureNow>"<<a.temperatureNow<<"</temperatureNow>" <<std::endl;
		out <<"  </annealer>" <<std::endl;
		return out;
	}
	/**
	* initialise the annealer.
	* totalTime is the runtime of your experiment, haltingPct allows you to set a point other than the end of the
	* total runtime at which the annealer should terminate
	*/
	void initialise(int totalTime, double haltingPct = 0 ) {
		this->haltingPoint = static_cast<int> (haltingPct * totalTime);
		this->iterationsRemaining = totalTime;
		this->temperatureNow = 1;
		this->coolingTerm = this->temperatureNow/this->iterationsRemaining;
		this->decrementInterval = static_cast<int> (this->coolingTerm * totalTime);
	}

	/**
	* call this method on each iteration of your experiment. It returns true if an annealing jump is allowed
	* or false otherwise
	*/
	bool reduce() {
		if (this->iterationsRemaining<this->haltingPoint) {
			return false; // no jump allowed
		}
		this->iterationsRemaining--;
		this->counterValue++;
		if (this->counterValue==this->decrementInterval) {
			this->counterValue = 0;
			this->temperatureNow-=this->coolingTerm;
		}
		this->randPick = maths.randomRange(0,1);
		if (this->randPick<this->temperatureNow) {
			return true; // allow a jump
		}
		// default, no jump
		return false;
	}
};