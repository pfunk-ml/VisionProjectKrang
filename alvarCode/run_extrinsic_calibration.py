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
			[0.00301466, -0.999006, 0.0444647, 98.1307],
			[-0.918255, 0.0148417, 0.395711, 86.7363],
			[-0.395978, -0.0420229, -0.917298, 241.414],
			[0, 0, 0, 1]
			])

	elif cam_id == 1 and marker_id == 6:
		return np.array([
			[0.0264212, -0.999625, 0.00723629, -90.3013],
			[-0.947547, -0.0227369, 0.318806, 90.3835],
			[-0.318521, -0.0152799, -0.947793, 227.253],
			[0, 0, 0, 1]
			])

	elif cam_id == 2 and marker_id == 6:
		return np.array([
			[0.0171995, -0.997923, -0.0620855, 102.191],
			[-0.998809, -0.0143131, -0.0466397, -37.1821],
			[0.0456542, 0.0628137, -0.996981, 181.257],
			[0, 0, 0, 1]
			])

	elif cam_id == 3 and marker_id == 6:
		return np.array([
			[-0.00318377, -0.99919, 0.0401201, -91.5387],
			[-0.99962, 0.00208108, -0.0274965, -49.6838],
			[0.0273907, -0.0401924, -0.998816, 183.519],
			[0, 0, 0, 1]
			])

	elif cam_id == 2 and marker_id == 4:
		return np.array([
			[0.995278, -0.0112632, -0.0964085, 98.8891],
			[-0.0170429, -0.998092, -0.0593375, 60.1939],
			[-0.0955563, 0.0607004, -0.993572, 178.075],
			[0, 0, 0, 1]
			])

	elif cam_id == 3 and marker_id == 4:
		return np.array([
			[0.999751, -0.00344481, 0.0220513, -94.7126],
			[-0.00365603, -0.999948, 0.00954557, 47.4743],
			[0.0220173, -0.00962381, -0.999711, 181.201],
			[0, 0, 0, 1]
			])

	elif cam_id == 4 and marker_id == 4:
		return np.array([
			[0.99997, 0.00341812, -0.00701323, 91.793],
			[-0.000231135, -0.885545, -0.464554, -59.6358],
			[-0.00779843, 0.464542, -0.885517, 236.009],
			[0, 0, 0, 1]
			])

	elif cam_id == 5 and marker_id == 4:
		return np.array([
			[0.998853, 0.0469302, 0.00950442, -80.3105],
			[0.0468563, -0.917106, -0.395879, -81.4092],
			[-0.00986211, 0.39587, -0.918253, 230.785],
			[0, 0, 0, 1]
			])

def parse_transform_str(transform_str_lines):
	return np.array([[float(el) for el in L.strip('[').strip('],\n').split(',')] for L in transform_str_lines])

def get_marker_transform(cam_id, marker_id, timeout=5):
	# return np.eye(4)
	cmd = './bin/getCameraTransform %d %d %d' % (cam_id, cam_id, marker_id)
	process = subprocess.Popen(cmd, 
			stdin=subprocess.PIPE, 
			stdout=subprocess.PIPE, 
			shell=True)

	transform_str_lines = []
	start_time = time.time()
	while True:
		line = process.stdout.readline()
		print 'read: ', line,
		if 'Detected marker with id:%d' % marker_id in line: 
			# if marker detected, grap the next 4 lines containing the transform
			for i in range(4):
				transform_str_lines.append(process.stdout.readline())

			# process.terminate() # doen't work for some reason
			os.system("pkill getCameraTransf")
			time.sleep(10) # necessary?  why do the camera's keep crashing?!?
			break
		else:
			if time.time() - start_time > timeout:
				print "No marker found before timeout"
				return None

	T_cam_marker = parse_transform_str(transform_str_lines)
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

	# obtain transform of "global" marker in each camera frame from 
	# the first rectangle.
	T_rect1_globals = []
	T_rect1_auxs = []
	for r1id in rect1_ids:
		T_rect1_globals.append(get_marker_transform_hc(r1id, global_id))
		T_rect1_auxs.append(get_marker_transform_hc(r1id, aux_id))

	# obtain transform of "aux" marker in each camera frame from 
	# the second rectangle.
	T_rect2_auxs = []
	for r2id in rect2_ids:
		T_rect2_auxs.append(get_marker_transform_hc(r2id, aux_id))

	# use the common camera (defaults to first in intersection between
	# rect1_ids and rect2_ids) to obtain global pose in the far rect
	common_cams = set(rect1_ids).intersection(set(rect2_ids))
	far_cams = set(rect2_ids).difference(set(rect1_ids))

	# obtain a common camera in which we found an aux transform:
	# cc = common_cams.pop() # pop the first common cam and use it for aux-hopping
	cc = [cam for cam in common_cams if T_rect1_auxs[rect1_ids.index(cam)] is not None][0]
	
	T_fc_globals = []
	for fc in far_cams:
		T_fc_aux = T_rect2_auxs[rect2_ids.index(fc)]
		T_cc_aux = T_rect1_globals[rect1_ids.index(cc)]
		T_aux_cc = np.linalg.inv(T_cc_aux)
		T_cc_global = T_rect1_globals[rect1_ids.index(cc)]
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
	for idx, cid in enumerate(sorted(all_ids)):
		T = all_transforms[all_ids.index(sorted(all_ids)[0])]
		print "   [   // camera %d" % cid
		for i in range(4):
			# print "      ", ','.join([str(T[i][j]) for j in range(4)]), 
			print "       [%4.6f, %4.6f, %4.6f, %4.6f]" % (T[i,0], T[i,1], T[i,2], T[i,3]),
			print "," if i < 3 else ""
		print "   ]," if idx < len(all_ids)-1 else "   ]" 
	print "],"
	# import ipdb;ipdb.set_trace()

if __name__ == '__main__':
	calibrate_all([2,3,4,5], [0,1,2,3], global_id=4, aux_id=6)
