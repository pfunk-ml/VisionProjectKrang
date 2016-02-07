#!/bin/bash

# Where your binaries are
BIN_DIR="./bin"

# Cleanup any channel that might be wandering around
echo "Deleting previous ACH channels, if any ..."
"${BIN_DIR}/cleanup"

# Create channels for centralProcess to publish
ach -C vision_obj_poses -o 666  -m 10 -n 64
ach -C vision_krang_pose -o 666  -m 10 -n 64
# ach -C krang_vision -o 666  -m 10 -n 64
ach -C debug_channel -o 666  -m 10 -n 64

# Fire camera processes. Assuming that:
# Camera 0 is on /dev/video0
# Camera 1 is on /dev/video1
# Camera 2 is on /dev/video2
# Camera 3 is on /dev/video3

# For some reason, we have to go from up to down
for i in 6 5 4 3 2 1 0
#for i in 3 2 1 0
do
    echo "************************************"
    echo "[START] Firing up camera $i [START]"
    echo "************************************"
    "${BIN_DIR}/camProcessMain" --visualize --devid=$i --camid=$i &
    sleep 3s

    echo "\n[DONE] Finished starting camera $i"

done

# Sleep for a bit to give the cameras time to run a bit
echo "************************************"
echo "Sleep for 2 seconds before starting the central main."
sleep 2s

echo "Starting the main camera process ..."
echo "************************************"

# Run the camCentralProcessMain. This retrieve the info from the cameras 
"${BIN_DIR}/camCentralProcessMain"

# Sleep just for the heck of it
#sleep 3s
