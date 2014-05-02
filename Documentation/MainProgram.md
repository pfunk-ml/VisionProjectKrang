Details of the main vision process.

1. CameraCentralProcess.cpp
function mainLoop() tells the story
* this->grabChannelsInfo() - receives the ach information sent by camProcess. Puts the data received from the camera processes in a data structure called mMarkerMsgs. This is an 2D array (techincally a 1D array of vectors, but let's ignore that). The outer index is the object. Each tag object then has a std:vector of MarkerMsg_t objects. Each of these represents the data sent about that tag by one of the cameras (that had data for it). If no camera captured a tag, there will be one MarkerMsg_t object here, but the .visible field will be -1.
* this->getWorldTransforms() - calls the worldModel code and gives it the raw tranforms stored in mMarkerMsgs. WorldModel will calculate the world transforms.
* this->createMessage() - gets the global pose info for each marker from worldModel, calls the getXYangTriple() helper in WorldModel to get just the x y and Rz angle, calls basicFileter's getEstimate function to get the filtered x y and angle and stores the final information into a 2D array called finalMsg.
* this->sendMessage() - simply sends finalMsg out on ach perception channel.

2. WorldModel.cpp
This class does all of the coordinate transformation. Here is a quick rundown of its methods.
Helpers
* getMatAsDouble, getDoubleArrAsMat - convert between double array and Eigen 4x4 matrix.
* void getXYangTriple - drop z and Rx, Ry from pose and return just the x, y and Rz angle.
* getAverageAffine - given a vector of tranforms and vector of weights, takes the weighted average of the x, y, z positions and gets a quarternion median (unweighted).
Class
* WorldModel - constructor. Need to pass in array of ARCameras and ARMarkers. These constructors just require IDs (which should be passed in from the global values, see Global.md).
* setOrigin,  initCamera - these were the old ways of setting the global transforms. Instead we call initializeCamera directly on the ARCameras.
* setMarkerLoc - has 3 overloads. One, you pass in a single tranform and camera, sets the global transform of the marker. The second, you give it a vector of tranforms and cameras and it uses the getAverageAffine to come up with an "Averaged" tranform. 3rd one is same as 2nd, but allows you to pass in weights for a weighted average.
* getMarkerLoc, getMarkerPose - Give you the (x,y,z) location and complete pose of a marker respectively. Will give you the value last calculated when you called setMarkerLoc on that marker.

3. ARCamera, ARMarker
You rarely care about these individually except for the few methods below
ARCamera.cpp
* initializeCamera - give it a tranform from the world to the camera. Allows you to use this camera - WorldModel will do all of the pose transformation for you, but only if the camera has been "set".
* ARCamera - constructor. Just pass in the id (code uses the 0 to numcameras-1 convention)
ARMarker.cpp
* ARMarker - constructor. Passes in ID (ALVAR id, see conventions in overview or Data.md).

4. basicFilter.cpp
Averages the last few x, y, and angles you give it to give a filetered output.
* basicFilter - constructor. Argument lets you set the number of histories it remembers. Default is 5 currently. 
* set_weights - set the weights of the filter.
* set_default_weights - sets the weights to be a default filter. Right now, the default is a simple ramp filter.
* getEstimate - pass in the current x, y, angle values, returns the filtered values.
One last note - if the values sent into getEstimate are all 0 (indicating to it that there was not valid measurement) it will assume that it received the values of the last valid input. The result is that if a marker dissapears, the filter will basically pretend the marker hasn't moved.

5. cleanup.cpp
Not too exciting. Just cleans up ach channels for you. If you run anything directly from the binaries (don't use the .sh file) you should run this first so you don't get ach errors.

