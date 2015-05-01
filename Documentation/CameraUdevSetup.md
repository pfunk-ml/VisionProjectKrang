
CAMERA UDEV SETUP

One of the annoyances of using /dev/video* names to identify cameras is that they are dynamically assigned during startup and usb events.  The original work-around for this was to un-plug and re-plug the cameras in the proper order.  However, Linux provides for persistent-naming machinery for this scenario using udev.  

After a bit of googling (see links below), the following steps offered a solution:

1. Identify each camera uniquely by serial number
	* plug in each camera in turn, so that it gets assigned to /dev/video0 (the default)
	* run udev info on it to obtain the serial:
		$ udevadm info -a --name=/dev/video0 | grep -i serial

2. Write a rules file that matches each camera
	* Create a rules file in the udev rules directory, e.g.:
		$ touch /etc/udev/rules.d/10-camera.rules
	* Add a rule line for each camera, setting the appropriate serial:
		$ SUBSYSTEM=="video4linux", ATTRS{serial}=="1054FC68", NAME="krangcam0", SYMLINK="krangcam0"

3. Test rules file to verify matches
	* With the camera still plugged and attached to /dev/video0, run:
		$ sudo udevadm test $(udevadm info -q path -n /dev/video0)
	* Inspect the output, and in particular the last line which should indicate the device
	  node that udev will assign:
	  $ ...
	  run: 'udev-acl --action=add --device=/dev/krangcam0'

RESOURCES

	Ubuntu tutorial: http://ubuntuforums.org/showthread.php?t=168221
	Writing udev rules: http://www.reactivated.net/writing_udev_rules.html
	Debugging: http://askubuntu.com/questions/187251/udev-webcam-rule-read-but-not-respected

--------------------------------------------------------------------------
contents of /etc/udev/rules.d/10-camera.rules:

# camera 5
SUBSYSTEM=="video4linux", ATTRS{serial}=="83E16B6B", NAME="video3" SYMLINK+="video3"

# camera 4
SUBSYSTEM=="video4linux", ATTRS{serial}=="99B35E63", NAME="video3" SYMLINK+="video3"

# camera 3
SUBSYSTEM=="video4linux", ATTRS{serial}=="35506B6F", NAME="video3" SYMLINK+="video3"

# camera 2
SUBSYSTEM=="video4linux", ATTRS{serial}=="C6E35E63", NAME="video2" SYMLINK+="video2"

# camera 1
SUBSYSTEM=="video4linux", ATTRS{serial}=="5E51EA65", NAME="video1" SYMLINK+="video1" 

# camera 0
SUBSYSTEM=="video4linux", ATTRS{serial}=="59A0EA61", NAME="video0" SYMLINK+="video0"


