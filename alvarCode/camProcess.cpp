/**
 * @function camProcess.cpp
 *
    This is a program for a single camera that detects AR marker(s) in the 
    camera view and outputs the marker(s) details to a ACH channel. It reads 
    the marker IDs to be detected from a configuration file.

    The name of the channel to publish the marker details should be mentioned 
    in configuration file and channel must exist before the execution of this 
    program. The details of the message format pushlished are mentioned in the 
    documentation.

    One component of the marker data is the transformation matrix. This 
    transformation matrix denotes pose of marker in the camera reference frame 
    and not in world reference frame.

    This program needs intrinsic parameters of the camera. So, camera should be 
    calibrated beforehand and the intrinsic parameters should be stored in 
    calibration file. The name of calibration is mentioned in the configuration 
    file.

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

#include "json/json.h"
#include "globalStuff/optparser.h"
    
alvar::Camera gCam;

char gConfigFile[] = "globalStuff/config.json";

/**< Camera details */
std::string gCalibFilename;


/**< Marker messages to be sent */
std::vector<MarkerMsg_t> gMarkerMsgs;

/**< Channel to send info */
ach_channel_t gChan_output;

/** Function declarations */
void videocallback( IplImage *_img );

/** 
 * @function init
 * @brief Initialize required parameters
 */
bool init( int _devIndex,
           int _camIndex,
           alvar::Capture **_cap ) {
  
  std::cout << "Reading /dev/video"<<_devIndex<<" and camera "<<_camIndex<< std::endl;
  gCalibFilename = CAM_CALIB_NAME[_camIndex];
  std::cout<<"Line "<<__LINE__<<":\n";

  /* Load calibration file */
  std::cout<<"** Loading calibration file: "<< gCalibFilename << std::endl;
  if ( gCam.SetCalib( gCalibFilename.c_str(), 
          gConfParams.width, gConfParams.height) ) {
    std::cout<<"\t Loaded camera calibration file successfully."<<std::endl;
  } else {
    gCam.SetRes( gConfParams.width, gConfParams.height );
    std::cout<<"\t Failed to load camera calibration file."<<std::endl;
  }
 
  /*-- Enumerate possible capture plugins --*/
  alvar::CaptureFactory::CapturePluginVector plugins;
  plugins = alvar::CaptureFactory::instance()->enumeratePlugins();
  if (plugins.size() < 1) {
    std::cout << "\t Could not find any capture plugins." << std::endl;
    return 0;
  }
  
  /*-- Display capture plugins --*/
  std::cout << "Available Plugins: ";
  outputEnumeratedPlugins(plugins);
  std::cout << std::endl;
  
  std::cout<<"Line:"<<__LINE__<<'\n';
  /* Enumerate possible capture devices. This give the following error.
      HIGHGUI ERROR: V4L: index 4 is not correct! 
     The error is due to bug in the opencv file CapturePluginHighgui.cpp
     So, nothing can be done about this. Updating opencv may help. */
  alvar::CaptureFactory::CaptureDeviceVector devices;
  devices = alvar::CaptureFactory::instance()->enumerateDevices(plugins.at(1));
  std::cout<<"Line "<<__LINE__<<": "<<"Number of devices = "<<devices.size()<<'\n';

  if (devices.size() < 1) {
    std::cout << "\t [X] Could not find any capture devices." << std::endl;
    return 0;
  }

  for( int i = 0; i < devices.size(); ++i ) {
    std::cout << "Device ID: "<< devices[i].id()<< std::endl;
  }
  
  /*-- Check if _devIndex can be used --*/
  int selectedDevice = -1;
  for( int i = 0; i < devices.size(); ++i ) {
    if( _devIndex == atoi( (devices[i].id()).c_str() ) ) {
       selectedDevice = i;
       break;    
    }
  }

  if( selectedDevice == -1 ) {
    std::cout << "Not /dev"<<_devIndex<<" found. Are you sure a camera is connected there?" << std::endl;
    return false;
  } 
  
  
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
 * Opens the ACH channel to publish data. If ACH channel doesn't exist, it is
 * created before opening.
 *
 * channelName : [IN] name of the channel to publish data
 * return val  : the channel handler of type ach_channel_t
 */
ach_channel_t initAchChannel( const char* channelName ) {
    
    enum ach_status r;
  
    /* Create ACH channel */
    r = ach_create( channelName, 10, 512, NULL );
    /* if channel not created and it doesn't exist */
    if( ACH_OK != r && ACH_EEXIST != r) {   
        fprintf( stderr, "Could not create channel: %s\n", ach_result_to_string(r) );
        exit(EXIT_FAILURE);
    }

    /* Open the channel */
    ach_channel_t channel;
    r = ach_open( &channel, channelName, NULL );
    if( r != ACH_OK ) {
      fprintf( stderr, "Could not open channel: %s\n", ach_result_to_string(r) );
      exit(EXIT_FAILURE);
    }
    
    std::cout << "\t Created channel: "<< channelName<< std::endl;
    return channel;
}

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
  // First get json file
  std::cout<<"Reading global data from "<<gConfigFile<<'\n';
  Json::Value config;
  parseJSONFile(gConfigFile, config);
  
  setGlobalData(config);
  std::cout << "\t * Global data has been initialized.\n";

  /** Set the camera index for this process */
  for( int i = 0; i < NUM_OBJECTS; ++i ) {
    gMarkerMsgs.push_back(MarkerMsg_t());
    gObjects[i].cam_id = camIndex;
  }

  /** Initialise GlutViewer and CvTestbed */
  CvTestbed::Instance().SetVideoCallback(videocallback);
  std::cout << "\t * Initialized GlutViewer and CvTestbed. "<< std::endl;

  
  /** Init parameters and camera */
  alvar::Capture *cap;
  init( devIndex, camIndex, &cap);

  /* Initialize ACH channel to publish data */
  gChan_output = initAchChannel(CAM_CHANNEL_NAME[camIndex].c_str());

  // Handle capture lifecycle and start video capture
  if (cap) {

    std::cout << "** Start capture **"<< std::endl;
    cap->start();
    cap->setResolution(gConfParams.width, gConfParams.height);
    
    char videoTitle[100];
    sprintf( videoTitle, "Marker Detector for camera %d", camIndex );    
    CvTestbed::Instance().StartVideo(cap, videoTitle );
    
    cap->stop();
    delete cap;  
  } 
  else {
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
  bool flip_image = (_img->origin ? true : false);
  if (flip_image) {
    cvFlip(_img);
    _img->origin = !_img->origin;
  }
  
  // Setup the marker detector
  static alvar::MarkerDetector<alvar::MarkerData> marker_detector;
  marker_detector.SetMarkerSize(gConfParams.markerSize); 

  // Perform detection
  marker_detector.Detect(_img, &gCam, true, true); // true, true


  bool detected;
  for( int i = 0; i < NUM_OBJECTS; ++i ) {

    gMarkerMsgs[i].marker_id = gObjects[i].marker_id;
    gMarkerMsgs[i].cam_id = gObjects[i].cam_id;

    detected = false;
    for( size_t j=0; j< marker_detector.markers->size(); j++ ) {
      int id = (*(marker_detector.markers))[j].GetId();   

      if( gObjects[i].marker_id == id ) {

      	//std::cout << "Detected marker with id:"<<id<< " ("<< gObjects[i].obj_name <<")"<< std::endl;
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

      	gMarkerMsgs[i].visible = 1;
       
      	detected = true;
      	break;
      }
        
    } // end of all markers checked

    if( detected == false ) {
      //std::cout << "NO detected marker with id "<< gObjects[i].marker_id<<"("<< gObjects[i].obj_name << ")"<<std::endl;
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

  // Convert gMarkerMsgs to pointer
  MarkerMsg_t gMarkerMsgsPtr[NUM_OBJECTS];
  for (int i = 0; i < NUM_OBJECTS; i++){
    gMarkerMsgsPtr[i] = gMarkerMsgs[i];
  }

  // Following cout lines added by Nehchal for debugging. To be removed.
  // std::cout<<"Line "<<__LINE__<<':'<<"size of MarkerMsg_t = "<<sizeof(MarkerMsg_t)<<'\n';
  // std::cout<<"Line "<<__LINE__<<':'<<"size of gMarkerMsgPtr = "<<sizeof(gMarkerMsgsPtr)<<'\n';
  // std::cout<<"Line "<<__LINE__<<':'<<"NUM_OBJECTS = "<<NUM_OBJECTS<<'\n';

  /* Print the marker messages */
  //for(int i=0; i<NUM_OBJECTS; i++)
  //  Object_printMarkerMsg(&gMarkerMsgsPtr[i]);
  //std::cout<<"---\n";

  /**< Send objects state to channel */
  ach_put( &gChan_output, gMarkerMsgsPtr, sizeof( gMarkerMsgsPtr ) );

}
