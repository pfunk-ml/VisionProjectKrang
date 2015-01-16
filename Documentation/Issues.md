ISSUES

Commonly occurring problems


#ISSUE

    Error while running camProcess
         On running,
           $ cd path/to/alvarCode
           $ ach -C cam0_channel -o 666  -m 10 -n 64
           $ ./bin/camProcess 0 0

         Following error Appears:
            VIDIOC_QUERYMENU: Invalid argument

    * Solution 
        Upgrading to OpenCV from 2.4.8 to 2.4.10 worked.

    [By Nehchal Jindal - Jan 8, 2015]

#ISSUE

      Error while runnign camProcess
         On running,
           $ cd path/to/alvarCode
           $ ach -C cam0_channel -o 666  -m 10 -n 64
           $ ./bin/camProcess 0 0

         Following error Appears:
            libv4l2: error setting pixformat: Device or resource busy

    * Solution
        Probably, some other program or process is using the camera device.

        Check which process is using the device.
          $ fuser /dev/video0
          /dev/video0 5859m

        The above commands shows the process ID. Check out process details.
          $ ps axl | grep 5859

        Kill the other process.
          $ kill -9 5859

    [By Nehchal Jindal - Jan 8, 2015]

