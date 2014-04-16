/**
 * @file cleanup.cpp
 * @brief Close/Delete any ACH channel that might have been left opened from before
 */

#include <stdlib.h>
#include <errno.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <ctype.h>
#include <stdbool.h>
#include <math.h>

#include <string.h>
#include <inttypes.h>
#include <ach.h>
#include <iostream>

#include "globalStuff/globalData.h"


int main( int argc, char* argv[] ) {

  setGlobalData();

  // Clean cameras
  int r;
  for( int i = 0; i < NUM_CAMERAS; ++i ) {
   
    r = ach_unlink( CAM_CHANNEL_NAME[i].c_str() ); 
    if( r == ACH_OK ) {
      std::cout << "\t [CLEANUP] Channel from cam "<<i<< " deleted. UNEXPECTED!!"<< std::endl; 
    } else if ( r == ACH_ENOENT ) { 
      std::cout << "\t [CLEANUP] Channel did not exist. As expected."<< std::endl;
    }	
    

  }

  // Clean krang_vision
    r = ach_unlink( PERCEPTION_CHANNEL ); 
    if( r == ACH_OK ) {
      std::cout << "\t [CLEANUP] Perception channel deleted. UNEXPECTED!!"<< std::endl; 
    } else if ( r == ACH_ENOENT ) { 
      std::cout << "\t [CLEANUP] Perception channel did not exist. As expected."<< std::endl;
    }


  // Clean debug channel
    r = ach_unlink( DEBUG_CHANNEL ); 
    if( r == ACH_OK ) {
      std::cout << "\t [CLEANUP] Debug channel deleted. UNEXPECTED!!"<< std::endl; 
    } else if ( r == ACH_ENOENT ) { 
      std::cout << "\t [CLEANUP] Debug channel did not exist. As expected."<< std::endl;
    }

  return 0;
  
}
