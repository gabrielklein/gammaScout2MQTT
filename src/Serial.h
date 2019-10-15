#ifndef INCLUDE_SERIAL_H_
#define INCLUDE_SERIAL_H_

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <sys/select.h>

#include "Config.h"

class Serial
{
public:
    Serial();
    bool connect(Config *config);
    bool send(std::string s);
    bool receive(std::string *s, int timeoutS = 1);

private:
    bool set_interface_attribs(int speed);
    int fd;
    fd_set read_fds, write_fds, except_fds;
};

#endif /* INCLUDE_SERIAL_H_ */