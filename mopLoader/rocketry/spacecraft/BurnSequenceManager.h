#pragma once
#include "BurnEvent.h"
#include "BurnLookup.h"
#include "BurnEventFire.h"
#include "BurnSequence.h"
#include "SequenceSearch.h"
#include <algorithm>
/**
* this class manages the burn sequences stored for a particle
*/
class BurnSequenceManager
{
private:
    /**
    * the array of BurnSequences. This is an unordered list.
    */
    BurnSequence *sequenceSet;

    /**
    * the number of burn sequences stored
    */
    int sequenceCount;

    /**
    * Mathematical functions and operations
    */
    Numerics maths;

    /**
    * sequence end state/step comparison method for use by sequence insertion sorter.
    * allows sequences to be compared using standard min.
    * returns the index of the sequence which is active first of the two given or
    * the first index if they are both the same
    * causes Moody to exit if indexes are invalid
    */
    int minSequence (int index1, int index2) {
        if ((index1<0)||(index1>=this->sequenceCount)) {
            std::cerr  << "> index1 supplied to minSequence is out of bounds. Invalid operation prevented - exiting" << std::endl;
            exit(0);
        }
        if ((index2<0)||(index2>=this->sequenceCount)) {
            std::cerr  << "> index2 supplied to minSequence is out of bounds. Invalid operation prevented - exiting" << std::endl;
            exit(0);
        }
        // ok, index error checks passed, time to sort out which sequence was first
        int actual = std::min(this->sequenceSet[index1].getEndState(),this->sequenceSet[index2].getEndState());
        if (actual==this->sequenceSet[index2].getEndState()) {
            return index2;
        } else {
            return index1;
        }
    }

    /**
    * given the index of a sequence, find the sequences that preceed and follow it, record their starting and stopping
    * points and the distance in steps between them and the subject.
    * with this information  we can work out the amount of room available for mutation
    * that involves changing the size/position of the subject sequence
    * also needs the amount of states in the experiment, and the number of steps in a state
    */
    SequenceSearch findSequenceNeighbors(int subject, int numberOfStates, int numberOfStepsPerState) {
        SequenceSearch tmp;
        int * sequenceList = new int[this->sequenceCount];
        // fill the unsorted list indexes
        // note that these are already sorted in order of index, but this is
        // not the same as sorted by sequence order, and that's what we need
        for (int a(0); a<this->sequenceCount; a++) {
            sequenceList[a] = a;
        }
        // first off, we need a sorted list of burn sequence indexes, in which the burns
        // appear by the order in which they are active, not the order in which they appear in the sequence set
        // this is determined by looking at sequence end state/step
        int i;
        int key;
        for (int x(1); x<this->sequenceCount; x++) {
            i= x;
            key = sequenceList[x];
            while ((i>0)&&(this->minSequence(sequenceList[i-1],key)==key)) {
                sequenceList[i] = sequenceList[i-1];
                i--;
            }
            sequenceList[i] =key;
        }

        // ok, now we need to inspect this array, finding our target sequence and getting the indexes of the
        // sequences on either side
        // special cases: if the target is first or last in this sorted array, we use the Advisor ants Before_First_Sequence or After_Last_Sequence to indicate this
        int loc(0);
        for (int r(0); r<this->sequenceCount; r++) {
            if (sequenceList[r] == subject) {
                loc = r;
                break;
            }
        }
        if (loc ==0) {// special case #1 this is the first sequence
            tmp.subjectIndex =   sequenceList[loc];
            tmp.subjectEndingState = this->sequenceSet[tmp.subjectIndex].getEndState();
            tmp.subjectEndingStep = this->sequenceSet[tmp.subjectIndex].getEndStep();
            tmp.subjectStartingState = this->sequenceSet[tmp.subjectIndex].getStartState();
            tmp.subjectStartingStep =  this->sequenceSet[tmp.subjectIndex].getStartStep();
            tmp.preceedingIndex =  Advisor::Before_First_Sequence;
            tmp.preceedingEndingState = 0;
            tmp.preceedingEndingStep = 0;
            tmp.preceedingStartingState = 0;
            tmp.preceedingStartingStep = 0;
            tmp.preceedingDistance = (tmp.subjectStartingState*numberOfStepsPerState)+tmp.subjectStartingStep;
            tmp.followingIndex =   sequenceList[loc+1];
            tmp.followingEndingState = this->sequenceSet[tmp.followingIndex].getEndState();
            tmp.followingEndingStep = this->sequenceSet[tmp.followingIndex].getEndStep();
            tmp.followingStartingState = this->sequenceSet[tmp.followingIndex].getStartState();
            tmp.followingStartingStep =  this->sequenceSet[tmp.followingIndex].getStartStep();
            tmp.followingDistance = ((tmp.followingStartingState-tmp.subjectEndingState)*numberOfStepsPerState)+((numberOfStepsPerState-tmp.subjectEndingStep)+tmp.followingStartingStep);
        } else if (loc==this->sequenceCount-1) {// special case #2 this is the last sequence
            tmp.subjectIndex =   sequenceList[loc];
            tmp.subjectEndingState = this->sequenceSet[tmp.subjectIndex].getEndState();
            tmp.subjectEndingStep = this->sequenceSet[tmp.subjectIndex].getEndStep();
            tmp.subjectStartingState = this->sequenceSet[tmp.subjectIndex].getStartState();
            tmp.subjectStartingStep =  this->sequenceSet[tmp.subjectIndex].getStartStep();
            tmp.followingIndex =  Advisor::After_Last_Sequence;
            tmp.followingEndingState = numberOfStates;
            tmp.followingEndingStep = 0;
            tmp.followingStartingState = numberOfStates;
            tmp.followingStartingStep = 0;
            tmp.preceedingIndex =  sequenceList[loc-1];
            tmp.preceedingEndingState = this->sequenceSet[tmp.preceedingIndex].getEndState();
            tmp.preceedingEndingStep = this->sequenceSet[tmp.preceedingIndex].getEndStep();
            tmp.preceedingStartingState = this->sequenceSet[tmp.preceedingIndex].getStartState();
            tmp.preceedingStartingStep =  this->sequenceSet[tmp.preceedingIndex].getStartStep();
            tmp.preceedingDistance = ((tmp.subjectStartingState-tmp.preceedingEndingState)*numberOfStepsPerState)+((numberOfStepsPerState-tmp.preceedingEndingStep)+tmp.subjectStartingStep);
        } else {// ordinary case - this is neither the first or last sequence
            tmp.subjectIndex =   sequenceList[loc];
            tmp.subjectEndingState = this->sequenceSet[tmp.subjectIndex].getEndState();
            tmp.subjectEndingStep = this->sequenceSet[tmp.subjectIndex].getEndStep();
            tmp.subjectStartingState = this->sequenceSet[tmp.subjectIndex].getStartState();
            tmp.subjectStartingStep =  this->sequenceSet[tmp.subjectIndex].getStartStep();
            tmp.preceedingIndex =  sequenceList[loc-1];
            tmp.preceedingEndingState = this->sequenceSet[tmp.preceedingIndex].getEndState();
            tmp.preceedingEndingStep = this->sequenceSet[tmp.preceedingIndex].getEndStep();
            tmp.preceedingStartingState = this->sequenceSet[tmp.preceedingIndex].getStartState();
            tmp.preceedingStartingStep =  this->sequenceSet[tmp.preceedingIndex].getStartStep();
            tmp.preceedingDistance = ((tmp.subjectStartingState-tmp.preceedingEndingState)*numberOfStepsPerState)+((numberOfStepsPerState-tmp.preceedingEndingStep)+tmp.subjectStartingStep);
            tmp.followingIndex =   sequenceList[loc+1];
            tmp.followingEndingState = this->sequenceSet[tmp.followingIndex].getEndState();
            tmp.followingEndingStep = this->sequenceSet[tmp.followingIndex].getEndStep();
            tmp.followingStartingState = this->sequenceSet[tmp.followingIndex].getStartState();
            tmp.followingStartingStep =  this->sequenceSet[tmp.followingIndex].getStartStep();
            tmp.followingDistance = ((tmp.followingStartingState-tmp.subjectEndingState)*numberOfStepsPerState)+((numberOfStepsPerState-tmp.subjectEndingStep)+tmp.followingStartingStep);
        }
        delete [] sequenceList;
        return tmp;
    }

    /**
    * adjust a sequence start state/step by adding the given number of steps
    * (makes a sequence start later)
    * does nothing if the result would invalidate the sequence timing
    */
    void moveSequenceStartUp(int steps, BurnSequence &subject, int numberOfStates, int numberOfStepsPerState) {
        if (subject.getStartStep()+steps<numberOfStepsPerState) {
            // adding the steps won't move us up another state, so
            // we need to check that we aren't going to move the start step
            // beyond the end step
            if (subject.getStartState()==subject.getEndState()) {
                if ((subject.getStartStep()+steps)<subject.getEndStep()) {
                    subject.setStartStep(subject.getStartStep()+steps);
                }
            } else {
                subject.setStartStep(subject.getEndStep()+steps);
            }
        } else if (subject.getStartStep()+steps==numberOfStepsPerState) {
            // adding the steps will move us up another state, so
            // have to make sure this won't make the start step equal to the end step,
            // as would happen if the end step was also 0
            // if we can't do this, we do nothing
            if (subject.getStartState()!=subject.getEndState()) {
                if (subject.getEndStep()!=0) {
                    subject.setStartState(subject.getStartState()+1);
                    subject.setStartStep(0);
                }
            }
        } else if(subject.getStartStep()+steps>numberOfStepsPerState) {
            // we need to add possibly more than one state, so we need to make sure we either
            // don't go past the end state, or if we would end up equal, that the start step isn't after
            // the end step.
            div_t pieces = div(steps+subject.getStartStep(),numberOfStepsPerState);
            // check first to make sure we won't exceed the max allowed
            // numer of states
            if (subject.getStartState()+pieces.quot<numberOfStates) {
                if(subject.getStartState()+pieces.quot==subject.getEndState()) {
                    // states will be equal, we need to check and possibly correct the start and end steps
                    if(subject.getStartStep()==subject.getEndStep()) {
                        // they will be equal, which is bad. The answer is to move the start
                        // and end step as far away from each other as possible.
                        // this is a bit harsh, but the other sequence timing mutations should smooth it out.
                        subject.setStartStep(0);
                        subject.setEndStep(numberOfStepsPerState-1);
                    } else if (subject.getStartStep()<subject.getEndStep()) {
                        // the change won't cause out of order problems, we can set the new start state
                        subject.setStartState(subject.getStartState()+pieces.quot);
                    }
                } else if (subject.getStartState()+pieces.quot<subject.getEndState()) {
                    // all is well, we can safely move the starting state up one.
                    subject.setStartState(subject.getStartState()+pieces.quot);
                }
            }
        }
    }

    /**
    * adjust a sequence end state/step by adding the given number of steps
    * (makes a sequence end later)
    *
    */
    void moveSequenceEndUp(int steps, BurnSequence &subject, int numberOfStates, int numberOfStepsPerState) {
        if (subject.getEndStep()+steps<numberOfStepsPerState) {
            // adding the steps won't move us up another state
            subject.setEndStep(subject.getEndStep()+steps);
        } else if (subject.getEndStep()+steps==numberOfStepsPerState) {
            if (subject.getEndState()+1<numberOfStates) {
                subject.setEndState(subject.getEndState()+1);
                subject.setEndStep(0);
            }
        } else if(subject.getEndStep()+steps>numberOfStepsPerState) {
            // we need to add possibly more than one state, so we need to make sure we
            // don't go past the end state
            div_t pieces = div(steps+subject.getEndStep(),numberOfStepsPerState);
            // check to make sure we won't exceed the max allowed
            // numer of states
            if (subject.getEndState()+pieces.quot<numberOfStates) {
                subject.setEndState(subject.getEndState()+pieces.quot);
                if (subject.getEndState()==numberOfStates-1) {
                    // end of the states, so we set steps to 0
                    subject.setEndStep(0);
                }
            }
        }
    }

    /**
    * adjust a sequence start state/step by removing the given number of steps
    * (makes a sequence start earlier)
    */
    void moveSequenceStartDown(int steps, BurnSequence &subject, int numberOfStepsPerState) {
        if (subject.getStartStep()-steps>=0) {
            // removing the number of steps specified will not drop us down another state
            subject.setStartStep(subject.getStartStep()-steps);
        }  else if(subject.getStartStep()+steps<0) {
            // we will be dropping down 1 or more states, so check
            // to ensure we don't go too far back
            if (subject.getStartState()!=0) {
                div_t pieces = div(steps+subject.getEndStep(),numberOfStepsPerState);
                if (subject.getStartState()-pieces.quot<0) {
                    subject.setStartState(0);
                } else {
                    subject.setStartState(subject.getStartState()-pieces.quot);
                }
            }
        }
    }

    /**
    * adjust a sequence end state/step by removing the given number of steps
    * (makes a sequence end earlier)
    */
    void moveSequenceEndDown(int steps, BurnSequence &subject, int numberOfStepsPerState) {
        if (subject.getEndStep()-steps>0) {
            if (subject.getStartState()==subject.getEndState()) {
                // start and end states are identical, prevent the eend step from
                // moving below the start step
                if (subject.getEndStep()-steps>subject.getStartStep()) {
                    subject.setEndStep(subject.getEndStep()-steps);
                }
            } else {
                subject.setEndStep(subject.getEndStep()-steps);
            }
        } else if (subject.getEndStep()+steps==0) {
            // this will put the end step at the start of the step, so we can only allow it
            // if the start state != the end state.
            if (subject.getStartState()<subject.getEndState()) {
                subject.setEndStep(0);
            }
        } else if(subject.getEndStep()+steps<0) {
            div_t pieces = div(steps+subject.getEndStep(),numberOfStepsPerState);
            if (subject.getEndState()-pieces.quot>subject.getStartState()) {
                // this won't move us below the start step
                subject.setEndState(subject.getEndState()-pieces.quot);
            } else if (subject.getEndState()-pieces.quot==subject.getStartState()) {
                // this will make the start and end step identical
                // so we make sure the start/end steps will stay in order
                if (subject.getEndStep()>subject.getStartStep()) {
                    subject.setEndState(subject.getEndState()-pieces.quot);
                }
            }
        }
    }

public:

    BurnSequenceManager(void)
    {
        this->sequenceCount = 0;
    }

    ~BurnSequenceManager(void)
    {
        if (this->sequenceCount!=0) {
            delete []this->sequenceSet;
        }
    }

    BurnSequenceManager( const BurnSequenceManager& bsm) {
        if (this->sequenceCount!=0) {
            delete [] this->sequenceSet;
            this->sequenceCount = 0;
        }
        this->sequenceCount = bsm.sequenceCount;
        if (this->sequenceCount!=0) {
            this->sequenceSet = new BurnSequence[this->sequenceCount];
            for (int x(0); x<this->sequenceCount; x++) {
                this->sequenceSet[x] = bsm.sequenceSet[x];
            }
        }
    }

    void fillFromExistingBurnSequenceManager (BurnSequenceManager & other)
    {
        if (this != &other) // protect against invalid self-assignment
        {
            if (this->sequenceCount!=0) {
                delete [] this->sequenceSet;
                this->sequenceCount = 0;
            }
            this->sequenceCount = other.sequenceCount;
            if (this->sequenceCount!=0) {
                this->sequenceSet = new BurnSequence[this->sequenceCount];
                for (int x(0); x<this->sequenceCount; x++) {
                    this->sequenceSet[x].fillFromExistingBurnSequence(other.sequenceSet[x]);
                }
            }
        }
    }

    /**
    * get the burn sequence specified by index
    */
    BurnSequence &getBurnSequence(int index) {
        if ((index<0)||(index>=this->sequenceCount)) {
            std::cerr  << "> index supplied to getBurnSequence is out of bounds for the set of sequences. Invalid operation prevented - exiting" << std::endl;
            exit(0);
        }
        return sequenceSet[index];
    }

    /**
    * empty this burn sequence manager, deleting the array of stored burn sequences
    */
    void clear() {
        if (this->sequenceCount!=0) {
            delete []this->sequenceSet;
        }
        this->sequenceCount = 0;
    }

    /**
    * get the number of burn sequences stored by this manager
    */
    int getSequenceCount() {
        return this->sequenceCount;
    }

    /**
    * add a new BurnSequence to this burn sequenceSet
    */
    void addBurnSequence (BurnSequence &b) {
        int newSize = this->sequenceCount;
        if (newSize==0) {
            this->sequenceSet =  new BurnSequence[1];
            this->sequenceSet[0].fillFromExistingBurnSequence(b);
            this->sequenceCount = 1;
        } else {
            newSize++;
            BurnSequence * bT = new BurnSequence[newSize];
            BurnSequence tmp;
            for (int x(0); x<newSize-1; x++) {
                bT[x].fillFromExistingBurnSequence(this->sequenceSet[x]);
            }
            bT[newSize-1].fillFromExistingBurnSequence(b);
            delete [] this->sequenceSet;
            this->sequenceSet = bT;
            bT = NULL;
            this->sequenceCount = newSize;
        }
    }

    /**
    * remove a burn sequence
    */
    void deleteBurnSequence ( int index) {
        if ((index<0)||(index>=this->sequenceCount)) {
            std::cerr  << "> index supplied to deleteBurnSequence is out of bounds. Invalid operation prevented - exiting" << std::endl;
            exit(0);
        }
        int x;
        int newSize = this->sequenceCount-1;
        if (newSize >0) {
            BurnSequence *bT = new BurnSequence[newSize];
            for (x=0; x<newSize; x++) {
                if (x!=index) {
                    bT[x].fillFromExistingBurnSequence(this->sequenceSet[x]);
                }
            }
            delete [] this->sequenceSet;
            this->sequenceSet= bT;
            bT = NULL;
            this->sequenceCount-=1;
        }
    }

    /**
    *  create all burn sequences, assigning timing and fuel allocation.
    * The maximum initial size of a burn sequence is specified in terms of steps.
    * These can be <> the size of one or more states.
    * We also need to provide the number of states used for the entire experiment run, and the steps in a state for timing to be worked out.
    * For the sequence timings to be valid they must not overlap. If this is too restrictive, reduce state length.
    */
    void buildBurnSequences(int num,int  maxBurns, int maxSequenceSize, int states, int numberOfStepsPerState) {
        if (num<=0) {
            std::cerr  << "> number of burns supplied to buildBurnSequences cannot be <=0. Invalid operation prevented - exiting" << std::endl;
            exit(0);
        }
        if (maxBurns<=0) {
            std::cerr  << "> max number of burns supplied to buildBurnSequences cannot be <=0. Invalid operation prevented - exiting" << std::endl;
            exit(0);
        }
        if (maxSequenceSize<=0) {
            std::cerr  << "> maximum sequence size supplied to buildBurnSequences cannot be <=0. Invalid operation prevented - exiting" << std::endl;
            exit(0);
        }
        if (states<=0) {
            std::cerr  << "> number of states supplied to buildBurnSequences cannot be <=0. Invalid operation prevented - exiting" << std::endl;
            exit(0);
        }
        if (numberOfStepsPerState<=0) {
            std::cerr  << "> number of steps in a state supplied to buildBurnSequences cannot be <=0. Invalid operation prevented - exiting" << std::endl;
            exit(0);
        }
        //create the burn sequences
        for (int i(0); i<num; i++) {
            BurnSequence tmp;
            tmp.createBurnSequence(maxBurns);
            this->addBurnSequence(tmp);
        }
        // assign fuel percentage allocations
        if (this->sequenceCount!=0) {
            double total(1);
            double pct(0);
            double * pctArray= new double[this->sequenceCount];
            int count(0);
            bool loop(true);
            int infinityPreventoriserOfDoom(0);
            while(loop) {
                count = 0;
                total = 1;
                infinityPreventoriserOfDoom++;
                for (int x(0); x<this->sequenceCount; x++) {
                    if (x==this->sequenceCount-1) {
                        if (total!=0) {
                            pctArray[this->sequenceCount-1] = total;
                            count++;
                        }
                        if (count==this->sequenceCount) {
                            loop = false;
                        }
                    } else {
                        pct = maths.randomRange(0.01,1);
                        pctArray[x] = pct*total;
                        total-=(pct*total);
                        if (total!=0) {
                            count++;
                        }
                    }
                }
                if (infinityPreventoriserOfDoom>1000) {
                    std::cerr <<"> Got stuck in infinate loop while trying to allocate fuel percentages to the burn sequences."<< std::endl;
                    exit(0);
                }
            }
            // randomise the array just a tad to avoid any bias
            double store(0);
            for (int y(0); y<100; y++) {
                // swap two entries
                int first = rand() % this->sequenceCount;
                int second = rand() % this->sequenceCount;
                store = pctArray[first];
                pctArray[first] = pctArray[second];
                pctArray[second] = store;
            }
            // and store the fuel percentages in the sequence
            for (int i(0); i<this->sequenceCount; i++) {
                this->sequenceSet[i].setFuelPct(pctArray[i]);
            }
            // set sequence timing, then check the generated timings to see if they are valid i.e do not overlap
            // if they are valid, drop through, if not, regenerate
            infinityPreventoriserOfDoom=0;
            loop = true;
            while(loop) {
                infinityPreventoriserOfDoom++;
                for (int j(0); j<this->sequenceCount; j++) {
                    int startState;
                    int startStep;
                    int endState;
                    int endStep;
                    // find the timing
                    // keep trying until the period is non zero,
                    // startState and endState are equal or endState is later,
                    // and if startState==endState, startStep >endStep.
                    while(true) {
                        int period;
                        int location;
                        while(1) {
                            period = maths.round(maths.randomRange(0.1,1)*maxSequenceSize);
                            if (period!=0) {
                                break;
                            }
                        }
                        location = maths.round(maths.randomRange(0,1)*((states*numberOfStepsPerState))-period);
                        //std::cout <<"location= " << location << std::endl;
                        div_t div1(div(location,numberOfStepsPerState));
                        startState =  div1.quot;
                        startStep  = div1.rem;
                        div_t div2(div(location+period,numberOfStepsPerState));
                        endState =  div2.quot;
                        endStep  = div2.rem;
                        if(startState<=endState) {
                            if(startStep<endStep) {
                                break;
                            }
                        }
                    }
                    // apply it
                    this->sequenceSet[j].setStartState(startState);
                    this->sequenceSet[j].setStartStep(startStep);
                    this->sequenceSet[j].setEndState(endState);
                    this->sequenceSet[j].setEndStep(endStep);
                }
                //validity check required here
                // for the calculated timings to be valid there must be no overlap of sequences active periods
                bool overlap = false;
                bool valid = true;
                for (int a(0); a<this->sequenceCount; a++) {
                    for (int b(0); b<this->sequenceCount; b++) {
                        if(a!=b) { // no self comparisions, that would be silly
                            overlap = maths.isValueWithinRange(this->sequenceSet[a].getStartState(),this->sequenceSet[b].getStartState(),this->sequenceSet[b].getEndState());
                            if (overlap) {
                                valid = false;
                            }
                            overlap = maths.isValueWithinRange(this->sequenceSet[a].getEndState(),this->sequenceSet[b].getStartState(),this->sequenceSet[b].getEndState());
                            if (overlap) {
                                valid = false;
                            }
                        }
                    }
                }
                // if valid is still true, drop out
                if(valid) {
                    loop = false;
                }
                if (infinityPreventoriserOfDoom>1000) {
                    std::cerr <<"> Got stuck in infinate loop while trying to set sequence active periods"<< std::endl;
                    exit(0);
                }
            }
            // finally, allocate fuel and timing to the individual burns in each sequence
            for (int m(0); m<this->sequenceCount; m++) {
                this->sequenceSet[m].allocateFuelToBurns();
                this->sequenceSet[m].calculateBurnSequenceTiming(numberOfStepsPerState);
            }
        }
    }

    /**
    *  rebuild all burn sequence timing tables
    */
    void rebuildBurnSequenceTimingTables(int steps) {
        for (int m(0); m<this->sequenceCount; m++) {
            this->sequenceSet[m].calculateBurnSequenceTiming(steps);
        }
    }

    /**
    * Check to see whether one of the burn sequences needs to fire a burn at this point (state/step)
    * return -1 if no burn is due or the index of the burn sequence that contains the
    * burn to fire
    */
    int scanBurnTables(int state, int step) {
        for (int x(0); x<this->sequenceCount; x++) {
            if (this->sequenceSet[x].checkBurnTable(state,step)!=-1) {
                return x;
            }
        }
        return -1;
    }

    /**
    * Fetch a burn event from the sequence indicated by index that the timing tables indicate is now due
    * Not this will only work if there actually is a burn event due at the current state and step.
    * establish whether there is with a call to scanBurnTables first, which will return the correct sequence index.
    * Note that once retreived by this method a burn event is considered to have been used/fired, and will no longer be available.
    * If all you want to do is read/alter a burn event, use the BurnSequence::getBurn() method
    */
    BurnEventFire getCurrentBurnEvent(int sequence, int state, int step) {
        if (sequence!=-1) {
            for (int x(0); x<this->getBurnSequence(sequence).getBurnCount(); x++) {
                //std::cout << "> checking Burn Event "<<x<<" of sequence " << sequence <<std::endl;
                if ((this->getBurnSequence(sequence).getBurn(x).stateUsed == state)&&(this->getBurnSequence(sequence).getBurn(x).stepUsed == step)) {
                    //std::cout << "> Burn Event "<<x<<" Fired by sequence " << sequence <<std::endl;
                    return this->getBurnSequence(sequence).fetchBurnEvent(x);
                }
            }
        }
        // return an empty BurnFireEvent if there wasn't one found
        BurnEventFire tmp;
        return tmp;
    }

    //********
    // mutation operators.
    // *******

    /**
    * mutate burn event position for a randomly selected sequence burn sequence (time of firing)
    * burn sequence timing is recalculated, so this is an expensive mutation
    * this is a wrapper for a mutation operator that belongs to the sequence being mutated
    */
    void mutateSingleBurnEventPosition(int steps) {
        int tmpSeq = rand() % this->sequenceCount;
        int tmpIndex = rand() % this->sequenceSet[tmpSeq].getBurnCount();
        this->sequenceSet[tmpSeq].mutateBurnEventPosition(tmpIndex,steps);
    }

    /**
    * mutate burn event thrust allocation for a single burn of a randomly selected sequence.
    * this is a wrapper for a mutation operator that belongs to the sequence being mutated
    */
    void mutateSingleBurnEventThrust() {
        int tmpSeq = rand() % this->sequenceCount;
        this->sequenceSet[tmpSeq].mutateRandomBurnEventThrust();
    }

    /**
    * mutate burn fuel allocation percentages for burns in a randomly selected sequence by taking from one and giving to another.
    * this is a wrapper for a mutation operator that belongs to the sequence being mutated
    */
    void mutateFuelAllocationForBurnsInSingleSequence() {
        int tmpSeq = rand() % this->sequenceCount;
        this->sequenceSet[tmpSeq].mutateFuelAllocations();
    }

    /**
    * mutate burn fuel allocation percentages for a randomly selected sequence by swapping allocations between burns.
    * the number of swaps to perform is set in the parameter 'swaps'.
    * this is a wrapper for a mutation operator that belongs to the sequence being mutated
    */
    void mutateRandomBurnFuelAllocationsBySwapping(int swaps) {
        int tmpSeq = rand() % this->sequenceCount;
        this->sequenceSet[tmpSeq].mutateFuelAllocationsBySwapping(swaps);
    }

    /**
    * swap the fuel allocations of two sequences
    */
    void mutateSwapSequenceFuelAllocations() {
        int seq1 = rand() % this->sequenceCount;
        int seq2 = rand() % this->sequenceCount;
        double tmp;
        tmp = this->sequenceSet[seq1].getFuelPct();
        this->sequenceSet[seq1].setFuelPct(this->sequenceSet[seq2].getFuelPct());
        this->sequenceSet[seq2].setFuelPct(tmp);
    }

    /**
    * mutate the fuel allocations of two sequences, taking from one and giving to another
    */
    void mutateSequenceFuelAllocations() {
        int seq1 = rand() % this->sequenceCount;
        int seq2 = rand() % this->sequenceCount;
        double change = this->sequenceSet[seq1].getFuelPct()*(sqrt(maths.sqr(maths.generateGaussian())));
        int choice = rand() %10;
        if (choice<5) {
            this->sequenceSet[seq1].setFuelPct(this->sequenceSet[seq1].getFuelPct()+change);
            this->sequenceSet[seq2].setFuelPct(this->sequenceSet[seq2].getFuelPct()-change);
        } else {
            this->sequenceSet[seq1].setFuelPct(this->sequenceSet[seq1].getFuelPct()-change);
            this->sequenceSet[seq2].setFuelPct(this->sequenceSet[seq2].getFuelPct()+change);
        }
    }

    /**
    * swap the positioning of two sequences, how many times to do this is determined by the parameter 'swaps'
    * This is done by the burn sequence manager, not the burn sequences involved, because it requires
    * knowledge of the set of sequences that a single sequence couldn't have
    * the burn timings of each sequence are recalculated after the swap is complete
    */
    void mutateSequencesBySwappingPosition(int swaps, int numberOfStepsPerState) {
        int Seq1(0);
        int Seq2(0);
        int tmpStartState;
        int tmpEndState;
        int tmpStartStep;
        int tmpEndStep;
        for (int x(0); x<swaps; x++) {
            Seq1 = rand() % this->sequenceCount;
            Seq2 = rand() % this->sequenceCount;
            tmpStartState=this->sequenceSet[Seq1].getStartState();
            tmpEndState=this->sequenceSet[Seq1].getEndState();
            tmpStartStep=this->sequenceSet[Seq1].getStartStep();
            tmpEndStep=this->sequenceSet[Seq1].getEndStep();
            this->sequenceSet[Seq1].setStartState(this->sequenceSet[Seq2].getStartState());
            this->sequenceSet[Seq1].setEndState(this->sequenceSet[Seq2].getEndState());
            this->sequenceSet[Seq1].setStartStep(this->sequenceSet[Seq2].getStartStep());
            this->sequenceSet[Seq1].setEndStep(this->sequenceSet[Seq2].getEndStep());
            this->sequenceSet[Seq2].setStartState(tmpStartState);
            this->sequenceSet[Seq2].setEndState(tmpEndState);
            this->sequenceSet[Seq2].setStartStep(tmpStartStep);
            this->sequenceSet[Seq2].setEndStep(tmpEndStep);
        }
        // recalculate burn timing for the effected sequences
        this->sequenceSet[Seq1].calculateBurnSequenceTiming(numberOfStepsPerState);
        this->sequenceSet[Seq2].calculateBurnSequenceTiming(numberOfStepsPerState);
    }

    /**
    * mutate the positioning of a randomly selected sequence (you cannot control which sequence this is).
    * limits: The sequence can only either shrink or grow or move in a manner which does not stray into the period occupied by another existing sequence.
    * parameter is the number of steps there are in a state.
    * This is done by the burn sequence manager, not the burn sequences involved, because it requires
    * knowledge of the set of sequences that a single sequence couldn't have.
    * the burn timings of the subject sequence is recalculated after the resize or move is complete
    */
    void mutateSequenceByChangingTiming( int numberOfStates, int numberOfStepsPerState) {
        int subject = rand() % this->sequenceCount;
        // find the neighbors of this sequence and their distances
        SequenceSearch info = this->findSequenceNeighbors(subject,numberOfStates,numberOfStepsPerState);
        int moveBy;
        int choose = rand() % 10;
        if(choose<5) { // move without resizing
            if(info.preceedingDistance==0) {
                // we can only move towards the following sequence
                moveBy = rand() % info.followingDistance;
                if (moveBy>0) {
                    this->moveSequenceEndUp(moveBy,this->sequenceSet[subject],numberOfStates,numberOfStepsPerState);
                    this->moveSequenceStartUp(moveBy,this->sequenceSet[subject],numberOfStates,numberOfStepsPerState);
                }
            } else if (info.followingDistance==0) {
                // we can only move towards the preceeding sequence
                moveBy = rand() % info.preceedingDistance;
                if (moveBy>0) {
                    this->moveSequenceStartDown(moveBy,this->sequenceSet[subject],numberOfStepsPerState);
                    this->moveSequenceEndDown(moveBy,this->sequenceSet[subject],numberOfStepsPerState);
                }
            } else {
                // we can move in either direction, yay!
                // pick some portion of the smaller distance and use that
                // to work out how far this sequence will be moved
                moveBy = rand() % (std::min(info.preceedingDistance, info.followingDistance));
                if (moveBy>0) {
                    int direction = rand() %10;
                    if(direction<5) { // down
                        this->moveSequenceStartDown(moveBy,this->sequenceSet[subject],numberOfStepsPerState);
                        this->moveSequenceEndDown(moveBy,this->sequenceSet[subject],numberOfStepsPerState);
                    } else { //up
                        this->moveSequenceEndUp(moveBy,this->sequenceSet[subject],numberOfStates,numberOfStepsPerState);
                        this->moveSequenceStartUp(moveBy,this->sequenceSet[subject],numberOfStates,numberOfStepsPerState);
                    }
                }
            }
        } else { // resize
            int select = rand() %2;
            int availableRange  = ((info.subjectEndingState-info.subjectStartingState)*numberOfStepsPerState)+((numberOfStepsPerState-info.subjectStartingStep)+info.subjectEndingStep);
            // the chances of availableRange being 0 are remote, but it might happen, and if it did
            // this mutation would fail, bigtime, so that would amount to a fail
            if (availableRange==0) {
                std::cerr  << "> Burn Sequence of zero size detected. Invalid operation prevented - exiting" << std::endl;
                exit(0);
            }
            moveBy = rand() % availableRange;
            if (moveBy>0) {
                switch(select) {
                case 0: {// shrinking
                    int select1 = rand() %10;
                    if(select1<5) {// move the start up
                        this->moveSequenceStartUp(moveBy,this->sequenceSet[subject],numberOfStates,numberOfStepsPerState);
                    } else {// move the end down
                        this->moveSequenceEndDown(moveBy,this->sequenceSet[subject],numberOfStepsPerState);
                    }
                }
                break;
                case 1: {// expanding
                    int select2 = rand() %10;
                    if(select2<5) {// move the start down
                        this->moveSequenceStartDown(moveBy,this->sequenceSet[subject],numberOfStepsPerState);
                    } else {// move the end up
                        this->moveSequenceEndUp(moveBy,this->sequenceSet[subject],numberOfStates,numberOfStepsPerState);
                    }
                }
                break;
                }
            }
        }
        this->sequenceSet[subject].calculateBurnSequenceTiming(numberOfStepsPerState);
    }
};
