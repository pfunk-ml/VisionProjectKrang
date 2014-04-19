/**
 * @file testSubscriber.cpp
 * @brief Test if perception data is arriving all right
 */
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <stdint.h>
#include <assert.h>
#include <ach.h>
#include <iostream>

#include <stdio.h>

#include "globalStuff/globalData.h"

#define NDIM 3
ach_channel_t channel;
ach_channel_t debug_channel;


// LOGGING FOR DEBUG
FILE* pF[NUM_OBJECTS];

/**
 * @brief Close before leaving
 */
void closeFiles() {

  // Close files for debug
  printf("Closing files! \n");
  for( int i = 0; i < NUM_OBJECTS; ++i ) {
    fclose( pF[i] );
  }

}


void print_arr_2d(double A[][NDIM], int n);

int main(int argc, char* argv[] ) {

  atexit( closeFiles );

  // open the channels
  int r = ach_open( &channel, PERCEPTION_CHANNEL, NULL );
  int rdebug = ach_open( &debug_channel, DEBUG_CHANNEL, NULL );

  assert(ACH_OK == r && ACH_OK == rdebug);

  r = ach_flush(&channel);
  r = ach_flush(&debug_channel);

  // test receive
  double percep[NUM_OBJECTS][NDIM] = {0};
  double debug[NUM_OBJECTS][NDIM] = {0};
  size_t frame_size;

  // Open files for debug
  for( int i = 0; i < NUM_OBJECTS; ++i ) {
   char name[50];
   sprintf( name, "logging_est%d.txt", i );
   pF[i] = fopen( name, "w+" );
  }

  int counter = 0;
  while( true ) {

    if( counter > 60 ) {
      std::cout << "Getting out of loop"<< std::endl;
      break;
    }

    // PERCEP CHANNEL
    r = ach_get( &channel, &percep, 
		 sizeof(percep), 
		 &frame_size, 
		 NULL, 
		 ACH_O_WAIT );
    
    std::cout << "Received traj (visible, x, y, angle): " << std::endl; 
    print_arr_2d( percep, NUM_OBJECTS );

    // DEBUG CHANNEL
    r = ach_get( &debug_channel, &debug, 
		 sizeof(debug), 
		 &frame_size, 
		 NULL, 
		 ACH_O_WAIT );
    
    std::cout << "[DEBUG] Received traj (visible, x, y, angle): " << std::endl; 
    print_arr_2d( debug, NUM_OBJECTS );


    // Store them (to cm and to degrees)
    for( int i = 0; i < NUM_OBJECTS; ++i ) {
       fprintf( pF[i], " %f %f %f %f %f %f \n", percep[i][0]*100.0, percep[i][1]*100.0, percep[i][2]*57.3, debug[i][0]*100.0, debug[i][1]*100.0, debug[i][2]*57.3 );
    }

    // Read every second
    usleep(0.25*1e6);

     counter++;
  }

  return 0;

}


/** print_arr_2d */
void print_arr_2d(double A[][NDIM], int n) {

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < NDIM; j++) {     
      printf("%2.3lf ", A[i][j] );      
    }    
    printf("\n");
  }
  
}
