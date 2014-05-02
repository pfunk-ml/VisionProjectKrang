alvarCode/globalStuff

Contains all of the stuff that needs to be defined globally.

1. globalData.h / globalData.cpp
--------------------------------------
* If you want to change the number of cameras, change the number of ALVAR tags, change the size of tags, change the ALVAR tags to other patterns or change the names of camera calibration files, the values in these files need to be changed.

* Main things it defines
* NUM_CAMERAS - The number of cameras that it expects to get input from
* NUM_OBJECTS - The number of AR Markers that it expects to be tracking
* vector<string> OBJECT_NAME - The names of the objects each of the AR tags represent
* vector<int> MARKER_ID - The id of the ALVAR marker. If you go to alvarCode/Data/patterns you cans see the ALVAR markers we have available with the ID in the filename "MarkerData_10_<ID>.png." Set these to whatever ALVAR markers you are using
* vector<int> MARKER_SIZE - The physical sizes of each of the markers in cm. The current markers are 20.3 cm.
* vector<string> CAM_CALIB_NAME - The names (relative paths) to the camera calibration files for each camera. The current calibration files are Data/camCalib_<#> with camera numbers 0 to 3.
* vector<string> CAM_CHANNEL_NAME - Names of the ACK channels for each camera. Right now the are called cam<#>_channel with numbers 0 to 3.

2. globalTransforms.h / globalTransforms.cpp
--------------------------------------------------
* If you want to change the camera to origin pose transforms or the sprite transforms, the values here have to be changed.

* Main things it defines
* Matrix4d gTworld_origin;
* gMarker_Origin_ID;
* gCamera_Origin_ID;
* vector<Matrix4d> gTworld_cam;
* vector<Matrix4d> gTmarker_sprite;

3. Object.h
----------------------------------------------------------
* Defines two data structures that get used a lot of places
* ObjectData_t - as far as I can tell, no longer used
* MarkerMsg_t - used to store information ALVAR collects about a tag includes:
  *  int marker_id - ALVAR id
  *  double trans[3][4] - affine tranformation (camera to marker)
  *  int visible - if the marker is "visible" to the camera
  *  int cam_id - the camera corresponding with the 
