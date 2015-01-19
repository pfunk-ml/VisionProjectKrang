/**
 * @file Object.h
 */
#pragma once

#include "worldModel/WorldModel.h"

/**< Info we store for each marker*/
struct ObjectData_t {
    char obj_name[50];
    int marker_id;
    int visible;
    //double width;
    double center[2];
    double trans[3][4];
    int cam_id;
};

/**< Message to be sent for each marker */
struct MarkerMsg_t {
    int marker_id;
    /* The 4x4 homogenous affine transformation matrix in the camGlobal frame.
    Only first three rows used. The fourth row is [0 0 0 1]. */
    double trans[3][4];
    int visible;        // is the marker visible in camera view
    int cam_id;         // id of the camera
};

/* Prints the marker message
 *  markerMsg :[IN] ptr to the instance of MarkerMsg_t */
void Object_printMarkerMsg(const MarkerMsg_t *markerMsg);