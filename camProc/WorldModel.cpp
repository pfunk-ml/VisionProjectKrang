// WorldModel Implementations

#include "WorldModel.h"
#include "ARCamera.h"
#include "ARMarker.h"

using namespace Eigen;
using namespace std;

// Constructor
WorldModel::WorldModel(ARCamera* arcameras, ARMarker* armarkers, int numcamera, int nummarker)
{
    cameras = arcameras;
    markers = armarkers;
    cameraNum = numcamera;
    markerNum = nummarker;
}

bool WorldModel::setOrigin(ARCamera camera, ARMarker marker, MatrixXd transform)
{
    return false;
}

bool WorldModel::initCamera(ARCamera cam2Init, ARCamera camAlready, ARMarker commonMark, MatrixXd transNewtoM, Matrix4d transOldtoM)
{
    return false;
}

bool WorldModel::setMarkerLoc(ARCamera camera, ARMarker marker, MatrixXd transform)
{
    return false;
}

Vector3d WorldModel::getMarkerLoc(ARMarker marker)
{
}

