#include "Serial.h"

Serial::Serial()
{
}

bool Serial::set_interface_attribs(int speed)
{
    struct termios tty;

    if (tcgetattr(fd, &tty) < 0)
    {
        printf("Error from tcgetattr: %s\n", strerror(errno));
        return false;
    }

    cfsetospeed(&tty, (speed_t)speed);
    cfsetispeed(&tty, (speed_t)speed);

    tty.c_cflag |= (CLOCAL | CREAD); /* ignore modem controls */
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS7;      /* 7-bit characters */
    tty.c_cflag &= ~PARENB;  /* no parity bit */
    tty.c_cflag &= ~CSTOPB;  /* only need 1 stop bit */
    tty.c_cflag &= ~CRTSCTS; /* no hardware flowcontrol */

    /* setup for non-canonical mode */
    tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
    tty.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
    tty.c_oflag &= ~OPOST;

    /* fetch bytes as they become available */
    tty.c_cc[VMIN] = 1;
    tty.c_cc[VTIME] = 1;

    if (tcsetattr(fd, TCSANOW, &tty) != 0)
    {
        printf("Error from tcsetattr: %s\n", strerror(errno));
        return false;
    }
    return true;
}

bool Serial::send(string s)
{
    //tcdrain(fd); /* delay for output */
    int wlen = write(fd, s.c_str(), s.length());

    //printf("SEND from write xxx: %d\n", (int)s.length());

    if (wlen < 1)
    {
        printf("Error from write: %d, %d\n", wlen, errno);
        return false;
    }
    tcdrain(fd); /* delay for output */

    return true;
}
bool Serial::receive(string *s, int timeoutS)
{
    unsigned char buf[80];
    int rdlen;
    
    if (timeoutS > 0)
    {
        // Set timeout to 1.0 seconds
        struct timeval timeout;
        timeout.tv_sec = timeoutS;
        timeout.tv_usec = 0;

        FD_ZERO(&read_fds);
        FD_ZERO(&write_fds);
        FD_ZERO(&except_fds);
        FD_SET(fd, &read_fds);

        // Return after 1 seconds if nothing available
        int r = select(fd + 1, &read_fds, &write_fds, &except_fds, &timeout);
        //printf("Timeout %d\n", r);
        if (r != 1)
        {            
            (*s) = "";
            return false;
        }
    }

    rdlen = read(fd, buf, sizeof(buf) - 1);
    if (rdlen < 0)
    {
        (*s) = "";
        return false;
    }

    // Remove \r or \n at the end
    while (rdlen > 0 && (buf[rdlen - 1] == 0x0a || buf[rdlen - 1] == 0x0d))
        rdlen--;

    buf[rdlen] = 0;

    // Display data as string
    //printf("Read %d: \"%s\"\n", rdlen, buf);

    // Remove \r or \n at start
    int start = 0;
    while (start < rdlen && (buf[start] == 0x0a || buf[start] == 0x0d))
        start++;

    //printf("--------------- %d\n", start);

    (*s) = (char *)(buf + start);

    /* Display data as hex
    unsigned char *p;
    printf("Read %d:", rdlen);
    for (p = buf; rdlen-- > 0; p++)
        printf(" 0x%x", *p);
    printf("\n");
    */

    return true;
}

/**
 * 
 */
bool Serial::connect(Config *config)
{

    int wlen;

    fd = open(config->driver.c_str(), O_RDWR | O_NOCTTY | O_SYNC);
    if (fd < 0)
    {
        printf("Error opening %s: %s\n", config->driver.c_str(), strerror(errno));
        return false;
    }

    set_interface_attribs(config->serial_speed);

    //set_mincount(1);

    return true;
}
