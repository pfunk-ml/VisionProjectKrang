/**
 * @file globalInfo.h
 */

#pragma once

#include <vector>
#include <string>
#include "Object.h"

#define NUM_CAMERAS 4
#define NUM_OBJECTS 3

extern std::vector<std::string> OBJECT_NAME;
extern std::vector<int> MARKER_ID;
extern std::vector<int> MARKER_SIZE;

extern std::vector<std::string> CAM_CALIB_NAME;
extern std::vector<std::string> CAM_CHANNEL_NAME;

extern ObjectData_t gObjects[NUM_OBJECTS];



#define PERCEPTION_CHANNEL "krang_vision"
#define DEBUG_CHANNEL "debug_channel"

/** Set values to global data */
void setGlobalData();
