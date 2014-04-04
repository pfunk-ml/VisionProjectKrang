/**
 * @file CameraCentralProcess.h
 */
#include "CameraCentralProcess.h"

#include "ARCamera.h"
#include "ARMarker.h"
#include "WorldModel.h"
#include "globalData.h"
#include "globalTransforms.h"
#include <iostream>
#include <stdio.h>


/**
 * @function CameraCentralProcess
 * @brief
 */
CameraCentralProcess::CameraCentralProcess() {

    setHardCodedValues();
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

    /** Markers */
    mMarkers.clear();
    for( int i = 0; i < NUM_OBJECTS; ++i ) {
	ARMarker marker(i);
	mMarkers.push_back( marker );
    }


    /**< Set hard-coded values for cameras */
    for( int i = 0; i < NUM_CAMERAS; ++i ) {
	mCameras[i].initializeCamera( gTworld_cam[i] );
	
    }


    /** World */
    mWorldModel = new WorldModel( mCameras, mMarkers );

    /** Set Origin as MARKER 0 AND CAMERA 0, USING TRANSFORMATION HARD-CODED */
    mWorldModel->setOrigin( 0, 0, gTworld_origin );

}

/**
 * @function startCamProcesses
 * @brief Starts all cameras
 */
bool CameraCentralProcess::startCamProcesses() {

    for( int i = 0; i < NUM_CAMERAS; ++i ) {
	if( !startCamProcess( i ) ) {
	    return false;
	}
    }

    return true;
}

/**
 * @function startCamProcess
 * @brief Start camera _i
 */
bool CameraCentralProcess::startCamProcess( const int &_i ) {

    if( _i < 0 || _i >= NUM_CAMERAS ) {
	std::cout << "[X] Could not start cam process "<<_i<< std::endl;
	return false; 
    }

    char num[2];
    sprintf( num, "%d", _i );
    char* arg_list[] = {"camProcess", num, NULL };
    this->spawnCamera( "./camProcess", arg_list );
    std::cout << "Done calling cam "<< _i << std::endl;

    return true;
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
      
      switch(i) {
      case 0 : name = CAM0_CHANNEL; break;
      case 1 : name = CAM1_CHANNEL; break;
      case 2 : name = CAM2_CHANNEL; break;
      case 3 : name = CAM3_CHANNEL; break;
      };
      
      ach_channel_t chan;
      r = ach_open( &chan, name.c_str(), NULL );
      if( ACH_OK == r ) {
	  mInput_channels.push_back( chan ); 
      } else {
	  std::cout << "Channel "<< name << " not opened "<< std::endl;
	  return false;
      }	    

  }

  std::cout << "All "<< NUM_CAMERAS<<" channels were open successfully."<<std::endl;


  // OUTPUT CHANNEL

  /** Delete if existing, then create */
  r = ach_unlink( PERCEPTION_CHANNEL );
  assert( ACH_OK == r || ACH_ENOENT == r );
  // TODO : CHECK THESE 10 AND 256 NUMBERS
  r = ach_create( PERCEPTION_CHANNEL, 10ul, 256ul, NULL );
  assert( ACH_OK == r );

  /**< Open the channel */
  r = ach_open( &mOutput_channel, PERCEPTION_CHANNEL, NULL );
    assert( ACH_OK == r );


  return true;
}

/**
 * @function mainLoop
 * @brief
 */
void CameraCentralProcess::mainLoop() {

    while( true ) {

	// Get info from channels
	this->grabChannelsInfo();

	// Calculate the markers transformation
	this->getWorldTransforms();

	// Create the message
	this->createMessage();

	// Send the message
	this->sendMessage();

	// Sleep for some time
	sleep( 0.1*1e6 ); // 10Hz
    }

}

/**
 * @function grabChannelsInfo
 * @brief Grab information from camera channels
 */
void CameraCentralProcess::grabChannelsInfo() {

    ObjectData_t tempObj[NUM_OBJECTS];
    int r; size_t fs;

    for( int i = 0; i < NUM_CAMERAS; ++i ) {
	r = ach_get( &mInput_channels[i],
		     tempObj,
		     sizeof(tempObj),
		     &fs, 
		     NULL, ACH_O_WAIT );

	assert( (ACH_OK == r || ACH_MISSED_FRAME == r ) &&
		sizeof(tempObj) == fs );

	if( i == 0 ) {
	    for( int j = 0; j < NUM_OBJECTS; ++j ) {
		mObjects[j] = tempObj[i];
	    }
	} // end if i == 0
	else {
	    for( int j = 0; j < NUM_OBJECTS; ++j ) {
		if( mObjects[j].visible == -1 && tempObj[j].visible >= 0 ) {
		    mObjects[j] = tempObj[j];
		}
	    }
	} // end else i == 0
    }

}

/**
 * @function getWorldTransforms
 * @brief Read message information stored in mObjects and fills the transform info
 */
void CameraCentralProcess::getWorldTransforms() {

    for( int i = 0; i < NUM_OBJECTS; ++i ) {	
	mWorldModel->setMarkerLoc( mObjects[i].cam_id,
				   mObjects[i].patt_id,
				   getDoubleArrAsMat(mObjects[i].trans) );	
    }
}


/**
 * @function createMessage
 * @brief Create message, by now just send transformation
 * (Que tipo de individuo quiere solo informacion planar?)
 */
void CameraCentralProcess::createMessage() {

    for( int i = 0; i < NUM_OBJECTS; ++i ) {
	Planning_output p;
	p.marker_id = i;

	Eigen::Matrix4d Tmarker = mWorldModel->getMarkerPose(i);
	for( int j = 0; j < 3; ++j ) {
	    for( int k = 0; k < 4; ++k ) {
		p.Tworld_marker[j][k] = Tmarker(j,k);
	    }
	}
	p.visible = mObjects[i].visible;

	mMsg[i] = p;
    }

}



/**
 * @function sendMessage
 * @brief Send message with objects locations over PERCEPTION_CHANNEL
 */
void CameraCentralProcess::sendMessage() {

    ach_put( &mOutput_channel,
	     mMsg,
	     sizeof( mMsg ) );

}
