/*
* This class is used to store a single MOP file entry, which represents the state of a particle, in abbreviated form for larger file sizes
*/
#pragma once
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
        this->x = mi.x;
        this->y = mi.y;
        this->z = mi.z;
        this->red = mi.red;
        this->green = mi.green;
        this->blue = mi.blue;
    }

    void fillFromExistingMopItem(const MopItem & other) {
        if (this != &other) // protect against invalid self-assignment
        {
            this->id = other.id;
            this->visualRepresentation = other.visualRepresentation;
            this->x = other.x;
            this->y = other.y;
            this->z = other.z;
            this->red = other.red;
            this->green = other.green;
            this->blue = other.blue;
        }
    }
    
    int id;
    int visualRepresentation; // what shape/size this particle will have when displayed in a visualisation
    //position in the WCS
    float x;
    float y;
    float z;
    // the colour of a particle
    int red;
    int green;
    int blue;
};
