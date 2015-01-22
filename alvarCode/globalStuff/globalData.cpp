/**
 * @file globalData.cpp
 */
#include <stdio.h>
#include <iostream>
#include "globalData.h"

/** Global */
int NUM_CAMERAS;
int NUM_OBJECTS;

std::vector<int> MARKER_ID;
std::vector<double> MARKER_SIZE;

std::vector<std::string> CAM_CALIB_NAME;
std::vector<std::string> CAM_CHANNEL_NAME;

std::vector<ObjectData_t> gObjects;

std::string PERCEPTION_CHANNEL;
std::string DEBUG_CHANNEL;

ConfParams_t gConfParams;

void setGlobalTransforms(Json::Value config) 
{
    Eigen::Matrix4d T_global2world;
    for (int r = 0; r < 4; r++)
    {
        for (int c = 0; c < 4; c++)
        {
            T_global2world(r, c) = config["gT_global2world"].get(r, 0).get(c, 0).asDouble();
        }
    }    
    
    gTworld_cam.resize(NUM_CAMERAS);
    for (int i = 0; i < NUM_CAMERAS; i++)
    {
        Eigen::Matrix4d T_global2cam;

        // Get original matrix
        for (int r = 0; r < 4; r++)
        {
            for (int c = 0; c < 4; c++)
            {
                T_global2cam(r, c) = config["gT_global2cam"].get(i, 0).get(r, 0).get(c, 0).asDouble();
            }
        }   

        // Pass to mm the translation
        T_global2cam(0,3) = T_global2cam(0,3) / 100.0;
        T_global2cam(1,3) = T_global2cam(1,3) / 100.0;
        T_global2cam(2,3) = T_global2cam(2,3) / 100.0;
    
        gTworld_cam[i] = T_global2world * T_global2cam.inverse();
    }

    Eigen::Matrix4d Ttemp = Eigen::Matrix4d::Identity();

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
    PERCEPTION_CHANNEL = config.get("perception_channel", "perc").asString();
    DEBUG_CHANNEL = config.get("debug_channel", "debug").asString();

    // Calibration file
    CAM_CALIB_NAME.resize(0);
    for( int i = 0; i < NUM_CAMERAS; ++i ) 
    {
        std::string name = config["cam_calib_name"].get(i, "calib").asString();
        CAM_CALIB_NAME.push_back(name);
    }
    
    // CAM_CALIB_NAME = config["cam_calib_name"].asString();

    // Channel names for cameras
    CAM_CHANNEL_NAME.resize(0);
    for( int i = 0; i < NUM_CAMERAS; ++i ) 
    {
        std::string name = config["cam_channel_name"].get(i, "chan").asString();
        CAM_CHANNEL_NAME.push_back(name);
    }
  
    // IDs
    MARKER_ID.resize(0);
    for (int i = 0; i < NUM_OBJECTS; i++)
    {
        int id = config["marker_id"].get(i, 0).asInt();
        MARKER_ID.push_back(id);
    }   

    if( MARKER_ID.size() != NUM_OBJECTS ) 
    {
        std::cout << "[X] Error initializing MARKER_ID!"<< std::endl;
    }

    /**< Initialize objects for each marker */
    for( int i = 0; i < NUM_OBJECTS; ++i ) 
    {
        ObjectData_t obj;

        sprintf( obj.obj_name, 
                "%s", config["object_name"].get(i, "obj").asString().c_str());

        obj.marker_id  = MARKER_ID[i];
        obj.visible = -1;
        //obj.width = MARKER_SIZE[i];
        obj.center[0] = 0; obj.center[1] = 0;
        obj.cam_id = -1;
    
        // Add object
        gObjects.push_back(obj);
    }

    gConfParams.markerSize = config["marker_size"].asDouble();
    gConfParams.width = config["width"].asInt();
    gConfParams.height = config["height"].asInt();

    setGlobalTransforms(config);
}
