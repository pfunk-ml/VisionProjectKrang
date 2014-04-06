camProc
=========

Requirements
-------------

* ach for inter-process communication
* yaml-cpp version 0.5.1 (not the one in Ubuntu 12.04, that is as old as 0.2 and we need the newest version)

HOWTO
------

1. Get the transformations of the cameras w.r.t. the world. Use ./camReader for this purpose.
   Before running camReader open the file and change :
   * gVIDEO_CHANNEL = X // X from /dev/X
   * gCAMERA_CALIB_NAME = FILENAME // FILENAME IS THE NAME OF THE CALIBRATION FILE TO USE FOR THE CAMERA 
   * gOBJECT_OBSERVED_INDEX = X // WHAT OBJECT YOU WILL TRACK. Names of each index are in "globalData.h"
