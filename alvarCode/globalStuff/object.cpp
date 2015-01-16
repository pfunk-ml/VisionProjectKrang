/**
 * @file object.cpp
 */

#include "Object.h"
#include <iostream>

void Object_printMarkerMsg(const MarkerMsg_t *markerMsg){
    int i, j;
    std::cout<<"Object ID:"<<markerMsg->marker_id<<'\n';
    std::cout<<"Is Visible:"<<markerMsg->visible<<'\n';
    std::cout<<"Transformation Matrix\n";
    for(i=0; i<3; i++){
        for(j=0; j<4; j++)
            std::cout<<markerMsg->trans[i][j];
        std::cout<<'\n';
    }
    std::cout<<'\n';
    std::cout<<"Camera ID:"<<markerMsg->cam_id<<'\n';

    return;
}
