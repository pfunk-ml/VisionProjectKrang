/**
 * @file CameraCentralProcess.h
 */
#pragma once
#include <Eigen/Geometry>
#include <vector>
#include <stdint.h>
#include <ach.h>
#include <string>
#include <cstring>

#include "globalStuff/globalData.h"
#include "globalStuff/Object.h"
#include "basicFilter.h"

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
    
    /* */
    void initSetup();
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
  ach_channel_t mDebug_channel;
  std::vector<std::vector<MarkerMsg_t> > mMarkerMsgs;
  std::vector<Planning_output> mMsg;
  std::vector<double*> finalMsg; // x,y,angle // visible,
  std::vector<double*> debugMsg;

  // Basic filter for smooth tracking of markers
  std::vector<basicFilter> mBf;

 public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW

};

