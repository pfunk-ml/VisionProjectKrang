/**
* @file CameraCentralProcess.h
*/
#include "CameraCentralProcess.h"

#include "worldModel/ARCamera.h"
#include "worldModel/ARMarker.h"
#include "worldModel/WorldModel.h"
#include "globalStuff/globalData.h"
#include "globalStuff/globalTransforms.h"
#include <iostream>
#include <stdio.h>


/**
* @function CameraCentralProcess
* @brief Set initial needed information
*/
CameraCentralProcess::CameraCentralProcess() {
  
  setGlobalData();
  setGlobalTransforms();

}

/**
 * @function
 * @brief
 */
CameraCentralProcess::~CameraCentralProcess() {
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
  for( int i = 0; i < NUM_CAMERAS; ++i ) {
    mCameras[i].initializeCamera( gTworld_cam[i] );  
  }
  
  
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
  
  std::cout << "Initializing input channels" << std::endl;
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
    std::cout << "\t XX No a single channel opened. ERROR XX" << std::endl;
    return false;
  }
  
  // OUTPUT CHANNEL

  /**< Open the channel */
  r = ach_open( &mOutput_channel, PERCEPTION_CHANNEL, NULL );
  assert( ACH_OK == r );
  
  /**< Open the debug channel */
  r = ach_open( &mDebug_channel, DEBUG_CHANNEL, NULL );
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
    if( !this->grabChannelsInfo() ) { break; }
    
    // Calculate the markers transformation
    this->getWorldTransforms();
    
    // Create the message
    this->createMessage();
    
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
 */
bool CameraCentralProcess::grabChannelsInfo() {
  
  MarkerMsg_t tempMm[NUM_OBJECTS];
  int r; size_t fs;

  // Clear earlier messages
  // Vector Changes here
  for ( int i = 0; i < NUM_OBJECTS; i++)
  {
    mMarkerMsgs[i].clear();
  }
  
  for( int i = 0; i < mInput_channels.size(); ++i ) {
    r = ach_get( &mInput_channels[i],
		 &tempMm,
		 sizeof(tempMm),
		 &fs,
		 NULL, ACH_O_LAST );
    
    if( ACH_OK != r && ACH_MISSED_FRAME != r ) {
      std::cout << "ACH status bad. EXITING MAIN LOOP" << std::endl;
      return false;
    }
    if( sizeof(tempMm) != fs ) {
      std::cout << "Frame size and message received are not the same. EXITING MAIN LOOP"<< std::endl;
      return false;
    }
    
    // Vector Changes here
    if( i == 0 ) {
      for( int j = 0; j < NUM_OBJECTS; ++j ) {
	mMarkerMsgs[j].push_back(tempMm[j]);
      }
    } // end if i == 0
    else {
      for( int j = 0; j < NUM_OBJECTS; ++j ) {
	    if( mMarkerMsgs[j][0].visible == -1 && tempMm[j].visible == 1 ) 
        {
          mMarkerMsgs[j].clear();
	      mMarkerMsgs[j].push_back(tempMm[j]);
	    }
        else if (tempMm[j].visible == 1)
        {
          mMarkerMsgs[j].push_back(tempMm[j]);
        }
      }
    } // end else i == 0
    
  } // end for
  
  
  return true;
}

/**
 * @function getWorldTransforms
 * @brief Read message information stored in mObjects and fills the transform info
 */
void CameraCentralProcess::getWorldTransforms() {
  
  for( int i = 0; i < NUM_OBJECTS; ++i ) 
  {

    // Vector Changes here
    std::vector<Eigen::Matrix4d> transforms;
    std::vector<int> cameraIDs;
    for (int ind = 0; ind < mMarkerMsgs[i].size(); ind++)
    {
      Eigen::Matrix4d Tf = getDoubleArrAsMat(mMarkerMsgs[i][ind].trans);
      int cameraID = mMarkerMsgs[i][ind].cam_id;
         
      // Divide distances by 100 (so now in meters)
      Tf(0,3) = Tf(0,3) / 100.0;
      Tf(1,3) = Tf(1,3) / 100.0;
      Tf(2,3) = Tf(2,3) / 100.0;
     
      // Append to vector
      transforms.push_back(Tf);
      cameraIDs.push_back(cameraID);
    }

    // Vector Changes here
    if (!mWorldModel->setMarkerLoc(cameraIDs, mMarkerMsgs[i][0].marker_id, transforms))
    {
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

  for( int i = 0; i < NUM_OBJECTS; ++i ) {
    // Vector Changes here
    Eigen::Matrix4d Tmarker = mWorldModel->getMarkerPose( mMarkerMsgs[i][0].marker_id );
    Eigen::Matrix4d Tsprite = Tmarker*gTmarker_sprite[i];
    getXYangTriple(Tsprite, x, y, theta);
    

    // If object is not visible, set x,y,theta to last estimated value
    // WE USED TO DO THIS...

    // Use filter (one filter per each object!)
    mBf[i].getEstimate( x, y, theta,
		     	x_est, y_est, theta_est );
    
    	finalMsg[i][0] =  x_est;
    	finalMsg[i][1] = y_est;
    	finalMsg[i][2] = theta_est;

    	debugMsg[i][0] = x;
    	debugMsg[i][1] = y;
    	debugMsg[i][2] = theta;

       
  } // end for
  
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


  ach_put( &mOutput_channel,
	   finalMsg,
	   sizeof( finalMsg ) );

  // Send debug
  ach_put( &mDebug_channel,
	   debugMsg,
	   sizeof( debugMsg ) );

}
