#!/bin/sh -e

### set the driver ###
insmod /home/pi/RaspberryPiMouse/lib/Pi?B+/`uname -r`/rtmouse.ko
sleep 1
chmod 666 /dev/rt*
