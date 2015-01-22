/**
 * @function globalTransform.cpp
 */
#include "globalTransforms.h"
#include "globalData.h"
#include <Eigen/Geometry>

/** Global */
std::vector<Eigen::Matrix4d> gTworld_cam;
std::vector<Eigen::Matrix4d> gTmarker_sprite;

/**
 * @function setHardCodedValues
 * @brief Set hard-coded values calculated offline
 */

