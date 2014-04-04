/**
 * @file
 * @brief WorldModel Implementations
 */
#include "WorldModel.h"
#include "ARCamera.h"
#include "ARMarker.h"


/**
 * @function getMatAsDouble
 * @brief Set an Eigen 4x4 matrix to double[3][4]
 */
double** getMatAsDouble( const Eigen::Matrix4d &_mat ) {

    // Create new double array [3][4]
    double** ret = new double*[3];
    for (int h = 0; h < 3; h++) {
        ret[h] = new double[4];
    }

    // Now put in matrix 4d stuff
    for( int i = 0; i < 3; i++ ) {
	for( int j = 0; j < 4; ++j ) { 
	    ret[i][j] = _mat(i,j);
	}
    }

    return ret;
}

/**
 * @function getDOubleArrAsMat
 * @brief Gets Eigen::Matrix4d from _trans double array
 */
Eigen::Matrix4d getDoubleArrAsMat( double _trans[3][4] ) {

    // Now put in matrix 4d stuff
    Eigen::Matrix4d mat;

    for( int i = 0; i < 3; ++i ) {
	for( int j = 0; j < 4; ++j ) {
	    mat(i,j) = _trans[i][j];
	}
    }

    mat.row(3) << 0,0,0,1;

    return mat;
}

/**
 * @function getXYangTriple
 */
void getXYangTriple( const Eigen::Matrix4d &_Tf, 
		     double& _x, 
		     double& _y, 
		     double& _ang ) {
    
    // Get x and y from rotation matrix
    _x = _Tf(0,3);
    _y = _Tf(1,3);

    // Get Euler angles
    Eigen::Matrix3d transformMat = _Tf.block(0, 0, 3, 3);
    Eigen::Vector3d ea = transformMat.eulerAngles(0, 1, 2);
    
    // Angle should be the Rz angle
    _ang = (double)ea[2];
}


/************************************/


/**
 * @function WorldModel
 * @brief Constructor
 */
WorldModel::WorldModel( const std::vector<ARCamera> &_cameras, 
			const std::vector<ARMarker> &_markers ) {

    cameras = _cameras;
    markers = _markers;
}

/**
 * @function setOrigin
 * @brief Set marker ID that shows origin and camera ID that visualizes it
 */
bool WorldModel::setOrigin( const int &_cameraID, 
			    const int &_markerID, 
			    const Eigen::Matrix4d &_transform ) {

    // Initialize camera with World to Camera transform (given in input)
    bool success = cameras[getCamInd(_cameraID)].initializeCamera(_transform);
    
    // Set marker as "world" marker
    worldMarkerID = _markerID;
    
    return success;
}

/**
 * @function initCamera
 * @brief Initialize camera world transformation from a known cam and marker visible to both
 */
bool WorldModel::initCamera( const int &_cam2_id, 
			     const int &_cam1_id, 
			     const Eigen::Matrix4d &_Tc2_marker, 
			     const Eigen::Matrix4d &_Tc1_marker ) {


    // Make sure camera is initialized
    if (!cameras[getCamInd(_cam1_id)].isInitialized())
        return false;

    // Get world to camera transform for new camera
    Eigen::Matrix4d Tworld_cam2 = cameras[getCamInd( _cam1_id)].getWorld2Cam() * _Tc1_marker.inverse() * _Tc2_marker;

    // Initialize camera with that transform
    bool success = cameras[getCamInd(_cam2_id)].initializeCamera( Tworld_cam2 );

    return success;
}

/**
 * @function getMarkerPose
 * @brief Retrieve marker (with ID _markerID) pose
 */
bool WorldModel::setMarkerLoc( const int &_cameraID, 
			       const int &_markerID, 
			       const Eigen::Matrix4d &_Tcam_marker ) {

    // Check that camera is initialized
    if (!cameras[getCamInd(_cameraID)].isInitialized())
        return false;

    // Get world to marker matrix
    Eigen::Matrix4d Tworld_marker = cameras[getCamInd(_cameraID)].getCam2World() * _Tcam_marker;

    // Set marker
    bool success = markers[getMarkInd(_markerID)].setMarker( Tworld_marker );

    return success;
}

/**
 * @function getMarkerLoc
 * @brief Retrieve marker (with ID _markerID) 3D position
 */
Eigen::Vector3d WorldModel::getMarkerLoc( const int &_markerID ) {
    return markers[this->getMarkInd(_markerID)].getLocInWorld();
}

/**
 * @function getMarkerPose
 * @brief Retrieve marker (with ID _markerID) pose
 */
Eigen::Matrix4d WorldModel::getMarkerPose( const int &_markerID) {
    return markers[this->getMarkInd(_markerID)].getMarkerPose();
} 

/**
 * @function getCamInd
 * @brief Get camera index in the cameras vector in World
 */
int WorldModel::getCamInd( const int &_ID ) {

    for (int i = 0; i < cameras.size(); i++) {
        if (cameras[i].getID() == _ID)
            return i;
    }
    
    // Return -1 if not in array
    return -1;
}


/**
 * @function getMarkInd
 * @brief Get marker index in the markers vector in world
 */
int WorldModel::getMarkInd( const int &_ID) {

    for (int i = 0; i < markers.size(); i++) {
        if (markers[i].getID() == _ID)
            return i;
    }
    
    // Return -1 if not in array
    return -1;
}

