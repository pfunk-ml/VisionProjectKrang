// ARCamera Implementations

#include "ARCamera.h"
#include "ARMarker.h"
#include <iostream>

using namespace Eigen;
using namespace std;

// Constructor
ARCamera::ARCamera(int itsid)
{
    id = itsid;
    initialized = false;
}

bool ARCamera::initializeCamera(Matrix4d world2camMeasured)
{
    // Debug
    /*cout << "initializeCamera on camera " << id << " with world2cam " << endl 
        << world2camMeasured << endl;*/

    // Set the World2Cam
    World2Cam = world2camMeasured;

    // Set the Cam2World as the inverse
    Cam2World = world2camMeasured.inverse();

    // Set as initialized
    initialized = true;

    return true;
}
