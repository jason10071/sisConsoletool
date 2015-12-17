#include "ZeusSiSTouchAdapter.h"

int
ZeusSiSTouchAdapter::updateFlow( int* buf, int buf_length, int file_length, bool update_bootloader, bool isNewBootloader, bool reserve_RODATA, bool update_parameter, bool restart )
{
    // int ret = 0;

    if( update_bootloader )
    {
        printf("Update bootloader data is not effect in zeus updating flow\n");
    }

    if( reserve_RODATA )
    {
        printf("Reserver RO data is not effect in zeus updating flow\n");
    }

    if( update_parameter )
    {
        printf("Update parameter is not effect in zeus updating flow\n");
    }

    if( !restart )
    {
        printf("no restart is not effect in zeus updating flow\n");
    }

    if (m_io->call_82() < 0 )
    {
        printf( "Reset(0x82) fail\n" );
        return 0;
    }

    if ( !write_buffer_to_address( buf, PARAM_BASE, buf_length, _4K, PARAM_BASE ))
    {
        printf( "write PARAM_BASE fail\n" );
        return 0;
    }

    if ( !write_buffer_to_address( buf, PHASE1_BASE, buf_length, _4K, PHASE1_BASE ) )
    {
        printf( "write PHASE1_BASE fail\n" );
        return 0;
    }

    if ( !write_buffer_to_address( buf, ZEUS_BASE, buf_length, _8K, ZEUS_BASE ) )
    {
        printf( "write ZEUS_BASE fail\n" );
        return 0;
    }

    if ( !write_buffer_to_address( buf, ZEUS_BASE + _8K, buf_length, file_length - ZEUS_BASE - _8K, ZEUS_BASE + _8K ) )
    {
        printf( "write ZEUS_BASE 2 fail\n" );
        return 0;
    }

    if ( !write_buffer_to_address( buf, LIB_BASE, buf_length, _4K, LIB_BASE ) )
    {
        printf( "write LIB_BASE fail\n" );
        return 0;
    }

    if ( !write_buffer_to_address( buf, BOOT_BASE, buf_length, _12K, BOOT_BASE ) )
    {
        printf( "write BOOT_BASE fail\n" );;
        return 0;
    }

    if (m_io->call_82() < 0 )
    {
        printf( "Reset(0x82) fail\n" );
        return 0;
    }
   

    return 1;
}

int
ZeusSiSTouchAdapter::updateCalibrationFlow(OpenShortData *data)
{
	printf("Reserver RO data is not effect in zeus\n");
	return 0;
}

int
ZeusSiSTouchAdapter::eraseCalibrationFlag(bool checkBootLoader)
{
	printf("Erase calibration flag is not effect in zeus\n");
	return 0;
}


void
ZeusSiSTouchAdapter::prepaerBuffer(int *buf, int buf_size)
{

    int index = 0;
    index = ZEUS_BASE / sizeof(int);
    if(index < buf_size)
    {
        buf[index] = invert_endian( 0x50383132 );
    }

    index = ( ZEUS_BASE / sizeof(int) ) + sizeof(int);
    if(index < buf_size)
    {
        buf[index] = invert_endian( 0xA0008080 );
    }
}


int
ZeusSiSTouchAdapter::isSiSFWFile(int* buf, int buf_length)
{
    if( buf[0x150 / 4] != 0x7375657a || buf[0x15c / 4] != 0x53695300 )
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

int
ZeusSiSTouchAdapter::parameterCheck(bool& update_bootloader, bool& update_bootloader_auto, bool &isNewBootloader, bool& reserve_RODATA, bool &update_parameter, bool force_update, int *buf, int file_Length)
{


    int firmwareInfo[FIRMWARE_INFO_LENGTH] = {0};

    int bootInfo[BOOT_INFO_LENGTH] = {0};

    int index = 0;

    //read id info
    printf("reading firmware info...\n");
    bool result = read_from_address(getFirmwareInfoAddr(), firmwareInfo, FIRMWARE_INFO_LENGTH);
    if(result)
    {
        printf("reading firmware info : success\n");
        index = (getFirmwareIdBase() - getFirmwareInfoBase()) / sizeof(int);
        printf("current firmware id: %08x, %08x ,%08x\n", invert_endian(firmwareInfo[index]), invert_endian(firmwareInfo[index + 1]), invert_endian(firmwareInfo[index + 2]));
        index = (getBootloaderIdBase() - getFirmwareInfoBase()) / sizeof(int);
        printf("current bootloader id: %08x\n", invert_endian(firmwareInfo[index]));
    }
    else
    {
        printf("reading firmware info : fail\n");
    }

    index = getFirmwareIdBase() / sizeof(int);
    printf("new firmware id: %08x, %08x ,%08x\n", invert_endian(buf[index]), invert_endian(buf[index + 1]), invert_endian(buf[index + 2]));
    index = getBootloaderIdBase() / sizeof(int);
    printf("new bootloader id: %08x\n", invert_endian(buf[index]));


    //read calibration flag
    printf("reading calibration and boot info\n");
    result = read_from_address(getBootFlagAddr(), bootInfo, BOOT_INFO_LENGTH);
    if(result)
    {
        printf("reading calibration and boot info : success\n");
        index = (getBootFlagBase() - getBootFlagBase()) / sizeof(int);
        printf("current boot flag: %08x\n", invert_endian(bootInfo[index]));
        index = (getCalibrationFlagBase() - getBootFlagBase()) / sizeof(int);
        printf("current calibration flag: %08x\n", invert_endian(bootInfo[index]));
    }
    else
    {
        printf("reading calibration and boot info : fail\n");
    }


    index = getBootFlagBase() / sizeof(int);
    printf("new boot flag: %08x\n", invert_endian(buf[index]));
    index = getCalibrationFlagBase() / sizeof(int);
    printf("new calibration flag: %08x\n", invert_endian(buf[index]));



    //nothing to do
    return SUCCESS;
}

