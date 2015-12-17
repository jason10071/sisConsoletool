#include "OpenShortParameter.h"

/*===========================================================================*/
OpenShortParameter::OpenShortParameter() :
    dumpError(false),
    resetFlag(false),
    noquery(false),
    time(DEFAULT_CALIBRATION_TIME)
{
    strcpy( samplefilename, "" );
    strcpy( ouputfilename, "" );
    strcpy( configfilename, "" );
}
/*===========================================================================*/
OpenShortParameter::~OpenShortParameter()
{
}
/*===========================================================================*/
int OpenShortParameter::parse( int argc, char** argv )
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
bool OpenShortParameter::parseArgument(char *arg)
{
    if(conParameter.parseArgument(arg))
    {
        return true;          
    }

    if ( strstr( arg, "-s=" ) == arg )
    {
        strcpy( samplefilename, arg + 3 );
        printf( "sample file set: %s\n", samplefilename );
    }
    else if ( strstr( arg, "-o=" ) == arg )
    {
        strcpy( ouputfilename, arg + 3 );
        printf( "result file set: %s\n", ouputfilename );
    }
    else if ( strstr( arg, "-c=" ) == arg )
    {
        strcpy( configfilename, arg + 3 );
        printf( "config file set: %s\n", configfilename );
    }
    else if ( strstr( arg, "--reset" ) == arg )
    {            
        printf( "reset flag is on.\n");
        resetFlag = true;
    }
    else if ( strcmp( arg, "-nq" ) == 0 )
    {
        noquery = true;
        printf( "noquery mode\n" );
    }
    else if ( strstr( arg, "-time=" ) == arg )
    {
        sscanf( arg + 6, "%d", &time );
        printf( "reset wating time: %d\n", time );
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
int OpenShortParameter::check()
{

    if(conParameter.check() == 0)
    {
        return 0;
    }

    return 1;
}   
/*===========================================================================*/
void OpenShortParameter::print_usage()
{
    printf( "\n" );
    printf( "Usage: openShort [-s=SAMPLE_FILENAME] [-o=RESULT_FILENAME] [-c=CONFIG_FILENAME] [-i=CONNECT] [-n=DEVICE_NAME] [--slave=SLAVEADDR] [-t=TIMEOUT] [-d=DELAY] [-io_delay=IO_DELAY] [-time=TIME] [-l] [-nq] [--no-diag] [--no-power] [--dump] [--reset]\n" );
    printf( "Test Open Short.\n" );
    printf( "    SAMPLE_FILENAME:    GoldenSample file, ex. sample.csv\n" );
    printf( "    RESULT_FILENAME:    Result file, ex. result.csv\n" );
    printf( "    CONFIG_FILENAME:    Config file, ex. config.ini\n" );
    printf( "    CONNECT:     'auto'|'zeus_i2c'|'zeus_usbbridge'|'zeus_internalusb'|'aegis_i2c'|'aegis_usbbridge'|'aegis_internalusb'|'aegis_slave'|'aegis_multi', default 'auto'\n" );
    printf( "    DEVICE_NAME: the device name to be operate, ex. sis_aegis_hid_bridge_touch_device");
    printf( "    SLAVEADDR:   specify slave address used in aegis_slave, default is 5\n" );
    printf( "    TIMEOUT:     integer, retry times for NACK, default 100\n" );
    printf( "    DELAY:       integer, delay time between every each query retry in millisecond, default 1\n" );
    printf( "    IO_DELAY:    integer, delay time after each query in millisecond, default 0\n" );
    printf( "    TIME:        reset waiting time, default 5 sec\n" );
    printf( "    -l:          verbose mode\n" );
    printf( "    -nq:         not query reset done, assume reset is done at time up\n" );
    printf( "    --no-diag:   disable diagnosis mode changing automatically *** only for experiment, don't use in normal flow ***\n" );
    printf( "    --no-power:  disable power mode changing automatically *** only for experiment, don't use in normal flow ***\n" );
    printf( "    --dump:      dump error result\n" );
    printf( "    --reset:     do reset before openshort\n" );		
    printf( "    ps. don't use space around '='\n" );
    printf( "    \n" );
    printf( "    'aegis_slave' won't be detected in aegis_auto, only can be specified manually\n" );
}
/*===========================================================================*/
