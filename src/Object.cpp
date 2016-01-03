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

    std::cout<<"Marker ID:"<<markerMsg->marker_id<<'\n';
    std::cout<<"Is Visible:"<<markerMsg->visible<<'\n';
    std::cout<<"Transformation Matrix\n";

    for(i=0; i<3; i++){
        for(j=0; j<4; j++)
            //std::cout<<markerMsg->trans[i][j];
            printf("%9.3f\t", markerMsg->trans[i][j]);
        std::cout<<'\n';
    }
    std::cout<<'\n';

    transMat = utils_getDoubleArrAsMat(markerMsg->trans);
    utils_getXYZAng(transMat, x, y, z, theta);
    std::cout<<"x-coorinate: "<<x<<'\n';
    std::cout<<"y-coorinate: "<<y<<'\n';
    std::cout<<"y-coorinate: "<<z<<'\n';
    std::cout<<"theta: "<<theta<<'\n';
    
    std::cout<<"--\n";
    return;
}

void Object_printMarkerMsgSingleLine(const MarkerMsg_t *markerMsg) {
    double x, y, z, theta;
    Eigen::Matrix4d transMat;

    printf(" \t id: %d ", markerMsg->marker_id);

    if (!markerMsg->visible){
        printf("[Marker not visible.]\n");
        return;
    }

    transMat = utils_getDoubleArrAsMat(markerMsg->trans);
    utils_getXYZAng(transMat, x, y, z, theta);
    printf("x: %.4f y: %.4f z: %.4f theta: %.4f \n", x, y, z, theta);
}
