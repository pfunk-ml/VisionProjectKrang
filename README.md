camProc
=========

Requirements
-------------

* ach for inter-process communication
* yaml-cpp version 0.5.1 (not the one in Ubuntu 12.04, that is as old as 0.2 and we need the newest version)

HOWTO
------

To run in our perception machine.

1. Set the ALVAR_PLUGIN_PATH to the directory where the plugins are (libalvarcapturepluginfile200.so). 
   For the perception machine this is /usr/local/lib, so you can just add this line to your ~/.bashrc file
   
   export ALVAR_PLUGIN_PATH=/usr/local/lib

2. In /VisionProjectKrang/alvarCode, run this script:
   
   ./visionClient.sh

3. You should be done

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
