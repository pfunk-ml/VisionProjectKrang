        ACH CHANNELS AND MESSAGE FORMATS
              Author: Nehchal Jindal
             Created On: Jan 8, 2015

    The vision system communicates the object positions via ACH channel. ACH is a publisher-subscriber framework.

THE CHANNELS

    Data from camera is published every fixed interval in the a packet. Let us call it MarkerMessage. MarkerMessage from each camera node is published to particular ACH channel.

        +----------------+--------------------+
        |    Camera ID   |  ACH channel name  |
        |----------------+--------------------+
        |       0        |  cam0_channel      |
        |       1        |  cam1_channel      |
        |       2        |  cam2_channel      |
        |       3        |  cam3_channel      |
        +----------------+--------------------+
      Table: The channels to which each camera publishes data

MESSAGE FORMATS

    ACH supports fixed-length data packets. The message length is specified during channel creation.

    Message length for MarkerMessage is 64 bytes.

    The message format is defined in file alvarCode/camProc/Object.h using struct MarkerMsg_t .

   

    According to camProcess.cpp, the message length is 336 bytes (112 bytes for each of 3 objects). But channel size is 64 ! Huh !

    
UNITS

    Angles are in radians. 

    The distances are in cms when published to camX_channel and in meters when published to krang_vision
