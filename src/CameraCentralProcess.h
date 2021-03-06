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
#include "Object.h"
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

typedef struct {
  MarkerMsg_t marker;
  int camID;
} MarkerMsgWrapper_t;

/**
 * @class CameraCentralProcess
 * This class does following:
    . Read poses of markers in camera frames published by several cameras.
    . Calculate the markers poses in world frame.
    . Calculate the object poses in world frame.
    . Publish object poses to ach channel.

 */
class CameraCentralProcess {
  
 public:

    CameraCentralProcess();
    ~CameraCentralProcess();
    
    /* Initializes mCameras and mMarkers. */
    void initSetup();
    int spawnCamera( char* _camProgram,
		     char** _argList );

    bool setupChannels();

    void mainLoop();

    /* Reads the ACH channels. */
    bool grabChannelsInfo();

    /*  */
    void getWorldTransforms();
    
    void createMessage();
    void printMessage();

    /* Send message with objects locations over output ACH channel */
    void sendMessage();
    
  
 private:

  void computeDistances(double* pose1, double* pose2);
  int getIndex(int markerID);
  void printDistances();

  /**< Environemnt */
  WorldModel* mWorldModel;
  std::vector<ARCamera> mCameras;
  std::vector<ARMarker> mMarkers;

  // input ACH channels to get marker positions in camera frames.
  std::vector<ach_channel_t> mInput_channels;
  std::vector<int> mCamIDs; // camera ID of each input channel

  // output ACH channel to send marker/object position in ?? frame
  // ach_channel_t mOutput_channel; 
  ach_channel_t mOutput_objPoses_channel; 
  ach_channel_t mOutput_krangPose_channel; 

  ach_channel_t mDebug_channel;  // output ACH channel

  /* 2D array. Each row represents one object. Single row contains marker 
     messages sent by different cameras corresponding to same object */
  //std::vector<std::vector<MarkerMsg_t> > mMarkerMsgs;
  std::vector<std::vector<MarkerMsgWrapper_t> > mMarkerMsgs;
  
  std::vector<Planning_output> mMsg;

  // has length of (num of objs) * 4 [x, y, z, angle]
  std::vector<double*> objPoses;     // poses of objects 
  std::vector<double*> mMarkerPoses; // poses of AR markers

  // has length of 3
  std::vector<double*> krangPose;

  
  std::vector<double*> debugMsg;

  // Basic filter for smooth tracking of markers
  std::vector<basicFilter> mBf;

 public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW

};

