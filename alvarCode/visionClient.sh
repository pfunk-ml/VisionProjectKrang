#!/bin/bash

# Where your binaries are
BIN_DIR="./bin"

# Cleanup any channel that might be wandering around
echo "-- Starting cleanup process. Previous ACH channels will be deleted. --"
"${BIN_DIR}/cleanup"

# Create channels for 4 cameras
ach -C cam0_channel -o 666  -m 10 -n 64
ach -C cam1_channel -o 666  -m 10 -n 64
ach -C cam2_channel -o 666  -m 10 -n 64
ach -C cam3_channel -o 666  -m 10 -n 64
ach -C krang_vision -o 666  -m 10 -n 64
ach -C debug_channel -o 666  -m 10 -n 64

echo "-- Channels cleaned. --"

# Fire camera processes. Assuming that:
# Camera 0 is on /dev/video0
# Camera 1 is on /dev/video1
# Camera 2 is on /dev/video2
# Camera 3 is on /dev/video3

# For some reason, we have to go from up to down
for i in 3 2 1 0
#for i in 0
do
    echo "************************************"
    echo "[START] Firing up camera $i [START]"
    echo "************************************"
    "${BIN_DIR}/camProcess" $i $i &
    sleep 3s

    echo "\n[DONE] Finishing starting camera $i"

done

# Sleep for a bit to give the cameras time to run a bit
echo "************************************"
echo "[SLEEP] Sleep for 2 seconds before starting the central main--"
sleep 2s
echo "[START] Start the main camera process!--"
echo "************************************"

# Run the camCentralProcessMain. This retrieve the info from the cameras 
"${BIN_DIR}/camCentralProcessMain" &

# Sleep just for the heck of it
sleep 3s

# Done
echo "************************************"
echo "Done setting ach channels for cameras and krang_vision"
echo "************************************"
