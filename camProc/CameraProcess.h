/**
 * @file cameraProcess.h
 * @brief
 */
#pragma once

#include <AR/param.h>

#include <Eigen/Geometry>
#include <string>
#include <vector>


/**
 * @class cameraProcess
 */
class CameraProcess {
  
 public:
  CameraProcess();
  ~CameraProcess();
  bool setup( const std::string &_ARTK_CONFIG );
  bool init();  
  bool start();
  void keyEvent( unsigned char _key,
			int _x,
			int _y );
  
  void mainLoop();
  void cleanup();
  void draw();

  bool getTfs();
  
 protected:
  std::string mPatternName;
  int mPatternId;
  std::string mParam_filename;
  Eigen::Isometry3d mTcam_obj;
  std::string mARTK_VariableName;
  int mDimX; int mDimY;
  int mThresh;
  int mCount;

  ARParam mCParam;

  double mPattern_center[2];
  double mPattern_width;
  double mPattern_trans[3][4];

 public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW; /** For fixed-sized Eigen members */

};
