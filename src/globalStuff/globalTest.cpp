#include "json/json.h"
#include "globalData.h"
#include "optparser.h"

#include <vector>
#include <iostream>
#include <string>

using namespace std;

int main()
{
    // First get json file
    Json::Value config;
    std::cout<<"Ha1";
    parseJSONFile("./config.json", config);
    std::cout<<"Ha1";
    // First set everything
    setGlobalData(config);
    //setGlobalTransforms(config);

    // Get number of cameras
    cout << "Number of cameras: " << gConfParams.numCameras << endl;
    
    // Get number of objects
    cout << "Numbers of objects: " << gConfParams.numObjects << endl;

    // Channel names
    cout << "Output object poses channel: " << VISION_OBJ_POSES_CHANNEL << endl;
    cout << "Output Krang pose channel: " << VISION_KRANG_POSE_CHANNEL << endl;
    cout << "Debug channel: " << DEBUG_CHANNEL << endl;

    // Get camera info
    for (int i = 0; i < gConfParams.numCameras; i++)
    {
        cout << "Camera " << i << endl;
        cout << "Calibration File: " << CAM_CALIB_NAME[i] << endl;
        cout << "Channel Name: " << CAM_CHANNEL_NAME[i] << endl; 
    }

    for (int i = 0; i < gConfParams.numCameras; i++)
    {
        cout << "T_cam2world[" << i << "]: " << endl << gTransforms.T_cam2world[i] << endl;
    }   
    for (int i = 0; i < gConfParams.numObjects; i++)
    { 
        cout << "T_sprite[" << i << "]: " << endl << gTransforms.T_sprite[i] << endl;
    }

    return 0;
}
