# Building rtl_433


Building rtl_433 with RTL-SDR or SoapySDR support is optional but using RTL-SDR is highly recommended.
The libraries and header files for RTL-SDR and/or SoapySDR should be installed beforehand.

## Linux

Depending on your system, you may need to install the following libraries.

Debian:

    sudo apt-get install libtool libusb-1.0-0-dev librtlsdr-dev rtl-sdr build-essential autoconf cmake pkg-config


### CMake

Installation using CMake:

    cd gammaScout2Mqtt
    mkdir build
    cd build
    cmake ..
    make
    make install


