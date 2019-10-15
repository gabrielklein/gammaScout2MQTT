#include "GMQTT.h"
#include "mongoose.h"

// --------------------------------

// We use this method to link c++ and c.
// It will not work if multiple instances of this class are created at the same time.
GMQTT *myGMqtt = NULL;
static void ev_handler_static(struct mg_connection *nc, int ev, void *p)
{
    if (myGMqtt != NULL)
    {
        myGMqtt->ev_handler(nc, ev, p);
    }
}

// --------------------------------

GMQTT::GMQTT(Config *config)
{
    this->config = config;

    address += config->host;
    address += ":";
    address += to_string(config->port);

    datatosend = "";
    connected = false;
}

void GMQTT::ev_handler(struct mg_connection *nc, int ev, void *p)
{
    struct mg_mqtt_message *msg = (struct mg_mqtt_message *)p;
    (void)nc;

    //if (ev != MG_EV_POLL)
    //    printf("USER HANDLER GOT EVENT %d\n", ev);

    switch (ev)
    {

    //define MG_EV_POLL 0    /* Sent to each connection on each mg_mgr_poll() call */
    case MG_EV_POLL:
    {
        if (connected)
        {
            if (datatosend.length() > 0)
            {
                mg_mqtt_publish(nc, config->topic.c_str(), 65, MG_MQTT_QOS(0), datatosend.c_str(), datatosend.length());
                printf("Sending [%s]: %s\n", address.c_str(), datatosend.c_str());
                datatosend = "";
            }
        }
        break;
    }

    //define MG_EV_ACCEPT 1  /* New connection accepted. union socket_address * */
    case MG_EV_ACCEPT:
    {

        break;
    }

    // define MG_EV_CONNECT 2 /* connect() succeeded or failed. int *  */
    case MG_EV_CONNECT:
    {
        struct mg_send_mqtt_handshake_opts opts;
        memset(&opts, 0, sizeof(opts));
        if (config->username.length() > 0)
            opts.user_name = config->username.c_str();
        else
            opts.user_name = NULL;
        if (config->password.length() > 0)
            opts.password = config->password.c_str();
        else
            opts.password = NULL;

        mg_set_protocol_mqtt(nc);
        mg_send_mqtt_handshake_opt(nc, "dummy", opts);
        break;
    }

    //define MG_EV_CLOSE 5   /* Connection is closed. NULL */
    case MG_EV_CLOSE:
    {
        printf("Cannot connect to MQTT: %s\n", address.c_str());
        connected = false;
        exit(1);
        break;
    }

    case MG_EV_MQTT_CONNACK:
        if (msg->connack_ret_code != MG_EV_MQTT_CONNACK_ACCEPTED)
        {
            printf("Got mqtt connection error: %d\n", msg->connack_ret_code);
            connected = false;
            exit(1);
        }

        printf("Connected to mqtt: %s\n", address.c_str());
        connected = true;

        break;
    }
}

/**
 * Connect to the serial port.
 * Exit in case of error 
 */
void GMQTT::init()
{

    int i;

    mg_mgr_init(&mgr, NULL);

    myGMqtt = this;

    if (mg_connect(&mgr, address.c_str(), ev_handler_static) == NULL)
    {
        fprintf(stderr, "mg_connect(%s) failed\n", address.c_str());
        exit(EXIT_FAILURE);
    }
}

void GMQTT::send(string datatosend)
{
    this->datatosend = datatosend;
    for (int i = 0; i < 10; i++)
        mg_mgr_poll(&mgr, 10);
}

void GMQTT::pool()
{
    for (int i = 0; i < 10; i++)
        mg_mgr_poll(&mgr, 10);
}