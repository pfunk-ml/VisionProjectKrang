// Object containing all camera info

#ifndef ARCAMERA_H
#define ARCAMERA_H

class ARCamera
{
    // Private member variables
    private:
        int id;     // The camera's unique id
        bool initialized;   // True if matrices are properly initialized
    
        Matrix4d Cam2World; // Matrix projects point in camera frame to point in world frame    
        Matrix4d World2Cam; // Matrix projects point in world frame to point in camera frame
};

#endif
