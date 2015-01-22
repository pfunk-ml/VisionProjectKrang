/**
 * @file globalInfo.h
 */

#pragma once

#include <vector>
#include <string>
#include "Object.h"
#include "json/json.h"

extern int NUM_CAMERAS;
extern int NUM_OBJECTS;

extern std::vector<int> MARKER_ID;
extern std::vector<std::string> CAM_CALIB_NAME;
extern std::vector<std::string> CAM_CHANNEL_NAME;

extern std::vector<ObjectData_t> gObjects;

extern std::string PERCEPTION_CHANNEL;
extern std::string DEBUG_CHANNEL;

extern std::vector<Eigen::Matrix4d> gTworld_cam;
extern std::vector<Eigen::Matrix4d> gTmarker_sprite;

// structure for configuration parameters 
typedef struct {
    float markerSize;   // length of marker edge in cms
    int width;          // width of camera frame???
    int height;         // height of camera height??
} ConfParams_t;

extern ConfParams_t gConfParams;

/** Set values to global data */
void setGlobalData(Json::Value config);
