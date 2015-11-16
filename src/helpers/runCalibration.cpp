/**
 * @file runCalibration.cpp
 * @brief
 */
#include <Camera.h>

#include <iostream>
 #include <stdlib.h>
#include "viz/CvTestbed.h"
#include "viz/Shared.h"



/* ======== START OF PARAMETERS ========*/

// Number of captures needed to perform the calibration
const int gCalib_count_max = 15;

// Size of marker's edge in cms.
const int gEtalon_square_size = 6.4; //7.9;

// Number of internal corners in the pattern (rows are always less than columns)
// For 8x8 chessboard, number of internal corners is 7.
const int gEtalon_rows = 6;
const int gEtalon_columns = 8;

/* ======== END OF PARAMETERS ======== */


// Name of calibration file to store the results
std::stringstream gCalibrationFilename;

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

/**
 * @brief Function to be called for every frame in the video stream.
 */
void videocallback( IplImage *_img ) {

    static bool calibrated = false;
    static int calib_count=0;
    static alvar::Camera cam;
    static alvar::ProjPoints pp;
    static int64 prev_tick=0;
    static bool initialized = false;

    if (!initialized) {
	   cam.SetRes( _img->width, _img->height );
	   prev_tick = cvGetTickCount();
	   initialized = true;
    }
  
    bool flip_image = ( _img->origin? true:false );
    if (flip_image) {
        cvFlip(_img);
        _img->origin = !_img->origin;
    }

    assert(_img);
    
    if (!calibrated) {
        // If we have already collected enough data to make the calibration
        // - We are ready to end the capture loop
        // - Calibrate
        // - Save the calibration file
        if ( calib_count >= gCalib_count_max ) {
            std::cout<<"*** Start calibrating process, please wait ***"<<std::endl;
            calib_count = 0;
            cam.Calibrate(pp);
            pp.Reset(); 
            cam.SaveCalib( gCalibrationFilename.str().c_str() );
            std::cout<<"*** Saving calibration: "<< gCalibrationFilename.str() << std::endl;
            std::cout<<"Calibration Complete.\n";
            calibrated = true;
            // exit the program
            exit(0);
        } 
        // If we are still collecting calibration data
        // - For every 1.5s add calibration data from detected 7*9 chessboard (and visualize it if true)
        else {
            int64 tick = cvGetTickCount();
            if( (tick - prev_tick) > (cvGetTickFrequency() * 1000 * 1000 * 1.5) ) {
                if( pp.AddPointsUsingChessboard( _img, gEtalon_square_size, 
						 gEtalon_rows, gEtalon_columns, true)) {
                    prev_tick = tick;
                    calib_count++;
		    std::cout << "Calibration captures: "<< calib_count << "/" << gCalib_count_max << std::endl;
                }
            }
        }
	
	// If already calibrated - NOT SURE WHAT THIS DOES
    } else {
        if ( pp.AddPointsUsingChessboard( _img, gEtalon_square_size, 
					  gEtalon_rows, gEtalon_columns, true) ) {
	    alvar::Pose pose;
            cam.CalcExteriorOrientation( pp.object_points, pp.image_points, &pose );
            cam.ProjectPoints( pp.object_points, &pose, pp.image_points );
            for (size_t i=0; i < pp.image_points.size(); i++ ) {
                cvCircle( _img, cvPoint( (int)pp.image_points[i].x, 
					 (int)pp.image_points[i].y), 
			  6, CV_RGB(0, 0, 255) );
            }
            pp.Reset();
        }
    }
    
    if (flip_image) {
        cvFlip(_img);
        _img->origin = !_img->origin;
    }

    if (quit_signal) exit(0); // exit cleanly on interrupt
}

/**
 * @function main
 */
int main( int argc, char *argv[] ){
    
    int devIndex = argc > 1 ? atoi(argv[1]) : 0;

    // Register the interrupt handler
    #ifdef __unix__
        signal(SIGINT,quit_signal_handler); // listen for ctrl-C
        signal(SIGTERM,quit_signal_handler); // if pkill or kill command is used
    #endif

    try {

        // Initialise CvTestbed
        CvTestbed::Instance().SetVideoCallback( videocallback );

        // Enumerate possible capture plugins
        CaptureFactory::CapturePluginVector plugins = CaptureFactory::instance()->enumeratePlugins();
        if (plugins.size() < 1) {
            std::cout << "[FATAL] Could not find any capture plugins." << std::endl;
            return 0;
        }

        // Display capture plugins
        std::cout << "[DEBUG] Available Plugins: " << std::endl;
        outputEnumeratedPlugins(plugins);
        std::cout << std::endl;
	
        // Enumerate possible capture devices
        CaptureFactory::CaptureDeviceVector devices = CaptureFactory::instance()->enumerateDevices();
        if (devices.size() < 1) {
            std::cout << "Could not find any capture devices." << std::endl;
            return 0;
        }

        
        // Display capture device (refers to /dev/videoX)
    	int selectedDevice;
    	std::cout << "Num available devices: "<< devices.size() << std::endl;
    	for( int i = 0; i < devices.size(); ++i ) {
    	  std::cout << "Device ["<<i<<"] ID: "<< devices[i].id() << 
    	    " name:"<< devices[i].uniqueName() << std::endl;
    	}
    	if( devIndex < devices.size() ) {
    	  selectedDevice = devIndex;
    	  std::cout << "Selected device ["<<devIndex<<"] ID: "<< devices[devIndex].id() << 
    	    " name:"<< devices[devIndex].uniqueName() << std::endl;
    	} 
        else {
    	  selectedDevice = 0;
    	  std::cout << "Setting default device to ZERO. Probably wrong"<< std::endl;
    	}
        
        // Create capture object from camera
        Capture *cap = CaptureFactory::instance()->createCapture(devices[selectedDevice]);
        std::string uniqueName = devices[selectedDevice].uniqueName();

        // Handle capture lifecycle and start video capture
        // Note that loadSettings/saveSettings are not supported by all plugins
        if (cap) {
            std::stringstream settingsFilename;
            settingsFilename << "camera_settings_" << uniqueName << ".xml";
            gCalibrationFilename << "camCalib_" << uniqueName << ".xml";
            
            cap->start();
            cap->setResolution(640, 480);
            
            if (cap->loadSettings(settingsFilename.str())) {
                std::cout << "Loading settings: " << settingsFilename.str() << std::endl;
            }

            std::stringstream title;
            title << "SampleCamCalib (" << cap->captureDevice().captureType() << ")";

            CvTestbed::Instance().StartVideo(cap, title.str().c_str());

            if (cap->saveSettings(settingsFilename.str())) {
                std::cout << "Saving settings: " << settingsFilename.str() << std::endl;
            }

            cap->stop();
            delete cap;
        }
        else if (CvTestbed::Instance().StartVideo(0, argv[0])) {
        }
        else {
            std::cout << "Could not initialize the selected capture backend." << std::endl;
        }

        return 0;
    }
    catch (const std::exception &e) {
      std::cout << "Exception: " << e.what() << std::endl;
    }
    catch (...) {
        std::cout << "Exception: unknown" << std::endl;
    }
}
