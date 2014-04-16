/**
 * @file basicFilter.h
 */

#include <iostream>
#include<vector>
#include <deque>

#pragma once



/**
 * @class basicFilter
 */
class basicFilter {

 public:
    basicFilter( int _numSteps = 10 );
    ~basicFilter();
    
    bool set_weights( std::vector<double> _weights );
    bool set_default_weights();

    std::vector<double> w; /**< Weights */
    double sum_w;
    std::deque<double> x_est; /**< Estimates for x pose */
    std::deque<double> y_est;
    std::deque<double> ang_est;

    void getEstimate( const double &_x_measured,
		      const double &_y_measured,
		      const double &_ang_measured,
		      double &_x_est,
		      double &_y_est,
		      double &_ang_est );


 private:
    int mNumSteps;
    bool mFlagFull;


};
