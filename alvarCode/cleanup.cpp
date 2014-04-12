

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

#include "globalData.h"


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
  
}
