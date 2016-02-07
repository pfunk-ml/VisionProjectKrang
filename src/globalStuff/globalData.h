/**
 * @file globalInfo.h
 */

#pragma once

#include <vector>
#include <string>
#include "Object.h"
#include "json/json.h"
 #include <Eigen/Geometry>

 #include <unistd.h>

extern std::vector<std::string> CAM_CALIB_NAME;
extern std::vector<std::string> CAM_CHANNEL_NAME;

extern std::string VISION_OBJ_POSES_CHANNEL;   // channel to write object poses
extern std::string VISION_KRANG_POSE_CHANNEL;  // channel to write krang pose

//extern std::string PERCEPTION_CHANNEL;
extern std::string DEBUG_CHANNEL;

// structure for configuration parameters 
typedef struct {
    float markerSize;   // length of marker edge in cms
    float calibrationMarkerSize; // length of calibration marker size in cms
    
    std::vector<int> markerIDs;  // ID of AR markers

    // Camera resolution parameters
    int width;
    int height;
    
    std::vector<std::string> objectNames;

    int numObjects; // Number of objects in the scene
    int numCameras; // Number of cameras
} ConfParams_t;

// Transforms in the environment
typedef struct {
    std::vector<Eigen::Matrix4d> T_cam2world; // transform
    std::vector<Eigen::Matrix4d> T_sprite;  // object pose in marker frame
} Transforms_t;

extern ConfParams_t gConfParams;
extern Transforms_t gTransforms;

/** Set values to global data */
void setGlobalData(Json::Value config);

/* Returns the index of marker ID in gConfParams.markerIDs.
 * Returns -1 if markerID not found. */
int GlobalData_getIndex(int markerID);
