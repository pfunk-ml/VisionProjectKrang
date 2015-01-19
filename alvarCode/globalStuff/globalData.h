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

extern std::vector<std::string> OBJECT_NAME;
extern std::vector<int> MARKER_ID;
// extern std::vector<double> MARKER_SIZE;

extern std::string CAM_CALIB_NAME;
extern std::vector<std::string> CAM_CHANNEL_NAME;

extern std::vector<ObjectData_t> gObjects;

extern std::string PERCEPTION_CHANNEL;
extern std::string DEBUG_CHANNEL;

/** Set values to global data */
void setGlobalData(Json::Value config);
