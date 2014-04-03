/**
 * @file globalInfo.h
 */

#pragma once

#include "Object.h"

#define OBJ0_PATT_NAME "Data/4x4_23.patt"
#define OBJ1_PATT_NAME "Data/4x4_87.patt"
#define OBJ2_PATT_NAME "Data/patt.sample1"
#define OBJ3_PATT_NAME "Data/patt.sample2"

#define OBJ0_MODEL_ID 0
#define OBJ1_MODEL_ID 1
#define OBJ2_MODEL_ID 2
#define OBJ3_MODEL_ID 3

#define OBJ0_SIZE 160.0
#define OBJ1_SIZE 160.0
#define OBJ2_SIZE 160.0
#define OBJ3_SIZE 160.0

#define CAM0_CHANNEL "cam0_channel"
#define CAM1_CHANNEL "cam1_channel"
#define CAM2_CHANNEL "cam2_channel"
#define CAM3_CHANNEL "cam3_channel"

#define NUM_CAMERAS 1

#define NUM_OBJECTS 4

extern ObjectData_t gObjects[4];

