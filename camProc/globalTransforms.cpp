/**
 * @function globalTransform.cpp
 */
#include "globalTransforms.h"
#include "globalData.h"


/** Global */
Eigen::Matrix4d gTworld_origin;
int gMarker_Origin_ID;
int gCamera_Origin_ID;
std::vector<Eigen::Matrix4d> gTworld_cam;

/**
 * @function setHardCodedValues
 * @brief Set hard-coded values calculated offline
 */
void setHardCodedValues() {
    
    gTworld_origin = Eigen::Matrix4d::Identity();


    gTworld_cam.resize(NUM_CAMERAS);
    for( int i = 0; i < NUM_CAMERAS; ++i ) {
	gTworld_cam[i] = Eigen::Matrix4d::Identity();
    }

    gMarker_Origin_ID = 0;
    gCamera_Origin_ID = 0;
}
