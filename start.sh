#!/bin/bash

cd "$(dirname "$0")"


# Test if configuration file exists
FILE=gammaScout2Mqtt.conf
if test -f "$FILE"; then
    echo "Using configuration file $FILE."
else
    echo "You need to copy gammaScout2Mqtt.conf.sample to gammaScout2Mqtt.conf and configure this file!"
    exit 1
fi

# Execute gammaScout2Mqtt
while true; do
    echo "Starting gammaScout2Mqtt..."
    ./gammaScout2Mqtt
	sleep 15
done
