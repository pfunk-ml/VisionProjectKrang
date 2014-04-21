/**
 * @file WorldModel.h
 * @brief  Class that holds all camera and marker information 
 *  Use this for all queries
 */
#pragma once

#include "ARCamera.h"
#include "ARMarker.h"
#include <iostream>
#include <Eigen/Dense>
#include <vector>


   
/** Helpers */
double** getMatAsDouble( const Eigen::Matrix4d &_mat );
Eigen::Matrix4d getDoubleArrAsMat( double _trans[3][4] );

void getXYangTriple( const Eigen::Matrix4d &_Tf, 
		     double& _x, 
		     double& _y, 
		     double& _ang );
Eigen::Matrix4d getAverageAffine(std::vector<Eigen::Matrix4d> _tf);
Eigen::Matrix4d getAverageAffine(std::vector<Eigen::Matrix4d> _tf, std::vector<double> _wt);
 


/**
 * @class WorldModel
 */
class WorldModel {

 private:
    std::vector<ARCamera> cameras;   // Cameras in world
    std::vector<ARMarker> markers;   // Markers in world
    int worldMarkerID;          // Keep track of the world marker (also in markers)
    
    
 public:
    // Constructor
    WorldModel( const std::vector<ARCamera> &_cameras, 
		const std::vector<ARMarker> &_markers );
    
    // Method to initialize the first camera to the origin marker
    bool setOrigin( const int &_cameraID, 
		    const int &_markerID, 
		    const Eigen::Matrix4d &_Tcam_marker );

    // Method to initialize a camera to the world frame
    bool initCamera( const int &_cam2_id, // Unknown camera 
		     const int &_cam1_id, // Known camera
		     const Eigen::Matrix4d &_Tc2_marker, 
		     const Eigen::Matrix4d &_Tc1_marker);
    
    // Method to set a marker's location in the world frame
    bool setMarkerLoc( const int &_cameraID, 
		       const int &_markerID, 
		       const Eigen::Matrix4d &_Tcam_marker );

    // Alternate setMarkerLoc that lets you set multiple poses rather than just the last
    bool setMarkerLoc( const std::vector<int> &_cameraIDs, 
		       const int &_markerID, 
		       const std::vector<Eigen::Matrix4d> &_Tcam_markers,
               const std::vector<double> &_confidences );

    // Same as previous but assumes same confidence
    bool setMarkerLoc( const std::vector<int> &_cameraIDs, 
		       const int &_markerID, 
		       const std::vector<Eigen::Matrix4d> &_Tcam_markers);

    // Method to get the location of a marker relative to the world marker
    Eigen::Vector3d getMarkerLoc( const int &_markerID );
    
    Eigen::Matrix4d getMarkerPose( const int &_markerID );
    
    
 private:
    // Method to get the position of a particular camera
    int getCamInd( const int &_ID );
    
    // Method to get the position of a particular marker
    int getMarkInd( const int &_ID );
};
