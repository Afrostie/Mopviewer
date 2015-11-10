#pragma once
/**
* This class represents a single burn for a spacecraft particle
*/
class BurnEventFire {
public:
    BurnEventFire(void)
    {
        this->fuelPct = 0;
        this->xMinus = 0;
        this->xPlus = 0;
        this->yMinus = 0;
        this->yPlus = 0;
        this->zMinus = 0;
        this->zPlus = 0;
    }

    /**
    *  the amount of fuel to be used in this burn, expressed as a percentage of the fuel available to the parent sequence
    */
    double fuelPct;

    /**
    * positive thrust percentage for the X axis
    */
    double xPlus;

    /**
    * negative thrust percentage for the X axis
    */
    double xMinus;

    /**
    * positive thrust percentage for the Y axis
    */
    double yPlus;

    /**
    * negative thrust percentage for the Y axis
    */
    double yMinus;

    /**
    * positive thrust percentage for the Z axis
    */
    double zPlus;

    /**
    * negative thrust percentage for the Z axis
    */
    double zMinus;
};