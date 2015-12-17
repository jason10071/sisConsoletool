#include "Parameter.h"
#include "SiSTouchIO.h"

/*===========================================================================*/
Parameter::Parameter() :
    con(SiSTouchIO::CON_AUTO),
    max_retry(DEFAULT_RETRY),
    delay(DEFAULT_DELAY),
    verbose(DEFAULT_VERBOSE),
    io_Delay(DEFAULT_IO_DELAY),
    slave_Addr(DEFAULT_SLAVE_ADDR),
    multi_Num(DEFAULT_MULTI_NUM),
    changeDiagMode (DEFAULT_CHANGE_DIAGMODE),
    changePWRMode  (DEFAULT_CHANGE_PWRMODE),
    
    m_detectHidrawFlag(DEFAULT_DETECT_HIDRAW_FLAG)
{

    strcpy( connect, "auto" );
    strcpy( devName, "" );
}
/*===========================================================================*/
Parameter::~Parameter()
{

}
/*===========================================================================*/
int Parameter::parse( int argc, char** argv )
{
    if ( argc <= 1 )
    {
        return 0;
    }

    printf( "parsing command parameter\n" );

    for( int i = 1; i < argc; i++ )
    {
        printf( "%d: [%s] ", i, argv[i] );

        if(!parseArgument(argv[i]))
        {
            printf( "unknown parameter\n" );
            return 0;
        }
    }

    return check();
}
/*===========================================================================*/
bool Parameter::parseArgument(char *arg)
{
    if ( strstr( arg, "-i=" ) == arg )
    {
        setConnectType(arg+3);
    }
    else if ( strstr( arg, "-t=" ) == arg )
    {
        sscanf( arg + 3, "%d", &max_retry );
        printf( "retry set: %d\n", max_retry );
    }
    else if ( strstr( arg, "-d=" ) == arg )
    {
        sscanf( arg + 3, "%d", &delay );
        printf( "delay set: %dms.\n", delay );
        delay *= 1000;
    }
    else if ( strstr( arg, "-n=" ) == arg)
    {
        strcpy(devName, arg + 3);
        printf( "device name: %s\n", devName );
    }
    else if ( strcmp( arg, "-l" ) == 0 )
    {
        verbose = 1;
        printf( "verbose mode\n" );
    }
    else if ( strstr( arg, "-io_delay=" ) == arg )
    {
        sscanf( arg + 10, "%d", &io_Delay );
        printf( "io_delay set: %dms.\n", io_Delay );
    }
    else if ( strstr( arg, "--slave=" ) == arg )
    {
        sscanf( arg + 8, "%d", &slave_Addr );
        printf( "slave addr : 0x%02x.\n", slave_Addr );
    }
    else if ( strcmp( arg, "--no-diag" ) == 0 )
    {
        changeDiagMode = 0;
        printf( "disable diagnosis mode changing automatically *** only for experiment, don't use in normal flow ***\n" );;
    }
    else if ( strcmp( arg, "--no-power" ) == 0 )
    {
        changePWRMode = 0;
        printf( "disable power mode changing automatically *** only for experiment, don't use in normal flow  ***\n" );
    }
	else if ( strcmp( arg, "--hidraw" ) == 0 )
    {
        m_detectHidrawFlag = 1;
        printf( "Enable detect hidraw device\n" );
    }
    else
    {
        return false;

    }
    return true;
}
/*===========================================================================*/
int Parameter::check()
{

    if ( strcmp(connect, AUTO) != 0 && strcmp(connect, ZEUS_I2C) != 0 && strcmp(connect, ZEUS_USB) != 0 && strcmp(connect, ZEUS_INTERNALUSB) != 0 && strcmp(connect, AEGIS_I2C) != 0 && strcmp(connect, AEGIS_USB) != 0 && strcmp(connect, AEGIS_INTERNALUSB) != 0 && strcmp(connect, AEGIS_SLAVE) != 0 && strcmp(connect, AEGIS_MULTI) != 0 )
    {
        printf( "error: connect type '%s' not supported\n", connect );
        return 0;
    }

    return 1;
}
/*===========================================================================*/
int Parameter::setActiveDeviceName()
{
	if ( strlen(devName) > 0 ) {
        if ( SiSTouchIO::setActiveDeviceName(devName, strlen(devName))!=1 ) {
            printf("Can't find device : %s\n", devName);
            return 0;
        }
	}

    return 1;
}
/*===========================================================================*/
void Parameter::setConnectType(char *connectName)
{
    strcpy( connect, connectName );

    if(strcmp(connectName, AUTO) == 0)
    {
        con = SiSTouchIO::CON_AUTO;
        printf( "target select: %s\n", AUTO );
    }
    else if(strcmp(connectName, ZEUS_I2C) == 0)
    {
        con = SiSTouchIO::CON_ZEUS_I2C;
        printf( "target select: %s\n", ZEUS_I2C );
    }
    else if(strcmp(connectName, ZEUS_USB) == 0)
    {
        con = SiSTouchIO::CON_ZEUS_USB;
        printf( "target select: %s\n", ZEUS_USB );
    }
    else if(strcmp(connectName, ZEUS_INTERNALUSB) == 0)
    {
        con = SiSTouchIO::CON_ZEUS_INTERUSB;
        printf( "target select: %s\n", ZEUS_INTERNALUSB );
    }
    else if(strcmp(connectName, AEGIS_I2C) == 0)
    {
        con = SiSTouchIO::CON_AEGIS_I2C;
        printf( "target select: %s\n", AEGIS_I2C );
    }
    else if(strcmp(connectName, AEGIS_USB) == 0)
    {
        con = SiSTouchIO::CON_AEGIS_USB;
        printf( "target select: %s\n", AEGIS_USB );
    }
    else if(strcmp(connectName, AEGIS_INTERNALUSB) == 0)
    {
        con = SiSTouchIO::CON_AEGIS_INTERUSB;
        printf( "target select: %s\n", AEGIS_INTERNALUSB );
    }
    else if(strcmp(connectName, AEGIS_SLAVE) == 0)
    {
        con = SiSTouchIO::CON_AEGIS_SLAVE;
        printf( "target select: %s\n", AEGIS_SLAVE );
    }
    else if(strcmp(connectName, AEGIS_MULTI) == 0)
    {
        con = SiSTouchIO::CON_AEGIS_MULTI;
        printf( "target select: %s\n", AEGIS_MULTI );
    }
    else if(strcmp(connectName, CUSTOM) == 0)
    {
        con = SiSTouchIO::CON_CUSTOM;
        printf( "target select: %s\n", CUSTOM );
    }
    else
    {
        con = SiSTouchIO::CON_AUTO;
        strcpy( connect, AUTO );
        printf( "target select: %s\n", AUTO );
    }

    return;

}
/*===========================================================================*/
void Parameter::print_sep()
{
    printf( "-----\n" );
}
/*===========================================================================*/
