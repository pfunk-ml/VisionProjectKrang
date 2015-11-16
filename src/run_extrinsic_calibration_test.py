'''
Created on Nov 16, 2015

@author: Nehchal Jindal
Georgia Institute of Technology

Description: This is test file for run_extrinsic_calibration.py
'''

import run_extrinsic_calibration as calib

import logging

if __name__ == '__main__':
    # setup up the python logging module
    # levels of logging are: DEBUG, INFO, WARNING, ERROR, CRITICAL
    logging.basicConfig(level = logging.DEBUG,
                        format = "%(filename)s %(lineno)s: [%(levelname)s] %(message)s" )

    logging.info("Testing get_marker_transform() function ...")

    for cam_id, marker_id in [(0, 6), (1, 6), (2, 6), (3, 6), (2, 4), (3, 4), (4, 4), (5, 4)]:
        T_cam_marker = calib.get_marker_transform(0, 6)
        assert T_cam_marker is not None
        print T_cam_marker, '\n'

    T_cam_marker = calib.get_marker_transform(1, 5)
    assert T_cam_marker is not None
    print T_cam_marker, '\n'