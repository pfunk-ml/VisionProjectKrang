/**
* @file CameraCentralProcess.h


  
*/
#include "CameraCentralProcess.h"

#include "worldModel/ARCamera.h"
#include "worldModel/ARMarker.h"
#include "worldModel/WorldModel.h"
#include "globalStuff/globalData.h"
#include "json/json.h"
#include "globalStuff/optparser.h"
#include <iostream>
#include <stdio.h>

#include <unistd.h>

#include <trajectory_io.h>
#include <utils.h>

/**
* @function CameraCentralProcess
* @brief Set initial needed information
*/
CameraCentralProcess::CameraCentralProcess() 
{
  // First get json file
  Json::Value config;
  parseJSONFile("globalStuff/config.json", config);
  
  setGlobalData(config);
  //setGlobalTransforms(config);

  std::vector<MarkerMsgWrapper_t> temp1;

  // Initialize the vectors
  for (int i = 0; i < gConfParams.numObjects; i++)
  {
    //mMarkerMsgs.push_back(std::vector<MarkerMsg_t>());\

    mMarkerMsgs.push_back(temp1);  
    mMsg.push_back(Planning_output());
    objPoses.push_back(new double[3]); // x,y,angle
    mMarkerPoses.push_back(new double[4]); // x, y, z, angle
    debugMsg.push_back(new double[3]);
    mBf.push_back(basicFilter());
  }
}

/**
 * @function
 * @brief
 */
CameraCentralProcess::~CameraCentralProcess() 
{
  for (int i = 0; i < gConfParams.numObjects; i++)
  {
    delete objPoses[i];
    delete debugMsg[i];
  }
}

/**
 * @function initSetup
 * @brief Initialize hard-coded stuff
 */
void CameraCentralProcess::initSetup() {
   
  /** Cameras */
  mCameras.clear();
  for( int i = 0; i < gConfParams.numCameras; ++i ) {
    ARCamera cam(i);
    mCameras.push_back( cam );
  }
  
  /** Markers WITH ID AS IDENTIFICATION!! */
  mMarkers.clear();
  for( int i = 0; i < gConfParams.numObjects; ++i ) {
    ARMarker marker( gConfParams.markerIDs[i] );
    mMarkers.push_back(marker);
  }
  
  
  /**< Set hard-coded values for cameras */
  for( int i = 0; i < gConfParams.numCameras; ++i )
    mCameras[i].initializeCamera( gTransforms.T_cam2world[i] );  
  
  
  /** World */
  mWorldModel = new WorldModel( mCameras, mMarkers );

  /** Set filter default weights */
  // (simple ramp shape)
  for( int i = 0; i < gConfParams.numObjects; ++i ) {
  	mBf[i].set_default_weights( GAUSSIAN_LEFT );
  }
}

/**
 * @function spawnCamera
 * @brief Fork and exec a subprocess running the camera program
 */
int CameraCentralProcess::spawnCamera( char* _camProgram,
				       char** _argList ) {
  pid_t child_pid;
  
  /** Duplicate this process */
  child_pid = fork();
  
  /**< Parent process */
  if( child_pid != 0 ) {
    return child_pid;
  }
  
  /**< Child process */
  else {
    execvp( _camProgram, _argList);
    /**< execvp only comes back if an error*/
    fprintf( stderr, "An error occurred in execvp %s calling the program \n", _camProgram );
    abort();
  }
}

/**
 * @function setupChannels
 * @brief Open camera channels and output channel
 */
bool CameraCentralProcess::setupChannels() {
  
  mInput_channels.resize(0);
  
  std::cout << "Initializing input channels..." << std::endl;
  enum ach_status r; 
  int counter; std::string name;
  
  counter = 0;
  for( int i = 0; i < gConfParams.numCameras; ++i ) {
    
    name = CAM_CHANNEL_NAME[i]; 
    
    ach_channel_t chan;
    r = ach_open( &chan, name.c_str(), NULL );
    if( ACH_OK == r ) {
      mInput_channels.push_back( chan );
      mCamIDs.push_back(i);
      std::cout << "\t ** Channel "<<name<<" opened successfully. **"<<std::endl;
    }
  }
  
  if( mInput_channels.size() == 0 ) {
    std::cout << "\t XX None of the channels opened. ERROR XX" << std::endl;
    return false;
  }
  
  char outputKrangPoseChannelName[1024];
  strcpy(outputKrangPoseChannelName, VISION_KRANG_POSE_CHANNEL.c_str());

  char debugChanChar[1024];
  strcpy(debugChanChar, DEBUG_CHANNEL.c_str());

  /* Create output ACH channels */
  r = ach_create( VISION_OBJ_POSES_CHANNEL.c_str(), 10, 512, NULL );
  /* if channel not created and it doesn't exist */
  if( ACH_OK != r && ACH_EEXIST != r) {   
      fprintf( stderr, "Could not create channel: %s\n", ach_result_to_string(r) );
      exit(EXIT_FAILURE);
  }

  r = ach_create( VISION_KRANG_POSE_CHANNEL.c_str(), 10, 512, NULL );
  /* if channel not created and it doesn't exist */
  if( ACH_OK != r && ACH_EEXIST != r) {   
      fprintf( stderr, "Could not create channel: %s\n", ach_result_to_string(r) );
      exit(EXIT_FAILURE);
  }

  /**< Open the channels */
  r = ach_open( &mOutput_objPoses_channel, VISION_OBJ_POSES_CHANNEL.c_str(), NULL );
  assert( ACH_OK == r );

  r = ach_open( &mOutput_krangPose_channel, VISION_KRANG_POSE_CHANNEL.c_str(), NULL );
  assert( ACH_OK == r );

  /* Create debug ACH channel */
  r = ach_create( debugChanChar, 10, 512, NULL );
  /* if channel not created and it doesn't exist */
  if( ACH_OK != r && ACH_EEXIST != r) { 
      fprintf( stderr, "Could not create channel: %s\n", ach_result_to_string(r) );
      exit(EXIT_FAILURE);
  }
  
  /**< Open the debug channel */
  r = ach_open( &mDebug_channel, debugChanChar, NULL );
  assert( ACH_OK == r );

  return true;
}

/**
 * @function mainLoop
 * @brief Send messages at 10 Hz
 */
void CameraCentralProcess::mainLoop() {
  
  while( true ) {
    
    // Get info from channels
   this->grabChannelsInfo();

    // Calculate the markers transformation
    this->getWorldTransforms();

    // Create the message
    this->createMessage();

    // Print the message
    this->printMessage();

    // Send the message
    this->sendMessage();
    
    // Important! If you just run the main loop like crazy it will get you an ACH error
    // crazy: CONTINUOUSLY WITHOUT SLEEP 
    // 10Hz        
    usleep(0.1*1e6);
    //usleep(0.1*1e7);
  }

}

/**
 * @function grabChannelsInfo
 * @brief Grab information from camera channels
 * return val: True if one or more channels were read successfully. False 
               if message is received from none of the cameras.
 */
bool CameraCentralProcess::grabChannelsInfo() {

  MarkerMsg_t tempMm[gConfParams.numObjects];  // temporary marker msgs
  int r;      // result
  size_t fs;  // frame size
  bool isMsgRcvd = true;

  // Clear earlier messages. mMarkerMsgs.size() should be equal to # of objects
  for ( int i = 0; i < mMarkerMsgs.size(); i++)
    mMarkerMsgs[i].clear();
  
  for( int i = 0; i < mInput_channels.size(); ++i ) 
  { // iterate over all channels
    // read the input channel
    r = ach_get( &mInput_channels[i],
                  &tempMm,
                  sizeof(tempMm),
                  &fs,
                  NULL, ACH_O_LAST );
    
    if( r != ACH_OK && r != ACH_MISSED_FRAME ) {
      std::cout << "Error: Channel "<<CAM_CHANNEL_NAME[i]<<": ACH status bad" << std::endl;
      isMsgRcvd = false;
    }

    else if( fs != sizeof(tempMm)) {
      std::cout << "Error: Channel "<<CAM_CHANNEL_NAME[i]<<": Frame size is not same as message received"<< std::endl;
      isMsgRcvd = false;
    }
    
    else{
      /* Iterate over each object. If object is visible in message read from 
        current channel, then add it to mMarkerMsgs */
      MarkerMsgWrapper_t markerMsgWrapper;
      markerMsgWrapper.camID = mCamIDs[i];
      for(int j=0; j < gConfParams.numObjects; j++) {     
        if(tempMm[j].visible){         
          markerMsgWrapper.marker = tempMm[j];
          mMarkerMsgs[j].push_back(markerMsgWrapper);
        }
      } // end for
    } // else
  } // end for
  return isMsgRcvd;
}

/**
 * @function getWorldTransforms
 * @brief Read message information stored in mMarkerMsgs and fills the transform info
 */
void CameraCentralProcess::getWorldTransforms() {
  
  for( int i = 0; i < gConfParams.numObjects; ++i )  {   // iterate over objects
    
    std::vector<Eigen::Matrix4d> transforms;
    std::vector<int> cameraIDs;

    for (int ind = 0; ind < mMarkerMsgs[i].size(); ind++) {

      // pose of marker in camera frame
      Eigen::Matrix4d Tf = utils_getDoubleArrAsMat(mMarkerMsgs[i][ind].marker.trans);
      
      int cameraID = mMarkerMsgs[i][ind].camID;
         
      // Divide distances (x, y, z) by 100 (so now in meters)
      Tf(0,3) = Tf(0,3) / 100.0;
      Tf(1,3) = Tf(1,3) / 100.0;
      Tf(2,3) = Tf(2,3) / 100.0;
     
      // Append to vector
      transforms.push_back(Tf);
      cameraIDs.push_back(cameraID);
    }

    if (mMarkerMsgs[i].size() > 0) {
      /* transform marker pose in cam frame to world frame */
      if (!mWorldModel->setMarkerLoc(cameraIDs, mMarkerMsgs[i][0].marker.marker_id, transforms))
        std::cout << "[camCentralProcess-- getWorldTransforms] ERROR - CAMERA IS PROBABLY NOT INITIALIZED"<< std::endl;
    }
  }
}


/**
 * @function createMessage
 * @brief Create message, by now just send transformation
 * (Que tipo de individuo quiere solo informacion planar?)
 * @ ADDING SPRITE TRANSFORMATION
 */
void CameraCentralProcess::createMessage() {

  double x, y, z, theta;
  double x_est, y_est, theta_est;

  Eigen::Matrix4d Pmarker_world; // marker pose in world frame 
  Eigen::Matrix4d Pobj_world; // obj pose in world frame

  for( int i = 0; i < gConfParams.numObjects; ++i ) {
   
    /* If object is not visible, set x, y, theta to zero to signal the filter 
       that these values are not being seen! 
       mMarkerMsgs has messages only if they are visible */
    if( mMarkerMsgs[i].size() == 0 ) {
      x = 0; y = 0; z = 0;theta = 0;
    }

    else{
      Pmarker_world = mWorldModel->getMarkerPose( 
                                        mMarkerMsgs[i][0].marker.marker_id );
      Pobj_world = Pmarker_world * gTransforms.T_sprite[i];
      utils_getXYZAng(Pobj_world, x, y, z, theta);
      utils_getXYZAng(Pmarker_world, mMarkerPoses[i][0], mMarkerPoses[i][1], 
                                mMarkerPoses[i][2], mMarkerPoses[i][3]);
    }

    // Use filter (one filter per each object!)
    mBf[i].getEstimate( x, y, theta, x_est, y_est, theta_est );

    objPoses[i][0] = x_est;
    objPoses[i][1] = y_est;
    objPoses[i][2] = theta_est;

    debugMsg[i][0] = x;
    debugMsg[i][1] = y;
    debugMsg[i][2] = theta;
  } // end for
}

/* Print the distances between markers. Useful to test calibration */
void CameraCentralProcess::printDistances() {
  double linDist, rotDist;

  printf("Distances between markers\n");

    for(int i=0; i < gConfParams.numObjects; ++i)
      for(int j = i+1; j < gConfParams.numObjects; ++j) {

        printf(" \t IDs %d and %d: ", gConfParams.markerIDs[i], gConfParams.markerIDs[j]);

        if(mMarkerMsgs[i].size() == 0 || mMarkerMsgs[j].size() == 0){
          printf("[At least one marker not visible.]\n");
        continue;
        }

        utils_computeDistances(mMarkerPoses[i], mMarkerPoses[j], 
          &linDist, &rotDist);
        printf("Linear: %.4f Angular: %.4f\n", linDist, rotDist);
      }
}

void CameraCentralProcess::printMessage() {
  int i;
  
  printf("All dimensions are in metres and radians\n");

  /* Print poses of the objects */
  printf("OBJECT Poses (in world frame)\n");
  for( int i = 0; i < gConfParams.numObjects; ++i ) 
    utils_printPose(objPoses[i], gConfParams.markerIDs[i], mMarkerMsgs[i].size() != 0);

  /* Print poses of the markers */
  printf("MARKER Poses (in world frame)\n");
  for( int i = 0; i < gConfParams.numObjects; ++i )
    utils_printPose(mMarkerPoses[i], gConfParams.markerIDs[i], mMarkerMsgs[i].size() != 0);

  /* Print the distances between markers. Useful to test calibration */
  printf("Distances between markers\n");
  
  int index2 = GlobalData_getIndex(2);
  int index3 = GlobalData_getIndex(3);
  int index4 = GlobalData_getIndex(4);
  int index6 = GlobalData_getIndex(6);

  printDistances();
  
  std::cout<<"--\n";
  return;
}

/**
* @function sendMessage
* @brief 
*/
void CameraCentralProcess::sendMessage() {

  // Convert objPoses and debugMsg to double[][]
  double objPosesPtr[gConfParams.numObjects-1][4];
  double krangPosePtr[3];
  double debugMsgPtr[gConfParams.numObjects][3];

  // copy the vectors into double arrays
  for (int i = 0; i < gConfParams.numObjects-1; i++) {
      objPosesPtr[i][0] = gConfParams.markerIDs[i+1]; // marker id
      objPosesPtr[i][1] = objPoses[i+1][0];
      objPosesPtr[i][2] = objPoses[i+1][1];
      objPosesPtr[i][3] = objPoses[i+1][2];
      
      debugMsgPtr[i][0] = debugMsg[i+1][0];
      debugMsgPtr[i][1] = debugMsg[i+1][1];
      debugMsgPtr[i][2] = debugMsg[i+1][2];
  }

  krangPosePtr[0] = objPoses[0][0];
  krangPosePtr[1] = objPoses[0][1];
  krangPosePtr[2] = objPoses[0][2];

  /* Create formatted message */
  trajectory_2d_t objPoses_trajectory_2d = {.n = gConfParams.numObjects-1, 
                                            .m = 4, 
                                            .data = objPosesPtr[0] };

  char objPoses_str[sizeof(int) * 2 + sizeof(double) * (gConfParams.numObjects-1) * 4];
  serialize_from_Trajectory2D(&objPoses_trajectory_2d, objPoses_str);

  trajectory_2d_t krangPose_trajectory_2d = {.n = 1, 
                                             .m = 3, 
                                             .data = krangPosePtr};                                 

  char krangPose_str[sizeof(int) * 2 + sizeof(double) * 3];
  serialize_from_Trajectory2D(&krangPose_trajectory_2d, krangPose_str);

  /* Send the messages on the ACH channels */
  enum ach_status r;

  r = ach_put( &mOutput_objPoses_channel, objPoses_str, sizeof(objPoses_str));
  assert(r == ACH_OK);

  // Don't send the message if robot is not visible
  if(mMarkerMsgs[0].size() != 0){
    r = ach_put( &mOutput_krangPose_channel, krangPose_str, sizeof(krangPose_str));
    assert(r == ACH_OK);
  }
}
