#include "json/json.h"
#include "globalData.h"
#include "globalTransforms.h"
#include "optparser.h"

#include <vector>
#include <iostream>
#include <string>

using namespace std;

int main()
{
    // First get json file
    Json::Value config;
    parseJSONFile("./config.json", config);

    // First set everything
    setGlobalData(config);
    setGlobalTransforms(config);

    // Get number of cameras
    cout << "Number of cameras: " << NUM_CAMERAS << endl;
    
    // Get number of objects
    cout << "Numbers of objects: " << NUM_OBJECTS << endl;

    // Channel names
    cout << "Perception channel: " << PERCEPTION_CHANNEL << endl;
    cout << "Debug channel: " << DEBUG_CHANNEL << endl;

    // Get object info
    for (int i = 0; i < NUM_OBJECTS; i++)
    {
        cout << "Object " << i << ": " << OBJECT_NAME[i] << endl;
        cout << "Marker ID: " << MARKER_ID[i] << endl;
        // cout << "Marker Size: " << MARKER_SIZE[i] << endl;
    }

    // Get camera info
    for (int i = 0; i < NUM_OBJECTS; i++)
    {
        cout << "Camera " << i << endl;
        cout << "Calibration File: " << CAM_CALIB_NAME << endl;
        cout << "Channel Name: " << CAM_CHANNEL_NAME[i] << endl; 
    }

    // Print world to origin
    cout << "gMarker_Origin_ID: " << gMarker_Origin_ID << endl;
    cout << "gCamera_Origin_ID: " << gCamera_Origin_ID << endl;
    cout << "gTworld_origin: " << endl << gTworld_origin << endl;
    for (int i = 0; i < NUM_CAMERAS; i++)
    {
        cout << "gTworld_cam[" << i << "]: " << endl << gTworld_cam[i] << endl;
    }   
    for (int i = 0; i < NUM_OBJECTS; i++)
    { 
        cout << "gTmarker_sprite[" << i << "]: " << endl << gTmarker_sprite[i] << endl;
    }

    return 0;
}
