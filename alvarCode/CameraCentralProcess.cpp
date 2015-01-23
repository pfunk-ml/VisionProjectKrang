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

  // Initialize the vectors
  for (int i = 0; i < NUM_OBJECTS; i++)
  {
    mMarkerMsgs.push_back(std::vector<MarkerMsg_t>());
    mMsg.push_back(Planning_output());
    finalMsg.push_back(new double[3]); // x,y,angle // visible,
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
  for (int i = 0; i < NUM_OBJECTS; i++)
  {
    delete finalMsg[i];
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
  for( int i = 0; i < NUM_CAMERAS; ++i ) {
    ARCamera cam(i);
    mCameras.push_back( cam );
  }
  
  /** Markers WITH ID AS IDENTIFICATION!! */
  mMarkers.clear();
  for( int i = 0; i < NUM_OBJECTS; ++i ) {
    ARMarker marker( gObjects[i].marker_id );
    mMarkers.push_back( marker );
  }
  
  
  /**< Set hard-coded values for cameras */
  for( int i = 0; i < NUM_CAMERAS; ++i )
    mCameras[i].initializeCamera( gTransforms.T_cam2world[i] );  
  
  
  /** World */
  mWorldModel = new WorldModel( mCameras, mMarkers );

  /** Set filter default weights */
  // (simple ramp shape)
  for( int i = 0; i < NUM_OBJECTS; ++i ) {
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
  int r; int counter; std::string name;
  
  counter = 0;
  for( int i = 0; i < NUM_CAMERAS; ++i ) {
    
    name = CAM_CHANNEL_NAME[i]; 
    
    ach_channel_t chan;
    r = ach_open( &chan, name.c_str(), NULL );
    if( ACH_OK == r ) {
      mInput_channels.push_back( chan );
      std::cout << "\t ** Channel "<<name<<" opened successfully. **"<<std::endl;
    }
    
  }
  
  if( mInput_channels.size() == 0 ) {
    std::cout << "\t XX Not a single channel opened. ERROR XX" << std::endl;
    return false;
  }
  
  // OUTPUT CHANNEL

  // Copy to char pointers
  char outputChanChar[1024];
  strcpy(outputChanChar, PERCEPTION_CHANNEL.c_str());
  char debugChanChar[1024];
  strcpy(debugChanChar, DEBUG_CHANNEL.c_str());

  /**< Open the channel */
  r = ach_open( &mOutput_channel, outputChanChar, NULL );
  assert( ACH_OK == r );
  
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
  }

}

/**
 * @function grabChannelsInfo
 * @brief Grab information from camera channels
 * return val: True if one or more channels were read successfully. False 
               if message is received from none of the cameras.
 */
bool CameraCentralProcess::grabChannelsInfo() {

  MarkerMsg_t tempMm[NUM_OBJECTS];  // temporary marker msgs
  int r;      // result
  size_t fs;  // frame size
  bool isMsgRcvd = true;

  // Clear earlier messages
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
      for(int j=0; j < NUM_OBJECTS; j++) {     
        if(tempMm[j].visible == 1)
          mMarkerMsgs[j].push_back(tempMm[j]);
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
  
  for( int i = 0; i < NUM_OBJECTS; ++i )  {   // iterate over objects
    
    std::vector<Eigen::Matrix4d> transforms;
    std::vector<int> cameraIDs;

    for (int ind = 0; ind < mMarkerMsgs[i].size(); ind++) {

      // pose of marker in camera frame
      Eigen::Matrix4d Tf = getDoubleArrAsMat(mMarkerMsgs[i][ind].trans);
      
      int cameraID = mMarkerMsgs[i][ind].cam_id;
         
      // Divide distances (x, y, z) by 100 (so now in meters)
      Tf(0,3) = Tf(0,3) / 100.0;
      Tf(1,3) = Tf(1,3) / 100.0;
      Tf(2,3) = Tf(2,3) / 100.0;
     
      // Append to vector
      transforms.push_back(Tf);
      cameraIDs.push_back(cameraID);
    }

    if (mMarkerMsgs[i].size() > 0) { 
      if (!mWorldModel->setMarkerLoc(cameraIDs, mMarkerMsgs[i][0].marker_id, transforms))
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

  double x, y, theta;  
  double x_est, y_est, theta_est;

  Eigen::Matrix4d Pmarker_world; // marker pose in world frame 
  Eigen::Matrix4d Pobj_world; // obj pose in world frame

  for( int i = 0; i < NUM_OBJECTS; ++i ) {
   
    /* If object is not visible, set x, y, theta to zero to signal the filter 
       that these values are not being seen! */
    if( mMarkerMsgs[i].size() == 0 ) {
      x = 0; y = 0; theta = 0;
    }
    else if(mMarkerMsgs[i][0].visible != 1){
      x = 0; y = 0; theta = 0;
    }
    else{
      Pmarker_world = mWorldModel->getMarkerPose( 
                                            mMarkerMsgs[i][0].marker_id );
      Pobj_world = Pmarker_world * gTransforms.T_sprite[i];
      getXYangTriple(Pobj_world, x, y, theta);
    }

    // cout<<__func__<<"() "<<__LINE__<<": "<<"x in world frame = "<<x;

    // Use filter (one filter per each object!)
    mBf[i].getEstimate( x, y, theta, x_est, y_est, theta_est );

    finalMsg[i][0] = x_est;
    finalMsg[i][1] = y_est;
    finalMsg[i][2] = theta_est;

    debugMsg[i][0] = x;
    debugMsg[i][1] = y;
    debugMsg[i][2] = theta;      
  } // end for
}

void CameraCentralProcess::printMessage() {
  int i;
  printf("  x           y         theta\n");
  for(i = 0; i < NUM_OBJECTS; i++) {
    printf("%9.3f %9.3f %9.3f\n", finalMsg[i][0], finalMsg[i][1], finalMsg[i][2]);
  }
  std::cout<<"--\n";
  return;
}

/**
* @function sendMessage
* @brief Send message with objects locations over PERCEPTION_CHANNEL
*/
void CameraCentralProcess::sendMessage() {
  
  for( int i = 0; i < NUM_OBJECTS; ++i ) {
    //printf(" Sending info for Marker [%d] with ID: %d \n", i, mMarkerMsgs[i].marker_id );
    //printf(" \t Camera id: %d \n", mMarkerMsgs[i].cam_id );
    //printf(" \t Transformation: x: %f y: %f theta: %f \n", finalMsg[i][0], finalMsg[i][1], finalMsg[i][2] );
  }

  // Convert finalMsg and debugMsg to double[][]
  double finalMsgPtr[NUM_OBJECTS][3];
  double debugMsgPtr[NUM_OBJECTS][3];
  for (int i = 0; i < NUM_OBJECTS; i++)
  {
    for (int j = 0; j < 3; j++)
    {
      finalMsgPtr[i][j] = finalMsg[i][j];
      debugMsgPtr[i][j] = debugMsg[i][j];
    }
  }

  ach_put( &mOutput_channel,
	   finalMsgPtr,
	   sizeof( finalMsgPtr ) );

  // Send debug
  ach_put( &mDebug_channel,
	   debugMsgPtr,
	   sizeof( debugMsgPtr ) );

}
