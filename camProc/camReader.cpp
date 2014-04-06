/**
 * @file camReader.cpp
 */
#include <AR/gsub.h>
#include <AR/video.h>
#include <AR/param.h>
#include <AR/ar.h>

#include "globalData.h"
#include "CameraProcess.h"
#include "Object.h"



#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <stdint.h>
#include <assert.h>
#include <ach.h>

#include <iostream>

/** Global variable declaration */
ObjectData_t gObjects[NUM_OBJECTS] = {
    { OBJ0_PATT_NAME, -1, OBJ0_MODEL_ID, 0, OBJ0_SIZE, {0.0,0.0}, 0 },
    { OBJ1_PATT_NAME, -1, OBJ1_MODEL_ID, 0, OBJ1_SIZE, {0.0,0.0}, 0 }, 
    { OBJ2_PATT_NAME, -1, OBJ2_MODEL_ID, 0, OBJ2_SIZE, {0.0,0.0}, 0 },
    { OBJ3_PATT_NAME, -1, OBJ3_MODEL_ID, 0, OBJ3_SIZE, {0.0,0.0}, 0 },
    { OBJ4_PATT_NAME, -1, OBJ4_MODEL_ID, 0, OBJ4_SIZE, {0.0,0.0}, 0 },
    { OBJ5_PATT_NAME, -1, OBJ5_MODEL_ID, 0, OBJ5_SIZE, {0.0,0.0}, 0 },
    //{ OBJ6_PATT_NAME, -1, OBJ6_MODEL_ID, 0, OBJ6_SIZE, {0.0,0.0}, 0 }
};

///******************////
// SET BEFORE USING
int gVIDEO_CHANNEL = 2;
char gCAMERA_CALIB_NAME[200] = "Data/cam1_calib.yaml";
int gOBJECT_OBSERVED_INDEX = 0;
///******************////

/**< Camera details */
CamData gCam;

/**< Static functions */
void initCam();
static void init(void);
static void cleanup(void);
static void keyEvent( unsigned char key,
		      int x, int y );
static void mainLoop(void);
static void draw( int object,
		  double trans[3][4] );

/**
 * @function main
 */
int main( int argc, char* argv[] ) {

  
  /** Set ENVIRONMENT variable for ARToolkit */
  char arg[400];
  sprintf( arg, ARTOOLKIT_DEFAULT_CONFIG, gVIDEO_CHANNEL );
  
  std::cout << "ARTOOLKIT CONFIG: "<< arg << std::endl;
  
  setenv( "ARTOOLKIT_CONFIG", arg, 1 );


    /**< Call this first */
    glutInit( &argc, argv );

    /**< Init the process */
    initCam();    
    init();


    /**< Start the capture */
    arVideoCapStart();

    argMainLoop( NULL, keyEvent, mainLoop );

    return 0;
}

/*******************************
 * FUNCTION IMPLEMENTATIONS
 *******************************/

/**
 * @function initCam
 * @brief Initializes some camera details
 */
void initCam() {
    gCam.thresh = 100;
    gCam.count = 0;
    gCam.cparam_name = gCAMERA_CALIB_NAME;
}


/**
 * @function init
 * @brief Initializes "stuff" required
 */
static void init( void ) {

    ARParam wparam;
    int i;

    /**< open the video path */
    if( arVideoOpen( "" ) < 0 ) exit(0);

    /**< find the size of the window */
    if( arVideoInqSize(&gCam.dimX, &gCam.dimY) < 0 ) exit(0);
    printf( "Image size (x,y) = (%d,%d)\n", gCam.dimX, gCam.dimY );

    /* set the initial camera parameters */
    // Load the parameters
    if( !parseYAMLCalibration( gCam.cparam_name,
			       wparam ) ) {
      std::cout << "Camera parameters YAML Load error"<<std::endl;
      exit(0);
    }


    arParamChangeSize( &wparam, gCam.dimX, gCam.dimY, &gCam.cparam );
    arInitCparam( &gCam.cparam );
    printf("*** Camera Parameter ***\n");
    arParamDisp( &gCam.cparam );

    for( i = 0; i < NUM_OBJECTS; i++ ) {
      if( ( gObjects[i].patt_id = arLoadPatt(gObjects[i].patt_name) ) < 0 ) {
	printf("Pattern load error: %s\n", gObjects[i].patt_name);
	exit(0);
      }
    }
    
    /* Open the graphics window */
    argInit( &gCam.cparam, 1.0, 0, 0, 0, 0 );
}



/**
 * @function keyEvent
 * @brief Stop camera stream if ESC is pressed
 */
static void   keyEvent( unsigned char key, 
			int x, int y ) {
  
  /* Quit if the ESC key is pressed */
  if( key == 0x1b ) {
    printf("*** %f (frame/sec)\n", (double) gCam.count/arUtilTimer());
    cleanup();
    exit(0);
  }
}

/**
 * @function  mainLoop 
 */
static void mainLoop(void) {

    ARUint8         *dataPtr;
    ARMarkerInfo    *marker_info;
    int             marker_num;
    int             i, j, k;

    /** Grab a vide frame */
    if( (dataPtr = (ARUint8 *)arVideoGetImage()) == NULL ) {
        arUtilSleep(2);
        return;
    }

    /** Augment the counter */
    if( gCam.count == 0 ) arUtilTimerReset();
    gCam.count++;

    /** Display image */
    argDrawMode2D();
    argDispImage( dataPtr, 0,0 );

    /* Detect the markers in the video frame */
    if( arDetectMarker( dataPtr, gCam.thresh, 
			&marker_info, &marker_num) < 0 ) {
        cleanup();
        exit(0);
    }

    /** Set next capture */
    arVideoCapNext();

    /** Draw a geometric figure for the markers */
    argDrawMode3D();
    argDraw3dCamera( 0, 0 );
    glClearDepth( 1.0 );
    glClear(GL_DEPTH_BUFFER_BIT);

    /* check for object visibility */
    for( i = 0; i < NUM_OBJECTS; i++ ) {
        k = -1;
        for( j = 0; j < marker_num; j++ ) {
            if( gObjects[i].patt_id == marker_info[j].id ) {
                if( k == -1 ) k = j;
                else if( marker_info[k].cf < marker_info[j].cf ) k = j;
            }
        }
	
	if( k >= 0 ) { 
	    gObjects[i].visible = true; 
	}
	else { gObjects[i].visible = false; }
        

        if( k >= 0 ) {
            arGetTransMat(&marker_info[k],
                          gObjects[i].center, gObjects[i].width,
                          gObjects[i].trans);
            draw( gObjects[i].model_id, gObjects[i].trans );
        }
    }

    std::cout << "TRANSFORMATION OF OBJECT "<< gOBJECT_OBSERVED_INDEX<<std::endl;
    for( int i = 0; i < 3; ++i ) {
      for( int j = 0; j < 4; ++j ) {
	std::cout << gObjects[gOBJECT_OBSERVED_INDEX].trans[i][j]<<" ";
      }
      std::cout << std::endl;
    }

    usleep(1e6);

    argSwapBuffers();
    

}


/** 
 * @function cleanup 
 * @brief Function called when program exits 
 */
static void cleanup(void) {
    
    arVideoCapStop();
    arVideoClose();
    argCleanup();
}

/**
 * @function draw
 */
static void draw( int _object, double _trans[3][4] ) {

    double    gl_para[16];
    GLfloat   mat_ambient[]     = {0.0, 0.0, 1.0, 1.0};
    GLfloat   mat_flash[]       = {0.0, 0.0, 1.0, 1.0};
    GLfloat   mat_flash_shiny[] = {50.0};
    GLfloat   light_position[]  = {100.0,-200.0,200.0,0.0};
    GLfloat   ambi[]            = {0.1, 0.1, 0.1, 0.1};
    GLfloat   lightZeroColor[]  = {0.9, 0.9, 0.9, 0.1};
    
    argDrawMode3D();
    argDraw3dCamera( 0, 0 );
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    
    /* load the camera transformation matrix */
    argConvGlpara( _trans, gl_para);
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixd( gl_para );

    switch( _object ) {
      case 0:
	mat_ambient[0] = 0.0; mat_ambient[1] = 0.0; mat_ambient[2] = 1.0;
	mat_flash[0] = 0.0; mat_flash[1] = 0.0; mat_flash[2] = 1.0;
        break;
      case 1:
	mat_ambient[0] = 1.0; mat_ambient[1] = 0.0; mat_ambient[2] = 0.0;
	mat_flash[0] = 1.0; mat_flash[1] = 0.0; mat_flash[2] = 0.0;
        break;
      case 2:
	mat_ambient[0] = 0.0; mat_ambient[1] = 1.0; mat_ambient[2] = 0.0;
	mat_flash[0] = 0.0; mat_flash[1] = 1.0; mat_flash[2] = 0.0;
        break;
      case 3:
	mat_ambient[0] = 1.0; mat_ambient[1] = 1.0; mat_ambient[2] = 0.0;
	mat_flash[0] = 1.0; mat_flash[1] = 1.0; mat_flash[2] = 0.0;
        break;
      case 4:
	mat_ambient[0] = 0.0; mat_ambient[1] = 1.0; mat_ambient[2] = 1.0;
	mat_flash[0] = 0.0; mat_flash[1] = 1.0; mat_flash[2] = 1.0;
        break;
      case 5:
	mat_ambient[0] = 1.0; mat_ambient[1] = 0.0; mat_ambient[2] = 1.0;
	mat_flash[0] = 1.0; mat_flash[1] = 0.0; mat_flash[2] = 1.0;
        break;
      case 6:
	mat_ambient[0] = 1.0; mat_ambient[1] = 1.0; mat_ambient[2] = 1.0;
	mat_flash[0] = 1.0; mat_flash[1] = 1.0; mat_flash[2] = 1.0;
        break;
    }

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
