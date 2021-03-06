SETUP

   When any device is connected physically to the computer it is assigned a unique name 

   To run in our perception machine.

   0. Connect cameras in order: Unplug all the cameras and then connect them in consecutive order (first camera 0, 
      then camera 1 and so on).
      This way, camera X is connected to /dev/videoX, which is considered in our script later. 
      If you reboot the machine, make sure to do the unplugging-plugging thing, since the /dev/ order might change
      when restarting.
      Remember to use the ports of the USB 3.0 card, since we need the cameras to be in independent ports.

   1. Set the ALVAR_PLUGIN_PATH to the directory where the plugins are (libalvarcapturepluginfile200.so). 
      For the perception machine this is /usr/local/lib, so you can just add this line to your ~/.bashrc file
      
      export ALVAR_PLUGIN_PATH=/usr/local/lib

   3. Go to /VisionProjectKrang/alvarCode and find the script visionClient. Here, 
      modify the variable BIN_DIR to the directory where your binaries for the alvarCode are.

   4. Build the code. This can be done simply by

      * Run CMake to generate Makefile
         $ cd path/to/alvarCode/build
         $ cmake ..

      * Run make for compilation. All binaries executables go to bin/ folder.
         $ cd path/to/alvarCode/build
         $ make

      Note: After making any changes to files, 'cmake' need not be re-run. 'make' should be run. If new files are created then changes may be needed to make to CMakeLists.txt file.



   5. Set the parameters in config.json
         

   6. Now you can run the script:
        $ cd path/to/alvarCode
        $ ./visionClient.sh

   6. You should be done.


BRIEF EXPLANATION
------------------

The visionClient script does the following:

1. Erase any existing channel that might have been left open by mistake ( **./cleanup**)
2. Create the ACH channels for the 4 cameras and the **krang_vision** that will go to the planning process
3. Fire up the 4 camera processes by calling:

   ./camProcess devX camX

   the first argument is the /dev/videoX to which the camera is plugged in. camX indicates the camera index,
   which is used to call the correct calibration file.
4. Start the camCentralProcessMain, which receives the info from the camera processes and outputs the krang_vision channel
