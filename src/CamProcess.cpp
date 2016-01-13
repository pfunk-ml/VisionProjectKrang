/**
 * @function CamProcess.cpp
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

#include <CamProcess.h>

#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <stdint.h>
#include <assert.h>

#include <MarkerDetector.h>
#include <utils.h>
#include "json/json.h"

#include "viz/CvTestbed.h"
#include "viz/Shared.h"

#include "globalStuff/globalData.h"
#include "Object.h"
#include "globalStuff/optparser.h"
#include "BasicFilter2.h"

#include <argp.h> // for command line arguments

using namespace std;

alvar::Camera gCam;

/* Pointer to quit_signal*/
volatile int *gQuit_signal_ptr;

/**< Camera details */
std::string gCalibFilename;

// has size of 3 * (num of objs) [x,y,angle]
std::vector<double*> mMarkerPoses; // poses of AR markers

/* List of marker messages to be sent for each object */
std::vector<MarkerMsg_t> gMarkerMsgs;

/**< Channel to send info */
ach_channel_t gChan_output;

/* Filters for x, y and z of each marker */
std::vector<BasicFilter2> gFilters_x;
std::vector<BasicFilter2> gFilters_y;
std::vector<BasicFilter2> gFilters_z;

static alvar::MarkerDetector<alvar::MarkerData> marker_detector;

struct context {
  int verbosity; // Verbosity level
  bool isVisOn; // is visualization on

} cx; // context

/* Constructor */
CamProcess::CamProcess() {}

/* Destructor */
CamProcess::~CamProcess() {}

/**
 * @function initAchChannel
 * Opens the ACH channel to publish data. If ACH channel doesn't exist, it is
 * created before opening.
 *
 * channelName : [IN] name of the channel to publish data
 * return val  : the channel handler of type ach_channel_t
 */
ach_channel_t CamProcess::initAchChannel( const char* channelName ) {
    
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

bool CamProcess::init( int _devIndex,
           int _camIndex,
           alvar::Capture **_cap,
           int verbosity,
           volatile int *quit_signal_ptr,
           bool vis,
           string channel_name) {
  
  cx.verbosity = verbosity;
  gQuit_signal_ptr = quit_signal_ptr;
  cx.isVisOn = vis;

  BasicFilter2 basicFilter2 = BasicFilter2(10, false);
  basicFilter2.set_default_weights(BasicFilter2::RAMP);

  MarkerMsg_t markerMsg;
  markerMsg.cam_id = _camIndex;

  /** Set the camera index for this process */
  for(int i = 0; i < gConfParams.numObjects; ++i) {
    gMarkerMsgs.push_back(markerMsg);
    gFilters_x.push_back(basicFilter2);
    gFilters_y.push_back(basicFilter2);
    gFilters_z.push_back(basicFilter2);
  }

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
   
  /* Display capture devices (DEBUG) */
  std::cout << "Enumerated Capture Devices:" << std::endl;
  outputEnumeratedDevices(devices, selectedDevice);
  std::cout << std::endl;
  
  /* Create capture object from camera */
  *_cap = CaptureFactory::instance()->createCapture( devices[selectedDevice] );

  // Setup the marker detector
  marker_detector.SetMarkerSize(gConfParams.markerSize, 5, 2.0);

  /* Initialize ACH channel to publish data */
  gChan_output = initAchChannel(channel_name.c_str());

  return true;
}

/* Returns the index of marker ID in gConfParams.markerIDs.
 * Returns -1 if markerID not found. */
int CamProcess::getIndex(int markerID) {
  for(int i = 0; i < gConfParams.numObjects; ++i)
    if(gConfParams.markerIDs[i] == markerID)
      return i;
  return -1;
}

/* Prints the linear and rotational distance between every pair of marker IDs. */
void printDistances() {
  double pose1[4], pose2[4]; /* Poses of the markers */
  double linDist, rotDist; 

  /* Print the distances between markers. Useful to test calibration */
  printf("Distances between markers\n");

  for(int i=0; i < gConfParams.numObjects; ++i)
    for(int j = i+1; j < gConfParams.numObjects; ++j) {
      
      printf(" \t IDs %d and %d: ", gMarkerMsgs[i].marker_id, gMarkerMsgs[j].marker_id);

      if(!gMarkerMsgs[i].visible || !gMarkerMsgs[j].visible){
        printf("[At least one marker not visible.]\n");
        continue;
      }

      /* Get both poses */
      utils_getXYZAng( utils_getDoubleArrAsMat(gMarkerMsgs[i].trans), 
        pose1[0], pose1[1], pose1[2], pose1[3]);
      
      utils_getXYZAng( utils_getDoubleArrAsMat(gMarkerMsgs[j].trans), 
        pose2[0], pose2[1], pose2[2], pose2[3]);

      utils_computeDistances(pose1, pose2, &linDist, &rotDist);

      printf("Linear: %.4f Angular: %.4f\n", linDist, rotDist);
    }
}



/**
 * @function videocallback
 * @brief Repeat at every captured frame
 */
void CamProcess::videocallback( IplImage *_img ) {

  if (*gQuit_signal_ptr) exit(0); // exit cleanly on interrupt

  // Flip the image if needed
  bool flip_image = (_img->origin ? true : false);
  if (flip_image) {
    cvFlip(_img);
    _img->origin = !_img->origin;
  }

  // Perform detection
  marker_detector.Detect(_img, &gCam, false, cx.isVisOn); // true, true

  for( int i = 0; i < gConfParams.numObjects; ++i ) {  // For each marker

    gMarkerMsgs[i].marker_id = gConfParams.markerIDs[i];

    gMarkerMsgs[i].visible = false;

    for( size_t j=0; j< marker_detector.markers->size(); j++ ) {   

      if(gMarkerMsgs[i].marker_id == (*(marker_detector.markers))[j].GetId()) {
        
      	alvar::Pose p = (*(marker_detector.markers))[j].pose;
      	double transf[16];
      	p.GetMatrixGL(transf, false);

      	/* Set message. transf[0-3] is first col of transformation matrix and 
        so on. */
        for(int k=0; k<16; k++)
          gMarkerMsgs[i].trans[k%4][k/4] = transf[k];

      	gMarkerMsgs[i].visible = true;
      	break;
      }    
    } // end of all markers checked

    // Apply the filter to smooth the values. Currenty applying on to 
    // x, y and z dimension and not to rotation values.
    gMarkerMsgs[i].trans[0][3] = gFilters_x[i].getEstimate(gMarkerMsgs[i].trans[0][3]);
    gMarkerMsgs[i].trans[1][3] = gFilters_y[i].getEstimate(gMarkerMsgs[i].trans[1][3]);
    gMarkerMsgs[i].trans[2][3] = gFilters_z[i].getEstimate(gMarkerMsgs[i].trans[2][3]);

  } // end for all objects

    // Put image back if it was flipped
  if (flip_image) {
    cvFlip(_img);
    _img->origin = !_img->origin;
  }

  /* If filters haven't received sufficient data to make reasonable estimates,
   then don't print of publish the poses. Checking only for one of the filters.
   */
  if(! gFilters_x[0].isSufficientDataReceived())
    return;

  /* Print marker details and distance between markers */
  if(cx.verbosity) {
    printf("----\nMARKER Poses\n");
    for (int i=0; i < gConfParams.numObjects; ++i) 
      Object_printMarkerMsgSingleLine(&gMarkerMsgs[i]);
    
    printDistances();
  }

  // Convert gMarkerMsgs to pointer
  MarkerMsg_t gMarkerMsgsPtr[gConfParams.numObjects];
  for (int i = 0; i < gConfParams.numObjects; i++){
    gMarkerMsgsPtr[i] = gMarkerMsgs[i];
  }

  /* Publish markers poses to ACH channel */
  ach_put( &gChan_output, gMarkerMsgsPtr, sizeof( gMarkerMsgsPtr ) );
}
