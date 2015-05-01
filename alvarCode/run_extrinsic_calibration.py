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
@title Entropy-Based Refinment of Textual Review Scores Using Sentiment Analysis
Created on April 10, 2015

@author: Jonathan Scholz
@author: Kaushik Subramanian
@author: Chad Stolper
'''

import subprocess
import numpy as np
import os
import time

def parse_transform_str(transform_str_lines):
	return np.array([[float(el) for el in L.strip('[').strip('],\n').split(',')] for L in transform_str_lines])

def get_global_transform(cam_id, marker_id):
	cmd = './bin/getCameraTransform %d %d %d' % (cam_id, cam_id, marker_id)
	process = subprocess.Popen(cmd, 
			stdin=subprocess.PIPE, 
			stdout=subprocess.PIPE, 
			shell=True)

	transform_str_lines = []
	while True:
		line = process.stdout.readline()
		print 'read: ', line,
		if 'Detected marker with id:%d' % marker_id in line: 
			# if marker detected, grap the next 4 lines containing the transfomr
			transform_str_lines.append(process.stdout.readline())
			transform_str_lines.append(process.stdout.readline())
			transform_str_lines.append(process.stdout.readline())
			transform_str_lines.append(process.stdout.readline())

			# process.terminate() # doen't work for some reason
			os.system("pkill getCameraTransf")
			time.sleep(1)
			break
		else:
			pass

	T_cam_marker = parse_transform_str(transform_str_lines)
	
	return T_cam_marker

def calibrate_all(rect_1_ids, rect_2_ids, global_id, aux_id):
	T_rect1_globals = []
	for r1id in rect_1_ids:
		T_rect1_globals.append(get_global_transform(r1id, global_id))

	T_rect2_globals = []
	for r2id in rect_2_ids:
		T_rect2_globals.append(get_global_transform(r1id, aux_id))

	import ipdb;ipdb.set_trace()

if __name__ == '__main__':
	calibrate_all([2,3,4,5], [0,1,2,3], global_id=4, aux_id=6)
