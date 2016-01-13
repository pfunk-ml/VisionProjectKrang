/**
 * @file BasicFilter2.cpp
 */

#include "BasicFilter2.h"

#include <math.h>
#include <iostream>

using namespace std;

/**
 * @function BasicFilter2
 * @brief Constructor
 */
BasicFilter2::BasicFilter2(int _numSteps, bool isAngular) {

    mNumSteps = _numSteps;
    mIsAngular = isAngular;

    x_est.resize(0);
    w.resize(0);

}

/**
 * @function ~BasicFilter2
 * @brief Destructor
 */
BasicFilter2::~BasicFilter2() {
}


/**
 * @function set_weights
 * @brief Set, well weights
 */
bool BasicFilter2::set_weights( std::vector<double> _weights ) {

  if( _weights.size() != mNumSteps) {
    std::cout << " Weights vector size should be same as number of steps\n";
    return false;
  }

  w = _weights;

  return true;
}

bool BasicFilter2::set_default_weights(int type) {
  
  w.resize(mNumSteps);

  // Simple ramp (very stupid)
  if( type == RAMP ) {  
    double dw = 2.0 / (double)(mNumSteps * (mNumSteps + 1));

    for( int i = 0; i < mNumSteps; ++i )
      w[i] = dw * (double)(i + 1);
  } 
//  else if( type == GAUSSIAN_LEFT ) {
//
//    double dev = 0.3;
//    double factor = 1.0 / ( sqrt(2*3.14157)*dev );
//
//    double dx = 1.0 / (double)( w.size() - 1 );
//    double x;
//    for( int i = 0; i < mNumSteps + 1; ++i ) {
//      x = dx*i - 1.0;
//      w[i] = factor*exp(-x*x/(2.0*dev*dev));
//    }
//  } // end else if
  
  else if(type == UNIFORM) {
    for(int i = 0; i < mNumSteps; ++i)
      w[i] = 1.0/(double)mNumSteps;
  }

  return true;
}

double BasicFilter2::getEstimate(double val) {

  // If not enough stuff stored in history, just fill the queue and return the 
  // current value.
  if( x_est.size() < mNumSteps ) {
    x_est.push_back(val);
    return val;
  }

  if( x_est.size() > mNumSteps ) {
    std::cout << "Error: ESTIMATE VALUES STORED ARE TOO MANY!!!" << std::endl;
  }

  // If the values are 0,0,0 (non visible), we just set the estimate to the last value seen
  // Here we might also evaluate if the values are too different from the previous ones. ASK JON OR MARTIN
  if( val == 0) {
    return x_est[ x_est.size() - 1];
  }

  // Update the deque. Get rid of oldest and put newest at the end
  x_est.pop_front();
  x_est.push_back(val);

  // Weighted average
  double x_sum = 0;
  for( int i = 0; i < x_est.size(); ++i ) {
    x_sum += x_est[i] * w[i];
  }
  
  return x_sum;
}

bool BasicFilter2::isSufficientDataReceived() {
  return (x_est.size() >= mNumSteps);
}