/**
 * @file utils.h
 * @author: Nehchal J. (nehchal@gatech.edu)
 * Created On: Dec 28, 2015
 */

 #pragma once

/* Prints the linear and angular distances between two poses. Input poses 
should be arrays of length 4 (x, y, z, theta in order). Linear distance is
positive number. Rotational distance is angular dispalacement of pose2 w.rt.
pose1.
 * pose1: [IN] The first pose.
 * pose1: [IN] The first pose.
 * linDist: [OUT] Euclidean distance between two poses
 * rotDist: [OUT] Rotational distance between two poses */ 
void utils_computeDistances(const double* pose1, 
    const double* pose2, double* linDist, double *rotDist);