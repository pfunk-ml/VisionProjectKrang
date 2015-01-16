/**
 * @file Object.h
 * @brief Defines the message formats for ACH channels.
 */
#pragma once

/**< Info we store for each marker*/
struct ObjectData_t {
    char *patt_name;
    int patt_id;
    int model_id;
    int visible;
    double width;
    double center[2];
    double trans[3][4];
    int cam_id;
};

/**< Message to be sent for each marker */
struct MarkerMsg_t {
    int id;             // the ID of the object
    
    /* homogenous transformation matrix. Only first 3 rows of 4x4 matrix are \
       represented. Fourth is row is assumed (0, 0, 0, 1) */
    double trans[3][4]; 
    int visible;        // is the object visible to camera
    int cam_id;         // ID of the camera
};

typedef struct MarkerMsg_t MarkerMsg_t

/* Prints the marker message to the stdout. 
 * markerMsg: [IN] pointer to marker msg to be printed */
void printMarkerMsg(const MarkerMsg_t *markerMsg);
