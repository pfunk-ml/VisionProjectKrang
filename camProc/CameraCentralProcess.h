/**
 * @file CameraCentralProcess.h
 */
#pragma once
#include <Eigen/Geometry>
#include <vector>
#include <stdint.h>
#include <ach.h>

#include "globalData.h"
#include "Object.h"

class ARMarker;
class ARCamera;
class WorldModel;

/*** Planning output */
struct Planning_output{
 
    int marker_id;
    double Tworld_marker[3][4];
    int visible;
};


/**
 * @class CameraCentralProcess
 */
class CameraCentralProcess {
  
 public:

    CameraCentralProcess();
    ~CameraCentralProcess();
    
    void initSetup();
    bool startCamProcesses();
    bool startCamProcess( const int &_i );
    int spawnCamera( char* _camProgram,
		     char** _argList );

    bool setupChannels();

    void mainLoop();
    bool grabChannelsInfo();
    void getWorldTransforms();
    void createMessage();
    void sendMessage();
    
  
 private:

  /**< Environemnt */
  WorldModel* mWorldModel;
  std::vector<ARCamera> mCameras;
  std::vector<ARMarker> mMarkers;

  std::vector<ach_channel_t> mInput_channels;
  ach_channel_t mOutput_channel;
  MarkerMsg_t mMarkerMsgs[NUM_OBJECTS];
  Planning_output mMsg[NUM_OBJECTS];
  double finalMsg[NUM_OBJECTS][4]; // visible,x,y,angle

 public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW

};

