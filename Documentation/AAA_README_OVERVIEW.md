Overview

VisionProjectKrang

SUB-DIRECTORIES:
* alvarCode - here is where all the current stuff is. See setup.md, global.md, OneCamera.md, MainProgram.md, Helpers.md, Simulation.md Workflow.md and Data.md for additional information on this code and functionality.
* CameraCalibration - data stored for old ros calibration. Ignore - see Helpers.md for up to date information on camera calibration
* camProc - old code from ar_toolkit. Defunct when we switched to ALVAR which has less error.
* MATLAB - had helper code to average Euler angles. Defunct - turns out averaging angles is bad.

QUICK LINKS
* calibration - see Helpers.md
* building and running - see setup.md
* dependencies - see setup.md
* printing new tags - see Data.md
* Adding new tags/cameras - see workflow.md
* info on running one camera/how ALVAR gets poses - OneCamera.md
* info on the main vision code - MainProgram.md
* info on the simulator - Simulation.md
* info on data stored in bin/Data - Data.md

CONVENTIONS
* The convention for cameras is to index them from zero (currently 0, 1, 2, 3). Cameras should also be setup so that their /dev/videoX address is the same as their number (see Setup.md for more info). These numbers should be the same for the ACH channels, the /dev addresses and all places in the code where cameras are somehow numbered or given an id.
* The ID of an AR marker is defined completely by ALVAR. See Data.md for more info, but in the Data/patterns folder you will see all of the available AR tags with their id number in the filename.
