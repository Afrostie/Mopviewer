#pragma once
/**
*
*/
struct SpacecraftInitialiser {
    /**
    * the name for this spacecraft
    */
    std::string name;

    /**
    * this spacecrafts mass
    */
    double mass;

    /**
    * the percentage of this particles total mass that will be structural content
    */
    double structurePercentage;

    /**
    * the percentage of this particles total mass that will be payload content
    */
    double payloadPercentage;

    /**
    * the percentage of this particles total mass that will be fuel content
    */
    double fuelPercentage;

    /**
    * the specific impulse to be used by this spacecraft
    */
    double specificImpulse;

    /**
    * the radius for this spacecraft
    */
    double radius;

    /**
    * the red portion of this spacecrafts colour
    */
    int red;

    /**
    * the greem portion of this spacecrafts colour
    */
    int green;

    /**
    * the blue portion of this spacecrafts colour
    */
    int blue;

    /**
    *  What types of particle this particle is allowed to interact with.
    */
    int interactionPermission;

    /**
    * what type this particle is - most likely 'spacecraft'
    */
    int identity;

    /**
    *  what size this particle will have when displayed in a visualisation
    */
    int visualRepresentation;
};