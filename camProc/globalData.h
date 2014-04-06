/**
 * @file globalInfo.h
 */

#pragma once

#include "Object.h"

#define ARTOOLKIT_DEFAULT_CONFIG "v4l2src device=/dev/video%d use-fixed-fps=false ! ffmpegcolorspace ! capsfilter caps=video/x-raw-rgb,bpp=24,width=640,height=480 ! identity name=artoolkit ! fakesink"

/*
#define OBJ0_PATT_NAME "Data/4x4_18.patt"
#define OBJ1_PATT_NAME "Data/4x4_87.patt"
#define OBJ2_PATT_NAME "Data/4x4_23.patt"
#define OBJ3_PATT_NAME "Data/4x4_83.patt"
#define OBJ4_PATT_NAME "Data/4x4_1.patt"
#define OBJ5_PATT_NAME "Data/4x4_16.patt"
#define OBJ6_PATT_NAME "Data/4x4_3.patt"

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

#define NUM_CAMERAS 4

#define NUM_OBJECTS 7

#define PERCEPTION_CHANNEL "perception_channel"

*/

#define OBJ0_PATT_NAME "Data/4x4_1.patt"
#define OBJ1_PATT_NAME "Data/4x4_10.patt"
#define OBJ2_PATT_NAME "Data/4x4_20.patt"
#define OBJ3_PATT_NAME "Data/4x4_30.patt"
#define OBJ4_PATT_NAME "Data/4x4_40.patt"
#define OBJ5_PATT_NAME "Data/4x4_50.patt"

#define OBJ0_MODEL_ID 0
#define OBJ1_MODEL_ID 1
#define OBJ2_MODEL_ID 2
#define OBJ3_MODEL_ID 3
#define OBJ4_MODEL_ID 4
#define OBJ5_MODEL_ID 5

#define OBJ0_SIZE 108.0
#define OBJ1_SIZE 108.0
#define OBJ2_SIZE 108.0
#define OBJ3_SIZE 108.0
#define OBJ4_SIZE 108.0
#define OBJ5_SIZE 108.0

#define CAM1_CHANNEL "cam1_channel"
#define CAM2_CHANNEL "cam2_channel"
#define CAM3_CHANNEL "cam3_channel"
#define CAM4_CHANNEL "cam4_channel"

#define CAM1_CALIB_NAME "Data/cam1_calib.yaml"
#define CAM2_CALIB_NAME "Data/cam1_calib.yaml"
#define CAM3_CALIB_NAME "Data/cam1_calib.yaml"
#define CAM4_CALIB_NAME "Data/cam1_calib.yaml"

#define NUM_CAMERAS 3

#define NUM_OBJECTS 6

extern ObjectData_t gObjects[NUM_OBJECTS];

#define PERCEPTION_CHANNEL "perception_channel"
