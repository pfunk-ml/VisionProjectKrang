                 USER MANUAL
        
            Project: Vision Krang
        Author: Nehchal J. (nehchal@gatech.edu)
                Jan 5, 2015

                   Draft 

TABLE OF CONTENTS
    
    Notation
    Description of Camera Rig
    Setting Up Camera Rig
    Connecting to Cameras to Desktop
    Coordinate Frames
    Transforms
    Files and Folders
    Calibration

NOTATION

    1. Position of object in frame A is written as P_A

    2. Transform from frame A to B is represented as T^A_B such that

            P_B = T^A_B * P_A

DESCRIPTION OF CAMERA RIG

    The camera rig consists of wooden frame in the shape of English letter 'H'. There are four cameras attached to the rig as shown below.

    
         CAM1 :-|                         |-: CAM3
                |                         |
                |                         |
                |                         |
                |-------------------------|
                |                         |
                |                         |
                |                         |
         CAM0 :-|                         |-: CAM2


        Fig: Top view of Camera Rig showing poisition of 
             four cameras attached to it. The cameras face
             into the paper.


    WebCam model: Logitech Webcam Pro 9000 2MP HD Webcam w/ Mic


SETTTING UP CAMERA RIG

    Rig has to be suspended from the roof in such a way that all four cameras lie in one horizotal plane and all four cameras face downwards. Ropes or any other means can be used for suspension. It is recommended to tie the ropes in such a way the height can be easily adjusted.

    The height of the rig has to be adjusted such that there is an overlap of one A4 page width between views of two cameras.


CONNECTING THE CAMERAS TO DESKTOP

    The cameras are connected to Desktop via USB port. When camera USB cable in connected to USB port, a new device node is formed in /dev folder with name such as "video0", "video1", etc. To see all device nodes, type following command.
        $ ls /dev

    Same camera may get different device node name every time it is inserted, depending on order the order in which cameras are connected. These device node names may changes after a restart of system. To ensure that the device node name remains same for a particular cameras, udev rules are to be written. See "CameraUdevSetup.md" for details.


COORDINATE FRAMES
    
    They are several frames defined in the system. Each frame is a 2D and position of object or robot is defined by 3-tuple (x, y, theta). Various frames are as follows.

        World Frame (W)
        CamGlobal Frame (Cg): The origin of this frame lies at centre of rectangle formed by 4 cameras.
        Cam0 Frame (C0)
        Cam1 Frame (C1)
        Cam2 Frame (C2)
        Cam3 Frame (C3)

    A coodinate frame is defined by position of origin and orientation of its x and y axis.


TRANSFORMS
    
    When position of object is known in one of the coordinate frame, transforms are used to calculate position of that object in another coordinate frame.

    Each transform from one coordinate frame to another coordinate frame is an affine transformation defined by 3x3 martix.

    Transforms from CamGlobal frame to World frame is defined manually.

    Transform from Camx to CamGlobal frame is calculated during calibration process for each camera.

FILES AND FOLDERS

    The project folders contains following files and sub-folders.

CALIBRATION

    Calibration step calculates the transformation from camera frames to CamGlobal frame.

    Each camera needs to be calibrated. Each camera has single calibration file located in 'Data' folder.
    Currently there are 4 calibration files, one for each of the camera. There is a naming convention for the calibration file name.
        Data/camCalib_0.xml
        Data/camCalib_1.xml
        Data/camCalib_2.xml
        Data/camCalib_3.xml

    Each calibration file contains:
        Intrinsic Matrix (3 x 3 Matrix)
        Distortion (Row vector of length 4)
        Width
        Height

        What does each mean?

    How to generate calibration file?
        See Documentation/Workflow.md

        Where to find global marker?
            Global marker ID is given as ID during the caliration step. Any marker can be used as global marker.

        Which ID corresponds to which marker?
            Folder for markers: Data/patterns
            There are 8 markers. Digit at end of file name, denotes the marker ID.

    Procedure for calibration
        1. Choose any 8 markers in Data/patterns folder as global marker and print it on paper.
        2. Place the marker on the ground such that it lies in the view of all four cameras. In what orientation?
        3. Run getCameraTransform for each camera
            $ ./bin/getCameraTransform x x globalMarkerID
            (replace x by camera ID ang globalMarkerID by actual ID of global marker)
            

    getCameraTransforms outputs the 4x4 homogenous transform matrix.

    runCalibration probably needs some checkboard !
    


