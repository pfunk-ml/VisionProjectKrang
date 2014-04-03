// Class that holds all camera and marker information 
// Use this for all queries

#ifndef WORLDMODEL_H
#define WORLDMODEL_H

// includes
#include "ARCamera.h"
#include "ARMarker.h"
#include <iostream>
#include <Eigen/Dense>
#include <vector>

using namespace Eigen;
using namespace std;

class WorldModel
{
    // Private member variables
    private:
        vector<ARCamera> cameras;   // Cameras in world
        vector<ARMarker> markers;   // Markers in world
        int worldMarkerID;          // Keep track of the world marker (also in markers)
    
    // Public functions
    public:
        // Constructor
        WorldModel(vector<ARCamera> arcameras, vector<ARMarker> armarkers);
        
        // Method to initialize the first camera to the origin marker
        bool setOrigin(int cameraID, int markerID, Matrix4d transform);

        // Method to initialize a camera to the world frame
        bool initCamera(int cam2InitID, int camAlreadyID, Matrix4d transMtoNew, Matrix4d transMtoOld);

        // Method to set a marker's location in the world frame
        bool setMarkerLoc(int cameraID, int markerID, Matrix4d transform);

        // Method to get the location of a marker relative to the world marker
        Vector3d getMarkerLoc(int markerID);

        Matrix4d getMarkerPose(int markerID);

    // Private functions
    private:
        // Method to get the position of a particular camera
        int getCamInd(int ID);

        // Method to get the position of a particular marker
        int getMarkInd(int ID);
};

#endif
