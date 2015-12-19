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

#include <argp.h> // for command line arguments

/* Keep the webcam from locking up when you interrupt a frame capture.
 * This function will be called when SIGINT signal is sent by the OS
 * (when Ctrl-C is pressed). 
 * Refence:
 *  https://lawlorcode.wordpress.com/2014/04/08/opencv-fix-for-v4l-vidioc_s_crop-error/
 */
volatile int quit_signal=0;
#ifdef __unix__
#include <signal.h>
extern "C" void quit_signal_handler(int signum) {
 if (quit_signal!=0) exit(0); // just exit already
 quit_signal=1;
 printf("Will quit at next camera frame\n");
}
#endif

alvar::Camera gCam;

char gConfigFile[] = "globalStuff/config.json";

/**< Camera details */
std::string gCalibFilename;


/**< Marker messages to be sent */
std::vector<MarkerMsg_t> gMarkerMsgs;

/**< Channel to send info */
ach_channel_t gChan_output;

bool gIsVisOn; // is visualization on?

/** Function declarations */
void videocallback( IplImage *_img );

// context struct
typedef struct {
  // set by command line arguments
  int opt_verbosity;
  int opt_devid;
  int opt_camid;
  bool opt_visualize;

} arguments_t;

/* GLOBAL VARIABLES (Ideally, global variables should be as minimum as 
   possible) */
arguments_t arguments = {
  .opt_verbosity = 0,
  .opt_devid = 0,
  .opt_camid = 0,
  .opt_visualize = false
};


// options
static struct argp_option options[] = {
  {"verbose", 'v', 0, 0, "Produce verbose output"},
  {"devid", 'd', "DEVICE_ID", 0, "Device ID"},
  {"camid", 'c', "CAMERA_ID", 0, "Camera ID"},
  {"visualize", 't', 0, 0, "Show visualization"},
  {0}
};

/// parser (fxn to parse single command line argument)
static int parse_opt( int key, char *arg, struct argp_state *state) {
  arguments_t *arguments = (arguments_t*)state->input;
  switch(key) {
  case 'v':
    arguments->opt_verbosity++;
    break;
  case 'd':
    arguments->opt_devid = atoi(arg);
    break;
  case 'c':
    arguments->opt_camid = atoi(arg);
    break;
  case 't':
    arguments->opt_visualize = true;
    break;
  default:
    return ARGP_ERR_UNKNOWN;
  }
  
  //somatic_d_argp_parse( key, arg, &cx->d_opts );
  return 0;
}

/// argp program version
const char *argp_program_version = "Krang Vision 1.0.0";
/// argp program bug address
const char *argp_program_bug_address = "nehchal@gatech.edu";
const char args_doc[] = "";
/// argp program doc line
static char doc[] = "Overhead vision system.";
/// argp object
static struct argp argp = {options, parse_opt, args_doc, doc};

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
  
  /* Enumerate possible capture devices. This give the following error.
      HIGHGUI ERROR: V4L: index 4 is not correct! 
     The error is due to bug in the opencv file CapturePluginHighgui.cpp
     So, nothing can be done about this. Updating opencv may help. */
  alvar::CaptureFactory::CaptureDeviceVector devices;
  devices = alvar::CaptureFactory::instance()->enumerateDevices(plugins.at(1));

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

  //argp_parse(&argp, argc, argv, 0, NULL, &cx);
  argp_parse(&argp, argc, argv, 0, 0, &arguments);

  // Register the interrupt handler
  #ifdef __unix__
    signal(SIGINT,quit_signal_handler); // listen for ctrl-C
    signal(SIGTERM,quit_signal_handler); // if pkill or kill command is used
  #endif

  //if( argc < 4 ) {
  //  std::cout << "Syntax: "<< argv[0] << " devX camX visualization"<< std::endl;
  //  std::cout << "Syntax: "<< argv[0] << "visualization: 0 for off and 1 for on"<< std::endl;
  //  return 1;
  //}

  /** Get device and camera indices from terminal */
  int devIndex = arguments.opt_devid;
  int camIndex = arguments.opt_camid;
  gIsVisOn = arguments.opt_visualize;

  //int devIndex = atoi( argv[1] );
  //int camIndex = atoi( argv[2] );
  //gIsVisOn = atoi( argv[3] );

  /** Setting global data */
  // First get json file
  std::cout<<"Reading global data from "<<gConfigFile<<'\n';
  Json::Value config;
  parseJSONFile(gConfigFile, config);
  
  setGlobalData(config);
  std::cout << "\t * Global data has been initialized.\n";


  /** Set the camera index for this process */
  for( int i = 0; i < NUM_OBJECTS; ++i ) {
    MarkerMsg_t markerMsg;
    markerMsg.cam_id = camIndex;
    gMarkerMsgs.push_back(markerMsg); 
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
  marker_detector.Detect(_img, &gCam, false, gIsVisOn); // true, true

  if(arguments.opt_verbosity)
    printf("----\nMARKER Poses\n");

  bool detected;
  for( int i = 0; i < NUM_OBJECTS; ++i ) {  // For each marker

    gMarkerMsgs[i].marker_id = gConfParams.markerIDs[i];

    detected = false;
    for( size_t j=0; j< marker_detector.markers->size(); j++ ) {
      int id = (*(marker_detector.markers))[j].GetId();   

      if( gMarkerMsgs[i].marker_id == id ) {
        
      	alvar::Pose p = (*(marker_detector.markers))[j].pose;
      	double transf[16];
      	p.GetMatrixGL( transf, false);

      	/* Set message. transf[0-3] is first col of transformation matrix and 
        so on. */
        for(int k=0; k<16; k++)
          gMarkerMsgs[i].trans[k%4][k/4] = transf[k];

      	gMarkerMsgs[i].visible = 1;

      	detected = true;
      	break;
      }
        
    } // end of all markers checked

    if( detected == false ) {
      for( int a = 0; a < 3; ++a ) {
        for( int b = 0; b < 4; ++b ) {
          gMarkerMsgs[i].trans[a][b] = 0;
        }
      }
      gMarkerMsgs[i].visible = -1;
    }

    // Print the marker details
    if(arguments.opt_verbosity)
      Object_printMarkerMsgSingleLine(&gMarkerMsgs[i]);
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

  /**< Send objects state to channel */
  ach_put( &gChan_output, gMarkerMsgsPtr, sizeof( gMarkerMsgsPtr ) );

  if (quit_signal) exit(0); // exit cleanly on interrupt
}
