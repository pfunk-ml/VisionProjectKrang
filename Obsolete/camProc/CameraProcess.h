/**
 * @file CameraProcess.h
 */
#pragma once

#include <GL/gl.h>
#include <GL/glut.h>

#include <AR/gsub.h>
#include <AR/video.h>
#include <AR/param.h>
#include <AR/ar.h>

#include <string>

#include "globalData.h"
#include "Object.h"

/**
 * @struct CamData
 */
struct CamData {

    /**< Camera configuration */    
    int dimX; int dimY;
    int thresh;
    int count;
    char* cparam_name;
    ARParam cparam;    
    
    /**< Pattern information */
    ObjectData_t data[NUM_OBJECTS];

    /** Env info */
    char* artoolkit_config;
};

/**
 * @file parseYAMLCalibration
 * @brief Parse a _filename (.yaml) file and obtains a 
 * ARParam object with the information for ARTookit to use 
 */
bool parseYAMLCalibration( std::string _filename,
			   ARParam &_cparam );
