// WorldModel Implementations

#include "WorldModel.h"
#include "ARCamera.h"
#include "ARMarker.h"

using namespace Eigen;
using namespace std;

// Constructor
WorldModel::WorldModel(vector<ARCamera> arcameras, vector<ARMarker> armarkers)
{
    cameras = arcameras;
    markers = armarkers;
}

bool WorldModel::setOrigin(int cameraID, int markerID, Matrix4d transform)
{
    // Debug
    /*cout << "Calling setOrigin with camera " << cameraID << " and marker " << markerID
        << " and transform " << endl << transform << endl;*/

    // Initialize camera with World to Camera transform (given in input)
    bool success = cameras[getCamInd(cameraID)].initializeCamera(transform);

    // Set marker as "world" marker
    worldMarkerID = markerID;

    return success;
}

bool WorldModel::initCamera(int cam2InitID, int camAlreadyID, Matrix4d transMtoNew, Matrix4d transMtoOld)
{
    // Debug
    /*cout << "Calling initCamera with uninit camera " << cam2InitID << " init camera "  
        << camAlreadyID << " and transform MtoNew " << endl << transMtoNew 
        << " and transform MtoOld " << endl << transMtoOld << endl;*/

    // Make sure camera is initialized
    if (!cameras[getCamInd(camAlreadyID)].isInitialized())
        return false;

    // Get world to camera transform for new camera
    Matrix4d world2new = cameras[getCamInd(camAlreadyID)].getWorld2Cam() * transMtoOld.inverse() * transMtoNew;

    // Debut
    //cout << "Creating matrix world2new " << endl << world2new << endl;

    // Initialize camera with that transform
    bool success = cameras[getCamInd(cam2InitID)].initializeCamera(world2new);

    return success;
}

bool WorldModel::setMarkerLoc(int cameraID, int markerID, Matrix4d transform)
{
    // Debug
    /*cout << "Calling setMarkerLoc with camera " << cameraID << " marker " 
        << markerID << " and transform " << endl << transform << endl; */

    // Check that camera is initialized
    if (!cameras[getCamInd(cameraID)].isInitialized())
        return false;

    // Get world to marker matrix
    Matrix4d world2mark = cameras[getCamInd(cameraID)].getCam2World() * transform;

    // Set marker
    bool success = markers[getMarkInd(markerID)].setMarker(world2mark);

    return success;
}

Vector3d WorldModel::getMarkerLoc(int markerID)
{
    return markers[getMarkInd(markerID)].getLocInWorld();
}

Matrix4d WorldModel::getMarkerPose(int markerID)
{
    return markers[getMarkInd(markerID)].getMarkerPose();
} 

int WorldModel::getCamInd(int ID)
{
    for (int i = 0; i < cameras.size(); i++)
    {
        if (cameras[i].getID() == ID)
            return i;
    }
    
    // Return -1 if not in array
    return -1;
}

int WorldModel::getMarkInd(int ID)
{
    for (int i = 0; i < markers.size(); i++)
    {
        if (markers[i].getID() == ID)
            return i;
    }
    
    // Return -1 if not in array
    return -1;
}

double** WorldModel::getMatAsDouble(Matrix4d mat)
{
    // Create new double array [3][4]
    double** ret = new double*[3];
    for (int h = 0; h < 3; h++)
    {
        ret[h] = new double[4];
    }

    // Now put in matrix 4d stuff
    ret[0][0] = mat(0,0);
    ret[0][1] = mat(0,1);
    ret[0][2] = mat(0,2);
    ret[0][3] = mat(0,3);
    ret[1][0] = mat(1,0);
    ret[1][1] = mat(1,1);
    ret[1][2] = mat(1,2);
    ret[1][3] = mat(1,3);
    ret[2][0] = mat(2,0);
    ret[2][1] = mat(2,1);
    ret[2][2] = mat(2,2);
    ret[2][3] = mat(2,3);

    return ret;
}

Matrix4d WorldModel::getDoubleArrAsMat(double trans[3][4])
{
    // Now put in matrix 4d stuff
    Matrix4d mat;
    mat(0,0) = trans[0][0];
    mat(0,1) = trans[0][1];
    mat(0,2) = trans[0][2];
    mat(0,3) = trans[0][3];
    mat(1,0) = trans[1][0];
    mat(1,1) = trans[1][1];
    mat(1,2) = trans[1][2];
    mat(1,3) = trans[1][3];
    mat(2,0) = trans[2][0];
    mat(2,1) = trans[2][1];
    mat(2,2) = trans[2][2];
    mat(2,3) = trans[2][3];
    mat(3,0) = 0;
    mat(3,1) = 0;
    mat(3,2) = 0;
    mat(3,3) = 1;

    return mat;
}

void WorldModel::getXYangTriple(Matrix4d mat, double& x, double& y, double& ang)
{
    // Get x and y from rotation matrix
    x = mat(0,3);
    y = mat(1,3);

    // Get Euler angles
    Matrix3d transformMat = mat.block(0, 0, 3, 3);
    Vector3d ea = transformMat.eulerAngles(0, 1, 2);
    
    // Angle should be the Rz angle
    ang = (double)ea[2];
}


