/**
 * @file
 * @brief WorldModel Implementations
 */
#include "WorldModel.h"
#include "ARCamera.h"
#include "ARMarker.h"
#include <math.h>

/************************************/

/**
 * @function getAverageAffine
 * @brief Returns weighted "average" rotation and position
 */
Eigen::Matrix4d getAverageAffine(std::vector<Eigen::Matrix4d> _tf, std::vector<double> _wt)
{
    // First get weighted average of x y and z
    double x_mean = 0, y_mean = 0, z_mean = 0, totalweight = 0;
    for (int i = 0; i < _tf.size(); i++)
    {
        double x = _tf[i](0, 3);
        double y = _tf[i](1, 3);
        double z = _tf[i](2, 3);
        double weight = _wt[i];

        // Add up weighted averages
        x_mean += weight * x;
        y_mean += weight * y;
        z_mean += weight * z;
        totalweight += weight;
    }

    // Normalize weight
    x_mean /= totalweight;
    y_mean /= totalweight;
    z_mean /= totalweight;

    // Now convert to quaternions
    std::vector<Eigen::Quaterniond> qs;
    for (int i = 0; i < _tf.size(); i++)
    {
        Eigen::Matrix3d mat;
        mat = _tf[i].block(0, 0, 3, 3);
        Eigen::Quaterniond q(mat);
        qs.push_back(q);
    }

    // Now find the median quaternion
    std::vector<double> distances(qs.size(), 0.0);
    for (int i = 0; i < qs.size(); i++)
    {
        double dist = 0;
        for (int j = 0; j < qs.size(); j++)
        {   
            if (j != i)
            {
                double dotprod = qs[i].dot(qs[j]);
                dotprod = abs(log(dotprod));
                dist += dotprod;
            }
        }
        // Update distance
        distances[i] = dist;
    }

    // Find min arg
    double bestdist = distances[0];
    int bestind = 0;
    for (int i = 1; i < distances.size(); i++)
    {
        if (distances[i] < bestdist)
        {
            bestdist = distances[i];
            bestind = i;
        }
    }

    // Convert best quat back to rotation
    Matrix3d rotation = qs[bestind].toRotationMatrix();

    // Now return affine with correct values
    Matrix4d affine;
    affine.block(0, 0, 3, 3) = rotation;
    affine(0, 3) = x_mean;
    affine(1, 3) = y_mean;
    affine(2, 3) = z_mean;
    affine.row(3) << 0,0,0,1;

    return affine;
}

/**
 * @function getAverageAffine
 * @brief Returns "average" rotation and position
 */
Eigen::Matrix4d getAverageAffine(std::vector<Eigen::Matrix4d> _tf)
{
    // Make them all equal weight
    std::vector<double> weights(_tf.size(), 1.0);
    
    // Call other method
    return getAverageAffine(_tf, weights);    
}

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
 * @brief Initialize camera to world transformation from a known cam and marker
          visible to both
 */
bool WorldModel::initCamera( const int &_cam2_id, 
			     const int &_cam1_id, 
			     const Eigen::Matrix4d &_Tc2_marker, 
			     const Eigen::Matrix4d &_Tc1_marker ) {


    // Make sure camera is initialized
    if (!cameras[getCamInd(_cam1_id)].isInitialized())
        return false;

    // Get world to camera transform for new camera
    Eigen::Matrix4d Tworld_cam2 = cameras[getCamInd( _cam1_id)].getWorld2Cam() 
                                        * _Tc1_marker.inverse() * _Tc2_marker;

    // Initialize camera with that transform
    bool success = cameras[getCamInd(_cam2_id)].initializeCamera( Tworld_cam2 );

    return success;
}

/**
 * @function setMarkerPose
 * @brief Set marker (with ID _markerID) pose
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
    bool success = markers[getMarkInd(_markerID)].setMarker(Tworld_marker);

    return success;
}

/**
 * @function setMarkerPose
 * @brief Set marker (with ID _markerID) pose
 */
bool WorldModel::setMarkerLoc( const std::vector<int> &_cameraIDs, 
		       const int &_markerID, 
		       const std::vector<Eigen::Matrix4d> &_Tcam_markers,
               const std::vector<double> &_confidences ) {

    // Check that cameras are initialized
    for (int i = 0; i < _cameraIDs.size(); i++)
    {
        int _cameraID = _cameraIDs[i];
        if (!cameras[getCamInd(_cameraID)].isInitialized())
            return false;
    }

    // Get world to marker matrices
    std::vector<Eigen::Matrix4d> world_mats;
    for (int i = 0; i < _Tcam_markers.size(); i++)
    {
        Eigen::Matrix4d _Tcam_marker = _Tcam_markers[i];
        int _cameraID = _cameraIDs[i];
        Eigen::Matrix4d Tworld_marker = cameras[getCamInd(_cameraID)].getCam2World() * _Tcam_marker;
        world_mats.push_back(Tworld_marker);
    }

    // Now get the "average"
    Eigen::Matrix4d avgmat = getAverageAffine(world_mats, _confidences);

    // Set marker
    bool success = markers[getMarkInd(_markerID)].setMarker(avgmat);

    return success;
}

bool WorldModel::setMarkerLoc( const std::vector<int> &_cameraIDs, 
		       const int &_markerID, 
		       const std::vector<Eigen::Matrix4d> &_Tcam_markers) {
    // Make them all equal weight
    std::vector<double> weights(_Tcam_markers.size(), 1.0);
    return setMarkerLoc(_cameraIDs, _markerID, _Tcam_markers, weights);
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

