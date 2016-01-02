/**
 * @file camProcessMain.cpp
 * @author: Nehchal J. (nehchal@gatech.edu)
 * Created On: Dec 28, 2015
 */


#include <camProcess.h>

#include "viz/CvTestbed.h"
#include "globalStuff/globalData.h"
#include "globalStuff/optparser.h"
#include "json/json.h"

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
static char doc[] = "Overhead vision system. Camera ID refers to ID given to physical device. Device ID refers to number following 'video' under /dev directory when camera is plugged in via USB.";
/// argp object
static struct argp argp = {options, parse_opt, args_doc, doc};

char gConfigFile[] = "globalStuff/config.json";

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

  /** Setting global data */
  // First get json file
  std::cout<<"Reading global data from "<<gConfigFile<<'\n';
  Json::Value config;
  parseJSONFile(gConfigFile, config);
  
  setGlobalData(config);
  std::cout << "\t * Global data has been initialized.\n";

  /** Initialise GlutViewer and CvTestbed */
  CvTestbed::Instance().SetVideoCallback(videocallback);
  std::cout << "\t * Initialized GlutViewer and CvTestbed. "<< std::endl;

  /** Init parameters and camera */
  alvar::Capture *cap;
  init( devIndex, camIndex, &cap, 
    arguments.opt_verbosity, 
    &quit_signal, 
    arguments.opt_visualize,
    CAM_CHANNEL_NAME[camIndex]);

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
