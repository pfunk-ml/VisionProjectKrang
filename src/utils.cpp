/**
 * @file utils.cpp
 * @author: Nehchal J. (nehchal@gatech.edu)
 * Created On: Dec 28, 2015
 */

#include "utils.h"

#include <math.h>
#include <stdio.h>


/**
 * @function utils_getXYZAng
 */
void utils_getXYZAng( const Eigen::Matrix4d &_Tf, 
             double& _x, 
             double& _y, 
             double& _z,
             double& _ang ) {
    
    // Get x and y from rotation matrix
    _x = _Tf(0,3);
    _y = _Tf(1,3);
    _z = _Tf(2,3);

    // Get Euler angles
    Eigen::Matrix3d transformMat = _Tf.block(0, 0, 3, 3);
    Eigen::Vector3d ea = transformMat.eulerAngles(0, 1, 2);
    
    // Angle should be the Rz angle
    _ang = (double)ea[2];
}

void utils_getXYZAng( const Eigen::Matrix4d &_Tf, 
             double pose[4]){
    utils_getXYZAng(_Tf, pose[0], pose[1], pose[2], pose[3]);
}

void utils_getXYZAng(const double T[3][4], double pose[4]) {
    Eigen::Matrix4d T_martrix4d = utils_getDoubleArrAsMat(T);
    utils_getXYZAng(T_martrix4d, pose);
}

/**
 * @function getMatAsDouble
 * @brief Set an Eigen 4x4 matrix to double[3][4]
 */
//double** getMatAsDouble( const Eigen::Matrix4d &_mat ) {
//
//    // Create new double array [3][4]
//    double** ret = new double*[3];
//    for (int h = 0; h < 3; h++) {
//        ret[h] = new double[4];
//    }
//
//    // Now put in matrix 4d stuff
//    for( int i = 0; i < 3; i++ ) {
//    for( int j = 0; j < 4; ++j ) { 
//        ret[i][j] = _mat(i,j);
//    }
//    }
//
//    return ret;
//}

/**
 * @function utils_getDOubleArrAsMat
 * @brief Gets Eigen::Matrix4d from T double array
 */
Eigen::Matrix4d utils_getDoubleArrAsMat(const double T[3][4]) {

    // Now put in matrix 4d stuff
    Eigen::Matrix4d martrix4d;

    for(int i = 0; i < 3; ++i)
        for(int j = 0; j < 4; ++j) 
            martrix4d(i,j) = T[i][j];

    martrix4d.row(3) << 0,0,0,1;

    return martrix4d;
}

void utils_computeDistances(const double* pose1, 
    const double* pose2, double* linDist, double *rotDist){

  *linDist = sqrt(pow(pose1[0] - pose2[0], 2) + pow(pose1[1] - pose2[1], 2)
                          + pow(pose1[2] - pose2[2], 2) );
  *rotDist = pose2[3] - pose1[3];
}

void utils_printPose(const double pose[4], int markerId, bool isVisible){
    printf("  id: %d ", markerId);

    if (!isVisible){
        printf("[Marker not visible.]\n");
        return;
    }

    printf("x: %.4f y: %.4f z: %.4f theta: %.4f\n", 
            pose[0], pose[1], pose[2], pose[3]);
    return;
}

void utils_printPose(const double T[3][4], int markerId, bool isVisible){
    double pose[4];
    utils_getXYZAng(T, pose);
    utils_printPose(pose, markerId, isVisible);
    return;
}