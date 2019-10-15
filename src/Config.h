#ifndef INCLUDE_CONFIG_H_
#define INCLUDE_CONFIG_H_

#include <string>
#include <termios.h>
using namespace std;

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <getopt.h>
#include <fstream>


static struct option long_options[] =
    {
        {"version", no_argument, 0, 'v'},
        {"device", required_argument, 0, 'd'},
        {"help", no_argument, 0, '?'},
        {"host", required_argument, 0, 'h'},
        {"port", required_argument, 0, 'p'},
        {"username", required_argument, 0, 'u'},
        {"password", required_argument, 0, 'P'},
        {"topic", required_argument, 0, 't'},
        {"seconds", required_argument, 0, 's'},
        {0, 0, 0, 0}};

class Config
{
public:
    Config(int argc, char *argv[]);

    void usage(char *argv[]);
    void version(char *argv[]);
    void param(int c, char *optarg, char *argv[]);
    bool readConf(char *filename, char *argv[]);

    bool success = false;

    string driver = "/dev/ttyUSB0";
    string host = "127.0.0.1";
    int port = 1883;
    string username = "";
    string password = "";
    string topic = "/gammascout";
    int everys = 30;

    int serial_speed = B460800;

private:
    string& trim(std::string& str, const std::string& chars = "\t\n\v\f\r ");

};

#endif /* INCLUDE_CONFIG_H_ */