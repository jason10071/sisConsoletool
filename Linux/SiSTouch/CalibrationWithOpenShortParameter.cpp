#include "CalibrationWithOpenShortParameter.h"

/*===========================================================================*/
CalibrationWithOpenShortParameter::CalibrationWithOpenShortParameter() :
    noquery(false),
    time(DEFAULT_CALIBRATION_TIME),
    dumpError(false)
{

    strcpy( ouputfilename, "" );
    strcpy( ouputfilename2, "" );
    strcpy( configfilename, "" );
}
/*===========================================================================*/
CalibrationWithOpenShortParameter::~CalibrationWithOpenShortParameter()
{

}
/*===========================================================================*/
int CalibrationWithOpenShortParameter::parse( int argc, char** argv )
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
bool CalibrationWithOpenShortParameter::parseArgument(char *arg)
{
    if(conParameter.parseArgument(arg))
    {
        return true;          
    }

    if ( strcmp( arg, "-nq" ) == 0 )
    {
        noquery = true;
        printf( "noquery mode\n" );
    }
    else if ( strstr( arg, "-time=" ) == arg )
    {
        sscanf( arg + 6, "%d", &time );
        printf( "calibration wating time: %d\n", time );
    }
    else if ( strstr( arg, "-o=" ) == arg )
    {
        strcpy( ouputfilename, arg + 3 );
        printf( "result file set: %s\n", ouputfilename );
    }
    else if ( strstr( arg, "-o2=" ) == arg )
    {
        strcpy( ouputfilename2, arg + 4 );
        printf( "result file 2 set: %s\n", ouputfilename2 );
    }
    else if ( strstr( arg, "-c=" ) == arg )
    {
        strcpy( configfilename, arg + 3 );
        printf( "config file set: %s\n", configfilename );
    }
    else if ( strstr( arg, "--dump" ) == arg )
    {
        printf( "enable dump error result.\n");
        dumpError = true;
    } 
    else
    {
        printf( "unknown parameter\n" );
        return false;;
    }
    return true;
}
/*===========================================================================*/
int CalibrationWithOpenShortParameter::check()
{

    if(conParameter.check() == 0)
    {
        return 0;
    }

    return 1;
}   
/*===========================================================================*/
void CalibrationWithOpenShortParameter::print_usage()
{
    printf( "\n" );
    printf( "Usage: calibrationWithOpenShort [-c=CONFIG_FILENAME] [-n=DEVICE_NAME] [-o=RESULT_FILENAME] [-o2=RESULT_FILENAME 2] [-i=CONNECT] [--slave=SLAVEADDR] [-t=TIMEOUT] [-d=DELAY] [-io_delay=IO_DELAY] [-l] [-time=TIME] [-nq]  [--no-diag] [--no-power] [--dump]\n" );
    printf( "call chip to calibraion and do openShort to check clibration data.\n" );        
    printf( "    CONFIG_FILENAME:    Config file, ex. config.ini\n" );
    printf( "    RESULT_FILENAME:    Result file, ex. result.csv\n" );
    printf( "    CONNECT:     'auto'|'zeus_i2c'|'zeus_usbbridge'|'zeus_internalusb'|'aegis_i2c'|'aegis_usbbridge'|'aegis_internalusb'|'aegis_slave'|'aegis_multi', default 'auto'\n" );
    printf( "    DEVICE_NAME: the device name to be operate, ex. sis_aegis_hid_bridge_touch_device\n");
    printf( "    SLAVEADDR:   specify slave address used in aegis_slave, default is 5\n" );
    printf( "    TIMEOUT:     integer, retry times for NACK, default 100\n" );
    printf( "    DELAY:       integer, delay time between every each query retry in millisecond, default 1\n" );
    printf( "    IO_DELAY:    integer, delay time after each query in millisecond, default 0\n" );
    printf( "    -l:          verbose mode\n" );
    printf( "    TIME:        calibration waiting time, default 5 sec\n" );
    printf( "    -nq:         not query calibration done, assume calibraion is done at time up\n" );
    printf( "    --no-diag:   disable diagnosis mode changing automatically *** only for experiment, don't use in normal flow ***\n" );
    printf( "    --no-power:  disable power mode changing automatically *** only for experiment, don't use in normal flow ***\n" );
    printf( "    --dump:      dump error result\n" );
    printf( "    ps. don't use space around '='\n" );
    printf( "    \n" );
    printf( "    'aegis_slave' won't be detected in aegis_auto, only can be specified manually\n" );
}
/*===========================================================================*/

