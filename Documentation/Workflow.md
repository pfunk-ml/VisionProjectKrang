1. Workflow to add a new tag
* Go to globalData.h and add one to the NUM_OBJECTS value. Go to the globalData.cpp file and add a OBJECT_NAME.push_back value, a MARKER_ID.push_back value, should be the ALVAR id, and add a MARKER_SIZE.push_back value which is the size in cm of the marker. 
* Go to globalTransforms.cpp and add gTmarker_sprite value at the new index of the object. Should be a transform from the center of the marker to the center of the object you are representing.

2. Workflow to add a new camera
* Calibrate the camera - see Helpers.md for details. Put the resulting config file in the Data folder and follow the naming convention (see Data.md).
* Lay down a "global marker" in view of all of the cameras. Run ./getCameraTransform on each camera to get the tranform to this global marker. Maker sure to run ./cleanup before each run. See Helpers.md for more info on how to run this.
* Go to globalTransforms.cpp. Update the switch(i) within to set the value fo the tranform for each camera. Number of cases should be the number of cameras.
* Go to globalData.h and update the value of NUM_CAMERAS.

Note: You should only have to update the information in globalStuff. Everything else should be reading from these files. The scenes may have to updated if you want to use the simulator, but you might have to ask Ana for help on that since I didn't really do any of the simulator stuff.
