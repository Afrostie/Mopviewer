#pragma once
#include "BurnEvent.h"
#include "BurnLookup.h"
#include "BurnEventFire.h"

/**
* This class manages a sequence of BurnEvents
*/
class BurnSequence
{
private:
    /**
    * the number of burns in this sequence. This is an unordered list
    */
    int burnCount;

    /**
    *  the actual sequence of burns
    */
    BurnEvent *sequence;

    /**
    *  a lookup table that can be accessed fast to check for burns to fire
    */
    BurnLookup *burnTable;

    /**
    * the current sixe of the burn lookup table
    */
    int burnTableSize;

    /**
    *  in what state does this burn sequence start
    */
    int startState;

    /**
    *  in what state does this burn sequence end
    */
    int endState;

    /**
    *  in what step does this burn sequence start
    */
    int startStep;

    /**
    *  in what step does this burn sequence end
    */
    int endStep;

    /**
    *  the period, in steps, that this burnSequence covers
    */
    int totalSpan;

    /**
    * Mathematical functions and operations
    */
    Numerics maths;

    /**
    *  how much of the total fuel is being made available to this burn sequence
    */
    double fuelPct;
public:

    BurnSequence(void)
    {
        this->burnCount = 0;
        this->burnTableSize = 0;
    }

    ~BurnSequence(void)
    {
        if (this->burnCount != 0) {
            delete [] this->sequence;
        }
        if (this->burnTableSize != 0) {
            delete []this->burnTable;
        }
    }

    BurnSequence( const BurnSequence& bs) {
        this->burnCount = bs.burnCount;
        this->startState = bs.startState;
        this->endState = bs.endState;
        this->startStep = bs.endStep;
        this->endStep = bs.endStep;
        this->totalSpan = bs.totalSpan;
        this->fuelPct = bs.fuelPct;
        if (this->burnCount!=0) {
            this->sequence = new BurnEvent[this->burnCount];
            for (int x(0); x<this->burnCount; x++) {
                this->sequence[x].fillFromExistingBurnEvent(bs.sequence[x]);
            }
        }
        this->burnTableSize = bs.burnTableSize;
        if (this->burnTableSize!=0) {
            this->burnTable = new BurnLookup[this->burnTableSize];
            for (int y(0); y<this->burnTableSize; y++) {
                this->burnTable[y].fillFromExistingBurnLookup(bs.burnTable[y]);
            }
        }
    }

    void fillFromExistingBurnSequence ( const BurnSequence & other)
    {
        if (this != &other) // protect against invalid self-assignment
        {
            if (this->burnCount!=0) {
                this->burnCount = 0;
                delete [] this->sequence;
            }
            this->burnCount = other.burnCount;
            this->startState = other.startState;
            this->endState = other.endState;
            this->startStep = other.endStep;
            this->endStep = other.endStep;
            this->totalSpan = other.totalSpan;
            this->fuelPct = other.fuelPct;
            if (this->burnCount!=0) {
                this->sequence = new BurnEvent[this->burnCount];
                for (int x(0); x<this->burnCount; x++) {
                    this->sequence[x].fillFromExistingBurnEvent(other.sequence[x]);
                }
            }
            this->burnTableSize = other.burnTableSize;
            if (this->burnTableSize!=0) {
                this->burnTable = new BurnLookup[this->burnTableSize];
                for (int y(0); y<this->burnTableSize; y++) {
                    this->burnTable[y].fillFromExistingBurnLookup(other.burnTable[y]);
                }
            }
        }
    }

    /**
    * get the number of burns in this sequence
    */
    int getBurnCount() {
        return this->burnCount;
    }
    /**
    * get a pointer to a given burn in the burn sequence
    */
    BurnEvent & getBurn(int index) {
        return this->sequence[index];
    }

    /**
    * set the active period of this sequence
    * note that this should not be called unless you have already verified that the timing
    * provided will not overlap with another.
    */
    void prepareSequenceActivePeriod(int numberOfStepsPerState, int startState, int endState, int startStep, int endStep) {
        this->startState=startState;
        this->endState=endState;
        this->startStep=startStep;
        this->endState=endStep;
        this->totalSpan = ((endState-startState)*numberOfStepsPerState)+(endStep-startStep);
        // recalculate the timing for the burns in this sequence
        this->calculateBurnSequenceTiming(numberOfStepsPerState);
    }

    /**
    * get the total time in simple steps of this sequence
    */
    int getActivePeriodAsSteps() {
        return this->totalSpan;
    }

    /**
    * get the state on which this burn sequence starts
    */
    int getStartState() {
        return this->startState;
    }

    /**
    *  get the state in which this burn sequence ends
    */
    int getEndState() {
        return this->endState;
    }

    /**
    * get the step on which this burn sequence starts
    */
    int getStartStep() {
        return this->startStep;
    }

    /**
    * get the step on which this burn sequence ends
    */
    int getEndStep() {
        return this->endStep;
    }

    /**
    * get the percentage of the available fuel which this burn sequences uses
    */
    double getFuelPct() {
        return this->fuelPct;
    }

    /**
    * set the percentage of the available fuel which this burn sequences uses
    */
    void setFuelPct( double fuelPct) {
        this->fuelPct = fuelPct;
    }
    /**
    * set the state in which this burn sequence starts
    */
    void setStartState( int startState) {
        this->startState = startState;
    }

    /**
    * set the state in which this burn sequence ends
    */
    void setEndState( int endState) {
        this->endState = endState;
    }

    /**
    * set the step on which this burn sequence starts
    */
    void setStartStep( int startStep) {
        this->startStep = startStep;
    }

    /**
    * set the step on which this burn sequence ends
    */
    void setEndStep( int endStep) {
        this->endStep = endStep;
    }

    /**
    * add a new burn event to this burn sequence using a prepared BurnEvent
    */
    void addBurnEvent (BurnEvent &b) {
        int newSize(this->burnCount);
        if (newSize==0) {
            this->sequence =  new BurnEvent[1];
            this->sequence[0].fillFromExistingBurnEvent(b);
            this->burnCount = 1;
        } else {
            newSize++;
            BurnEvent * bT = new BurnEvent[newSize];
            for (int x(0); x<newSize-1; x++) {
                bT[x].fillFromExistingBurnEvent(this->sequence[x]);
            }
            bT[newSize-1].fillFromExistingBurnEvent(b);
            delete [] this->sequence;
            this->sequence= bT;
            bT = NULL;
            this->burnCount = newSize;
        }
    }

    /**
    * remove a burn event
    */
    void deleteBurnEvent ( int index) {
        int x;
        int newSize = this->burnCount-1;
        if (newSize >0) {
            BurnEvent *bT = new BurnEvent[newSize];
            for (x=0; x<newSize; x++) {
                if (x!=index) {
                    bT[x].fillFromExistingBurnEvent(this->sequence[x]);
                }
            }
            delete [] this->sequence;
            this->sequence= bT;
            bT = NULL;
            this->burnCount-=1;
        }
    }

    /**
    * add a burn event to the fast access burn lookup table
    */
    void addToBurnLookupTable (int state, int step) {
        int newSize(this->burnTableSize);
        if (newSize==0) {
            this->burnTable =  new BurnLookup[1];
            this->burnTable[0].state = state;
            this->burnTable[0].step = step;
            this->burnTableSize = 1;
        } else {
            newSize++;
            BurnLookup * bT = new BurnLookup[newSize];
            for (int x(0); x<newSize-1; x++) {
                bT[x].fillFromExistingBurnLookup(this->burnTable[x]);
            }
            bT[newSize-1].state = state;
            bT[newSize-1].step = step;
            delete [] this->burnTable;
            this->burnTable= bT;
            bT=NULL;
            this->burnTableSize = newSize;
            //this->sortBurnLookupTable();
        }
    }

    /**
    * burn lookup table sort. Uses insertion sort because the list will usually be mostly sorted.
    * Sorts first by state, then again by steps within each state
    * (unfinished method, do not use)
    */
    void sortBurnLookupTable () {
        BurnLookup tmp;
        int i, j;
        for (i=1; i<this->burnTableSize; i++) {
            j=i;
            tmp.state=this->burnTable[j].state;
            tmp.step=this->burnTable[j].step;
            while ((j>0) && (this->burnTable[j-1].state>this->burnTable[j].state));
            {
                this->burnTable[j].state=this->burnTable[j-1].state;
                this->burnTable[j].step=this->burnTable[j-1].step;
                j--;
            }
            this->burnTable[j].state=tmp.state;
            this->burnTable[j].step=tmp.step;
        }
        // and now by step within states
    }

    /**
    * remove a burn event from the lookup table
    */
    void deleteFromBurnLookupTable ( int index) {
        int x;
        int newSize = this->burnTableSize-1;
        if (newSize >0) {
            BurnLookup *bT = new BurnLookup[newSize];
            for (x=0; x<newSize; x++) {
                if (x!=index) {
                    bT[x].fillFromExistingBurnLookup(this->burnTable[x]);
                }
            }
            delete [] this->burnTable;
            this->burnTable= bT;
            bT = NULL;
            this->burnTableSize-=1;
        }
    }

    /**
    * remove the last burn event from the lookup table
    */
    void deleteTopFromBurnLookupTable () {
        delete [] (this->burnTable+this->burnTableSize-1);
        this->burnTableSize-=1;
    }

    /**
    * rebuild the burn lookup table after a change to the burn sequence timing
    */
    void rebuildBurnLookupTable () {
        if ( this->burnTableSize!=0) {
            delete [] this->burnTable;
        }
        this->burnTableSize=0;
        for (int x(0); x<this->burnCount; x++) {
            this->addToBurnLookupTable(this->sequence[x].stateUsed,this->sequence[x].stepUsed);
        }
    }

    /**
    * calculate burn sequence span and apply exact timings to owned burn sequences based on their positionPct values.
    */
    void calculateBurnSequenceTiming(int numberOfStepsPerState) {
        int statePart((this->endState-this->startState)*numberOfStepsPerState);
        int stepPart((this->endStep-this->startStep));
        this->totalSpan=(statePart+stepPart);
        for (int x(0); x<this->burnCount; x++) {
            // get the distance into the burn sequence in terms of steps that this burn is going to fire.
            int originalFirePos(static_cast<int>(this->sequence[x].positionPct*this->totalSpan));
            // now work out exactly which state and step this burn event will fire in
            // first off, will this burn happen between the starting state and the beginning of the next state?
            div_t div1(div(originalFirePos,numberOfStepsPerState));
            this->sequence[x].stateUsed =  this->startState+div1.quot;
            this->sequence[x].stepUsed  = div1.rem;
        }
        // and rebuild the burn sequence lookup table
        this->rebuildBurnLookupTable();
    }

    /**
    * quick check to see whether a burn from this sequence needs to be applied on this state/step
    */
    int checkBurnTable(int state, int step) {
        for (int x(0); x<this->burnTableSize; x++) {
            if ((this->burnTable[x].state == state)&&(this->burnTable[x].step == step)) {
                return x;
            }
        }
        return -1; // default fail to find
    }

    /**
    * make a random burn event (does not calculate this burns fuel percentage allocation)
    */
    BurnEvent createRandomBurnEvent() {
        BurnEvent tmp;
        tmp.positionPct = maths.randomRange(0,1);
        double total(1);
        double pct(0);
        double pctArray[6];
        int count(0);
        int infinityPreventoriserOfDoom(0);
        bool loop = true;
        while(loop) {
            total = 1;
            count = 0;
            infinityPreventoriserOfDoom++;
            for (int x(0); x<6; x++) {
                if (x==5) {
                    if (total!=0) {
                        pctArray[5] = total;
                        count++;
                    }
                } else {
                    pct = maths.randomRange(0,1);
                    pctArray[x] = pct*total;
                    total-=(pct*total);
                    if (total!=0) {
                        count++;
                    }
                }
            }
            if (count==6) {
                loop = false;
            }
            if (infinityPreventoriserOfDoom>1000) {
                std::cerr <<"> Got stuck in infinate loop while creating a new random burn."<< std::endl;
                char c;
                std::cin >> c;
                exit(0);
            }
        }
        // randomise the array just a tad to avoid any bias
        double store(0);
        for (int y(0); y<100; y++) {
            // swap two entries
            int first = rand() % 6;
            int second = rand() % 6;
            store = pctArray[first];
            pctArray[first] = pctArray[second];
            pctArray[second] = store;
        }
        tmp.xMinus = pctArray[0];
        tmp.xPlus  = pctArray[1];
        tmp.yMinus = pctArray[2];
        tmp.yPlus  = pctArray[3];
        tmp.zMinus = pctArray[4];
        tmp.zPlus  = pctArray[5];
        return tmp;
    }

    /**
    *  set up the fuel percentages for the individual burns
    */
    void allocateFuelToBurns() {
        if (this->burnCount!=0) {
            double total(1);
            double pct(0);
            double * pctArray= new double[this->burnCount];
            int count(0);
            bool loop = true;
            int infinityPreventoriserOfDoom(0);
            while(loop) {
                count =0;
                total = 1;
                infinityPreventoriserOfDoom++;
                for (int x(0); x<this->burnCount; x++) {
                    if (x==this->burnCount-1) {
                        if (total>0) {
                            pctArray[this->burnCount-1] = total;
                            count++;
                            if (count==this->burnCount) {
                                loop = false;
                            }
                        }
                    } else {
                        pct = maths.randomRange(0,1);
                        pctArray[x] = pct*total;

                        total-=(pct*total);

                        if (total!=0) {
                            count++;
                        }
                    }
                }
                if (infinityPreventoriserOfDoom>1000) {
                    std::cerr <<"> Got stuck in infinate loop while trying to allocate fuel percentages to the burns in this sequence"<< std::endl;
                    char c;
                    std::cin >> c;
                    exit(0);
                }
            }
            // randomise the array just a tad to avoid any bias
            double store(0);
            for (int y(0); y<100; y++) {
                // swap two entries
                int first = rand() % this->burnCount;
                int second = rand() % this->burnCount;
                store = pctArray[first];
                pctArray[first] = pctArray[second];
                pctArray[second] = store;
            }
            // and store the fuel percentages in the sequence
            for (int i(0); i<this->burnCount; i++) {
                this->sequence[i].fuelPct = pctArray[i];
            }
        }
    }

    /**
    * Retreive a single burn event from those stored so it can be used.
    */
    BurnEventFire fetchBurnEvent(int index) {
        BurnEventFire tmp;
        if ((index>-1)&&(index<this->burnCount)) {
            tmp.fuelPct = this->sequence[index].fuelPct*this->fuelPct; // this burns portion of the fuel available to the sequence
            tmp.xMinus = this->sequence[index].xMinus;
            tmp.xPlus = this->sequence[index].xPlus;
            tmp.yMinus = this->sequence[index].yMinus;
            tmp.yPlus = this->sequence[index].yPlus;
            tmp.zMinus = this->sequence[index].zMinus;
            tmp.zPlus = this->sequence[index].zPlus;
        }
        return tmp;
    }

    /**
    * build a new random burn sequence of size, um, size...
    * we don't set it's fuel percentage, active period or burn timing here, although we do set the
    * fuel percentages for its individual burns, since those are all relative to the
    * values the burn sequence will be given later by the BurnSequenceManager
    */
    void createBurnSequence(int size) {
        for (int x(0); x<size; x++) {
            BurnEvent tmp = this->createRandomBurnEvent();
            this->addBurnEvent(tmp);
        }
        this->allocateFuelToBurns();
    }

    /**
    * import a burn sequence from an existing sequence
    */
    void importBurnSequence(BurnSequence & bs) {
        if (this->burnCount!=0) {
            delete [] this->sequence;
        }
        this->burnCount = 0;
        this->startState = bs.startState;
        this->endState = bs.endState;
        this->startStep = bs.startStep;
        this->endStep = bs.endStep;
        for(int x(0); x<bs.burnCount; x++) {
            this->addBurnEvent(bs.getBurn(x));
        }
    }

    /**
    *  export the contents of this sequence
    */
    BurnSequence exportBurnSequence() {
        BurnSequence bs;
        bs.startState = this->startState;
        bs.endState = this->endState;
        bs.startStep = this->startStep;
        bs.endStep = this->endStep;
        for(int x(0); x<this->burnCount; x++) {
            bs.addBurnEvent(this->getBurn(x));
        }
        return bs;
    }

    //********
    // mutation operators. If accessing this class via BurnSequenceManager, this being the recommended aproach, these need not be called directly, use the wrappers in that class instead
    // *******

    /**
    * mutate burn event position in the burn sequence (time of firing)
    * burn sequence timing is recalculated, so this is an expensive mutation
    */
    void mutateBurnEventPosition(int index, int numberOfStepsPerState) {
        this->sequence[index].positionPct = maths.randomRange(0,1);
        this->calculateBurnSequenceTiming(numberOfStepsPerState);
    }

    /**
    * mutate burn fuel allocation percentages for this sequence by taking from one and giving to another.
    */
    void mutateFuelAllocations() {
        if (this->burnCount!=0) {
            int b1 = rand() % this->burnCount;
            int b2 = rand() % this->burnCount;
            double tmpPct(maths.randomRange(0,1)*this->sequence[b1].fuelPct);
            this->sequence[b1].fuelPct-= tmpPct;
            this->sequence[b2].fuelPct+= tmpPct;
        }
    }

    /**
    * mutate a single BurnEvents thrust allocation
    */
    void mutateRandomBurnEventThrust() {
        if (this->burnCount!=0) {
            int index = rand() % this->burnCount;
            this->sequence[index].mutateBurnEventFuelAxisAllocation();
        }
    }

    /**
    * mutate a single BurnEvents fuel allocation percentages by swapping allocations around.
    * the number of swaps to perform is set in the parameter 'swaps'
    */
    void mutateFuelAllocationsBySwapping(int swaps) {
        if (this->burnCount!=0) {
            int index = rand() % this->burnCount;
            this->sequence[index].mutateFuelAllocationsBySwapping(swaps);
        }
    }
};