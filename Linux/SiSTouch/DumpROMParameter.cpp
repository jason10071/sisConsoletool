#include "DumpROMParameter.h"

/*===========================================================================*/
DumpROMParameter::DumpROMParameter()
{
    strcpy( filename, "dump_fw_M.bin" );
    strcpy( slave_filenames[0], "dump_fw_S0.bin" );
    strcpy( slave_filenames[1], "dump_fw_S1.bin" );
    strcpy( slave_filenames[2], "dump_fw_S2.bin" );
    strcpy( slave_filenames[3], "dump_fw_S3.bin" );
    strcpy( slave_filenames[4], "dump_fw_S4.bin" );
    strcpy( slave_filenames[5], "dump_fw_S5.bin" );
}
/*===========================================================================*/
DumpROMParameter::~DumpROMParameter()
{
}
/*===========================================================================*/
int DumpROMParameter::parse( int argc, char** argv )
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
bool DumpROMParameter::parseArgument(char *arg)
{
    if(conParameter.parseArgument(arg)) { return true; }

    if ( strstr( arg, "-f=" ) == arg ) {
        strcpy( filename, arg + 3 );
        printf( "file set: %s\n", filename );
    } else if ( strstr( arg, "-s0=" ) == arg ) {
        strcpy( slave_filenames[0], arg + 4 );
        printf( "slave file 0 set: %s\n", slave_filenames[0] );
    } else if ( strstr( arg, "-s1=" ) == arg ) {
        strcpy( slave_filenames[1], arg + 4 );
        printf( "slave file 1 set: %s\n", slave_filenames[1] );
    } else if ( strstr( arg, "-s2=" ) == arg ) {
        strcpy( slave_filenames[2], arg + 4 );
        printf( "slave file 2 set: %s\n", slave_filenames[2] );
    } else if ( strstr( arg, "-s3=" ) == arg ) {
        strcpy( slave_filenames[3], arg + 4 );
        printf( "slave file 3 set: %s\n", slave_filenames[3] );
    } else if ( strstr( arg, "-s4=" ) == arg ) {
        strcpy( slave_filenames[4], arg + 4 );
        printf( "slave file 4 set: %s\n", slave_filenames[4] );
    } else if ( strstr( arg, "-s5=" ) == arg ) {
        strcpy( slave_filenames[5], arg + 4 );
        printf( "slave file 5 set: %s\n", slave_filenames[5] );
    } else {
        return false;
    }

    return true;
}
/*===========================================================================*/
int DumpROMParameter::check()
{
    if(conParameter.check() == 0)
    {
        return 0;
    }

    return 1;
}
/*===========================================================================*/
void DumpROMParameter::print_usage()
{
    printf("\nUsage: dumpROM [-f=FILENAME] [-s0=SLAVE0_FILENAME] [-s1=SLAVE1_FILENAME] [-s2=SLAVE2_FILENAME] [-s3=SLAVE3_FILENAME] [-s4=SLAVE4_FILENAME] [-s5=SLAVE5_FILENAME] [-l]\n"
           "\nDump FW in ROM\n"
           "           FILENAME:  master chip firmware file, default=dump_fw_M.bin\n"
           "    SLAVE0_FILENAME: slave 0 chip firmware file, default=dump_fw_S0.bin\n"
           "    SLAVE1_FILENAME: slave 1 chip firmware file, default=dump_fw_S1.bin\n"
           "    SLAVE2_FILENAME: slave 2 chip firmware file, default=dump_fw_S2.bin\n"
           "    SLAVE2_FILENAME: slave 3 chip firmware file, default=dump_fw_S3.bin\n"
           "    SLAVE2_FILENAME: slave 4 chip firmware file, default=dump_fw_S4.bin\n"
           "    SLAVE2_FILENAME: slave 5 chip firmware file, default=dump_fw_S5.bin\n"
           "                 -l: verbose mode\n" );
}
/*===========================================================================*/
void DumpROMParameter::showFileNames()
{
    printf("Master = %s\n", this->filename);
    for (int i = 0; i < MAX_SLAVE_NUM; i++) {
        printf("Slave%d = %s\n", i, this->slave_filenames[i]);
    }
}
/*===========================================================================*/
