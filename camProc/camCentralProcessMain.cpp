/**
 * @file camCentralProcessMain.cpp
 */
#include "CameraCentralProcess.h"
#include <iostream>

int main( int argc, char* argv ) {

    CameraCentralProcess mCc;
    std::cout << "Init setup" <<std::endl;
    mCc.initSetup();
    std::cout << "Setup channels" <<std::endl;
    mCc.setupChannels();
    std::cout << "Sleep 3 seconds before starting loop"<< std::endl;
    sleep(3);
    std::cout << "Start main loop" <<std::endl;
    mCc.mainLoop();

    return 0;

}
