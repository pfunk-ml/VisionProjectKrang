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
    for( int i = 1; i <= NUM_CAMERAS; ++i ) {
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
    // And this is WHY YOU ALWAYS START WITH INDEX 0 IN C++!!!!
    for( int i = 1; i <= NUM_CAMERAS; ++i ) {
	mCameras[i-1].initializeCamera( gTworld_cam[i-1] );
	
    }


    /** World */
    mWorldModel = new WorldModel( mCameras, mMarkers );

    /** Set Origin as MARKER 0 AND CAMERA 0, USING TRANSFORMATION HARD-CODED */
    mWorldModel->setOrigin( 0, 0, gTworld_origin );

}

/**
 * @function startCamProcesses
 * @brief Starts all cameras  DO NOT USE IF DEV IS NOT THE SAME AS CAM
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
 * @brief Start camera _i DO NOT USE IF DEV IS NOT THE SAME AS CAM
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
  for( int i = 1; i <= NUM_CAMERAS; ++i ) {
      
      switch(i) {
      case 1 : name = CAM1_CHANNEL; break;
      case 2 : name = CAM2_CHANNEL; break;
      case 3 : name = CAM3_CHANNEL; break;
      case 4 : name = CAM4_CHANNEL; break;
      };
      
      ach_channel_t chan;
      r = ach_open( &chan, name.c_str(), NULL );
      if( ACH_OK == r ) {
	  mInput_channels.push_back( chan ); 
	  std::cout << "Channel "<<name<<" opened successfully."<<std::endl;
      }

  }

  if( mInput_channels.size() == 0 ) { 
      std::cout << "\t No a single channel opened. ERROR" << std::endl;
      return false; 
  }

  // OUTPUT CHANNEL

  /** Delete if existing, then create */
  r = ach_unlink( PERCEPTION_CHANNEL );
  assert( ACH_OK == r || ACH_ENOENT == r );
  // TODO : CHECK THESE 10 AND 256 NUMBERS
  r = ach_create( PERCEPTION_CHANNEL, 30ul, 256ul, NULL );
  assert( ACH_OK == r );

  /**< Open the channel */
  r = ach_open( &mOutput_channel, PERCEPTION_CHANNEL, NULL );
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
      //////////////////
        for( int c = 0; c < NUM_OBJECTS; ++c ) {
	        std::cout << "RECEIVED Object "<<c<< std::endl;
		std::cout << "Tf: \n"<< std::endl;
		for( int a = 0; a <3; ++a ) {
	    		for( int b = 0; b < 4; ++b ) {
                		std::cout << tempMm[c].trans[a][b]<<" ";
	    		} std::cout << std::endl;
		} std::cout << std::endl;
		std::cout << "Visibility: "<< tempMm[c].visible << std::endl;
     }
   //////////////////////

	
	if( i == 0 ) {
	    for( int j = 0; j < NUM_OBJECTS; ++j ) {
		mMarkerMsgs[j] = tempMm[j];
	    }
	} // end if i == 0
	else {
	    for( int j = 0; j < NUM_OBJECTS; ++j ) {
		if( mMarkerMsgs[j].visible == -1 && tempMm[j].visible >= 0 ) {
		    mMarkerMsgs[j] = tempMm[j];
		}
	    }
	} // end else i == 0
    }

    return true;
}

/**
 * @function getWorldTransforms
 * @brief Read message information stored in mObjects and fills the transform info
 */
void CameraCentralProcess::getWorldTransforms() {

    for( int i = 0; i < NUM_OBJECTS; ++i ) {
	Eigen::Matrix4d Tf = getDoubleArrAsMat(mMarkerMsgs[i].trans);
	
	Tf(0,3) = Tf(0,3) / 1000.0;
	Tf(1,3) = Tf(1,3) / 1000.0;
	Tf(2,3) = Tf(2,3) / 1000.0;

	mWorldModel->setMarkerLoc( mMarkerMsgs[i].cam_id,
				   mMarkerMsgs[i].id,
				   Tf );	
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
	p.marker_id = mMarkerMsgs[i].id;

	Eigen::Matrix4d Tmarker = mWorldModel->getMarkerPose(i);
	for( int j = 0; j < 3; ++j ) {
	    for( int k = 0; k < 4; ++k ) {
		p.Tworld_marker[j][k] = Tmarker(j,k);
	    }
	}
	p.visible = mMarkerMsgs[i].visible;

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
