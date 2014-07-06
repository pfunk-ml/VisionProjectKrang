/**
 * @function globalTransform.h
 */
#pragma once

#include <Eigen/Core>
#include <vector>
#include "json/json.h"

/** Global */
extern Eigen::Matrix4d gTworld_origin;
extern int gMarker_Origin_ID;
extern int gCamera_Origin_ID;
extern std::vector<Eigen::Matrix4d> gTworld_cam;
extern std::vector<Eigen::Matrix4d> gTmarker_sprite;

void setGlobalTransforms(Json::Value config);
