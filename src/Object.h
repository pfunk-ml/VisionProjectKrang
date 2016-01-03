/**
 * @file Object.h
 */
#pragma once

//#include "worldModel/WorldModel.h"
#include "utils.h"

/**< Message to be sent for each marker */
struct MarkerMsg_t {
    int marker_id;
    /* The 4x4 homogenous affine transformation matrix in the camGlobal frame.
    Only first three rows used. The fourth row is [0 0 0 1]. */
    double trans[3][4];

    // is marker visible in camera view. True if visible, false otherwise
    bool visible;
    int cam_id;         // id of the camera
};

/* Prints the marker message
 * markerMsg :[IN] ptr to the instance of MarkerMsg_t */
void Object_printMarkerMsg(const MarkerMsg_t *markerMsg);

/* Prints the marker message briefly
 * markerMsg :[IN] ptr to the instance of MarkerMsg_t */
void Object_printMarkerMsgSingleLine(const MarkerMsg_t *markerMsg);