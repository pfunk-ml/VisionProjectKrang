/**
 * @file globalData.cpp
 */
#include <stdio.h>
#include <iostream>
#include "globalData.h"

/** Global */
std::vector<std::string> OBJECT_NAME;
std::vector<int> MARKER_ID;
std::vector<int> MARKER_SIZE;

std::vector<std::string> CAM_CALIB_NAME;
std::vector<std::string> CAM_CHANNEL_NAME;

ObjectData_t gObjects[NUM_OBJECTS];


/**
 * @function setGlobalData
 * @brief Set global information 
 */
void setGlobalData() {

  // Calibration file
  CAM_CALIB_NAME.resize(0);
  for( int i = 0; i < NUM_CAMERAS; ++i ) {
    char name[30];
    sprintf( name, "Data/camCalib_%d.xml", i);
    CAM_CALIB_NAME.push_back( std::string(name) );
  }

  // Channel names for cameras
  CAM_CHANNEL_NAME.resize(0);
  for( int i = 0; i < NUM_CAMERAS; ++i ) {
    char name[30];
    sprintf( name, "cam%d_channel", i);
    CAM_CHANNEL_NAME.push_back( std::string(name) );
  }
  
  // Objects 
  OBJECT_NAME.resize(0);
  OBJECT_NAME.push_back( std::string("robot") );
  OBJECT_NAME.push_back( std::string("table") );
  OBJECT_NAME.push_back( std::string("chair") ); 

  if( OBJECT_NAME.size() != NUM_OBJECTS ) {
    std::cout << "[X] Error initializing OBJECT_NAMES!"<< std::endl;
  }

  MARKER_ID.resize(0);
  MARKER_ID.push_back( 1 );
  MARKER_ID.push_back( 5 );
  MARKER_ID.push_back( 6 );

  if( MARKER_ID.size() != NUM_OBJECTS ) {
    std::cout << "[X] Error initializing MARKER_ID!"<< std::endl;
  }
  
  MARKER_SIZE.resize(0);
  MARKER_SIZE.push_back( 20.3 );
  MARKER_SIZE.push_back( 20.3 );
  MARKER_SIZE.push_back( 20.3 );

  if( MARKER_SIZE.size() != NUM_OBJECTS ) {
    std::cout << "[X] Error initializing MARKER_SIZE!"<< std::endl;
  }


  /**< Initialize objects for each marker */
  for( int i = 0; i < NUM_OBJECTS; ++i ) {
    ObjectData_t obj;
    char name[50];
    sprintf( obj.obj_name, "%s", OBJECT_NAME[i].c_str() ); 
    //obj.obj_name = name;
    obj.marker_id  = MARKER_ID[i];
    obj.visible = -1;
    obj.width = MARKER_SIZE[i];
    obj.center[0] = 0; obj.center[1] = 0;
    obj.cam_id = -1;
    
    // Add object
    gObjects[i] = obj;
  }
}
