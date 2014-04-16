/**
 * @function camProcess.cpp
 */
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <stdint.h>
#include <assert.h>
#include <ach.h>

#include <MarkerDetector.h>

#include "viz/CvTestbed.h"
#include "viz/Shared.h"

#include "globalStuff/globalData.h"
#include "globalStuff/Object.h"

const int gMarker_size = 20; // 20.3 cm
alvar::Camera gCam;

int gWidth = 640;
int gHeight = 480;

/**< Camera details */
std::string gCalibFilename;


/**< Marker messages to be sent */
MarkerMsg_t gMarkerMsgs[NUM_OBJECTS];

/**< Channel to send info */
ach_channel_t gChan_output;
std::string gChannelName;

/** Function declarations */
void videocallback( IplImage *_img );
bool init( int _devIndex, 
	   int _camIndex,
	   alvar::Capture **_cap ); 
void initAchChannel( int _camIndex );

/**
 * @function main
 */
int main(int argc, char *argv[]) {
  
  if( argc < 3 ) {
    std::cout << "Syntax: "<< argv[0] << " devX camX "<< std::endl;
    return 1;
  }

  /** Get device and camera indices from terminal */
  int devIndex = atoi( argv[1] );
  int camIndex = atoi( argv[2] );

  /** Setting global data */
  setGlobalData();
  std::cout << "\t * Setting global data done." << std::endl;

  /** Set the camera index for this process */
  for( int i = 0; i < NUM_OBJECTS; ++i ) {
    gObjects[i].cam_id = camIndex;
  }

  /** Initialise GlutViewer and CvTestbed */
  CvTestbed::Instance().SetVideoCallback(videocallback);
  std::cout << "\t * Initialized GlutViewer and CvTestbed "<< std::endl;

  
  /** Init parameters and camera */
  alvar::Capture *cap;
  init( devIndex, camIndex, &cap);
  initAchChannel( camIndex );


  // Handle capture lifecycle and start video capture
  if (cap) {

    std::cout << "** Start capture"<< std::endl;
    cap->start();
    cap->setResolution(gWidth, gHeight);
    
    char videoTitle[100];
    sprintf( videoTitle, "Marker Detector for camera %d", camIndex );    
    CvTestbed::Instance().StartVideo(cap, videoTitle );
    
    cap->stop();
    delete cap;
    
  } else {
    std::cout << "[X] Could not initialize the selected capture backend." << std::endl;
    return 1;
  }
  
  return 0;
  
}



/**
 * @function videocallback
 * @brief Repeat at every captured frame
 */
void videocallback( IplImage *_img ) {

  // Flip the image if needed
  bool flip_image = (_img->origin? true:false);
  if (flip_image) {
    cvFlip(_img);
    _img->origin = !_img->origin;
  }
  
  // Setup the marker detector
  static alvar::MarkerDetector<alvar::MarkerData> marker_detector;
  marker_detector.SetMarkerSize(gMarker_size); 

  // Perform detection
  marker_detector.Detect(_img, &gCam, true, true);


  bool detected;
  for( int i = 0; i < NUM_OBJECTS; ++i ) {

      gMarkerMsgs[i].marker_id = gObjects[i].marker_id;
      gMarkerMsgs[i].cam_id = gObjects[i].cam_id;

    detected = false;
    for( size_t j=0; j< marker_detector.markers->size(); j++ ) {
      int id = (*(marker_detector.markers))[j].GetId();   

      if( gObjects[i].marker_id == id ) {

	std::cout << "Detected marker with id:"<<id<< " ("<< gObjects[i].obj_name <<")"<< std::endl;
	alvar::Pose p = (*(marker_detector.markers))[j].pose;
	double transf[16];
	p.GetMatrixGL( transf, false);


	// Set message
	gMarkerMsgs[i].trans[0][0] = transf[0];
	gMarkerMsgs[i].trans[0][1] = transf[4];
	gMarkerMsgs[i].trans[0][2] = transf[8];
	gMarkerMsgs[i].trans[0][3] = transf[12];

	gMarkerMsgs[i].trans[1][0] = transf[1];
	gMarkerMsgs[i].trans[1][1] = transf[5];
	gMarkerMsgs[i].trans[1][2] = transf[9];
	gMarkerMsgs[i].trans[1][3] = transf[13];

	gMarkerMsgs[i].trans[2][0] = transf[2];
	gMarkerMsgs[i].trans[2][1] = transf[6];
	gMarkerMsgs[i].trans[2][2] = transf[10];
	gMarkerMsgs[i].trans[2][3] = transf[14];

	for( int row = 0; row < 3; ++row ) {
	  for( int col = 0; col <4; ++col ) {
	    std::cout << gMarkerMsgs[i].trans[row][col] << " "; 
	  }  std::cout << std::endl;
	} 
	std::cout << transf[3] << " "<< transf[7]<<" "<< transf[11]<< " "<< transf[15] << std::endl;
	std::cout << std::endl;
	gMarkerMsgs[i].visible = 1;
 
	detected = true;
	break;
      }
      
    } // end of all markers checked

    if( detected == false ) {
      std::cout << "NO detected marker with id "<< gObjects[i].marker_id<<"("<< gObjects[i].obj_name << ")"<<std::endl;
      for( int a = 0; a < 3; ++a ) {
        for( int b = 0; b < 4; ++b ) {
          gMarkerMsgs[i].trans[a][b] = 0;
        }
      }
      gMarkerMsgs[i].visible = -1;
    }

  } // end for all objects

  // Put image back if it was flipped
  if (flip_image) {
    cvFlip(_img);
    _img->origin = !_img->origin;
  }

    /**< Send objects state to channel */
    ach_put( &gChan_output,
	     gMarkerMsgs,
	     sizeof( gMarkerMsgs ) );

}


/** 
 * @function init
 * @brief Initialize required parameters
 */
bool init( int _devIndex, 
	   int _camIndex,
	   alvar::Capture **_cap ) {
  
  std::cout << "Reading /dev/video"<<_devIndex<<" and camera "<<_camIndex<< std::endl;  
  gCalibFilename = CAM_CALIB_NAME[_camIndex];

  /** Load calibration file */
  std::cout<<"** Loading calibration file: "<< gCalibFilename << std::endl;
  if ( gCam.SetCalib( gCalibFilename.c_str(), 
		      gWidth, gHeight) ) {
    std::cout<<"\t Loaded camera calibration file successfully"<<std::endl;
  } else {
    gCam.SetRes( gWidth, gHeight );
    std::cout<<"\t Failed to load camera calibration file"<<std::endl;
  }

  /** Create capture object from camera */
  
  /*-- Enumerate possible capture plugins --*/
  alvar::CaptureFactory::CapturePluginVector plugins = alvar::CaptureFactory::instance()->enumeratePlugins();
  if (plugins.size() < 1) {
    std::cout << "\t Could not find any capture plugins." << std::endl;
    return 0;
  }
  
  /*-- Display capture plugins --*/
  std::cout << "Available Plugins: ";
  outputEnumeratedPlugins(plugins);
  std::cout << std::endl;
  
  /*-- Enumerate possible capture devices --*/
  alvar::CaptureFactory::CaptureDeviceVector devices = alvar::CaptureFactory::instance()->enumerateDevices();
  if (devices.size() < 1) {
    std::cout << "\t [X] Could not find any capture devices." << std::endl;
    return 0;
  }
  
  /*-- Check if _devIndex can be used --*/
  int selectedDevice;
  if( _devIndex > devices.size() ) {
    std::cout << "Device index /dev/video"<<_devIndex<<" bigger than the size of devices vector." << std::endl;
    return false;
  } 
  
  selectedDevice = _devIndex;
  
  
  /**- Display capture devices (DEBUG) --*/
  std::cout << "Enumerated Capture Devices:" << std::endl;
  outputEnumeratedDevices(devices, selectedDevice);
  std::cout << std::endl;
  
  /*-- Create capture object from camera --*/
   *_cap = CaptureFactory::instance()->createCapture( devices[selectedDevice] );

  return true;
}


/**
 * @function initAchChannel
 */
void initAchChannel( int _camIndex ) {
    
    int r;

    /**< Create output channel, try deleting in case it exists */
    gChannelName = CAM_CHANNEL_NAME[ _camIndex ];
	
    // NOW WE DO THIS ON THE SCRIPT FILE 
    /*
    r = ach_unlink( gChannelName.c_str() );
    assert( ACH_OK == r || ACH_ENOENT == r );

    // TODO : CHECK THESE 10 AND 256 NUMBERS
    r = ach_create( gChannelName.c_str(), 10, 64, NULL );
    assert( ACH_OK == r );
    // Set mode
    mode_t mode = 0666;
    ach_chmod( &gChan_output, mode );
    */

    /**< Open the channel */
    r = ach_open( &gChan_output, gChannelName.c_str(), NULL );
    assert( ACH_OK == r );

    std::cout << "\t Created channel: "<< gChannelName<< std::endl;
}

