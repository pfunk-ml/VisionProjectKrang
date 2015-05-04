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

def parse_transform_str(transform_str_lines):
	return np.array([[float(el) for el in L.strip('[').strip('],\n').split(',')] for L in transform_str_lines])

def get_marker_transform(cam_id, marker_id, timeout=5):
	return np.eye(4)
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
			time.sleep(1) # necessary?  why do the camera's keep crashing?!?
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
		T_rect1_globals.append(get_marker_transform(r1id, global_id))
		T_rect1_auxs.append(get_marker_transform(r1id, aux_id))

	# obtain transform of "aux" marker in each camera frame from 
	# the second rectangle.
	T_rect2_auxs = []
	for r2id in rect2_ids:
		T_rect2_auxs.append(get_marker_transform(r2id, aux_id))

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
			print "      ", T[i], 
			print "," if i < 3 else ""
		print "   ]," if idx < len(all_ids)-1 else "   ]" 
	print "],"
	# import ipdb;ipdb.set_trace()

if __name__ == '__main__':
	calibrate_all([2,3,4,5], [0,1,2,3], global_id=4, aux_id=6)
