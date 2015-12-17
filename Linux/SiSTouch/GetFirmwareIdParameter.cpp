#include "GetFirmwareIdParameter.h"


/*===========================================================================*/
GetFirmwareIdParameter::GetFirmwareIdParameter() : fromFile(false) ,fromInput(false)
{
    strcpy( input, "" );
    strcpy( slave_input[0], "" );
    strcpy( slave_input[1], "" );
    strcpy( slave_input[2], "" );
    strcpy( slave_input[3], "" );
    strcpy( slave_input[4], "" );
    strcpy( slave_input[5], "" );
    strcpy( filename, "" );
    strcpy( slave_filenames[0], "" );
    strcpy( slave_filenames[1], "" );
    strcpy( slave_filenames[2], "" );
    strcpy( slave_filenames[3], "" );
    strcpy( slave_filenames[4], "" );
    strcpy( slave_filenames[5], "" );
}
/*===========================================================================*/
GetFirmwareIdParameter::~GetFirmwareIdParameter()
{
}
/*===========================================================================*/
int GetFirmwareIdParameter::parse( int argc, char** argv )
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
bool GetFirmwareIdParameter::parseArgument(char *arg)
{
    if(conParameter.parseArgument(arg))
    {
        return true;
    }

    if ( strstr( arg, "-f=" ) == arg )
    {
        strcpy( filename, arg + 3 );
        printf( "file set: %s\n", filename );
        fromFile = true;
    }
    else if ( strstr( arg, "-s=" ) == arg )
    {
        strcpy( slave_filenames[0], arg + 3 );
        printf( "slave file set: %s\n", slave_filenames[0] );
        fromFile = true;
        if(conParameter.multi_Num < 2)
        {
            conParameter.multi_Num = 2;
        }
    }
    else if ( strstr( arg, "-s0=" ) == arg )
    {
        strcpy( slave_filenames[0], arg + 4 );
        printf( "slave file 0 set: %s\n", slave_filenames[0] );
        fromFile = true;
        if(conParameter.multi_Num < 2)
        {
            conParameter.multi_Num = 2;
        }
    }
    else if ( strstr( arg, "-s1=" ) == arg )
    {
        strcpy( slave_filenames[1], arg + 4 );
        printf( "slave file 1 set: %s\n", slave_filenames[1] );
        fromFile = true;
        if(conParameter.multi_Num < 3)
        {
            conParameter.multi_Num = 3;
        }
    }
    else if ( strstr( arg, "-s2=" ) == arg )
    {
        strcpy( slave_filenames[2], arg + 4 );
        printf( "slave file 2 set: %s\n", slave_filenames[2] );
        fromFile = true;
        if(conParameter.multi_Num < 4)
        {
            conParameter.multi_Num = 4;
        }
    }
    else if ( strstr( arg, "-s3=" ) == arg )
    {
        strcpy( slave_filenames[3], arg + 4 );
        printf( "slave file 3 set: %s\n", slave_filenames[3] );
        fromFile = true;
        if(conParameter.multi_Num < 5)
        {
            conParameter.multi_Num = 5;
        }
    }
    else if ( strstr( arg, "-s4=" ) == arg )
    {
        strcpy( slave_filenames[4], arg + 4 );
        printf( "slave file 4 set: %s\n", slave_filenames[4] );
        fromFile = true;
        if(conParameter.multi_Num < 6)
        {
            conParameter.multi_Num = 6;
        }
    }
    else if ( strstr( arg, "-s5=" ) == arg )
    {
        strcpy( slave_filenames[5], arg + 4 );
        printf( "slave file 5 set: %s\n", slave_filenames[5] );
        fromFile = true;
        if(conParameter.multi_Num < 7)
        {
            conParameter.multi_Num = 7;
        }
    }
    else if ( strstr( arg, "--input=" ) == arg )
    {
        strcpy( input, arg + 8 );
        printf( "input set: %s\n", input );
        fromInput = true;
    }
    else if ( strstr( arg, "--slave-input=" ) == arg )
    {
        strcpy( slave_input[0], arg + 14 );
        printf( "slave input set: %s\n", slave_input[0] );
        fromInput = true;
        if(conParameter.multi_Num < 2)
        {
            conParameter.multi_Num = 2;
        }
    }
    else if ( strstr( arg, "--slave-input0=" ) == arg )
    {
        strcpy( slave_input[0], arg + 15 );
        printf( "slave 0 input set: %s\n", slave_input[0] );
        fromInput = true;
        if(conParameter.multi_Num < 2)
        {
            conParameter.multi_Num = 2;
        }
    }
    else if ( strstr( arg, "--slave-input1=" ) == arg )
    {
        strcpy( slave_input[1], arg + 15 );
        printf( "slave 1 input set: %s\n", slave_input[1] );
        fromInput = true;
        if(conParameter.multi_Num < 3)
        {
            conParameter.multi_Num = 3;
        }
    }
    else if ( strstr( arg, "--slave-input2=" ) == arg )
    {
        strcpy( slave_input[2], arg + 15 );
        printf( "slave 2 input set: %s\n", slave_input[2] );
        fromInput = true;
        if(conParameter.multi_Num < 4)
        {
            conParameter.multi_Num = 4;
        }
    }
    else if ( strstr( arg, "--slave-input3=" ) == arg )
    {
        strcpy( slave_input[3], arg + 15 );
        printf( "slave 3 input set: %s\n", slave_input[3] );
        fromInput = true;
        if(conParameter.multi_Num < 5)
        {
            conParameter.multi_Num = 5;
        }
    }
    else if ( strstr( arg, "--slave-input4=" ) == arg )
    {
        strcpy( slave_input[4], arg + 15 );
        printf( "slave 4 input set: %s\n", slave_input[4] );
        fromInput = true;
        if(conParameter.multi_Num < 6)
        {
            conParameter.multi_Num = 6;
        }
    }
    else if ( strstr( arg, "--slave-input5=" ) == arg )
    {
        strcpy( slave_input[5], arg + 15 );
        printf( "slave 5 input set: %s\n", slave_input[5] );
        fromInput = true;
        if(conParameter.multi_Num < 7)
        {
            conParameter.multi_Num = 7;
        }
    }
    else
    {
        printf( "unknown parameter\n" );
        return false;;
    }

    return true;
}
/*===========================================================================*/
int GetFirmwareIdParameter::check()
{
    if(conParameter.check() == 0)
    {
        return 0;
    }

    if(fromInput && fromFile)
    {
        printf("conflict!! Cannot check fw from input and from file at the same time\n");
        return 0;
    }

    if(fromFile)
    {
        for(int i = 1; i < conParameter.multi_Num; i++)
        {
            if( strcmp(slave_filenames[i-1], "") == 0)
            {
                printf("error: slave %d fw file not set\n", (i-1));
                return 0;
            }
        }
    }
    else if(fromInput)
    {
        for(int i = 1; i < conParameter.multi_Num; i++)
        {
            if( strcmp(slave_input[i-1], "") == 0)
            {
                printf("error: slave %d input not set\n", (i-1));
                return 0;
            }
        }
    }

    return 1;
}
/*===========================================================================*/
void GetFirmwareIdParameter::print_usage()
{
    printf( "\n" );
    printf( "Usage: getFirmwareId [-f=FILENAME] [-s|s0=SLAVE0_FILENAME] [-s1=SLAVE1_FILENAME] [-s2=SLAVE2_FILENAME] [-i=CONNECT] [-n=DEVICE_NAME] [--slave=SLAVEADDR] [-t=TIMEOUT] [-d=DELAY] [-io-delay=IO_DELAY] [--input=INPUTID] [--slave-input|--slave-input0=SLAVE0_INPUTID] [--slave-input1=SLAVE_INPUTID1] [--slave-input2=SLAVE_INPUTID2] [-l] [--no-diag] [--no-power]\n" );
    printf( "Show firmware ID.\n" );
    printf( "    FILENAME:    firmware file, ex. flora.bin\n" );
    printf( "    SLAVE0_FILENAME:    slave 0 chip firmware file, ex. flora_slv0.bin\n" );
    printf( "    SLAVE1_FILENAME:    slave 1 chip firmware file, ex. flora_slv1.bin\n" );
    printf( "    SLAVE2_FILENAME:    slave 2 chip firmware file, ex. flora_slv2.bin\n" );
    printf( "    CONNECT:     'auto'|'zeus_i2c'|'zeus_usbbridge'|'zeus_internalusb'|'aegis_i2c'|'aegis_usbbridge'|'aegis_internalusb'|'aegis_slave'|'aegis_multi', default 'auto'\n" );
    printf( "    DEVICE_NAME: the device name to be operate, ex. sis_aegis_hid_bridge_touch_device");
    printf( "    SLAVEADDR:   specify slave address used in aegis_slave, default is 5\n" );
    printf( "    TIMEOUT:     integer, retry times for NACK, default 100\n" );
    printf( "    DELAY:       integer, delay time between every each query retry in millisecond, default 1\n" );
    printf( "    IO_DELAY:    integer, delay time after each query in millisecond, default 0\n" );
    printf( "    INPUTID:     hex string of firmwaer id, ex 9903937356554e4104ffffff\n" );
    printf( "    SLAVE0_INPUTID:     hex string of slave 0 firmwaer id, ex 9903937356554e4104ffffff\n" );
    printf( "    SLAVE1_INPUTID:     hex string of slave 1 firmwaer id, ex 9903937356554e4104ffffff\n" );
    printf( "    SLAVE2_INPUTID:     hex string of slave 2 firmwaer id, ex 9903937356554e4104ffffff\n" );
    printf( "    -l:          verbose mode\n" );
    printf( "    --no-diag:   disable diagnosis mode changing automatically *** only for experiment, don't use in normal flow ***\n" );
    printf( "    --no-power:  disable power mode changing automatically *** only for experiment, don't use in normal flow ***\n" );
    printf( "    ps. don't use space around '='\n" );
    printf( "    \n" );
    printf( "    'aegis_slave' won't be detected in aegis_auto, only can be specified manually\n" );
}
/*===========================================================================*/
