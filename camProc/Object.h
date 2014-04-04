/**
 * @file Object.h
 */
#pragma once

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

ObjectData_t* read_ObjectData( char *_name,
			       int *_objectNum );
