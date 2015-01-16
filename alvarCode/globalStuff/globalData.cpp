/**
 * @file globalData.cpp
 */
#include <stdio.h>
#include <iostream>
#include "globalData.h"

/** Global */
int NUM_CAMERAS;
int NUM_OBJECTS;

std::vector<std::string> OBJECT_NAME;
std::vector<int> MARKER_ID;
std::vector<double> MARKER_SIZE;

std::vector<std::string> CAM_CALIB_NAME;
std::vector<std::string> CAM_CHANNEL_NAME;

std::vector<ObjectData_t> gObjects;

std::string PERCEPTION_CHANNEL;
std::string DEBUG_CHANNEL;

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

    // Channel names for cameras
    CAM_CHANNEL_NAME.resize(0);
    for( int i = 0; i < NUM_CAMERAS; ++i ) 
    {
        std::string name = config["cam_channel_name"].get(i, "chan").asString();
        CAM_CHANNEL_NAME.push_back(name);
    }
  
    // Objects 
    OBJECT_NAME.resize(0);
    for (int i = 0; i < NUM_OBJECTS; i++)
    {
        std::string name = config["object_name"].get(i, "obj").asString();
        OBJECT_NAME.push_back(name);
    }

    if( OBJECT_NAME.size() != NUM_OBJECTS ) 
    {
        std::cout << "[X] Error initializing OBJECT_NAMES!"<< std::endl;
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

    // Marker Sizes
    MARKER_SIZE.resize(0);
    for (int i = 0; i < NUM_OBJECTS; i++)
    {
        double size = config["marker_size"].get(i, 0).asDouble();
        MARKER_SIZE.push_back(size);
    } 

    if( MARKER_SIZE.size() != NUM_OBJECTS ) 
    {
        std::cout << "[X] Error initializing MARKER_SIZE!"<< std::endl;
    }

    /**< Initialize objects for each marker */
    for( int i = 0; i < NUM_OBJECTS; ++i ) 
    {
        ObjectData_t obj;
        char name[50];
        sprintf( obj.obj_name, "%s", OBJECT_NAME[i].c_str() ); 
        //obj.obj_name = name;
        obj.marker_id  = MARKER_ID[i];
        obj.visible = -1;
        obj.width = MARKER_SIZE[i];
        obj.center[0] = 0; obj.center[1] = 0;
        obj.cam_id = -1;
    
        // Add object
        gObjects.push_back(obj);
    }
}
