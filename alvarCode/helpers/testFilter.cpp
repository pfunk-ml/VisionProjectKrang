/**
 * @file testFilter.cpp
 */
#include "../basicFilter.h"
#include <iostream>
#include <random>
#include <stdlib.h>
#include <stdio.h>

/**
 * @function main
 */
int main( int argc, char *argv[] ) {

  std::vector<double> samples;
  std::vector<double> estimate;

  std::default_random_engine generator;
  std::normal_distribution<double> distribution(2.0,0.02);


  // Generate samples
  int N = 100;

  for( int i = 0; i < N; ++i ) {
    samples.push_back( distribution(generator) );
  }

  for( int i = 0; i < N; ++i ) {
    std::cout << "Sample ["<<i<<"]: "<< samples[i] <<std::endl;
  }

  // Create the filter
  int numSteps = 10;
  basicFilter BF( numSteps );
  BF.set_default_weights();

  std::cout << "Default weights: "<< std::endl;
  for( int i = 0; i < BF.w.size(); ++i ) {
    std::cout<< "w["<<i<<"]: "<<BF.w[i]<<std::endl;
  }
  std::cout <<"Sum of weights: "<< BF.sum_w<< std::endl;

  // Get estimates
  double est1, est2, est3;
  for( int i = 0; i < N; ++i ) {
    BF.getEstimate( samples[i], samples[i], samples[i],
		    est1, est2, est3 );
    estimate.push_back( est1 );
    std::cout <<"["<<i<<"] Measure: "<< samples[i]<< " Estimate: "<<est1 << std::endl;
  }

  FILE* f;
  f = fopen("testFilter.txt", "w");
  for( int i = 0; i < N; ++i ) {
    fprintf( f, "%d %f %f \n", i, samples[i], estimate[i] );
  }
  fclose(f);


  return 0;
}
