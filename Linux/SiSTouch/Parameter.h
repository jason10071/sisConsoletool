#ifndef __ARGUMENT_H
#define __ARGUMENT_H

#include <cstdio>
#include <cstring>

#define AUTO "auto"
#define ZEUS_I2C "zeus_i2c"
#define ZEUS_USB "zeus_usb"
#define ZEUS_INTERNALUSB "zeus_internalusb"
#define AEGIS_I2C "aegis_i2c"
#define AEGIS_USB "aegis_usb"
#define AEGIS_INTERNALUSB "aegis_internalusb"
#define AEGIS_SLAVE "aegis_slave"
#define AEGIS_MULTI "aegis_multi"
#define CUSTOM "custom"

#define MAX_SLAVE_NUM 7

enum
{
    MAX_TRANS_LENGTH = 17,
    MAX_FILENAME_LENGTH = 512,
    MAX_DEVNAME_LENGTH = 128,
    MAX_CONNECT_CHAR = 32,

    DEFAULT_DELAY = 1000,
    DEFAULT_RETRY = 100,
    DEFAULT_IO_DELAY = 0,

    DEFAULT_SLAVE_ADDR = 5,

    DEFAULT_MULTI_NUM = 1,

    DEFAULT_VERBOSE = 0,
    DEFAULT_CHANGE_DIAGMODE = 1,
    DEFAULT_CHANGE_PWRMODE = 1,

	DEFAULT_DETECT_HIDRAW_FLAG = 0,
};

struct Parameter
{
    int con;
    char connect[MAX_CONNECT_CHAR];
    int max_retry;
    int delay;	// us.
    int verbose;
    int io_Delay; //ms
    int slave_Addr;
    int multi_Num;
    int changeDiagMode;
    int changePWRMode;
    char devName[MAX_DEVNAME_LENGTH];

	int m_detectHidrawFlag;


    Parameter();
    ~Parameter();
    int parse( int argc, char** argv );
    bool parseArgument(char *arg);
    int check();
    int setActiveDeviceName();
    void setConnectType(char *connectName);
    void print_sep();
};

#endif
