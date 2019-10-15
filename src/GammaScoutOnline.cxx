#include "GammaScoutOnline.h"

GammaScoutOnline::GammaScoutOnline(Config *config)
{
    this->config = config;
}

/**
 * Connect to the serial port.
 * Exit in case of error 
 */
void GammaScoutOnline::init()
{

    fprintf(stdout, "Connecting to Gamma Scout...\n");

    if (!serial.connect(config))
    {
        //printf("Error connection to %s\n", config->driver.c_str());
        printf("Exit...\n");
        exit(-1);
    }

    std::string st;
    goInOnlineMode();

    fprintf(stdout, "Connected to Gamma Scout Online.\n");
}

/**
 * Get data (if anything available)
 */
void GammaScoutOnline::getUSV(string *output)
{

    string data;
    serial.receive(&data);


    if (data.length() == 0)
    {
        *output = "";
        return;
    }

    if (data.find("uSv/h") != std::string::npos && data.at(data.length() - 10) == ',')
    {
        data.replace(data.length() - 10, 1, ".");
        *output = data.substr(0, data.length() - 6 /*remove uSv/h*/);
    }
    else
    {
        printf("Received something strange %s\n", data.c_str());
        *output = "";
    }
}

/**
 * Go in "online2 mode", return true in case of success
 */
bool GammaScoutOnline::goInOnlineMode()
{

    std::string st;

    // If we find uSv/h, we are already in the correct mode (quick start)
    serial.receive(&st, 3);
    if (st.find("uSv/h") != std::string::npos)
    {
        return true;
    }

    // 'X'	exits online mode
    serial.send("X");
    serial.receive(&st);

    //'R'	switches to doserate online mode (only for online devices)
    serial.send("R");
    serial.receive(&st);

    for (int i = 0; i < 3; i++)
    {
        serial.receive(&st);
        // v = version
        serial.send("v");
        serial.receive(&st, 3);

        // If we receive Standard, it's probably a device that doesn't support online mode
        if (st.find("Standard") != std::string::npos)
        {
            fprintf(stdout, "Online mode doesn't seams to be available on this device!\n");
            fprintf(stdout, "You need to have a Gamma Scout Online version.\n");
            fprintf(stdout, "This software was not tested with versions of the firmware before 7.0.\n");
            exit(-1);
        }

        // We expect to received Online 2
        if (st.find("Online 02") != std::string::npos)
        {
            // Wait to receive xxxx uSv/h
            serial.receive(&st, 5);
            if (st.find("uSv/h") != std::string::npos)
            {
                return true;
            }
        }
    }

    fprintf(stdout, "The connected device doesn't respond as expected!\n");
    fprintf(stdout, "You need to have a Gamma Scout Online.\n");
    fprintf(stdout, "This software was not tested with versions of the firmware before 7.0.\n");
    exit(-1);

    // return false;
}