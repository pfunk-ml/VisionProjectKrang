All of these programs are built from source in alvarCode/helpers but were written by the creators of ALVAR. The descriptions of these are by in large copied from the user manual at http://virtual.vtt.fi/virtual/proj2/multimedia/media/ALVAR_v2_User_Manual.pdf. 

1. generateMarkers.cpp
* ALVAR. Can safely ignore. From user manual - "This is an example that demonstrates the generation of MarkerData for MarkerArtoolkit. markers and saving the image using SaveMarkerImage . This application can be used to generate markers and multimarker setups that can be used with SampleMarkerDetector and SampleMultiMarker. 

2. getCameraTransform.cpp
* Useful, this is how you should get transforms if you add another camera. Takes in the same devX camX arguments as the OneCamera process. Will output camera tranforms to the terminal.

3. runCalibration.cpp
* Useful, this will calibrate a camera for you and output the formated calibration file to a location specified in the terminal. Should set gEtalon_square_size, gEtalon_rows, and gEtalon_columns if you change the checkerboard pattern you are using. Has same arguments as getCameraTransform and OneCamera process.

4. testFilter.cpp
* A debug program to test the filtering. Don't really need to go into detail.

5. testFilterPlot.cpp
* Plotted the instantaneous locations vs. the filter's output when we were testing the filter.

6. testSubscriber.cpp
* Tested ach stuff to make sure the main program sent it correctly.

