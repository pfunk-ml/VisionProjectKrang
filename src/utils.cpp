/**
 * @file utils.cpp
 * @author: Nehchal J. (nehchal@gatech.edu)
 * Created On: Dec 28, 2015
 */

#include <utils.h>

#include <math.h>

void utils_computeDistances(const double* pose1, 
    const double* pose2, double* linDist, double *rotDist){

  *linDist = sqrt(pow(pose1[0] - pose2[0], 2) + pow(pose1[1] - pose2[1], 2)
                          + pow(pose1[2] - pose2[2], 2) );
  *rotDist = pose2[3] - pose1[3];
}