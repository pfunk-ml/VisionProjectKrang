                 USER MANUAL
        
            Project: Vision Krang
        Author: Nehchal J. (nehchal@gatech.edu)
                Jan 5, 2015

                   Draft 

TABLE OF CONTENTS
    
    . Requirements
    . Dependencies

    . Description of Camera Rig
    . Setting Up Camera Rig

    . AR Markers
    . Files and Folders
    . Connecting the Cameras to Desktop

    . Setting Up
    . Compilation
    . Parameters
    . Calibration
    . Running One Camera
    . Running Multiple Cameras
    . Trouble shooting
    
    . Notation
    . Reference Frames
    . Transforms

REQUIREMENTS

   * ach for inter-process communication
   * yaml-cpp version 0.5.1 (not the one in Ubuntu 12.04, that is as old as 0.2 and we need the newest version)
   * jsoncpp (http://jsoncpp.sourceforge.net/)

DEPENDENCIES

   * ach
   * alvar
   * opencv (tested with 2.4.8)
   * glut
   * opengl
   * eigen

BUILDING AND COMPILING
    
    $ cd path/to/VisionProjectKrang/src
    $ mkdir build
    $ cd build
    $ cmake ..
    $ make

    The executable are created in src/bin folder

RUNNING THE CODE
    
    $ cd path/to/VisionProjectKrang/src
    $ ./visionClient.sh

PARAMETERS
    
    There are several parameters that need to be set. These parameters are used both for calibration of cameras and tracking of AR markers.

    Most parameters are set in globalStuff/config.json .

    This file is read by calibration and tracking programs at the start and global variables are set. No re-compilation is required after making any changes to this file.

    Some parameters need to be set in
        helpers/runCalibration.cpp
        camProcess.cpp

    Remember to re-compile after making changes to above files.

    * Adding new parameters
        // TODO: Here goes the content
    
CALIBRATION

    Calibration step calculates various intrinsic and extrinsic parameters.

    Intrinsic Parameters
    --------------------

    Intrinsic parameters of a camera don't depend on how the camera is mounted. Theoretically, all cameras of same model should have same intrinsic parameters. But, there may be small variations in lens position, sensor position, etc. So, it is preferable calibrate each of the camera separately. 

    Each camera needs to be calibrated. Each camera has single calibration file located in 'Data' folder.
    Currently there are 6 calibration files, one for each of the camera. There is a naming convention for the calibration file.
        Data/camCalib0.xml
        Data/camCalib1.xml
        Data/camCalib2.xml
        Data/camCalib3.xml
        Data/camCalib4.xml
        Data/camCalib5.xml
        Data/camCalib6.xml

    Each calibration file contains:
        Intrinsic Matrix (3 x 3 Matrix)
        Distortion (Row vector of length 4)
        Width
        Height

    Procedure for calibration of single camera for intrinsic parameters.
    
        1. Print checkerboard pattern on paper.
        2. Place the checkerboard such that it can be visible in camera view.
        3. Change parameters of checkerboard in the src/helpers/runCalibration.cpp file.
        4. Compile
                $ cd path/to/src/build
                $ cmake ..
                $ make
        5. Run runCalibration for each camera
            $ cd path/to/src
            $ ./bin/runCalibration x
            
            Replace x by camera ID. For example, for Cam 0
                $./bin/runCalibration 0
        
        6. Move the checkerboard in different directions and change orientations, while the program captures different scenes.
        7. Copy or move the generated calibration file to a Data/ folder named camCalib0.xml for cam 0, camCalib1.xml for cam 1 and so on.


    Extrinsic Parameters
    --------------------

    Note: If intrinsic calibration is done, then it is recommended to do extrinsic calibration as well.

    Extrinsic parameters depend upon the how the camera is mounted. Extrinsic parameters refers to the transformation matrix from world frame to camera frame (Tworld_cam). 

        P_world = Tworld_cam * P_cam

    Each camera has its own matrix. This matrix is defined in configuration file config.json .

    Intrinsic parameters of camera should be calibrated before calibrating extrinsic paramters.

        Procedure for calibration of extrinsic parameters.

            1. Print a global AR marker with some ID on paper.
            2. Place it such that it is completely visible in the view of all cameras.
            3. Get transform for each camera.
                $ cd path/to/src
                $ ./bin/getCameraTransform devX camX  OBJ_ID
                
               For example, for devX = 0, camX = 0 and marker ID = 5
                $ ./bin/getCameraTransform 0 0 5
            4. Copy the output transform matrix into config.json at appropirate location in appropriate format.

        Where to find global marker?
            Global marker ID is given as ID during the caliration step. Any marker can be used as global marker.

        Which ID corresponds to which marker?
            Folder for markers: src/Data/markers_big
                                src/Data/markers_small
            There are 8 markers. Digit at end of file name, denotes the marker ID.


    Python Script has been written to do all above extrinsic calibration. See run_extrinsic_calibration.py file.


RUNNING ONE CAMERA

    TODO: Add Details Here

RUNNING MULTIPLE CAMERAS

    TODO: Add Details Here

DESCRIPTION OF CAMERA RIG

    The camera rig consists of wooden frame in the shape of English letter 'H'. There are four cameras attached to the rig as shown below.

    
         CAM1 :-|             |-: CAM3      |-: CAM5
                |             |             |       
                |             |             |       
                |             |             |       
                |-------------|-------------|       
                |             |             |       
                |             |             |       
                |             |             |       
         CAM0 :-|             |-: CAM2      |-: CAM4


        Fig: Top view of Camera Rig showing position of 
             six cameras attached to it. The cameras face
             into the paper.


    WebCam model: Logitech Webcam Pro 9000 2MP HD Webcam w/ Mic

AR MARKERS
    
    AR Markers are used to track the objects and the robot.

    There are 9 Markers in total with ID 0 to 8. That means maximum 9 objects can be tracked by the vision system.

    Markers are located in .../alvarCode/Data/markers_big/

    Creating the markers
    --------------------
        Markers can be created using program samplemarkercreator available in alvar-2.0.0-bin-linux64-gcc44 .
         $ cd to bin folder
         $ ./samplemarkercreator -s 40.0 2 # to print marker ID 2

        A PNG file will be created.

    Printing the markers
    --------------------
        1. Open PNG in Image Viewer and convert to PDF using 'print to file option'. Use following printing options:
            Pages per side: 1
            Paper size: US Letter
            Orientation: Portrait
            Scale: 100%

          US Letter paper can be found in Mopier printer in RIM lab.

          If markers are to be used for calibration, then use Paper size: A3

        2. Print the PDF on the printer

    Make sure that length of edge of printed marker is 20.3 cm. 

    [ Marker ID 1 is known to perform poorly. Don't use it.
         -- Nehchal, Mar 24, 2015 ]

FILES AND FOLDERS

    The project folders contains following files and sub-folders.

        * alvarCode/ - here is where all the current stuff is. See setup.md, global.md, OneCamera.md, MainProgram.md, Helpers.md, Simulation.md Workflow.md and Data.md for additional information on this code and functionality.
        
            alvarCode has following files and sub-directories.

            globalStuff/ : Configuration parameters.

            Data/Markers/ : All the AR markers pattern images and corresponding xml files. Surprisingly this folder in not used by camProcess during execution. How it gets the pattern design info?

            camProcess.cpp : The program for single camera that tracks AR markers.
        
        Following folders are obsolete. 

            * CameraCalibration/ - data stored for old ros calibration. Ignore - see Helpers.md for up to date information on camera calibration       

            * camProc/ - old code from ar_toolkit. Defunct when we switched to ALVAR which has less error.
        
            * MATLAB/ - had helper code to average Euler angles. Defunct - turns out averaging angles is bad.

SETTTING UP CAMERA RIG

    Rig has to be suspended from the roof in such a way that all four cameras lie in one horizotal plane and all four cameras face downwards. Ropes or any other means can be used for suspension. It is recommended to tie the ropes in such a way the height can be easily adjusted.

    The height of the rig has to be adjusted such that there is an overlap of one A4 page width between views of two cameras.


CONNECTING THE CAMERAS TO DESKTOP

    The cameras are connected to Desktop via USB port. When camera USB cable in connected to USB port, a new device node is formed in /dev folder with name such as "video0", "video1", etc. To see all device nodes, type following command.
        $ ls /dev

    Same camera may get different device node name every time it is inserted, depending on order the order in which cameras are connected. These device node names may changes after a restart of system. To ensure that the device node name remains same for a particular cameras, udev rules are to be written. See "CameraUdevSetup.md" for details.


REFERENCE FRAMES
    
    They are several frames defined in the system. Each frame is a 3D. Various frames are as follows
        
      -- World Frame (W): This is the outermost frame.
            
      -- Camera Frame: The origin of camera frame lies somewhere at the respective camera (consider camera as a point). The direction towards which camera is facing is the positive z-axis.
                Cam0 Frame (C0) 
                Cam1 Frame (C1)
                Cam2 Frame (C2)
                Cam3 Frame (C3)
                Cam4 Frame (C4)
                Cam5 Frame (C5)

                y-axis 
                 ^       
                 |
                 |            
             __  |   
            |  | +------------> z-axis
            |__|

            Camera facing towards right

            Fig: The Cam Frame. Origin is at the 
              camera. x-axis is into the paper.

     -- Marker frame: This frame is attached to each AR marker. Origin lies at center of marker. z-axis comes out the marker plane. x and y-axis are aligned in marker plane.

     -- Global (G): The origin of this frame lies somewhere near the centre of rectangle formed by cameras 0-3. The global marker is placed here during the extrinsic calibration process. So, in that case the marker frame is same as global frame.  

     -- Auxilliary (A): The origin of this frame lies somewhere near the centre of rectangle formed by cameras 2-5 (i.e. the other rectangle).  The auxilliary marker here during the extrinsic calibration process. This tag is necessary for when calibrating the rig when all cameras can't see a single tag for the purpose of computing their extrinsics.  Instead we use a single tag for the first camera rectangle, and then move/add a marker in the second rectangle and obtain the transforms for the remaining cameras.

     We obtain the (inverse) extrinsics of the first rectangle with respect to a marker we'll call "global":
     T_cam0_global  <-- given by runCalibration
     T_cam1_global  <-- given by runCalibration
     T_cam2_global  <-- given by runCalibration
     T_cam3_global  <-- given by runCalibration

     We then obtain the extrinsics of the second rectangle with respect to a marker we'll call "auxilliary" as:
     T_cam4_global = T_cam4_aux * inv(T_cam3_aux) * T_cam3_global
     T_cam5_global = T_cam5_aux * inv(T_cam3_aux) * T_cam3_global
     Note: in this example we chose cam3 from the middle cameras for aux calibration.  We could just as well use cam2, but we prefer a single camera in view of both markers to minimize error.  

     Steps:
     1) Use runCalibration to obtain T_cam[0123]_global and T_cam[345]_aux.
     2) Use getAuxTransforms.py to obtain T_cam[45]_global


     After placing these transforms in config.json, camCentalProcess can obtain each camera's world pose as follows:
     T_world_cam4 = T_world_global * T_global_cam4

     

    A coodinate frame is defined by position of the origin and orientation of its x, y and z axis.

    Pose of object refers to its position and orientation. Transformation of a pose of object from one coordinate frame to another can be specified by a 4x4 affine homogenous matrix.

    Transformation from Camera to Global frame is calculated during extrinsic calibration.

    Transformation from Global frame to world frame is calculated manually.

TRANSFORMS
    
    When position of object is known in one of the coordinate frame, transforms are used to calculate position of that object in another coordinate frame.

    Each transform from one coordinate frame to another coordinate frame is an affine transformation defined by 3x3 martix.

    Transforms from World frame to Origin frame is defined manually.

    Transform from CamX to world frame is calculated during calibration process for each camera.



