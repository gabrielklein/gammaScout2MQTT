#!/bin/bash

# This script will create the build directory and compile the application

FILE=src/gammaScout2Mqtt.cxx
if test -f "$FILE"; then
    echo "Compiling...."
else
    echo "src/gammaScout2Mqtt.cxx doesn't exists, are you at the right place?"
    exit 1
fi

rm -rf gammaScout2Mqtt
rm -rf build
mkdir build
cd build
cmake ..
make

cp src/gammaScout2Mqtt ../

cd ..

# Display usage

echo ""
echo "-------------------------"
echo ""
echo "Usage:"
echo ""
./gammaScout2Mqtt --help
echo ""


FILE=gammaScout2Mqtt.conf
if test -f "$FILE"; then
    echo "Done"
else
    echo "You need to copy gammaScout2Mqtt.conf.sample to gammaScout2Mqtt.conf and configure this file"
    exit 1
fi


