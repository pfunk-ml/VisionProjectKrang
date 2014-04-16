#!/bin/bash

# Put this script on the same
# directory where the executables are
BIN_DIR="/home/ana/Research/VisionProjectKrang/alvarCode/bin"

# Cleanup any channel that might be wandering around
echo "-- Start cleanup process! --"
"${BIN_DIR}/cleanup"

echo "-- Cleaned channels! --"

# Create channels for 2 cameras
ach -C cam0_channel -o 666  -m 10 -n 64
ach -C cam1_channel -o 666  -m 10 -n 64
ach -C krang_vision -o 666  -m 10 -n 64

# Fire camera processes. Assuming that:
# Camera 0 is on /dev/video0
# Camera 1 is on /dev/video1

# For some reason we have to start camera 1 first, so keep the order
echo "************************************"
echo "[START] Firing up camera 1 [START]"
echo "************************************"
"${BIN_DIR}/camProcess" 1 1 &
sleep 3s


echo "\n[DONE] Finishing starting camera 1"

echo "************************************"
echo "[START] Firing up camera 0"
echo "************************************"
"${BIN_DIR}/camProcess" 0 0 &
sleep 3s
echo "[DONE] Finishing starting camera 0"


# Sleep for a bit to give the cameras time to run a bit
echo "************************************"
echo "[SLEEP] Sleep for 2 seconds before starting the central main--"
sleep 2s
echo "[START] Start the main camera process!--"
echo "************************************"

# Run the camCentralProcessMain. This retrieve the info from the cameras 
"${BIN_DIR}/camCentralProcessMain" &

# Sleep just for the heck of it
sleep 2s

# Done
echo "************************************"
echo "Done setting ach channels for cameras and krang_vision"
echo "************************************"
