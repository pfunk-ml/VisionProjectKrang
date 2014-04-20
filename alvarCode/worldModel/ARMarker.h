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
    
        Matrix4d World2Marker;  // Matrix from world to the marker
        Vector3d LocInWorld;    // The location of the world in world coordinates (3D) 

    public:
        // Constructor
        ARMarker(int itsid);

        // Set the marker by giving it the World2Marker matrix
        bool setMarker(Matrix4d world2marker, int cam_id);

        // Getters and setters
        int getID() { return id; };
        Vector3d getLocInWorld() { return LocInWorld; };
        Matrix4d getMarkerPose() { return World2Marker; };
};

#endif
