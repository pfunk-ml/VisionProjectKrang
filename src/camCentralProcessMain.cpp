/**
 * @file camCentralProcessMain.cpp

  This program listens to all the ACH channels to receive marker data sent by 
  each of the camProcess program. There is one instance of camProcess for each 
  camera and each one publishes to separate ACH channel.

  The received marker data contains pose of marker in the respective camera 
  reference frame. This program transforms the pose to the world frame and 
  publishes the data on perception channel. The name of perception channel 
  should be mentioned in configuration file.

  The received coordinates are in cms. The published coordinates are in meters
  and radians.

  This program uses 'world to camera' transform matrices for all the cameras. 
  These matrices are read from the configuration file.

 */

#include "CameraCentralProcess.h"
#include <iostream>

#include <unistd.h>

int main( int argc, char* argv[] ) {

    CameraCentralProcess cameraCentralProcess;

    std::cout << "* Init setup" <<std::endl;
    cameraCentralProcess.initSetup();

    std::cout << "* Setup channels" <<std::endl;
    cameraCentralProcess.setupChannels();

    std::cout << "* Sleep 1 seconds before starting loop"<< std::endl;
    sleep(1);
    
    std::cout << "Start main loop" <<std::endl;
    cameraCentralProcess.mainLoop();

    return 0;
}
