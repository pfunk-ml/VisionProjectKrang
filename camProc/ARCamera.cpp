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

bool ARCamera::initializeCamera(Matrix4d _Tworld_cam )
{

    // Set the World2Cam
    Cam2World = _Tworld_cam;
    

    // Set the Cam2World as the inverse
    World2Cam = Cam2World.inverse();

    // Set as initialized
    initialized = true;

    return true;
}
