// Object containing all camera info

#ifndef ARCAMERA_H
#define ARCAMERA_H

#include <Eigen/Dense>

using namespace Eigen;
using namespace std;

class ARCamera
{
    // Private member variables
    private:
        int id;     // The camera's unique id
        bool initialized;   // True if matrices are properly initialized
    
        Matrix4d Cam2World; // Matrix projects point in camera frame to point in world frame    
        Matrix4d World2Cam; // Matrix projects point in world frame to point in camera frame

    public:
        // Constructor
        ARCamera(int itsid);
        
        // Initialize the camera by giving it the World2Cam matrix
        bool initializeCamera(Matrix4d world2camMeasured);

        // Getters and setters
        int getID() { return id; };
        bool isInitialized() { return initialized; };
        Matrix4d getCam2World() { return Cam2World; };
        Matrix4d getWorld2Cam() { return World2Cam; };

};

#endif
