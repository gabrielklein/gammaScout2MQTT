#ifndef INCLUDE_GAMMA_SCOUT_ONLINE_H_
#define INCLUDE_GAMMA_SCOUT_ONLINE_H_

#include "Config.h"
#include "Serial.h"
class GammaScoutOnline
{
public:
    GammaScoutOnline(Config *config);
    void init();
    void getUSV(string *output);

private:
    Config *config;
    Serial serial;
    bool goInOnlineMode();
    

};


#endif /* INCLUDE_GAMMA_SCOUT_ONLINE_H_ */