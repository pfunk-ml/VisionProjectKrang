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
        Ttemp <<-0.979466, 0.198151, -0.0371784,-355.565, 
0.139548, 0.533241, -0.834374, -60.1962, 
-0.145507, -0.822429, -0.549943, 1477.6,
0,0,0,1; 
        } break;
        case 2: {
        Ttemp << -0.224655, 0.973019, -0.0525722, -334.696, 
0.766954, 0.143282, -0.625501, 560.75, 
-0.601092, -0.180843, -0.77845, 1885.78,
0,0,0,1;
       } break;

       case 3: {
       Ttemp << -0.897004, 0.440907, -0.0313868, -424.817, 
0.263074, 0.475451, -0.839487, -35.8484, 
-0.355213, -0.76128, -0.542473, 1329.19,
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
