

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

  // Clean cameras
  int r;
  for( int i = 1; i <= NUM_CAMERAS; ++i ) {
   switch(i) {
    case 1: r = ach_unlink(CAM1_CHANNEL); break;
    case 2: r = ach_unlink(CAM2_CHANNEL); break;
    case 3: r = ach_unlink(CAM3_CHANNEL); break;
    case 4: r = ach_unlink(CAM4_CHANNEL); break;
   }

    if( r == ACH_OK ) {
       std::cout << "\t [CLEANUP] Channel from cam "<<i<< " deleted. UNEXPECTED!!"<< std::endl; 
    } else if ( r == ACH_ENOENT ) { 
       std::cout << "\t [CLEANUP] Channel did not exist. As expected."<< std::endl;
    }	


  }

}
