// ARMarker Implementations

#include "ARMarker.h"
#include <Eigen/Dense>

using namespace Eigen;
using namespace std;

// Constructor
ARMarker::ARMarker(int itsid)
{
    id = itsid;
    initialized = false;
}

bool ARMarker::setMarker(Matrix4d transform)
{
    // Initialize if not already
    initialized = true;

    // Set matrix
    World2Marker = transform;
    
    // Extract the position
    double x = transform(0, 3);
    double y = transform(1, 3);
    double z = transform(2, 3);

    // Create vector
    Vector3d location(x, y, z);
    LocInWorld = location;

    return true;    
}

