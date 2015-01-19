/**
 * @function globalTransform.cpp
 */
#include "globalTransforms.h"
#include "globalData.h"
#include <Eigen/Geometry>

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
    Eigen::Matrix4d Ttemp = Eigen::Matrix4d::Identity();

    gTworld_cam.resize(NUM_CAMERAS);
    for( int i = 1; i <= NUM_CAMERAS; ++i ) {
	gTworld_cam[i-1] = Eigen::Matrix4d::Identity();

        switch(i) {
        
        case 1: {
        Ttemp << -0.0393178, -0.983661, 0.175683, 617.585, 
-0.923571, -0.0313356, -0.382145, 320.315, 
0.381406, -0.177281, -0.907249, 2788.08, 

0,0,0,1; 
        } break;
        case 2: {
        Ttemp << -0.0209255, 0.982799, 0.183487, 405.668, 
0.979953, -0.0162071, 0.198567, 937.231,
0.198125, 0.183963, -0.962759, 2787.7,
0,0,0,1;
       } break;

       case 3: {
       Ttemp << -0.0640467, 0.992156, 0.107349, 588.075, 
0.977493, 0.0840373, -0.193507, -471.83,
-0.201011, 0.0925395, -0.975208, 2681.29, 
0,0,0,1;
       } break;

       case 4: {
        Ttemp << 0.0613112, -0.99745, 0.0365191, 811.238, 
-0.997808, -0.0603376, 0.0271917, 914.69, 
-0.0249189, -0.0381062, -0.998963, 2436.59, 
0,0,0,1;

       } break;
       } // end switch

	// Pass to mm the translation
        Ttemp(0,3) = Ttemp(0,3) / 1000.0;
        Ttemp(1,3) = Ttemp(1,3) / 1000.0;
        Ttemp(2,3) = Ttemp(2,3) / 1000.0;
    
        gTworld_cam[i-1] = Ttemp.inverse();

    }

    
    gMarker_Origin_ID = 0;
    gCamera_Origin_ID = 0;
}
