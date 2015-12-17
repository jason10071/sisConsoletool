#include "OKFWParameter.h"

/*===========================================================================*/
OKFWParameter::OKFWParameter()
{

}
/*===========================================================================*/
OKFWParameter::~OKFWParameter()
{


}
/*===========================================================================*/
int OKFWParameter::parse( int argc, char** argv )
{
    printf( "parsing command parameter\n" );

    for( int i = 1; i < argc; i++ )
    {
        printf( "%d: [%s] ", i, argv[i] );

        if(!parseArgument(argv[i]))
        {
            printf( "unknown parameter: %s\n", argv[i] );
            return 0;
        }
    }

    return check();
}
/*===========================================================================*/
bool OKFWParameter::parseArgument(char *arg)
{

    if(conParameter.parseArgument(arg))
    {
        return true;
    }

    return false;;
}
/*===========================================================================*/
int OKFWParameter::check()
{

    if(conParameter.check() == 0)
    {
        return 0;
    }

    return 1;
}
/*===========================================================================*/
void OKFWParameter::print_usage()
{
    printf( "\n" );
    printf( "Usage: isOKFW [-i=CONNECT] [-n=DEVICE_NAME] [--slave=SLAVEADDR] [-t=TIMEOUT] [-d=DELAY] [-io-delay=IO_DELAY] [-l] [--no-diag] [--no-power]\n" );
    printf( "Check FW boot flag.\n" );
    printf( "    CONNECT:     'auto'|'zeus_i2c'|'zeus_usbbridge'|'zeus_internalusb'|'aeg_i2c'|'aeg_usbbridge'|'aeg_internalusb'|'aeg_slave'|'aeg_multi', default 'auto'\n" );
    printf( "    DEVICE_NAME: the device name to be operate, ex. s_aeg_hid_bridge_touch_device");
    printf( "    SLAVEADDR:   specify slave address used in aeg_slave, default  5\n" );
    printf( "    TIMEOUT:     integer, retry times for NACK, default 10000\n" );
    printf( "    DELAY:       integer, delay time between every each query retry in millecond, default 1\n" );
    printf( "    IO_DELAY:    integer, delay time after each query in millecond, default 0\n" );
    printf( "    -l:          verbose mode\n" );
    printf( "    --no-diag:   dable diagnos mode changing automatically *** only for experiment, don't use in normal flow ***\n" );
    printf( "    --no-power:  dable power mode changing automatically *** only for experiment, don't use in normal flow ***\n" );
    printf( "    ps. don't use space around '='\n" );
    printf( "    \n" );
    printf( "    'aeg_slave' won't be detected in aeg_auto, only can be specified manually\n" );
}
/*===========================================================================*/
