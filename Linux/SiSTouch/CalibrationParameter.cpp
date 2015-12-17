#include "CalibrationParameter.h"

/*===========================================================================*/
CalibrationParameter::CalibrationParameter() :
    noquery(false),
    checkFlag(false),
    eraseFlag(false),
    time(DEFAULT_CALIBRATION_TIME)
{

}
/*===========================================================================*/
CalibrationParameter::~CalibrationParameter()
{
}
/*===========================================================================*/
int CalibrationParameter::parse( int argc, char** argv )
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

    if ( checkFlag == true && eraseFlag == true )
    {
        printf( "could not check & erase calibration flag at the same time\n" );
        return 0;
    }
    else
    {
        return check();
    }

}
/*===========================================================================*/
bool CalibrationParameter::parseArgument(char *arg)
{

    if(conParameter.parseArgument(arg))
    {
        return true;
    }

    if ( strstr( arg, "-q" ) == arg )
    {
        checkFlag = true;
        printf( "check calibration flag, drop other options\n" );
    }
    else if ( strstr( arg, "-c" ) == arg )
    {
        eraseFlag = true;
        printf( "erase calibration flag, drop other options\n" );
    }
    else if ( strcmp( arg, "-nq" ) == 0 )
    {
        noquery = true;
        printf( "noquery mode\n" );
    }
    else if ( strstr( arg, "-time=" ) == arg )
    {
        sscanf( arg + 6, "%d", &time );
        printf( "calibration wating time: %d\n", time );
    }
    else
    {
        printf( "unknown parameter\n" );
        return false;;
    }
    return true;
}
/*===========================================================================*/
int CalibrationParameter::check()
{

    if(conParameter.check() == 0)
    {
        return 0;
    }

    return 1;
}
/*===========================================================================*/
void CalibrationParameter::print_usage()
{
    printf( "\n" );
    printf( "Usage: calibration [-i=CONNECT] [-n=DEVICE_NAME] [--slave=SLAVEADDR] [-t=TIMEOUT] [-d=DELAY] [-io_delay=IO_DELAY] [-l] [-time=TIME] [-nq] [-c] [-e] [--no-diag] [--no-power]\n" );
    printf( "call chip to calibraion.\n" );
    printf( "    CONNECT:     'auto'|'zeus_i2c'|'zeus_usbbridge'|'zeus_internalusb'|'aegis_i2c'|'aegis_usbbridge'|'aegis_internalusb'|'aegis_slave'|'aegis_multi', default 'auto'\n" );
    printf( "    DEVICE_NAME: the device name to be operate, ex. sis_aegis_hid_bridge_touch_device\n");
    printf( "    SLAVEADDR:   specify slave address used in aegis_slave, default is 5\n" );
    printf( "    TIMEOUT:     integer, retry times for NACK, default 100\n" );
    printf( "    DELAY:       integer, delay time between every each query retry in millisecond, default 1\n" );
    printf( "    IO_DELAY:    integer, delay time after each query in millisecond, default 0\n" );
    printf( "    -l:          verbose mode\n" );
    printf( "    TIME:        calibration waiting time, default 5 sec\n" );
    printf( "    -nq:         not query calibration done, assume calibraion is done at time up\n" );
    printf( "    -c:          check calibration flag, ***drop other options***\n" );
    printf( "    -e:          erase calibration flag, ***drop other options***\n" );
    printf( "    --no-diag:   disable diagnosis mode changing automatically *** only for experiment, don't use in normal flow ***\n" );
    printf( "    --no-power:  disable power mode changing automatically *** only for experiment, don't use in normal flow ***\n" );
    printf( "    ps. don't use space around '='\n" );
    printf( "    \n" );
    printf( "    'aegis_slave' won't be detected in aegis_auto, only can be specified manually\n" );
}
/*===========================================================================*/
