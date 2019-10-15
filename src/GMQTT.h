#ifndef INCLUDE_GMQTT_H_
#define INCLUDE_GMQTT_H_

#include "Config.h"
#include "mongoose.h"
class GMQTT
{
public:
    GMQTT(Config *config);
    void init();
    void ev_handler(struct mg_connection *nc, int ev, void *p);
    void send(string datatosend);
    void pool();

private:
    Config *config;
    string address;
    string datatosend;
    bool connected;
    struct mg_mgr mgr;
};

#endif /* INCLUDE_GMQTT_H_ */