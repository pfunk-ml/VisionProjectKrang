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
    
        Matrix4d Cam2World; // Matrix projects point in camera frame to point 
                            // in world frame    
        Matrix4d World2Cam; // Matrix projects point in world frame to point 
                            // in camera frame

    public:
        // Constructor
        ARCamera(int itsid);
        
        /* Initializes the camera by giving it World2Cam matrix. It calculates 
           and stores the inverse to get Cam2World transformation matrix.
            _Tworld_cam : [IN] The 4x4 transformation matrix from cam frame 
                               to world frame
            return val  : Always return true */
        bool initializeCamera(Matrix4d _Tworld_cam );

        // Getters and setters
        int getID() { return id; };
        
        bool isInitialized() { return initialized; };

        /* Returns 4x4 transformation matrix from camera to world frame. */
        Matrix4d getCam2World() { return Cam2World; };

        /* Return 4x4 transformation matrix from world to camera frame. */
        Matrix4d getWorld2Cam() { return World2Cam; };
};

#endif
