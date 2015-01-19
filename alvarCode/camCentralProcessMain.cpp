/**
 * @file camCentralProcessMain.cpp

  This program listens to all the ACH channels to receive marker data sent by 
  each of the camProcess program. There is one instance of camProcess for each 
  camera and each one publishes to separate ACH channel.

  The received marker data contains pose of marker in the respective camera 
  reference frame. This program transforms the pose to the world frame and 
  publishes the data on perception channel. The name of perception channel 
  should be mentioned in configuration file.

 */

#include "CameraCentralProcess.h"
#include <iostream>

int main( int argc, char* argv[] ) {

    CameraCentralProcess mCc;
    std::cout << "* Init setup" <<std::endl;
    mCc.initSetup();
    std::cout << "* Setup channels" <<std::endl;
    mCc.setupChannels();
    std::cout << "* Sleep 1 seconds before starting loop"<< std::endl;
    sleep(1);
    std::cout << "Start main loop" <<std::endl;
    mCc.mainLoop();

    return 0;
}
