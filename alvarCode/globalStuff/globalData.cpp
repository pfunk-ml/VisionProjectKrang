/**
 * @file globalData.cpp
 */
#include <stdio.h>
#include <iostream>
#include "globalData.h"

/** Global */
int NUM_CAMERAS;
int NUM_OBJECTS;

//std::vector<double> MARKER_SIZE;

std::vector<std::string> CAM_CALIB_NAME;
std::vector<std::string> CAM_CHANNEL_NAME;

//std::string PERCEPTION_CHANNEL;
std::string VISION_OBJ_POSES_CHANNEL;   // channel to write object poses
std::string VISION_KRANG_POSE_CHANNEL;  // channel to write krang pose
std::string DEBUG_CHANNEL;

ConfParams_t gConfParams;
Transforms_t gTransforms;

void setGlobalTransforms(Json::Value config) 
{
    Eigen::Matrix4d T_global2world;
    for (int r = 0; r < 4; r++)
    {
        for (int c = 0; c < 4; c++)
        {
            T_global2world(r, c) = config["T_global2world"].get(r, 0).get(c, 0).asDouble();
        }
    }    
    
    gTransforms.T_cam2world.resize(NUM_CAMERAS);
    for (int i = 0; i < NUM_CAMERAS; i++)
    {
        Eigen::Matrix4d T_global2cam;

        // Get original matrix
        for (int r = 0; r < 4; r++)
        {
            for (int c = 0; c < 4; c++)
            {
                T_global2cam(r, c) = config["T_global2cam"].get(i, 0).get(r, 0).get(c, 0).asDouble();
            }
        }   

        // Pass to mm the translation
        T_global2cam(0,3) = T_global2cam(0,3) / 100.0;
        T_global2cam(1,3) = T_global2cam(1,3) / 100.0;
        T_global2cam(2,3) = T_global2cam(2,3) / 100.0;
    
        gTransforms.T_cam2world[i] = T_global2world * T_global2cam.inverse();
    }

    Eigen::Matrix4d Ttemp = Eigen::Matrix4d::Identity();

    // Transformation from marker to sprite
    gTransforms.T_sprite.resize(NUM_OBJECTS);
    for (int i = 0; i < NUM_OBJECTS; i++)
    {
        Eigen::Matrix4d Ttemp;

        // Get original matrix
        for (int r = 0; r < 4; r++)
        {
            for (int c = 0; c < 4; c++)
            {
                Ttemp(r, c) = config["T_sprite"].get(i, 0).get(r, 0).get(c, 0).asDouble();
            }
        }

        // Set sprite transform
        gTransforms.T_sprite[i] = Ttemp;
    }
}

/**
 * @function setGlobalData
 * @brief Set global information 
 */
void setGlobalData(Json::Value config) 
{
    // Numbers of things
    NUM_CAMERAS = config.get("num_cameras", 0).asInt();
    NUM_OBJECTS = config.get("num_object", 0).asInt();

    // Channel names
    // PERCEPTION_CHANNEL = config.get("perception_channel", "perc").asString();
    VISION_OBJ_POSES_CHANNEL = config.get("vision_obj_poses_channel", "perc").asString();
    VISION_KRANG_POSE_CHANNEL = config.get("vision_krang_pose_channel", "perc").asString();
    DEBUG_CHANNEL = config.get("debug_channel", "debug").asString();

    // Calibration file
    CAM_CALIB_NAME.resize(0);
    for( int i = 0; i < NUM_CAMERAS; ++i ) 
    {
        std::string name = config["cam_calib_name"].get(i, "calib").asString();
        CAM_CALIB_NAME.push_back(name);
    }

    // Channel names for cameras
    CAM_CHANNEL_NAME.resize(0);
    for( int i = 0; i < NUM_CAMERAS; ++i ) 
    {
        std::string name = config["cam_channel_name"].get(i, "chan").asString();
        CAM_CHANNEL_NAME.push_back(name);
    }

    /* Initialize objects for each object */
    for( int i = 0; i < NUM_OBJECTS; ++i ) {
        gConfParams.objectNames.push_back(config["object_name"]
                                                .get(i, "obj").asString());
        gConfParams.markerIDs.push_back(config["marker_id"].get(i, 0).asInt());
    }

    gConfParams.markerSize = config["marker_size"].asDouble();
    gConfParams.width = config["width"].asInt();
    gConfParams.height = config["height"].asInt();

    setGlobalTransforms(config);
}
