/**
 * @file basicFilter.h
 */

#include <iostream>
#include<vector>
#include <deque>

#pragma once

enum WEIGHT_TYPE {

  RAMP = 0,
  GAUSSIAN_LEFT  
};


/**
 * @class basicFilter
 */
class basicFilter {

 public:
    /* Constructor
     *  _numSteps: [IN] */
    basicFilter(int _numSteps = 5);
    ~basicFilter();
    
    /* Sets the weights.
     *  _weights: */
    bool set_weights( std::vector<double> _weights );
    
    /* Sets default weights 
     *  _type:*/
    bool set_default_weights( int _type = RAMP );

    std::vector<double> w; /**< Weights */
    double sum_w;
    std::deque<double> x_est; /**< Estimates for x pose */
    std::deque<double> y_est;
    std::deque<double> ang_est;

    /* Get an estimate for the values
     *  _x_measured: [IN] measured value of x
     *  _y_measured: [IN] measured value of y
     *  _ang_measured: [IN] measured value of theta
     *  _x_est: [OUT] estimated value of x
     *  _y_est: [OUT] estimated value of y
     *  _ang_est: [OUT] estimated value of theta */
    void getEstimate(double _x_measured,
        double _y_measured, 
        double _ang_measured,
		double &_x_est, double &_y_est, double &_ang_est);

 private:
    int mNumSteps;
    bool mFlagFull;


};
