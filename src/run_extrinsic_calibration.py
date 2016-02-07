#!/usr/bin/env python
# Copyright (c) 2008-2012, Georgia Tech Research Corporation
# All rights reserved.
#
# Author(s): Jonathan Scholz <jkscholz@gatech.edu>
# Georgia Instutiute of Technology
#
# This file is provided under the following "BSD-style" License:
#
#   Redistribution and use in source and binary forms, with or
#   without modification, are permitted provided that the following
#   conditions are met:
#
#  * Redistributions of source code must retain the above copyright
#     notice, this list of conditions and the following disclaimer.
#
#  * Redistributions in binary form must reproduce the above
#     copyright notice, this list of conditions and the following
#     disclaimer in the documentation and/or other materials provided
#     with the distribution.
#
#   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
#   CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
#   INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
#   MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
#   DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
#   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
#   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
#   LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
#   USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
#   AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
#   LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
#   ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
#   POSSIBILITY OF SUCH DAMAGE.

'''
@title run_extrinsic_calibration
Created on May 1, 2015

@author: Jonathan Scholz
		 Nehchal Jindal
'''

import subprocess
import numpy as np
import os
import time

import logging

def parse_transform_str(transform_str_lines):
	return np.array([[float(el) for el in L.strip('[').strip('],\n').split(',')] for L in transform_str_lines])

def get_marker_transform(cam_id, marker_id, timeout=3):
	'''
	Calculates the transform from marker frame to camera frame. It starts the 
	getCameraTransform program, and reads the transform from stdout. None is 
	returned if |cam_id| or |marker_id| is None.
	Args:
	  cam_id: (int) The camera ID
	  marker_id: (int) The marker ID
	  timeout: (float) The maximum time to wait for getCameraTransform program 
	  		to detect the marker

	Returns transformation matrix as 4x4 numpy.ndarray if marker is visible in 
	camera FOV. None, otherwise.
	'''
	print "------------------------------------------"
	logging.info("Obtaining transform for Camera ID: " + str(cam_id) + " Marker ID: " + str(marker_id))

	if cam_id is None or marker_id is None:
		return None

	cmd = './bin/getCameraTransform %d %d %d' % (cam_id, cam_id, marker_id)
	process = subprocess.Popen(cmd, 
			stdin=subprocess.PIPE, 
			stdout=subprocess.PIPE, 
			shell=True)

	transform_str_lines = []
	start_time = time.time()
	while True:
		line = process.stdout.readline()
		#print 'read: ', line,
		if 'Detected marker with id:%d' % marker_id in line: 
			# if marker detected, grap the next 4 lines containing the transform
			for i in range(4):
				transform_str_lines.append(process.stdout.readline())

			#process.terminate() # doen't work for some reason
			os.system("pkill getCameraTransf")
			T_cam_marker = parse_transform_str(transform_str_lines)
			logging.info(str(T_cam_marker))

			#time.sleep(5) # necessary?  why do the camera's keep crashing?!?
			break
		else:
			if time.time() - start_time > timeout:
				os.system("pkill getCameraTransf")
				time.sleep(5)
				logging.error("No marker found before timeout (" + str(timeout) + ' secs)')
				return None
	
	print '\n'

	return T_cam_marker

def print_transforms(T_cam_global_dict):
    ''' Prints the transforms which can be copy-pasted into config.json`
    Arg(s)
     T_cam_global_dict: (dict) key is camera id and value is numpy 4x4 matrix 
        representing T_cam_global

    Returns None '''
    print "\"T_global2cam\" : // i.e T_cam_global]\n" \
        + "["
    
    for idx, cam_id in enumerate(T_cam_global_dict.keys()):
        T = T_cam_global_dict[cam_id]
        print "   [   // camera %d" % cam_id
        for i in range(4):
            # print "      ", ','.join([str(T[i][j]) for j in range(4)]), 
            print "       [%4.6f, %4.6f, %4.6f, %4.6f]" % (T[i,0], T[i,1], T[i,2], T[i,3]),
            print "," if i < 3 else ""
        print "   ]," if idx < len(T_cam_global_dict.keys())-1 else "   ]" 
    print "],"

def calibrate_all(marker_ids, cam_ids_list):
    ''' Prints the (inverse) extrinsics of all cameras with respect to
    a single "global" marker, i.e. the pose of the global marker in each
    camera frame. This should be plugged into config.json

    Args:
     marker_ids: (list of ints) IDs of markers in-order starting with global 
        marker id
     cam_ids_list: (list of lists) ith element is list of camera ids which can
        see ith marker in |marker_ids|.

    Returns True, if calibration was successful. False, otherwise. '''

    assert(len(marker_ids) == len(cam_ids_list))

    # Dictionary of global marker to cameras transforms keyed by camera id
    T_cam_global = {}

    for ind, marker_id in enumerate(marker_ids):
        cam_ids = cam_ids_list[ind]

        # Get T_marker_global (Transform from global marker frame to marker_id
        # frame)
        if ind == 0:
            prev_cam_ids = []
            T_marker_global = np.identity(4)
        else:
            # Find common cam (ccam). Get T_ccam_marker
            prev_cam_ids = cam_ids_list[ind-1]
            common_cams = set(cam_ids).intersection(set(prev_cam_ids))

            # Get any element of common_cams
            ccam = next(iter(common_cams))

            T_ccam_marker = get_marker_transform(ccam, marker_id)
            assert(T_ccam_marker is not None)

            T_ccam_global = T_cam_global[ccam]
            T_marker_global = np.linalg.inv(T_ccam_marker).dot(T_ccam_global)

        # For all cams not common. Get T_cam_marker
        # Multiply it by T_marker_global to get T_cam_global
        diff_cams = set(cam_ids).difference(set(prev_cam_ids))

        for cam_id in diff_cams:
            T_cam_marker = get_marker_transform(cam_id, marker_id)
            assert(T_cam_marker is not None)

            T_cam_global[cam_id] = T_cam_marker.dot(T_marker_global)

    print_transforms(T_cam_global)

    return True

if __name__ == '__main__':

	# setup up the python logging module
    # levels of logging are: DEBUG, INFO, WARNING, ERROR, CRITICAL
    logging.basicConfig(level = logging.DEBUG,
                        format = "%(filename)s %(lineno)s: [%(levelname)s] %(message)s" )

    logging.info("Extrinsic Calibration Started.")
    
    res = calibrate_all([6, 2, 4],[[5,4,3,2], [3,2,1,0], [1, 6]] )

    if res:
    	logging.info("Extrinsic calibration successfully completed.")
    else:
    	logging.fatal("Extrinsic calibration failed.")
