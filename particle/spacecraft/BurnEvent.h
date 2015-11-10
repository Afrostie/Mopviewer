#pragma once
#include "../../etc/Numerics.h"
/**
* This represents a single burn for a spacecraft particle
* This class only has public members, since member values are all accessed and changed
* externally, but has methods to perform mutations
*/
class BurnEvent {
public:
    BurnEvent(void) {
        this->fuelPct =0;
        this->positionPct =0;
        this->stateUsed =0;
        this->stepUsed =0;
        this->xMinus =0;
        this->xPlus =0;
        this->yMinus =0;
        this->yPlus =0;
        this->zMinus =0;
        this->zPlus =0;
    }

    BurnEvent(const BurnEvent& bs) {
        this->fuelPct = bs.fuelPct;
        this->positionPct = bs.positionPct;
        this->stateUsed = bs.stateUsed;
        this->stepUsed = bs.stepUsed;
        this->xMinus = bs.xMinus;
        this->xPlus = bs.xPlus;
        this->yMinus = bs.yMinus;
        this->yPlus = bs.yPlus;
        this->zMinus = bs.zMinus;
        this->zPlus = bs.zPlus;
    }

    void fillFromExistingBurnEvent (const BurnEvent & other) {
        if (this != &other) // protect against invalid self-assignment
        {
            this->fuelPct     = other.fuelPct;
            this->positionPct = other.positionPct;
            this->stateUsed   = other.stateUsed;
            this->stepUsed    = other.stepUsed;
            this->xMinus      = other.xMinus;
            this->xPlus       = other.xPlus;
            this->yMinus      = other.yMinus;
            this->yPlus       = other.yPlus;
            this->zMinus      = other.zMinus;
            this->zPlus       = other.zPlus;
        }
    }

    bool operator == (const BurnEvent & other) {
        if (this->fuelPct != other.fuelPct) {
            return false;
        }
        if (this->positionPct != other.positionPct) {
            return false;
        }
        if (this->stateUsed != other.stateUsed) {
            return false;
        }
        if (this->stepUsed != other.stepUsed) {
            return false;
        }
        if (this->xMinus != other.xMinus) {
            return false;
        }
        if (this->xPlus != other.xPlus) {
            return false;
        }
        if (this->yMinus != other.yMinus) {
            return false;
        }
        if (this->yPlus != other.yPlus) {
            return false;
        }
        if (this->zMinus != other.zMinus) {
            return false;
        }
        if (this->zPlus != other.zPlus) {
            return false;
        }
        return true;
    }

    bool operator  != (const BurnEvent & other) {
        return !(*this==other);
    }

    /**
    * when in the time span occupied by the parent sequence will this burn event occur.
    *  The burn point is expressed as a percentage of the time span occupied by the parent sequence
    */
    double positionPct;

    /**
    *  the state in which this burn is to be used (calculated dynamically, do not set directly)
    */
    int stateUsed;

    /**
    *  the step on which this burn is to be used (calculated dynamically, do not set directly)
    */
    int stepUsed;

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

    /**
    * Mathematical functions and operations
    */
    Numerics maths;

    /**
    * mutate burn fuel allocation percentages by swapping allocations around.
    * the number of swaps to perform is set in the parameter 'swaps'
    */
    void mutateFuelAllocationsBySwapping(int swaps) {
        double tmpPct;
        double store[6];
        store[0] = this->xPlus;
        store[1] = this->xMinus;
        store[2] = this->yPlus;
        store[3] = this->yMinus;
        store[4] = this->zPlus;
        store[5] = this->zMinus;
        for (int x =0; x<swaps; x++) {
            int b1 = rand() % 6;
            int b2 = rand() % 6;
            tmpPct = store[b1];
            store[b1] = store[b2];
            store[b2] = tmpPct;
        }
        this->xPlus = store[0];
        this->xMinus = store[1];
        this->yPlus = store[2];
        this->yMinus = store[3];
        this->zPlus = store[4];
        this->zMinus =store[5] ;
    }

    /**
    * mutate burn event fuel allocation
    */
    void mutateBurnEventFuelAxisAllocation() {
        double tmpPct;
        double store[6];
        store[0] = this->xPlus;
        store[1] = this->xMinus;
        store[2] = this->yPlus;
        store[3] = this->yMinus;
        store[4] = this->zPlus;
        store[5] = this->zMinus;
        int b1 = rand() % 6;
        int b2 = rand() % 6;
        tmpPct = maths.randomRange(0,1)*store[b1];
        store[b1] -=tmpPct;
        store[b2] += tmpPct;
        this->xPlus = store[0];
        this->xMinus = store[1];
        this->yPlus = store[2];
        this->yMinus = store[3];
        this->zPlus = store[4];
        this->zMinus =store[5] ;
    }
};