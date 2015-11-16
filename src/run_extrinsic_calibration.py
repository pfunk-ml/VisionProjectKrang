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
'''

import subprocess
import numpy as np
import os
import time

import logging

def get_marker_transform_hc(cam_id, marker_id, timeout=5):
	'''
	Hard-coded version of get_marker_transform.  
	The original version seems to cause usb problems with
	the cameras due to issues with v4l on ubuntu 12.04, and 
	we can never get 6 consecutive calls to getCameraTransform
	to work successfully.  
	If you can get it to run at all though, paste in the 
	transforms here and the rest of the calibration 
	process should work.
	'''
	if cam_id == 0 and marker_id == 6:
		return np.array([
			[0.0049698, -0.999795, 0.0196425, 101.551],
			[-0.915179, 0.00336906, 0.403034, 97.699],
			[-0.403018, -0.0199794, -0.914974, 253.681],
			[0, 0, 0, 1]
			])

	elif cam_id == 1 and marker_id == 6:
		return np.array([
			[0.0370507, -0.999279, 0.00826099, -93.3028],
			[-0.943466, -0.0322541, 0.329896, 100.509],
			[-0.329392, -0.0200168, -0.943981, 236.296],
			[0, 0, 0, 1]
			])

	elif cam_id == 2 and marker_id == 6:
		return np.array([
			[0.0199837, -0.997936, -0.0610263, 101.936],
			[-0.998301, -0.0165745, -0.0558679, -29.4638],
			[0.0547411, 0.062039, -0.996571, 180.828],
			[0, 0, 0, 1]
			])

	elif cam_id == 3 and marker_id == 6:
		return np.array([
			[0.00975362, -0.999729, 0.0211292, -90.8994],
			[-0.999948, -0.00968754, 0.00322745, -41.6171],
			[-0.00302189, -0.0211596, -0.999772, 181.577],
			[0, 0, 0, 1]
			])

	elif cam_id == 2 and marker_id == 4:
		return np.array([
			[0.995531, 0.00545728, -0.0942751, 98.7846],
			[-0.00343741, -0.995573, -0.0939291, 60.3185],
			[-0.0943703, 0.0938335, -0.991105, 177.88],
			[0, 0, 0, 1]
			])

	elif cam_id == 3 and marker_id == 4:
		return np.array([
			[0.999016, -0.00796222, 0.043622, -95.1191],
			[-0.00719451, -0.999817, -0.0177279, 47.6227],
			[0.0437552, 0.0173966, -0.998891, 181.6],
			[0, 0, 0, 1]
			])

	elif cam_id == 4 and marker_id == 4:
		return np.array([
			[0.999981, 0.000644149, -0.00616534, 91.8732],
			[-0.00230364, -0.884742, -0.466075, -59.5887],
			[-0.00575496, 0.46608, -0.884724, 235.839],
			[0, 0, 0, 1]
			])

	elif cam_id == 5 and marker_id == 4:
		return np.array([
			[0.998922, 0.0456529, 0.00838936, -80.7382],
			[0.0451711, -0.914494, -0.40207, -81.6923],
			[-0.0106836, 0.402015, -0.915571, 231.816],
			[0, 0, 0, 1]
			])

def parse_transform_str(transform_str_lines):
	return np.array([[float(el) for el in L.strip('[').strip('],\n').split(',')] for L in transform_str_lines])

def get_marker_transform(cam_id, marker_id, timeout=5):
	# return np.eye(4)
	print "------------------------------------------"
	logging.info("Obtaining transform for Camera ID: " + str(cam_id) + " Marker ID: " + str(marker_id))

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

			# process.terminate() # doen't work for some reason
			os.system("pkill getCameraTransf")
			time.sleep(5) # necessary?  why do the camera's keep crashing?!?
			break
		else:
			if time.time() - start_time > timeout:
				os.system("pkill getCameraTransf")
				time.sleep(5)
				logging.error("No marker found before timeout (" + str(timeout) + ' secs)')
				return None

	T_cam_marker = parse_transform_str(transform_str_lines)
	logging.info(str(T_cam_marker))
	print '\n'
	return T_cam_marker

def calibrate_all(rect1_ids, rect2_ids, global_id, aux_id):
	'''
	Prints the (inverse) extrinsics of all cameras with respect 
	to a single "global" marker.  I.E. the pose of the global 
	marker in each camera frame.  This should be plugged into 
	config.json, which allows camCentralProcess to obtain the
	global-pose, and through one more hop, the world-pose of
	any marker seen by any camera.  

	This process is configured for a setup in which it is not
	possible to put the global marker in view of all cameras.  
	Rather than measuring the world pose or offset of multiple
	tags, we place multiple tags simultaneously, and do the 
	math here to obtain all camera poses with respect to the
	global marker.

	:param rect1_ids: The camera ids in the first rectangle, in which 
		global_id is always visible, and aux_id is visible in at least one 
	:param rect2_ids: The camera ids in the second rectangle, in which 
		aux_id is always visible.  At least one camera must be in both
		rect1_ids and rect2_ids.  
	:param global_id: The "global" marker which all cameras are calibrated
		with respect to.  
	:param aux_id: The "auxilliary" marker which is used to calibrate 
		cameras which are not in view of the global marker
	'''
	# select method for obtaining marker transforms:
	get_marker_transform_func = get_marker_transform
	# get_marker_transform_func = get_marker_transform_hc

	# obtain transform of "global" marker in each camera frame from 
	# the first rectangle.
	T_rect1_globals = []
	T_rect1_auxs = []
	for r1id in rect1_ids:
		T_rect1_globals.append(get_marker_transform_func(r1id, global_id))
		T_rect1_auxs.append(get_marker_transform_func(r1id, aux_id))

	# obtain transform of "aux" marker in each camera frame from 
	# the second rectangle.
	T_rect2_auxs = []
	for r2id in rect2_ids:
		T_rect2_auxs.append(get_marker_transform_func(r2id, aux_id))

	# use the common camera (defaults to first in intersection between
	# rect1_ids and rect2_ids) to obtain global pose in the far rect
	common_cams = set(rect1_ids).intersection(set(rect2_ids))
	far_cams = set(rect2_ids).difference(set(rect1_ids))

	# obtain a common camera in which we found an aux transform:
	# cc = common_cams.pop() # pop the first common cam and use it for aux-hopping
	cc = [cam for cam in common_cams if T_rect1_auxs[rect1_ids.index(cam)] is not None][0]
	
	T_fc_globals = []
	for fc in far_cams:
		T_cc_global = T_rect1_globals[rect1_ids.index(cc)]
		T_cc_aux = T_rect1_auxs[rect1_ids.index(cc)]
		T_fc_aux = T_rect2_auxs[rect2_ids.index(fc)]
		T_aux_cc = np.linalg.inv(T_cc_aux)
		T_fc_global = T_fc_aux.dot(T_aux_cc).dot(T_cc_global)
		T_fc_globals.append(T_fc_global)
	
	# pack up results
	all_ids = rect1_ids + list(far_cams)
	all_transforms = T_rect1_globals + T_fc_globals
	
	## print output in a pastable form:
	np.set_printoptions(suppress=True, precision=7)
	print '''// Global to camera transformation matrices
// x, y, z in last columns are in cms.
"T_global2cam" : // i.e T_cam_global
['''
	sorted_ids = sorted(all_ids)
	for idx, cid in enumerate(sorted_ids):
		T = all_transforms[all_ids.index(cid)]
		print "   [   // camera %d" % cid
		for i in range(4):
			# print "      ", ','.join([str(T[i][j]) for j in range(4)]), 
			print "       [%4.6f, %4.6f, %4.6f, %4.6f]" % (T[i,0], T[i,1], T[i,2], T[i,3]),
			print "," if i < 3 else ""
		print "   ]," if idx < len(all_ids)-1 else "   ]" 
	print "],"
	# import ipdb;ipdb.set_trace()

if __name__ == '__main__':

	    # setup up the python logging module
    # levels of logging are: DEBUG, INFO, WARNING, ERROR, CRITICAL
    logging.basicConfig(level = logging.DEBUG,
                        format = "%(filename)s %(lineno)s: [%(levelname)s] %(message)s" )

    logging.info("Extrinsic Calibration Started.")
    
    calibrate_all([2,3,4,5], [0,1,2,3], global_id=4, aux_id=6)