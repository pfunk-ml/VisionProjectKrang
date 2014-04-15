#!/bin/bash

# Krang IP address
KRANG_IP=192.168.1.53

# Send channels opened from camProcess onto the server
sudo achd -r push $KRANG_IP cam0_channel &
sudo achd -r push $KRANG_IP cam1_channel &

echo "Done sending stuff over the server!"
