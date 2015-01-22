/**
 * @file object.cpp
 */

#include "Object.h"
#include <iostream>
 #include <stdio.h>

void Object_printMarkerMsg(const MarkerMsg_t *markerMsg) {
    int i, j;
    double x, y, z, theta;
    Eigen::Matrix4d transMat;

    std::cout<<"Object ID:"<<markerMsg->marker_id<<'\n';
    std::cout<<"Is Visible:"<<markerMsg->visible<<'\n';
    std::cout<<"Transformation Matrix\n";

    for(i=0; i<3; i++){
        for(j=0; j<4; j++)
            //std::cout<<markerMsg->trans[i][j];
            printf("%9.3f\t", markerMsg->trans[i][j]);
        std::cout<<'\n';
    }
    std::cout<<'\n';

    transMat = getDoubleArrAsMat(markerMsg->trans);
    getXYangTriple(transMat, x, y, theta);
    z = markerMsg->trans[2][3];
    std::cout<<"x-coorinate: "<<x<<'\n';
    std::cout<<"y-coorinate: "<<y<<'\n';
    std::cout<<"y-coorinate: "<<z<<'\n';
    std::cout<<"theta: "<<theta<<'\n';
    
    std::cout<<"Camera ID:"<<markerMsg->cam_id<<'\n';
    std::cout<<"--\n";
    return;
}
