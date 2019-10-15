TOFIX !!!
Received: 0.118
Received: 0.117
Received: 0.118
Received: 0.119
Received: 0.121
Received: 0.122
Received: 0.122
Received: 0.122
Received something strange 0,1
terminate called after throwing an instance of 'std::out_of_range'
  what():  basic_string::at: __n (which is 4294967294) >= this->size() (which is 8)
Aborted

https://stackedit.io/app#

-----------------------------

# gammaScout2MQTT
*Linux / Raspberry C++ to send data from a Gamma Scout Online to a MQTT server*

This application is running on a Linux operating system ideally like a Raspberry.

This software will collect data from a [Gamma Scout Online](https://www.gamma-scout.com/en/online/) using USB serial port, clean the data and sent it to a MQTT server.

Example
> ./gammaScout2Mqtt -h 192.168.0.10
> Connecting to Gamma Scout...
> Connected to Gamma Scout Online.
> Connected to mqtt: 192.168.0.10:1883
> Received: 0.169
> Received: 0.169
> Received: 0.175
> Received: 0.175
> Received: 0.163
> Sending [192.168.0.10:1883]: 0.163
> Received: 0.157
> Received: 0.157
> Received: 0.151
> Received: 0.138

# Tested with

 - Gamma Scout Online Hardware Version 
 - Ubuntu 19.04
 - Raspbian (Raspberry PI Zero W)

# Prepare to build it

You need to connect to the terminal.

Add the user that will run the application to "dialout" (allow access to serial port)

    sudo adduser $USER dialout

First update the system

    sudo su
    apt-get update; apt-get upgrade; apt-get dist-upgrade;apt autoremove
    reboot

If you are running a **raspberry**, update the firmware of the raspberry

    sudo rpi-update
    sudo reboot

Install some package to be able to build the application

    sudo apt-get install cmake git
    
You can now clone the project

    git clone https://github.com/gabrielklein/gammaScout2MQTT.git

# Build it

The easiest way to build it is to run

    ./build.sh`

Or if you want to do it manually

    mkdir build
    cd build
    cmake ..
    make

You can get help using

    ./gammaScout2Mqtt --help

# Usage

If you use the following command, data will be sent on the MQTT server 10.0.0.60, topic /gammascout/monitor

    /gammaScout2Mqtt -h 10.0.0.60 -t /gammascout/monitor
    
Some other parameters

    ./gammaScout2Mqtt --help

> ./gammaScout2Mqtt Version 0.1
> 
> Read data from an online gamma scout and send it to a MQTT server
> 
> -?, --help: Display some help
> -v, --version: Display the version of this software
> -d, --device: Device to use, default /dev/ttyUSB0
> 
> Settings for MQTT
> -h, --host: Host, default 127.0.0.1
> -p, --port: Port, default 1883
> -u, --username: User name
> -P, --password: Password
> -s, --seconds: Send the value every s, 0 = as soon as possible, default 30 seconds
> -t, --topic: Base topic, default /GammaScout

# Production

If you want to run it on a production system, 

Exit your cron and add the following line 
> @reboot /{PATH_TO_GAMMASCOUT2MQTT}/start.sh &>/tmp/gammaScout2Mqtt.log
