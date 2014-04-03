/**
 * @file CameraCentralProcess.h
 */
#include "CameraCentralProcess.h"

#include <ach.h>

#include "../globalData.h"


/**
 * @function CameraCentralProcess
 * @brief
 */
CameraCentralProcess::CameraCentralProcess() {
  mCamChannelData.resize( NUM_CAMERAS );
}

/**
 * @function 
 * @brief
 */
CameraCentralProcess::~CameraCentralProcess() {
}

/**
 * @function 
 * @brief
 */
bool CameraCentralProcess::initChannels() {

  std::cout << "Initializing channels" << std::endl;
  int r; int counter; std::string name;

	counter = 0;
	for( int i = 0; i < NUM_OBJECTS; ++i ) {

	    switch(i) {
	    case 0 : name = CAM0_CHANNEL; break;
	    case 1 : name = CAM1_CHANNEL; break;
	    case 2 : name = CAM2_CHANNEL; break;
	    case 3 : name = CAM3_CHANNEL; break;
	    };
	    
	    ach_channel_t chan;
	    r = ach_open( &chan, name.c_str(), NULL );
	    if( ACH_OK == r ) {
		mCam_Channels.push_back( chan );
		counter++; 
		std::cout << "Opened successfully channel "<<name << std::endl;
	    } else {
		std::cout << "Channel "<< name << " not opened "<< std::endl;
	    }	    
	}

  
}

/**
 * @function 
 * @brief
 */
void CameraCentralProcess::mainLoop() {

}

/**
 * @function 
 * @brief
 */
void CameraCentralProcess::grabChannelsInfo() {
}

/**
 * @function 
 * @brief
 */
void CameraCentralProcess::getWorldTransform() {
}

/**
 * @function 
 * @brief
 */
void CameraCentralProcess::sendMessage() {

}
