
#!/bin/bash

#KRANG_IP=192.168.1.53

ach -C cam0_channel -o 666  -m 10 -n 64
ach -C cam1_channel -o 666  -m 10 -n 64
ach -C krang_vision -o 666  -m 10 -n 64


#sudo achd -r pull $KRANG_IP krang_global &
#sudo achd -r push $KRANG_IP krang_vision &
#sudo achd -r push $KRANG_IP krang_base_waypts &


echo "Done setting ach channels for cameras and krang_vision"
