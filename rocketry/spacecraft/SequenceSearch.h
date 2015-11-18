#pragma once
/**
* This class is used to hold the result of a call to BurnSequenceManager::findSequenceNeighbors
*/
class SequenceSearch
{
public:
    int subjectStartingState;
    int subjectEndingState;
    int subjectStartingStep;
    int subjectEndingStep;
    int preceedingStartingState;
    int preceedingEndingState;
    int preceedingStartingStep;
    int preceedingEndingStep;
    int preceedingDistance;
    int followingStartingState;
    int followingEndingState;
    int followingStartingStep;
    int followingEndingStep;
    int followingDistance;
    int subjectIndex;
    int preceedingIndex;
    int followingIndex;
    int stepsBefore;
    int stepsAfter;
    SequenceSearch(void)
    {
    }

    ~SequenceSearch(void)
    {
    }
};