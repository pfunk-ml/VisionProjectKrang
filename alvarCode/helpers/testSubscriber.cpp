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

void print_arr_2d(double A[][NDIM], int n);

int main(int argc, char* argv[] ) {

  // open the channels
  int r = ach_open( &channel, PERCEPTION_CHANNEL, NULL );
  int rdebug = ach_open( &debug_channel, DEBUG_CHANNEL, NULL );

  assert(ACH_OK == r && ACH_OK == rdebug);

  r = ach_flush(&channel);
  r = ach_flush(&debug_channel);

  // test receive
  double rtraj[NUM_OBJECTS][NDIM] = {0};
  size_t frame_size;

  while( true ) {

    r = ach_get( &channel, &rtraj, 
		 sizeof(rtraj), 
		 &frame_size, 
		 NULL, 
		 ACH_O_WAIT );
    
    std::cout << "Received traj (visible, x, y, angle): " << std::endl; 
    print_arr_2d( rtraj, NUM_OBJECTS );

    // DEBUG CHANNEL
    r = ach_get( &debug_channel, &rtraj, 
		 sizeof(rtraj), 
		 &frame_size, 
		 NULL, 
		 ACH_O_WAIT );
    
    std::cout << "[DEBUG] Received traj (visible, x, y, angle): " << std::endl; 
    print_arr_2d( rtraj, NUM_OBJECTS );


    // Read every second
    usleep(1.0*1e6);

  }

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
