/**
 * @file BasicFilter2.h
 * @author Nehchal J. (nehchal@gatech.edu)
 */

#include <iostream>
#include<vector>
#include <deque>

#pragma once




/**
 * @class BasicFilter2
 *
 * This class implements a basic filter to smoothen the measured values.
 * The value is estimated by taking weighted average over certain number of 
 * latest measured values.
 * It takes care of wrapping for angular values. If values are angular it is 
 * assumed values lie in the interval [-pi, pi).
 * 
 * This filter is simplified form of basicFilter class. basicFilter accepts
 * 3 streams of data while BasicFilter2 works with only one stream of data.
 *
 * Note:
 *  Weights must be set using set_weights() or set_default_weights() before 
 *  making a call to getEstimate() function to get an estimated value.
 */
class BasicFilter2 {

 public:
  enum WEIGHT_TYPE {
    RAMP = 0,
    GAUSSIAN_LEFT,
    UNIFORM  
  };

    /* Constructor
     *  _numSteps: [IN] Number of values to consider for average. Should be 
     *          non-zero positive integer.
     *  _isAngular: [IN] True if values represent angle. False otherwise.*/
    BasicFilter2(int _numSteps = 5, bool isAngular = false);
    ~BasicFilter2();
    
    /* Sets the weights. Length of _weights vector should be equal to _numSteps
     * passed in constructor. Weights should sum to 1.
     *  _weights: [IN] The weights for calculated weighted average
     * 
     * Returns true if weights were successfully set. False, otherwise. */
    bool set_weights( std::vector<double> _weights );
    
    /* Sets the type of default weights. 
     *  _type: [IN] */
    bool set_default_weights(int _type = RAMP);

    std::vector<double> w; /**< Weights. Summation is always 1 */
    std::deque<double> x_est; /**< Estimates for x pose */

    /* Get an estimate for the values
     *  val: [IN] measured value
     * Returns estimated value. */
    double getEstimate(double val);

    /* Returns True if length of x_est is equal to mNumSteps. False otherwise. 
    */
    bool isSufficientDataReceived();

 private:
    int mNumSteps;
    bool mIsAngular;
};
