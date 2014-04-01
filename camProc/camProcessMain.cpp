/**
 * @file camProcessMain.cpp
 */
#include <GL/gl.h>
#include <GL/glut.h>

#include <iostream>
#include "CameraProcess.h"



int main( int argc, char* argv[] ) {

  // 0. Declare needed variables
  CameraProcess camProc;
  std::string ARTK_Config;

  // 1. Read argv[1] 
  if( argc == 1 ) {
    std::cout << "Usage: "<< argv[0] << " ARTOOLKIT_CONFIG_NAME"<< std::endl;
    return 1;
  }
  
  ARTK_Config = std::string( argv[1] );
  camProc.setup( ARTK_Config );

  // 2. Loop
  glutInit( &argc, argv );
  camProc.init();
  camProc.start();

  return 0;

}
