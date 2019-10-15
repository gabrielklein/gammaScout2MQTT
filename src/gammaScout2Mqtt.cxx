/** @file
 *  Main file of gammaScout2Mqtt
    Copyright (C) 2019 Gabriel Klein
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.
*/
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include "VersionConfig.h"
#include "Config.h"
#include "GammaScoutOnline.h"
#include "GMQTT.h"

/**
 * gammaScout2Mqtt
 * -d /dev/ttyUSB0 port where the Gamma Scout is connected to
 * -h localhost host of the MQTT server
 * -p port
 * -u username
 * -P password
 * -t topic 
 */
int main(int argc, char *argv[])
{

  // Read parameters
  Config config(argc, argv);

  // Initialize MQTT
  GMQTT mqtt(&config);
  mqtt.init();

  // Initialize the Gamma Scout
  GammaScoutOnline gso(&config);
  gso.init();

  // Iterate over GammaScout
  timespec ts;
  clock_gettime(CLOCK_REALTIME, &ts);
  int timeLastSent = ts.tv_sec;
  int timeLastReceived = ts.tv_sec;
  while (true)
  {
    clock_gettime(CLOCK_REALTIME, &ts);

    string data = "";
    gso.getUSV(&data);
    if (data.length() > 0)
    {
      timeLastReceived = ts.tv_sec;
      printf("Received: %s\n", data.c_str());
      // Send the value every x seconds
      int dur = ts.tv_sec - timeLastSent;
      if (dur >= config.everys)
      {
        timeLastSent = ts.tv_sec;
        mqtt.send(data);
      }
    }
    else
    {
      int dur = ts.tv_sec - timeLastReceived;
      if (dur >= 10)
      {
        printf("Error, nothing received from the GammaScout since 10s, exit.\n");
        exit(-1);
      }
      mqtt.pool();
    }
  }

  return 0;
}
