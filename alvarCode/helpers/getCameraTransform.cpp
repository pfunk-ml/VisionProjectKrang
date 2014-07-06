/**
 * @function markerDetector.cpp
 */
#include <MarkerDetector.h>

#include "viz/CvTestbed.h"
#include "viz/GlutViewer.h"
#include "viz/Shared.h"

#include "globalStuff/globalData.h"

const int gMarker_size = 20; // 20.3 cm
alvar::Camera gCam;
Drawable d[32];

int gWidth = 640;
int gHeight = 480;

int gObjID;
std::string gCalibFilename;

/** Function declarations */
void videocallback( IplImage *_img );
bool init( int _devIndex, 
	   int _camIndex,
	   alvar::Capture **_cap ); 

/**
 * @function main
 */
int main(int argc, char *argv[]) {
  
  if( argc < 4 ) {
    std::cout << "Syntax: "<< argv[0] << " devX camX  OBJ_ID"<< std::endl;
    return 1;
  }

  /** Setting global data */
  Json::Value config;
  parseJSONFile("/home/kenneth/VisionProjectKrang/alvarCode/globalStuff/config.json", config);
  setGlobalData(config);
  std::cout << "\t * Setting global data done." << std::endl;

  /** Initialise GlutViewer and CvTestbed */
  GlutViewer::Start( argc, argv, gWidth, gHeight );
  CvTestbed::Instance().SetVideoCallback(videocallback);
  std::cout << "\t * Initialized GlutViewer and CvTestbed "<< std::endl;

  /** Get device and camera indices from terminal */
  int devIndex = atoi( argv[1] );
  int camIndex = atoi( argv[2] );
  gObjID = atoi( argv[3] );

  /** Init parameters and camera */
  alvar::Capture *cap;
  init( devIndex, camIndex, &cap);


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
  
  // Draw recognized markers
  GlutViewer::DrawableClear();

  bool detected;
  detected = false;
  for( size_t j=0; j< marker_detector.markers->size(); j++ ) {
    int id = (*(marker_detector.markers))[j].GetId();   

    std::cout << "Detected marker with id: "<< id << std::endl;

    if( gObjID == id ) {
      
      std::cout << "Detected marker with id:"<<gObjID<< std::endl;
      alvar::Pose p = (*(marker_detector.markers))[j].pose;
      double transf[16];
      p.GetMatrixGL( transf, false);
      
      for( int col = 0; col < 4; ++col ) {
	for( int row = 0; row < 4; ++row ) {
	  std::cout << transf[col+row*4] << " ";
	}
	std::cout << std::endl;
      }
      
      std::cout << std::endl;
      
      double r = 1.0 - double(id+1)/32.0;
      double g = 1.0 - double(id*3%32+1)/32.0;
      double b = 1.0 - double(id*7%32+1)/32.0;
      d[j].SetColor(r, g, b);
      
      GlutViewer::DrawableAdd(&(d[j]));
      detected = true;
      break;
    }
      
  } // end of all markers checked

    if( detected == false ) {
      std::cout << "NO detected marker with id "<< gObjID<<"("<<std::endl;
    }

    // Put image back if it was flipped
    if (flip_image) {
      cvFlip(_img);
      _img->origin = !_img->origin;
    }

    usleep(1.0*1e6);
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
  
  /** Set camera matrix into the viewer */
  double p[16];
  gCam.GetOpenglProjectionMatrix( p,
				  gWidth,
				  gHeight );
  
  GlutViewer::SetGlProjectionMatrix(p);
  for (int i=0; i<32; i++) {
    d[i].SetScale( gMarker_size);
  }
  std::cout << "\t * Set Viewer with camera matrix"<<std::endl;
  

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

