// Class that holds all camera and marker information 
// Use this for all queries

#ifndef WORLDMODEL_H
#define WORLDMODEL_H

// includes
#include "ARCamera.h"
#include "ARMarker.h"
#include <iostream>
#include <Eigen/Dense>;

using namespace Eigen;
using namespace std;

class WorldModel
{
    // Private member variables
    private:
        ARCamera* cameras;      // Cameras in world
        ARMarker* markers;      // Markers in world
        int cameraNum;          // Number of cameras
        int markerNum;          // Number of markers
    
    // Public functions
    public:
        // Constructor
        WorldModel(ARCamera* arcameras, ARMarker* armarkers, int numcamera, int nummarker)
        {
            cameras = arcameras;
            markers = armarkers;
            cameraNum = numcamera;
            markerNum = numMarker;
        }

        // Method to initialize the first camera to the origin marker
        bool setOrigin(ARCamera camera, ARMarker marker, MatrixXd transform);

        // Method to initialize a camera to the world frame
        bool initCamera(ARCamera cam2Init, ARCamera camAlready, ARMarker commonMark, MatrixXd transNewtoM, Matrix4d transOldtoM);

        // Method to set a marker's location in the world frame
        bool setMarkerLoc(ARCamera camera, ARMarker marker, MatrixXd transform);

        // Method to get the location of a marker relative to the world marker
        Vector3d getMarkerLoc(ARMarker marker);
};

#endif
