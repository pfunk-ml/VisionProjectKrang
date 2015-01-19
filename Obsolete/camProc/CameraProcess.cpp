/**
 * @file CameraProcess.cpp
 */
#include <yaml-cpp/yaml.h>
#include <yaml-cpp/node/node.h>
#include <yaml-cpp/node/parse.h>
#include "CameraProcess.h"
#include <stdio.h>

/**
 * @function parseYAMLCalibration
 */
bool parseYAMLCalibration( std::string _filename,
			   ARParam &_cparam ) {

  YAML::Node config = YAML::LoadFile( _filename );
  YAML::Node node;
  YAML::Node subnode;

  // xdim
  if( config["image_width"] ) {
    _cparam.xsize = config["image_width"].as<int>(); 
  } else { return false; } 

  // ydim
  if( config["image_height"] ) {
    _cparam.ysize = config["image_height"].as<int>(); 
    
  } else { return false; }

  // P
  node = config["projection_matrix"]; 
  if( !node ) { return false; }
  subnode = node["data"];
  if( !subnode ) { return false; }
  assert( subnode.Type() == YAML::NodeType::Sequence );
  
  double P[12];

  for( std::size_t i = 0; i < subnode.size(); ++i ) {
    P[i] = subnode[i].as<double>();
  }

  // Read in AR
  _cparam.mat[0][0] = P[0];
  _cparam.mat[1][0] = P[4];
  _cparam.mat[2][0] = P[8];
  _cparam.mat[0][1] = P[1];
  _cparam.mat[1][1] = P[5];
  _cparam.mat[2][1] = P[9];
  _cparam.mat[0][2] = P[2];
  _cparam.mat[1][2] = P[6];
  _cparam.mat[2][2] = P[10];
  _cparam.mat[0][3] = P[3];
  _cparam.mat[1][3] = P[7];
  _cparam.mat[2][3] = P[11];
  
  // Read distortion
  node = config["distortion_coefficients"]; 
  if( !node ) { return false; }
  subnode = node["data"];
  if( !subnode ) { return false; }
  assert( subnode.Type() == YAML::NodeType::Sequence );
  
  double D[5];

  for( std::size_t i = 0; i < subnode.size(); ++i ) {
    D[i] = subnode[i].as<double>();
  }


  // K
  node = config["camera_matrix"]; 
  if( !node ) { return false; }
  subnode = node["data"];
  if( !subnode ) { return false; }
  assert( subnode.Type() == YAML::NodeType::Sequence );
  
  double K[9];

  for( std::size_t i = 0; i < subnode.size(); ++i ) {
    K[i] = subnode[i].as<double>();
  }

  // Read in AR
  _cparam.dist_factor[0] = K[2];       // x0 = cX from openCV calibration
  _cparam.dist_factor[1] = K[5];       // y0 = cY from openCV calibration
  _cparam.dist_factor[2] = -100*D[0]; // f = -100*k1 from CV. Note, we had to do mm^2 to m^2, hence 10^8->10^2
  _cparam.dist_factor[3] = 1.0;       // scale factor, should probably be >1, but who cares...



  return true;

}
