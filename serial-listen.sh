#!/bin/bash
#
# serial-listen.sh
#
# This script is used to setup the serial connection on the character device
# file associated with the Arduino UNO. If the Arduino is assigned a
# different device file, change the appropriate variable below.
#
# The baud_rate should match the baud rate used in the program. If the baud rate
# below is not correct, either nothing or garbage will print when this script
# is executed.

device_file=/dev/ttyACM0
baud_rate=115200 # does not work even though makefile

stty -F $device_file $baud_rate raw -clocal -echo
cat $device_file
