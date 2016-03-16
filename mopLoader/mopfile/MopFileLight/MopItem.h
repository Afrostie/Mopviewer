/*
* This class is used to store a single MOP file entry, which represents the state of a particle, in abbreviated form for larger file sizes
*/
#pragma once
#include <iostream>
/**
* This class represents a single particle recorded to the mopfile.
*/
class MopItem
{
public:
    MopItem() {
    }

    ~MopItem() {
    }

    MopItem(const MopItem& mi) {
        this->id = mi.id;
        this->visualRepresentation = mi.visualRepresentation;
        this->red = mi.red;
        this->green = mi.green;
        this->blue = mi.blue;
        this->x = mi.x;
        this->y = mi.y;
        this->z = mi.z;
    }

    void fillFromExistingMopItem(const MopItem & other) {
        if (this != &other) // protect against invalid self-assignment
        {
            this->id = other.id;
            this->visualRepresentation = other.visualRepresentation;
            this->red = other.red;
            this->green = other.green;
            this->blue = other.blue;
            this->x = other.x;
            this->y = other.y;
            this->z = other.z;
        }
    }

    int id;
    int visualRepresentation; // what shape/size this particle will have when displayed in a visualisation
    int red;
    int green;
    int blue;
    //position in the WCS
    double x;
    double y;
    double z;
    // the colour of a particle

};
