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
    
    /* Method to initialize the first camera to the origin marker
        _cameraID   : [IN] ID of the camera
        _markerID   : [IN] ID of the marker
        _Tcam_marker: [IN] Pose of marker in camera frame */
    bool setOrigin( const int &_cameraID, 
		    const int &_markerID, 
		    const Eigen::Matrix4d &_Tcam_marker );

    // Method to initialize a camera to the world frame
    bool initCamera( const int &_cam2_id, // Unknown camera 
		     const int &_cam1_id, // Known camera
		     const Eigen::Matrix4d &_Tc2_marker, 
		     const Eigen::Matrix4d &_Tc1_marker);
    
    /* Method to set marker's pose in the world frame
        _cameraID   : [IN] ID of the camera
        _markerID   : [IN] ID of the marker
        _Tcam_marker: [IN] 4x4 matrix. Pose of marker in camera frame */
    bool setMarkerLoc( const int &_cameraID, 
		       const int &_markerID, 
		       const Eigen::Matrix4d &_Tcam_marker );

    /* Method to set poses of a marker visible in multiple cameras, in world frame.
        _cameraID     : [IN] the array of camera IDs
        _markerID     : [IN] ID of the marker
        _Tcam_markers : [IN] the array of pose of marker in each camera
        _confidence   : ??? */
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
    
    /* Returns marker pose in world frame
        _markerID : [IN] marker ID
        return val: 4x4 matrix. Marker pose in world frame. */
    Eigen::Matrix4d getMarkerPose( const int &_markerID );
    
    
 private:
    // Method to get the position of a particular camera
    int getCamInd( const int &_ID );
    
    // Method to get the position of a particular marker
    int getMarkInd( const int &_ID );
};
