1. camProcess.cpp - the single camera process program
---------------------------------------------------------
* This is the main program that you run to get the raw ALVAR transformations. The binary for this file should be in bin and called camProcess. You run one process of the main program in this file for every camera. 

* Arguments
./camProcess devX camX
the first argument is the /dev/videoX to which the camera is plugged in. camX indicates the camera index, which is used to call the correct calibration file.

* functions
   * main - initializes global data, calls inits
   * videocallback - detects ALVAR markers and saves the poses and other info to gMarkerMsgs. Outputs this to ach.
   * init - initializes ALVAR, camera, etc.
   * initAchChannel - inits ach
