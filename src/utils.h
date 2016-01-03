/**
 * @file utils.h
 * @author: Nehchal J. (nehchal@gatech.edu)
 * Created On: Dec 28, 2015
 */

#pragma once

#include <Eigen/Dense>
 #include <vector>


/** Helpers */
//double** getMatAsDouble( const Eigen::Matrix4d &_mat );

/* Converts a 3x4 double array to Matrix4d matrix
    _trans    : [IN] the 3x4 transformation matrix
    return val: the 4x4 homogenous matrix */
Eigen::Matrix4d utils_getDoubleArrAsMat( const double _trans[3][4] );

/* Obtain x, y, z and theta from transformation matrix.
    _Tf : [IN]  the ptr to 4x4 homogenous transformation matrix
    _x  : [OUT] x-coordinate
    _y  : [OUT] y-coordinate
    _z  : [OUT] z-coordinate
    _ang: [OUT] the angle about z-axis */
void utils_getXYZAng( const Eigen::Matrix4d &_Tf, 
             double& _x, 
             double& _y, 
             double& _z,
             double& _ang );

/* Obtain x, y, z and theta from transformation matrix.
    _Tf : [IN]  the ptr to 4x4 homogenous transformation matrix
    pose: [OUT] The extracted pose (x, y, z, theta in order) */
void utils_getXYZAng( const Eigen::Matrix4d &_Tf, 
             double pose[4]);

/* Computes the linear and angular distances between two poses. Input poses 
should be arrays of length 4 (x, y, z, theta in order). Linear distance is
positive number. Rotational distance is angular dispalacement of pose2 w.rt.
pose1.
 * pose1: [IN] The first pose.
 * pose1: [IN] The first pose.
 * linDist: [OUT] Euclidean distance between two poses
 * rotDist: [OUT] Rotational distance between two poses */ 
void utils_computeDistances(const double* pose1, 
    const double* pose2, double* linDist, double *rotDist);