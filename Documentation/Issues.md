ISSUES

Commonly occurring issues and problems


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

      Error while running camProcess
         On running,
           $ cd path/to/alvarCode
           $ ach -C cam0_channel -o 666  -m 10 -n 64
           $ ./bin/camProcess 0 0

         Following error appears:
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

#ISSUE

      Error while running camProcess
         On running,
           $ cd path/to/alvarCode
           $ ach -C cam0_channel -o 666  -m 10 -n 64
           $ ./bin/camProcess 0 0

         Following error appears:
            libv4l2: error turning on stream: Invalid argument

      The problem occured when previous intance of camProcess was terminated by pressing Ctrl + C .

    * Solution
        No reported solution.
        Try physically disconnecting the camera and again reconnecting the camera to the computer.

        [By Nehchal Jindal - Jan 20, 2015]
