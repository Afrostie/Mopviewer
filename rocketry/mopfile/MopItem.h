/*
* This class is used to store a single MOP file entry, which represents the state of a particle
*/
#pragma once
#include <string>
#include <cstring>
/**
* This class represents a single particle recorded to the mopfile.
*/
class MopItem
{
public:
    MopItem() {
        memset(this->name, '\0',50 );
    }
    ~MopItem() {
    }
    MopItem(const MopItem& mi) {
        this->mass = mi.mass;
        this->radius = mi.radius;
        this->x = mi.x;
        this->y = mi.y;
        this->z = mi.z;
        this->xd = mi.xd;
        this->yd = mi.yd;
        this->zd = mi.zd;
        this->xf = mi.xf;
        this->yf = mi.yf;
        this->zf = mi.zf;
        this->totalForce = mi.totalForce;
        this->red = mi.red;
        this->green = mi.green;
        this->blue = mi.blue;
        memset(this->name, '\0',50 );
        strcpy(this->name,mi.name);
        this->identity = mi.identity;
        this->interactionPermission = mi.interactionPermission;
        this->visualRepresentation = mi.visualRepresentation;
        this->gapNum = mi.gapNum;
        this->scanLinePos = mi.scanLinePos;
        this->sliceNum = mi.sliceNum;
        this->scanLine = mi.scanLine;
    }

    void fillFromExistingMopItem(const MopItem & other) {
        if (this != &other) // protect against invalid self-assignment
        {
            this->mass = other.mass;
            this->radius = other.radius;
            this->x = other.x;
            this->y = other.y;
            this->z = other.z;
            this->xd = other.xd;
            this->yd = other.yd;
            this->zd = other.zd;
            this->xf = other.xf;
            this->yf = other.yf;
            this->zf = other.zf;
            this->totalForce = other.totalForce;
            this->red = other.red;
            this->red = other.red;
            this->green = other.green;
            this->blue = other.blue;
            strcpy(this->name,other.name);
            this->identity = other.identity;
            this->interactionPermission = other.interactionPermission;
            this->visualRepresentation = other.visualRepresentation;
            this->gapNum = other.gapNum;
            this->scanLinePos = other.scanLinePos;
            this->sliceNum = other.sliceNum;
            this->scanLine = other.scanLine;
        }
    }

    float mass;
    float radius;
    //position in the WCS
    float x;
    float y;
    float z;
    //Velocity in the WCS
    float xd;
    float yd;
    float zd;
    //Current summed forces acting on this particle
    float xf;
    float yf;
    float zf;
    // same, all axis combined
    float totalForce;
    // the colour of a particle
    int red;
    int green;
    int blue;
    char name[50]; // name of particle
    int sliceNum;
    int gapNum;
    int scanLinePos;
    int scanLine;

    std::string nameToStdString() {
        std::string tmp;
        int i = 0;
        while (this->name[i]!='\0') {
            tmp.push_back(this->name[i]);
            i++;
        }
        return tmp;
    }

    int interactionPermission; // Used to say what types of particle this particle is allowed to interact with.
    int identity;   // used to tell this particle what type it is (collapsor/normal particle/placemark/spacecraft)
    int visualRepresentation; // what shape/size this particle will have when displayed in a visualisation
};
