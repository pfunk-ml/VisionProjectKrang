// Object containing all marker info

#ifndef ARMARKER_H
#define ARMARKER_H

class ARMARKER
{
    // Private member variables
    private:
        int id;     // The markers's unique id
        bool initialized;   // True if marker is properly initialized
    
        Vector3d LocInWorld;    // The location of the world in world coordinates (3D) 
};

#endif
