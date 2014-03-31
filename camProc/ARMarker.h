// Object containing all marker info

#ifndef ARMARKER_H
#define ARMARKER_H

#include <Eigen/Dense>

using namespace Eigen;
using namespace std;

class ARMarker
{
    // Private member variables
    private:
        int id;     // The markers's unique id
        bool initialized;   // True if marker is properly initialized
    
        Vector3d LocInWorld;    // The location of the world in world coordinates (3D) 
};

#endif
