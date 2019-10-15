#!/bin/bash

# This script will scan USB port and display a "user friendly" version
# 
# Based on this piece of code : https://gist.github.com/ScottHelme/e793516d2b322ed3a4c795a429eb5d04

# Only show devices that start with tty (ttyUSB0, ...)
FILTER="tty"

for sysdevpath in $(find /sys/bus/usb/devices/usb*/ -name dev | grep "$FILTER" ); do
    (
        syspath="${sysdevpath%/dev}"
        devname="$(udevadm info -q name -p $syspath)"
        [[ "$devname" == "bus/"* ]]
        eval "$(udevadm info -q property --export -p $syspath)"
        [[ -z "$ID_SERIAL" ]]
        echo "/dev/$devname - $ID_SERIAL"
    )
done
