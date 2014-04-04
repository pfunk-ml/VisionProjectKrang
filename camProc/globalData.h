/**
 * @file globalInfo.h
 */

#pragma once

#include "Object.h"

#define ARTOOLKIT_DEFAULT_CONFIG "v4l2src device=/dev/video%d use-fixed-fps=false ! ffmpegcolorspace ! capsfilter caps=video/x-raw-rgb,bpp=24,width=640,height=480 ! identity name=artoolkit ! fakesink"

#define OBJ0_PATT_NAME "Data/4x4_18.patt"
#define OBJ1_PATT_NAME "Data/4x4_87.patt"
#define OBJ2_PATT_NAME "Data/4x4_23.patt"
#define OBJ3_PATT_NAME "Data/4x4_83.patt"
#define OBJ4_PATT_NAME "Data/4x4_1.patt"
#define OBJ5_PATT_NAME "Data/4x4_16.patt"
#define OBJ6_PATT_NAME "Data/4x4_3.patt"

#define OBJ0_MODEL_ID 0
#define OBJ1_MODEL_ID 1
#define OBJ2_MODEL_ID 2
#define OBJ3_MODEL_ID 3
#define OBJ4_MODEL_ID 4
#define OBJ5_MODEL_ID 5
#define OBJ6_MODEL_ID 6


#define OBJ0_SIZE 160.0
#define OBJ1_SIZE 160.0
#define OBJ2_SIZE 160.0
#define OBJ3_SIZE 160.0
#define OBJ4_SIZE 160.0
#define OBJ5_SIZE 160.0
#define OBJ6_SIZE 160.0


#define CAM0_CHANNEL "cam0_channel"
#define CAM1_CHANNEL "cam1_channel"
#define CAM2_CHANNEL "cam2_channel"
#define CAM3_CHANNEL "cam3_channel"

#define NUM_CAMERAS 1

#define NUM_OBJECTS 7

extern ObjectData_t gObjects[7];

#define PERCEPTION_CHANNEL "perception_channel"
