#include "Config.h"
#include "VersionConfig.h"

static int verbose_flag;

Config::Config(int argc, char *argv[])
{
    int c, ch;
    int option_index = 0;

    /**
 * gammaScout2Mqtt
 * -d /dev/ttyUSB0 port where the Gamma Scout is connected to
 * -h localhost host of the MQTT server
 * -p port
 * -u username
 * -P password
 * -t topic 
 */

    // Read configuration
    readConf("gammaScout2Mqtt.conf", argv);

    // Parse parameters
    while ((c = getopt_long(argc, argv, "v?d:h:p:u:P:t:s:",
                            long_options, &option_index)) != -1)
    {
        param(c, optarg, argv);
    }

    this->success = true;
}

/**
 * Read a configuration file
 */
bool Config::readConf(char *filename, char *argv[])
{

    FILE *file;
    if (!(file = fopen(filename, "r")))
    {
        fprintf(stdout, "Configuration file: %s not found\n", filename);
        return false;
    }
    fclose(file);

    fprintf(stdout, "Reading configuration file: %s\n", filename);

    std::ifstream input(filename);
    for (std::string line; getline(input, line);)
    {
        line = trim(line);
        if (line.length() > 0 && line.at(0) != '#')
        {
            int p = line.find_first_of("\t \\=");
            string key;
            string value;
            if (p < 0)
            {
                key = line;
            }
            else
            {
                key = line.substr(0, p);
                value = line.substr(p + 1);
                value = trim(value);
            }

            for (struct option opt : long_options)
            {
                if (opt.name == 0)
                    break;
                if (key.compare(opt.name) == 0)
                {
                    param(opt.val, (char *)value.c_str(), argv);
                }
            }
        }
    }

    input.close();

    return true;
}

/**
 * Trim a string
 */
string &Config::trim(std::string &str, const std::string &chars)
{
    // Source: http://www.martinbroadhurst.com/how-to-trim-a-stdstring.html
    str.erase(0, str.find_first_not_of(chars));
    str.erase(str.find_last_not_of(chars) + 1);
    return str;
}

/**
 * Add a param to the configuration
 */
void Config::param(int c, char *optarg, char *argv[])
{
    switch (c)
    {
    case 'd': //device
        this->driver = optarg;
        break;

    case 'h': //host
        this->host = optarg;
        break;

    case 'v': //version
        this->version(argv);
        return;

    case 'p': //port
        this->port = atoi(optarg);
        break;

    case 'u': //username
        this->username = optarg;
        break;

    case 'P': //password
        this->password = optarg;
        break;

    case 't': // topic
        this->topic = optarg;
        break;

    case 's': //seconds
        this->everys = atoi(optarg);
        break;

    case '?': // help
        this->usage(argv);
        return;

    default:
        //abort();
        break;
    }
}

/**
 * Show usage and set "success" to false
 */
void Config::usage(char *argv[])
{
    fprintf(stdout, "%s Version %d.%d\n",
            argv[0],
            VERSION_MAJOR,
            VERSION_MINOR);

    fprintf(stdout, "\nRead data from an online gamma scout and send it to a MQTT server\n\n");
    fprintf(stdout, "-?, --help: Display some help\n");
    fprintf(stdout, "-v, --version: Display the version of this software\n");
    fprintf(stdout, "-d, --device: Device to use, default /dev/ttyUSB0\n");
    fprintf(stdout, "\nSettings for MQTT\n");
    fprintf(stdout, "-h, --host: Host, default 127.0.0.1\n");
    fprintf(stdout, "-p, --port: Port, default 1883\n");
    fprintf(stdout, "-u, --username: User name\n");
    fprintf(stdout, "-P, --password: Password\n");
    fprintf(stdout, "-s, --seconds: Send the value every s, 0 = as soon as possible, default 30 seconds\n");
    fprintf(stdout, "-t, --topic: Base topic, default /gammascout\n");

    this->success = false;
    exit(-1);
}

/**
 * Show version and set "success" to false 
 */
void Config::version(char *argv[])
{
    fprintf(stdout, "%s Version %d.%d\n",
            argv[0],
            VERSION_MAJOR,
            VERSION_MINOR);
    fprintf(stdout, "Usage: %s --help to get more informations\n", argv[0]);
    this->success = false;
    exit(-1);
}