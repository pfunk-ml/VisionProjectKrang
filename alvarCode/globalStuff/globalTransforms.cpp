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
void setGlobalTransforms(Json::Value config) 
{
    for (int r = 0; r < 4; r++)
    {
        for (int c = 0; c < 4; c++)
        {
            gTworld_origin(r, c) = config["gTworld_origin"].get(r, 0).get(c, 0).asDouble();
        }
    }    
    
    gTworld_cam.resize(NUM_CAMERAS);
    for (int i = 0; i < NUM_CAMERAS; i++)
    {
        Eigen::Matrix4d Ttemp;

        // Get original matrix
        for (int r = 0; r < 4; r++)
        {
            for (int c = 0; c < 4; c++)
            {
                Ttemp(r, c) = config["gTworld_cam"].get(i, 0).get(r, 0).get(c, 0).asDouble();
            }
        }   

        // Pass to mm the translation
        Ttemp(0,3) = Ttemp(0,3) / 100.0;
        Ttemp(1,3) = Ttemp(1,3) / 100.0;
        Ttemp(2,3) = Ttemp(2,3) / 100.0;
    
        gTworld_cam[i] = gTworld_origin*Ttemp.inverse(); // World To Marker 4 - Marker 4 To Camera
    }

    Eigen::Matrix4d Ttemp = Eigen::Matrix4d::Identity();
    
    gMarker_Origin_ID = config.get("marker_origin_id", 0).asInt();
    gCamera_Origin_ID = config.get("camera_origin_id", 0).asInt();

    // Transformation from marker to sprite
    gTmarker_sprite.resize(NUM_OBJECTS);
    for (int i = 0; i < NUM_OBJECTS; i++)
    {
        Eigen::Matrix4d Ttemp;

        // Get original matrix
        for (int r = 0; r < 4; r++)
        {
            for (int c = 0; c < 4; c++)
            {
                Ttemp(r, c) = config["gTmarker_sprite"].get(i, 0).get(r, 0).get(c, 0).asDouble();
            }
        }

        // Set sprite transform
        gTmarker_sprite[i] = Ttemp;
    }
}
