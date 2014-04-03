/**
 * @file CameraCentralProcess.h
 */
#pragma once
#include <Eigen/Geometry>
#include <vector>


/**
 * @class CameraCentralProcess
 */
class CameraCentralProcess {
  
 public:

  CameraCentralProcess();
  ~CameraCentralProcess();
  bool initChannels();
  void mainLoop();
  void grabChannelsInfo();
  void getWorldTransform();
  void sendMessage();
  
  
 private:

  /** Information from each channel */
  std::vector<CamChannelData> mCamChannelData;

  /** Tf from world to markers */
  std::vector<Eigen::Isometry3d> mTworld_marker;
 
  /** Channels */
  std::vector<ach_channel_t> mChannels;
  
 public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW

};

/**
 * @structure CamChannelData
 */
struct CamChannelData {

  Eigen::Isometry3d mTw_cam;
  int mID;
  std::vector<Eigen::Isometry3d> mTcam_marker;
  std::vector<int> mMarker_visible;

  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};
