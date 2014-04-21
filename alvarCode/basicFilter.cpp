/**
 * @file basicFilter.cpp
 */

#include "basicFilter.h"

/**
 * @function basicFilter
 * @brief Constructor
 */
basicFilter::basicFilter( int _numSteps ) {

    mNumSteps = _numSteps;
    mFlagFull = false;

    x_est.resize(0);
    y_est.resize(0);
    ang_est.resize(0);
    w.resize(0);

}

/**
 * @function ~basicFilter
 * @brief Destructor
 */
basicFilter::~basicFilter() {

}


/**
 * @function set_weights
 * @brief Set, well weights
 */
bool basicFilter::set_weights( std::vector<double> _weights ) {

    if( _weights.size() != mNumSteps + 1 ) {
	std::cout << " Weight size should be one more than steps"<< std::endl;
	return false;
    }

    w = _weights;
    sum_w = 0;

    for( int i = 0; i < w.size(); ++i ) {
	sum_w += w[i];
    }
}

/**
 * @function set_default_weights
 */
bool basicFilter::set_default_weights() {

    // Simple ramp (very stupid)
    w.resize( mNumSteps + 1);
    double dw = 1.0 / (double)( w.size() - 1 );
    for( int i = 0; i < mNumSteps + 1; ++i ) {
      w[i] = dw*(double)i;
    }

    // Set total sum
    sum_w = 0;
    for( int i = 0; i < w.size(); ++i ) {
	sum_w += w[i];
    }
}

/**
 * @function getEstimate
 */
void basicFilter::getEstimate( const double &_x_measured,
			       const double &_y_measured,
			       const double &_ang_measured,
			       double &_x_est,
			       double &_y_est,
			       double &_ang_est ) {

  // If not enough stuff stored in history, just fill it
  if( x_est.size() < mNumSteps ) {

    _x_est = _x_measured;
    _y_est = _y_measured;
    _ang_est = _ang_measured;

    x_est.push_back( _x_est );
    y_est.push_back( _y_est );
    ang_est.push_back( _ang_est );
    return;

  }
  if( x_est.size() > mNumSteps ) {
    std::cout << " ESTIMATE VALUES STORED ARE TOO MANY!!!" << std::endl;
  }

  // If the values are 0,0,0 (non visible), we just set the estimate to the last value seen
  // Here we might also evaluate if the values are too different from the previous ones. ASK JON OR MARTIN
  if( _x_measured == 0 && _y_measured == 0 && _ang_measured == 0 ) {
  
    _x_est = x_est[ x_est.size() - 1];
    _y_est = y_est[ y_est.size() - 1];
    _ang_est = ang_est[ ang_est.size() - 1];
  }
  
  // If not, normal proceedings
  else {

    // Weighted average
    double x_sum = 0;
    double y_sum = 0;
    double ang_sum_x = 0;
    double ang_sum_y = 0;

    for( int i = 0; i < x_est.size(); ++i ) {
	x_sum += x_est[i]*w[i];
	y_sum += y_est[i]*w[i];
	ang_sum_x += ang_est[i]*cos(x)*w[i];
    ang_sum_y += ang_est[i]*sin(x)*w[i];
    }
    
    // Add measurement
    x_sum += _x_measured*w[mNumSteps];
    y_sum += _y_measured*w[mNumSteps];
    ang_sum_x += _ang_measured*cos(x)*w[mNumSteps];
    ang_sum_y += _ang_measured*sin(x)*w[mNumSteps];

    // Average
    _x_est = x_sum / sum_w;
    _y_est = y_sum / sum_w;
    //_ang_est = ang_sum / sum_w;
    
    // Get angle 
    _ang_est = atan2(ang_sum_y, ang_sum_x);
    

  }
    // Update the deque. Get rid of oldest and put newest at the end
    x_est.pop_front();
    x_est.push_back( _x_est );

    y_est.pop_front();
    y_est.push_back( _y_est );

    ang_est.pop_front();
    ang_est.push_back( _ang_est );

}

