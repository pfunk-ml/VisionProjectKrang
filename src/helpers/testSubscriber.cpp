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
#include <string>
#include <cstring>

#include <stdio.h>

#include "globalStuff/globalData.h"

#define NDIM 3
ach_channel_t channel;
ach_channel_t debug_channel;

void print_arr_2d(double A[][NDIM], int n);

int main(int argc, char* argv[] ) 
{
  // Copy to char pointers
  char outputChanChar[1024];
  strcpy(outputChanChar, VISION_OBJ_POSES_CHANNEL.c_str());
  char debugChanChar[1024];
  strcpy(debugChanChar, DEBUG_CHANNEL.c_str());

  // open the channels
  int r = ach_open( &channel, outputChanChar, NULL );
  int rdebug = ach_open( &debug_channel, debugChanChar, NULL );

  assert(ACH_OK == r && ACH_OK == rdebug);

  r = ach_flush(&channel);
  r = ach_flush(&debug_channel);

  // test receive
  double rtraj[gConfParams.numObjects][NDIM];
  memset(rtraj, 0, gConfParams.numObjects*NDIM*sizeof(double));
  size_t frame_size;

  while( true ) {

    r = ach_get( &channel, &rtraj, 
		 sizeof(rtraj), 
		 &frame_size, 
		 NULL, 
		 ACH_O_WAIT );
    
    std::cout << "Received traj (visible, x, y, angle): " << std::endl; 
    print_arr_2d( rtraj, gConfParams.numObjects );

    // DEBUG CHANNEL
    r = ach_get( &debug_channel, &rtraj, 
		 sizeof(rtraj), 
		 &frame_size, 
		 NULL, 
		 ACH_O_WAIT );
    
    std::cout << "[DEBUG] Received traj (visible, x, y, angle): " << std::endl; 
    print_arr_2d( rtraj, gConfParams.numObjects );


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
