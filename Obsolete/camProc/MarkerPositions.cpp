/**
 * @function MarkerPositions.cpp
 */

#include "ARMarker.h"
#include "ARCamera.h"
#include "WorldModel.h"
#include <vector>

int main(int argc, char **argv) 
{
    // Let's make up some markers and cameras
    ARMarker markglobal(0);
    ARMarker markc1c2(1);
    ARMarker markc2c3(2);
    ARMarker move1(6);
    ARMarker move2(9);
    ARMarker move3(19);
    ARCamera cam1(1);
    ARCamera cam2(2);
    ARCamera cam3(3);
    vector<ARMarker> markers;
    markers.push_back(markglobal);
    markers.push_back(markc1c2);
    markers.push_back(markc2c3);
    markers.push_back(move1);
    markers.push_back(move2);
    markers.push_back(move3);
    vector<ARCamera> cameras;
    cameras.push_back(cam1);
    cameras.push_back(cam2);
    cameras.push_back(cam3);

    // Initialize WorldModel
    WorldModel world = WorldModel(cameras, markers);

    // Make up some transforms
    Matrix4d W2C1;
    W2C1(0,0) = 1;
    W2C1(0,1) = 0;
    W2C1(0,2) = 0;
    W2C1(0,3) = -50;
    W2C1(1,0) = 0;
    W2C1(1,1) = -1;
    W2C1(1,2) = 0;
    W2C1(1,3) = 0;
    W2C1(2,0) = 0;
    W2C1(2,1) = 0;
    W2C1(2,2) = -1;
    W2C1(2,3) = 100;
    W2C1(3,0) = 0;
    W2C1(3,1) = 0;
    W2C1(3,2) = 0;
    W2C1(3,3) = 1;
    Matrix4d MB12C1;
    MB12C1(0,0) = 1;
    MB12C1(0,1) = 0;
    MB12C1(0,2) = 0;
    MB12C1(0,3) = 0;
    MB12C1(1,0) = 0;
    MB12C1(1,1) = -1;
    MB12C1(1,2) = 0;
    MB12C1(1,3) = -100;
    MB12C1(2,0) = 0;
    MB12C1(2,1) = 0;
    MB12C1(2,2) = -1;
    MB12C1(2,3) = 100;
    MB12C1(3,0) = 0;
    MB12C1(3,1) = 0;
    MB12C1(3,2) = 0;
    MB12C1(3,3) = 1;
    Matrix4d MB12C2;
    MB12C2(0,0) = 1;
    MB12C2(0,1) = 0;
    MB12C2(0,2) = 0;
    MB12C2(0,3) = 0;
    MB12C2(1,0) = 0;
    MB12C2(1,1) = -1;
    MB12C2(1,2) = 0;
    MB12C2(1,3) = 0;
    MB12C2(2,0) = 0;
    MB12C2(2,1) = 0;
    MB12C2(2,2) = -1;
    MB12C2(2,3) = 100;
    MB12C2(3,0) = 0;
    MB12C2(3,1) = 0;
    MB12C2(3,2) = 0;
    MB12C2(3,3) = 1;
    Matrix4d MB22C2;
    MB22C2(0,0) = 1;
    MB22C2(0,1) = 0;
    MB22C2(0,2) = 0;
    MB22C2(0,3) = 0;
    MB22C2(1,0) = 0;
    MB22C2(1,1) = -1;
    MB22C2(1,2) = 0;
    MB22C2(1,3) = -100;
    MB22C2(2,0) = 0;
    MB22C2(2,1) = 0;
    MB22C2(2,2) = -1;
    MB22C2(2,3) = 100;
    MB22C2(3,0) = 0;
    MB22C2(3,1) = 0;
    MB22C2(3,2) = 0;
    MB22C2(3,3) = 1;
    Matrix4d MB22C3;
    MB22C3(0,0) = 1;
    MB22C3(0,1) = 0;
    MB22C3(0,2) = 0;
    MB22C3(0,3) = 0;
    MB22C3(1,0) = 0;
    MB22C3(1,1) =-1;
    MB22C3(1,2) = 0;
    MB22C3(1,3) = 0;
    MB22C3(2,0) = 0;
    MB22C3(2,1) = 0;
    MB22C3(2,2) =-1;
    MB22C3(2,3) = 100;
    MB22C3(3,0) = 0;
    MB22C3(3,1) = 0;
    MB22C3(3,2) = 0;
    MB22C3(3,3) = 1;
    Matrix4d M12C1;
    M12C1(0,0) = 1;
    M12C1(0,1) = 0;
    M12C1(0,2) = 0;
    M12C1(0,3) = 25;
    M12C1(1,0) = 0;
    M12C1(1,1) = -1;
    M12C1(1,2) = 0;
    M12C1(1,3) = -50;
    M12C1(2,0) = 0;
    M12C1(2,1) = 0;
    M12C1(2,2) = -1;
    M12C1(2,3) = 100;
    M12C1(3,0) = 0;
    M12C1(3,1) = 0;
    M12C1(3,2) = 0;
    M12C1(3,3) = 1;
    Matrix4d M32C2;    
    M32C2(0,0) = 1;
    M32C2(0,1) = 0;
    M32C2(0,2) = 0;
    M32C2(0,3) = -50;
    M32C2(1,0) = 0;
    M32C2(1,1) = -1;
    M32C2(1,2) = 0;
    M32C2(1,3) = 0;
    M32C2(2,0) = 0;
    M32C2(2,1) = 0;
    M32C2(2,2) = -1;
    M32C2(2,3) = 100;
    M32C2(3,0) = 0;
    M32C2(3,1) = 0;
    M32C2(3,2) = 0;
    M32C2(3,3) = 1;
    Matrix4d M22C3;
    M22C3(0,0) = 1;
    M22C3(0,1) = 0;
    M22C3(0,2) = 0;
    M22C3(0,3) = 25;
    M22C3(1,0) = 0;
    M22C3(1,1) = -1;
    M22C3(1,2) = 0;
    M22C3(1,3) = 50;
    M22C3(2,0) = 0;
    M22C3(2,1) = 0;
    M22C3(2,2) = -1;
    M22C3(2,3) = 100;
    M22C3(3,0) = 0;
    M22C3(3,1) = 0;
    M22C3(3,2) = 0;
    M22C3(3,3) = 1;

    // Now initialize world correctly
    bool success = world.setOrigin(1, 0, W2C1);
    success &= world.initCamera(2, 1, MB12C2, MB12C1);
    success &= world.initCamera(3, 2, MB22C3, MB22C2);
    success &= world.setMarkerLoc(1, 0, W2C1);
    success &= world.setMarkerLoc(1, 1, MB12C1);
    success &= world.setMarkerLoc(2, 2, MB22C2);
    success &= world.setMarkerLoc(1, 6, M12C1);
    success &= world.setMarkerLoc(3, 9, M22C3);
    success &= world.setMarkerLoc(2, 19, M32C2);

    if (!success)
        cout << "Error Will Robenson!" << endl;

    // Now print out locations
    cout << "World at: " << world.getMarkerLoc(0) << "\n";
    cout << "Marker between C1C2 at: " << world.getMarkerLoc(1) << "\n";
    cout << "Marker between C2C3 at: " << world.getMarkerLoc(2) << "\n";
    cout << "Marker move1 at: " << world.getMarkerLoc(6) << "\n";
    cout << "Marker move2 at: " << world.getMarkerLoc(9) << "\n";
    cout << "Marker move3 at: " << world.getMarkerLoc(19) << "\n";

    // Test other functions
    double testMatrix[3][4];
    testMatrix[0][0] = 0;
    testMatrix[0][1] = -1;
    testMatrix[0][2] = 0;
    testMatrix[0][3] = 10;
    testMatrix[1][0] = 1;
    testMatrix[1][1] = 0;
    testMatrix[1][2] = 0;
    testMatrix[1][3] = 20;
    testMatrix[2][0] = 0;
    testMatrix[2][1] = 0;
    testMatrix[2][2] = 1;
    testMatrix[2][3] = 30;

    Matrix4d transformMat = world.getDoubleArrAsMat(testMatrix);
    cout << "Transformed: " << endl << transformMat << endl;
    
    double x, y, ang;
    world.getXYangTriple(transformMat, x, y, ang);

    cout << "x: " << x << " y: " << y << " ang: " << ang << endl;
}

