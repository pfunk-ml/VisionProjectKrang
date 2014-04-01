/**
 * @file cameraProcess.cpp
 */
#include <GL/gl.h>
#include <GL/glut.h>

#include <AR/gsub.h>
#include <AR/video.h>
#include <AR/ar.h>

#include <iostream>
#include <functional>
#include "CameraProcess.h"

using namespace std::placeholders;

void KEYEvent( unsigned char _key,
				     int _x,
				     int _y ) {
    
    if( _key == 0x1b ) {
        //printf("*** %f (frame/sec)\n", (double)mCount/arUtilTimer());
        //cleanup();
        exit(0);
    }

}

/**
 * @function CameraProcess
 */
CameraProcess::CameraProcess() {

  mCount = 0;
  mPattern_width = 80.0;
  mPattern_center[0] = 0.0; mPattern_center[1] = 0.0;
  mParam_filename = "Data/camera_para.dat";

}

/**
 * @function ~CameraProcess
 */
CameraProcess::~CameraProcess() {

}

/**
 * @function setup
 */
bool CameraProcess::setup( const std::string &_ARTK_CONFIG ) {

  mARTK_VariableName = _ARTK_CONFIG;

  setenv( "ARTOOLKIT_CONFIG", mARTK_VariableName.c_str(), 1 );
  
  

}

bool CameraProcess::init() {

  ARParam wparam;
  
  /** Open the video path */
  if( arVideoOpen("") < 0 ) { exit(0); }
  
  /** Find the size of the window */
  if( arVideoInqSize( &mDimX, &mDimY ) < 0 ) { exit(0); }
  
  std::cout <<"Image size: "<< mDimX <<","<<mDimY<<std::endl;

  /** Set camera initial parameters */
  if( arParamLoad( mParam_filename.c_str(),
		   1, &wparam ) < 0 ) {
    std::cout << "Camera parameters not loaded correctly" << std::endl;
    exit(0);
  }

  arParamChangeSize( &wparam, mDimX, mDimY, &mCParam );
  arInitCparam( &mCParam );
  
  std::cout << "Camera parameters"<< std::endl;
  arParamDisp( &mCParam );

  /**< Load patterns */
  if( mPatternId = arLoadPatt( mPatternName.c_str() ) < 0 ) {
    std::cout << "Pattern "<< mPatternName << " load error" << std::endl;
    }

  /**< Open the graphics window */
  argInit( &mCParam, 1.0, 0, 0, 0, 0 );
}

/**
 * @function start
 */
bool CameraProcess::start() {
  arVideoCapStart();
  argMainLoop( NULL, keyEvent, NULL );// keyEvent mainLoop
 }

/**
 * @function keyEvent
 * @brief Quit if the ESC key is pressed
 */
void CameraProcess::keyEvent( unsigned char _key,
				     int _x,
				     int _y ) {
    
    if( _key == 0x1b ) {
        printf("*** %f (frame/sec)\n", (double)mCount/arUtilTimer());
        cleanup();
        exit(0);
    }

}

/* main loop */
void CameraProcess::mainLoop(void) {

  ARUint8 *dataPtr;
  ARMarkerInfo *marker_info;
  int marker_num;
  int j, k;

  /* grab a vide frame */
  if( (dataPtr = (ARUint8 *)arVideoGetImage()) == NULL ) {
    arUtilSleep(2);
    return;
  }
  
  if( mCount == 0 ) arUtilTimerReset();
  mCount++;

  argDrawMode2D();
  argDispImage( dataPtr, 0,0 );

  /* Detect the markers in the video frame */
  if( arDetectMarker(dataPtr, mThresh, &marker_info, &marker_num) < 0 ) {
    cleanup();
    exit(0);
  }
  
  arVideoCapNext();

  /* Check for object visibility */
  k = -1;
  for( j = 0; j < marker_num; j++ ) {
    if( mPatternId == marker_info[j].id ) {
      if( k == -1 ) k = j;
      else if( marker_info[k].cf < marker_info[j].cf ) k = j;
    }
  }
  if( k == -1 ) {
    argSwapBuffers();
    return;
  }
  
  /* Get the transformation between the marker and the real camera */
  arGetTransMat( &marker_info[k], 
		 mPattern_center, 
		 mPattern_width,
		 mPattern_trans );

  draw();
  
  argSwapBuffers();
}

/* cleanup function called when program exits */
void CameraProcess::cleanup(void) {

    arVideoCapStop();
    arVideoClose();
    argCleanup();
}

/**
 * @function draw
 */
void CameraProcess::draw( void ) {

  double    gl_para[16];
  GLfloat   mat_ambient[]     = {0.0, 0.0, 1.0, 1.0};
  GLfloat   mat_flash[]       = {0.0, 0.0, 1.0, 1.0};
  GLfloat   mat_flash_shiny[] = {50.0};
  GLfloat   light_position[]  = {100.0,-200.0,200.0,0.0};
  
  GLfloat   ambi[]            = {0.1, 0.1, 0.1, 0.1};
  GLfloat   lightZeroColor[]  = {0.9, 0.9, 0.9, 0.1};
  
  argDrawMode3D();
  argDraw3dCamera( 0, 0 );
  glClearDepth( 1.0 );
  glClear(GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  
  /* load the camera transformation matrix */
  argConvGlpara( mPattern_trans, 
		 gl_para );
  glMatrixMode(GL_MODELVIEW);
  glLoadMatrixd( gl_para );
  
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  glLightfv(GL_LIGHT0, GL_AMBIENT, ambi);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, lightZeroColor);
  glMaterialfv(GL_FRONT, GL_SPECULAR, mat_flash);
  glMaterialfv(GL_FRONT, GL_SHININESS, mat_flash_shiny);	
  glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
  glMatrixMode(GL_MODELVIEW);
  glTranslatef( 0.0, 0.0, 25.0 );
  glutSolidCube(50.0);
  glDisable( GL_LIGHTING );
  
  glDisable( GL_DEPTH_TEST );
}


/**
 * @function getTfs
 */
bool CameraProcess::getTfs() {

  return true;
}
