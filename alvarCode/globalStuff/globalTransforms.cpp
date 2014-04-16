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
std::vector<Eigen::Matrix4d> gTmarker_sprite;

/**
 * @function setHardCodedValues
 * @brief Set hard-coded values calculated offline
 */
void setGlobalTransforms() {
    
    gTworld_origin = Eigen::Matrix4d::Identity();
    gTworld_origin(0,0) = 0; gTworld_origin(0,1) = 1; gTworld_origin(0,2) = 0; gTworld_origin(0,3) = 2.7686;
    gTworld_origin(1,0) = -1; gTworld_origin(1,1) = 0; gTworld_origin(1,2) = 0; gTworld_origin(1,3) = 2.3114;
    gTworld_origin(2,0) = 0; gTworld_origin(2,1) = 0; gTworld_origin(2,2) = 1; gTworld_origin(2,3) = 0;
    gTworld_origin(3,0) = 0; gTworld_origin(3,1) = 0; gTworld_origin(3,2) = 0; gTworld_origin(3,3) = 1;

    Eigen::Matrix4d Ttemp = Eigen::Matrix4d::Identity();

    gTworld_cam.resize(NUM_CAMERAS);
    for( int i = 0; i < NUM_CAMERAS; ++i ) {
	gTworld_cam[i] = Eigen::Matrix4d::Identity();

        switch(i) {
        
        case 0: {
        Ttemp << -0.998209, -0.0232161, -0.0551266, 89.288, 
-0.0219696, 0.999491, -0.0231106, 101.508, 
0.0556351, -0.0218581, -0.998212, 296.085, 
0, 0, 0, 1;  
        } break;
        case 1: {
        Ttemp << 0.981348, 0.00231326, 0.192223, 74.5487, 
0.000432792, -0.999952, 0.00982415, -64.8524, 
0.192237, -0.00955773, -0.981302, 273.846, 
0, 0, 0, 1 ;
       } break;

       case 2: {
       Ttemp << -0.999564, 0.0238067, 0.017449, 76.459, 
0.0123426, 0.874116, -0.48556, 43.9597, 
-0.026812, -0.485134, -0.874029, 273.28, 
0, 0, 0, 1;
       } break;

       case 3: {
        Ttemp << 0.980693, -0.0429465, 0.190779, 62.4286, 
-0.0417944, -0.999076, -0.0100601, 99.9049, 
0.191035, 0.00189234, -0.981581, 318.945, 
0, 0, 0, 1;

       } break;
       } // end switch

	// Pass to mm the translation
        Ttemp(0,3) = Ttemp(0,3) / 100.0;
        Ttemp(1,3) = Ttemp(1,3) / 100.0;
        Ttemp(2,3) = Ttemp(2,3) / 100.0;
    
        gTworld_cam[i] = gTworld_origin*Ttemp.inverse(); // World To Marker 4 - Marker 4 To Camera

    }

    
    gMarker_Origin_ID = 4;
    gCamera_Origin_ID = 0;

   // Transformation from marker to sprite
   gTmarker_sprite.resize(NUM_OBJECTS);

    // Ttable [OBJECT 0]
   Eigen::Matrix4d Trobot = Eigen::Matrix4d::Identity();
   gTmarker_sprite[0] = Trobot;

    // Ttable [OBJECT 1]
   Eigen::Matrix4d Ttable;
   Ttable << 1,0,0,-0.4572, 0,1,0,-0.4572, 0,0,1,0, 0,0,0,1;
   gTmarker_sprite[1] = Ttable;

   // Ttable [OBJECT 2]
   Eigen::Matrix4d Tchair;
   Tchair << 0, 1, 0, -0.279, -1, 0, 0, 0.2286, 0,0,1,0, 0,0,0,1;
   gTmarker_sprite[2] = Tchair;


}
