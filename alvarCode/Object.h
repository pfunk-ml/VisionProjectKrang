/**
 * @file Object.h
 */
#pragma once

/**< Info we store for each marker*/
struct ObjectData_t {
    char obj_name[50];
    int marker_id;
    int visible;
    double width;
    double center[2];
    double trans[3][4];
    int cam_id;
};

/**< Message to be sent for each marker */
struct MarkerMsg_t {
    int marker_id;
    double trans[3][4];
    int visible;
    int cam_id;
};

