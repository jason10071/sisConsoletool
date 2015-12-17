#include "SiSTouchAdapter.h"
#include "ZeusSiSTouchAdapter.h"
#include "AegisSiSTouchAdapter.h"
#include <cstring>
#include <cstdlib>

const char SiSTouchAdapter::phase2StatusStr[STATUS_INDEX_MAX][64] =
{
	"Enable Palm Rejection : ",
	"Enable Water Proof : ",
	"Enable Moiset : ",
	"Enable Press Detect : ",
	"Initial Palm is OK ? ",
	"Enable Dynamic Calibrate : ",
	"Enable Frequency Hopping : "
};

bool SiSTouchAdapter::phase2Status[STATUS_INDEX_MAX];

int SiSTouchAdapter::invert_endian( int data )
{
    int out = 0;

    for (unsigned int i = 0; i < sizeof(int); i++ )
    {
        int shift = ( sizeof(int) - i - 1 ) * 8;
        out |= ( ( data >> i * 8 ) & 0xff ) << shift ;
    }

    return out;
}

bool SiSTouchAdapter::read_from_buffer( int* buf, int buf_length, unsigned int offset, int* id, int size )
{

    for( int i = 0; i < size; i++ )
    {
        int index = offset / sizeof(int) + i;
        if( index >= buf_length)
        {
            return false;
        }

        id[i] = buf[ offset / sizeof(int) + i];
    }

    return true;
}

bool SiSTouchAdapter::read_from_address( unsigned int addr, int* buffer, int size )
{

    int read_Length = get_READ_LENGTH();
    bool isVirtual = isVirtualAddr(addr);

    for (int i = 0; i < size; i += read_Length)
    {
        int length = ((size - i) >= read_Length)?read_Length:(size - i);
        int ret = 0;

        //sometimes device is in sleep
        for(int retry = 0; retry < SLEEP_RETRY; retry++)
        {

            ret = m_io->call_86( addr , buffer + i, length );
            if(ret >= 0)
            {
                break;
            }

            //100ms
            usleep(SLEEP_RETRY_TIME);
        }

        if ( ret < 0 )
        {
            printf( "Read(0x86) Fail\n" );
            return false;
        }

        if(!isVirtual)
        {
            addr += (length * sizeof(int));
        }
        else
        {
            addr += length;
        }
    }

    return true;
}

bool SiSTouchAdapter::isVirtualAddr(unsigned int addr)
{
    if( (addr & getAddressBlockMask()) == getVirtualAddressMask())
    {
        return true;
    }
    else
    {
        return false;
    }
}

int SiSTouchAdapter::read_file_to_buffer( FILE *file, long offset, int size, int* buf )
{
    fseek(file, offset, SEEK_SET);
    return fread(buf, sizeof(int), size,file);
}

bool SiSTouchAdapter::write_buffer_to_address( int* buf, int offset, int buf_size, int length, unsigned int addr )
{
    int read_Length = get_READ_LENGTH();
    int* temp = new int[read_Length];

    int ret = 0;

    int send_legnth = 0;
    int actual_pending_byte_length = 0;
    int actual_pending_int_length = 0;

    while ( send_legnth < length )
    {
        unsigned int inc = length > send_legnth + _12K ? _12K : length - send_legnth;
        send_legnth += inc;
        int pack_num = inc / (read_Length * sizeof(int)) + ((inc % (read_Length * sizeof(int))) == 0 ? 0 : 1 );

        int idx_base = offset / sizeof(int);

        for(int block_retry = 0; block_retry < 33; block_retry++)
        {
            ret = 0;
            printf("block_retry: %d of max 33\n", block_retry+1 );

            printf("Write to addr = %08x pack_num=%d \n", addr, pack_num);

            ret = m_io->call_83( addr, pack_num ) ;
            if ( ret < 0 )
            {
                printf( "Write Header(0x83) fail\n" );
                continue;
            }

            for ( int i = 0; i < pack_num; i++ )
            {
                for(int j = 0; j < read_Length; j++)
                {
                    int idx = i * read_Length + j;
                    temp[j] = ( (idx * sizeof(int)) < inc && (idx + idx_base) < buf_size ) ? buf[idx + idx_base] : 0;
                }

                actual_pending_byte_length = get_actual_pending_byte_length(inc - i *read_Length * sizeof(int)) ;
                actual_pending_int_length = actual_pending_byte_length / sizeof(int) + ((actual_pending_byte_length % sizeof(int)) == 0 ? 0 : 1);

                printf(".");
                fflush(stdout);

                ret = m_io->call_84( temp, actual_pending_int_length );

                if ( ret < 0 )
                {
                    break;
                }
            }

            printf( "\n" );

            if( ret < 0 )
            {
                printf( "Write(0x84) fail\n" );
                continue;
            }

            ret = m_io->call_04();
			printf( "waiting for Query(0x04)\n" );
            usleep(1 * 1000 * 1000);

            if ( ret < 0 )
            {
                printf( "Query(0x04) fail\n" );
                continue;
            }
            else
            {
                printf ("write all %d packets successfully\n", pack_num);
                break;
            }
        }

        if( ret < 0 )
        {
            printf( "retry time out\n");
            break;
        }

        addr += inc;
        offset += inc;
    }

    delete[] temp;

    return ( ret < 0 ) ? false : true;
}

long SiSTouchAdapter::getFileLength(FILE* file)
{
    long file_length = 0;

    if(file == 0)
    {
        return 0;
    }

    fseek(file, 0L, SEEK_END);
    file_length = ftell(file);
    fseek(file, 0L, SEEK_SET);

    return file_length;
}

int SiSTouchAdapter::doCompareId(FILE* file)
{
    int device_firmware_id[FIRMWARE_ID_LENGTH] = {0};
    int file_firmware_id[FIRMWARE_ID_LENGTH] = {0};

    if(!getFileId(file, file_firmware_id, FIRMWARE_ID_LENGTH))
    {
        return ERROR_FAIL_GET_FILE_ID;
    }

    int getDeviceIdResult = getDeviceId(device_firmware_id, FIRMWARE_ID_LENGTH);

    if(getDeviceIdResult != SUCCESS && getDeviceIdResult != ERROR_START_DRIVER )
    {
        return ERROR_FAIL_GET_DEVICE_ID;
    }

    bool result = compareId(file_firmware_id, device_firmware_id, FIRMWARE_ID_LENGTH);

    return result ? RESULT_SAME : RESULT_DIFFERENT;
}

int SiSTouchAdapter::doCompareId(char* input)
{
    int input_firmware_id[FIRMWARE_ID_LENGTH] = {0};
    int device_firmware_id[FIRMWARE_ID_LENGTH] = {0};

    if(!getInputId(input, input_firmware_id, FIRMWARE_ID_LENGTH))
    {
        return ERROR_FAIL_GET_INPUT_ID;
    }

    int getDeviceIdResult = getDeviceId(device_firmware_id, FIRMWARE_ID_LENGTH);

    if(getDeviceIdResult != SUCCESS && getDeviceIdResult != ERROR_START_DRIVER )
    {
        return ERROR_FAIL_GET_DEVICE_ID;
    }

    bool result = compareId(input_firmware_id, device_firmware_id, FIRMWARE_ID_LENGTH);

    return result ? RESULT_SAME : RESULT_DIFFERENT;
}

int SiSTouchAdapter::showDeviceId()
{
    int device_firmware_id[FIRMWARE_ID_LENGTH] = {0};
    if(getDeviceId(device_firmware_id, FIRMWARE_ID_LENGTH) != SUCCESS)
    {
        return ERROR_FAIL_GET_DEVICE_ID;
    }

    return SUCCESS;
}

bool SiSTouchAdapter::compareId(int* idA, int* idB, int length)
{
    for(int i = 0; i < length; i++)
    {
        if( idA[i] != idB[i] )
        {
            return false;
        }
    }
    return true;

}

int SiSTouchAdapter::doCheckCalibrationFlag()
{
    int input_calibration_flag[CALIBRATION_FLAG_LENGTH] = {0};
    int device_calibration_flag[CALIBRATION_FLAG_LENGTH] = {0};

    input_calibration_flag[0] = getCalibrationFlagValue();

    if(getCalibrationFlag(device_calibration_flag, CALIBRATION_FLAG_LENGTH) != SUCCESS)
    {
        return ERROR_FAIL_CALIBRATION_FLAG;
    }

    bool result = compareId(input_calibration_flag, device_calibration_flag, CALIBRATION_FLAG_LENGTH);

    return result ? RESULT_SAME : RESULT_DIFFERENT;
}

int SiSTouchAdapter::doEraseCalibrationFlag()
{
    int ret = SUCCESS;

    if ( m_io->stop_driver() < 0 )
    {
        printf( "stop driver fail\n" );
        return ERROR_STOP_DRIVER;
    }

	ret = eraseCalibrationFlag(true);

    if ( ret == SUCCESS )
    {
    	printf( "erase RO success, reset ...\n" );

		if (  m_io->call_82() < 0 )
        {
            printf( "Reset(0x82) fail\n" );
            return ERROR_IN_UPDATEFLOW;
        }
    }

    if ( m_io->start_driver() < 0 )
    {
        printf( "start driver fail\n" );

        if(ret == SUCCESS)
        {
            ret = ERROR_START_DRIVER;
        }
    }

    return ret;
}

int SiSTouchAdapter::doCheckFWState()
{
    int input_boot_flag[BOOT_FLAG_LENGTH] = {0};
    int device_boot_flag[BOOT_FLAG_LENGTH] = {0};

    input_boot_flag[0] = getBootFlagValue();

    if(getBootFlag(device_boot_flag, BOOT_FLAG_LENGTH) != SUCCESS)
    {
        return ERROR_FAIL_BOOT_FLAG;
    }

    bool result = compareId(input_boot_flag, device_boot_flag, BOOT_FLAG_LENGTH);

    return result ? RESULT_SAME : RESULT_DIFFERENT;
}

bool SiSTouchAdapter::getInputId(const char* input, int* input_firmware_id, int id_Length)
{
    if(!input)
    {
        printf("null input\n");
        return false;
    }

    if(!input_firmware_id)
    {
        printf("null input_firmware_id array\n");
        return false;
    }

    if( id_Length > FIRMWARE_ID_LENGTH )
    {
        id_Length = FIRMWARE_ID_LENGTH;
    }

    int inputLength = strlen(input);
    int bufLength = id_Length * sizeof(int);
    const int byte_per_char = 2;

    if(inputLength != bufLength * byte_per_char)
    {
        printf("not valid id length %d\n", inputLength);
        return false;
    }

    for( int i = 0; i < id_Length; i++ )
    {
        int ret = sscanf((input + i * 8),"%08x", &(input_firmware_id[i]));
        if(ret != 1)
        {
            printf("not valid hex string\n");
            return false;
        }
    }

    printf( "input firmware id: " );
    for( int i = 0; i < id_Length; i++ )
        printf( "%08x,", input_firmware_id[i] );
    printf( "\n" );

    return true;
}

bool SiSTouchAdapter::getFileId(FILE* file, int* file_firmware_id, int id_Length)
{

    if(!file)
    {
        printf("null file descriptor\n");
        return false;
    }

    if(!file_firmware_id)
    {
        printf("null file_firmware_id array\n");
        return false;
    }

    if( id_Length > FIRMWARE_ID_LENGTH )
    {
        id_Length = FIRMWARE_ID_LENGTH;
    }

    int buf[MAX_BUF_LENGTH] = {0};

    int file_length = getFileLength(file);

    /* int read_Length = */ read_file_to_buffer( file, 0, MAX_BUF_LENGTH, buf );

    if( ! checkFileBufIsValid( file_length, buf, MAX_BUF_LENGTH ))
    {
        return false;
    }

    if(read_from_buffer( buf, MAX_BUF_LENGTH, getFirmwareIdBase(), file_firmware_id, id_Length) )
    {
        printf( "file firmware id: " );
        for( int i = 0; i < id_Length; i++ )
        {
            file_firmware_id[i] = invert_endian( file_firmware_id[i]);
            printf( "%08x,", file_firmware_id[i] );
        }
        printf( "\n" );
    }
    else
    {
        printf("cannot read file firmware id\n");
        return false;
    }

    return true;
}

bool SiSTouchAdapter::getLayoutOffset(int &offset)
{
	int buf[1] = {0};

	bool ret = read_from_address(getLayoutOffsetAddress(), buf, 1);

	if(!ret)
	{
		printf("cannot get layout offset from device\n");
		return ret;
	}

	offset = ( buf[0] & 0x0000ff00 ) >> 8;

    return ret;
}

int SiSTouchAdapter::getDeviceId(int* device_firmware_id, int id_Length)
{
    int ret = SUCCESS;

    if(!device_firmware_id)
    {
        printf("null device_firmware_id array\n");
        return ERROR_NULL_INPUT_ARRAY;
    }

    if( id_Length > FIRMWARE_ID_LENGTH )
    {
        id_Length = FIRMWARE_ID_LENGTH;
    }

    if(m_io->stop_driver() < 0)
    {
        printf( "stop driver fail\n" );
        return ERROR_STOP_DRIVER;
    }

    bool result = read_from_address(getFirmwareIdAddr(),device_firmware_id, id_Length);
    if( result )
    {
        printf( "device firmware id: " );
        for( int i = 0; i < id_Length; i++ )
        {
            device_firmware_id[i] = invert_endian( device_firmware_id[i]);
            printf( "%08x,", device_firmware_id[i] );
        }
        printf( "\n" );
    }
    else
    {
        ret = ERROR_IN_READ_DATA;
        printf("cannot read firmware id\n");
    }

    if ( m_io->start_driver() < 0 )
    {
        printf( "start driver fail\n" );

        if(ret == SUCCESS)
        {
            ret = ERROR_START_DRIVER;
        }
    }

    return ret;
}

int SiSTouchAdapter::getCalibrationFlag(int *calibration_flag, int flag_Length)
{
    int ret = SUCCESS;

    if(!calibration_flag)
    {
        printf("null *calibration_flag array\n");
        return ERROR_NULL_INPUT_ARRAY;
    }

    if( flag_Length > CALIBRATION_FLAG_LENGTH )
    {
        flag_Length = CALIBRATION_FLAG_LENGTH;
    }

    if(m_io->stop_driver() < 0)
    {
        printf( "stop driver fail\n" );
        return ERROR_STOP_DRIVER;
    }

    bool result = read_from_address(getCalibrationFlagAddr(), calibration_flag, flag_Length);
    if( result )
    {
        printf( "device calibration flag: " );
        for( int i = 0; i < flag_Length; i++ )
        {
            calibration_flag[i] = invert_endian( calibration_flag[i]);
            printf( "%08x,", calibration_flag[i] );
        }
        printf( "\n" );
    }
    else
    {
        ret = ERROR_IN_READ_DATA;
        printf("cannot read calibration flag\n");
    }

    if ( m_io->start_driver() < 0 )
    {
        printf( "start driver fail\n" );

        if(ret == SUCCESS)
        {
            ret = ERROR_START_DRIVER;
        }
    }

    return ret;
}

int SiSTouchAdapter::getBootFlag(int *boot_flag, int flag_Length)
{

    int ret = SUCCESS;

    if(!boot_flag)
    {
        printf("null *boot_flag entry\n");
        return ERROR_NULL_INPUT_ARRAY;
    }

    if( flag_Length > BOOT_FLAG_LENGTH  )
    {
        flag_Length = BOOT_FLAG_LENGTH ;
    }

    if(m_io->stop_driver() < 0)
    {
        printf( "stop driver fail\n" );
        return ERROR_STOP_DRIVER;
    }

    bool result = read_from_address(getBootFlagAddr(), boot_flag, flag_Length);
    if( result )
    {
        printf( "fw boot flag: " );
        for( int i = 0; i < flag_Length; i++ )
        {
            boot_flag[i] = invert_endian( boot_flag[i]);
            printf( "%08x,", boot_flag[i] );
        }
        printf( "\n" );
    }
    else
    {
        ret = ERROR_IN_READ_DATA;
        printf("cannot read boot flag\n");
    }

    if ( m_io->start_driver() < 0 )
    {
        printf( "start driver fail\n" );

        if(ret == SUCCESS)
        {
            ret = ERROR_START_DRIVER;
        }
    }

    return ret;
}

int SiSTouchAdapter::getFwMode()
{
    int ret = SUCCESS;
    int fw_mode[STATUS_INDEX_MAX]={0};

    // Enable FW to report data to AP (Disable FW to report touch to OS)
    if(m_io->stop_driver() < 0)
    {
        printf( "stop driver fail\n" );
        return ERROR_STOP_DRIVER;
    }

    // printf("0x%8x\n",getFwModeAddr());
    bool result = read_from_address(getFwModeAddr(), fw_mode, STATUS_INDEX_MAX);

    if( result )
    {
    	for (int i=0; i<STATUS_INDEX_MAX; i++) {
    		//printf( "fw_mode[%d]=%d : ", i, fw_mode[i] );
			printf( "%s", phase2StatusStr[i] );
			if ( fw_mode[i]>=1 ) {
				phase2Status[i]=true;
				printf( "Yes\n");
			}
			else {
				phase2Status[i]=false;
				printf( "No\n");
			}
    	}
    }
    else
    {
        ret = ERROR_IN_READ_DATA;
        printf("cannot read FW mode\n");
    }

    if ( m_io->start_driver() < 0 )
    {
        printf( "start driver fail\n" );

        if(ret == SUCCESS)
        {
            ret = ERROR_START_DRIVER;
        }
    }

    return ret;
}

bool SiSTouchAdapter::checkFileBufIsValid( int file_length, int* buf, int buf_length )
{
    if(file_length > _64K)
    {
        printf( "file length exceed 64K\n");
        return false;
    }

    if(!isSiSFWFile(buf, buf_length))
    {
        printf("file is not sis fw file\n");
        return false;
    }

    return true;
}

bool SiSTouchAdapter::callReset()
{
    int ret = 0;
    //if the chip is in sleep mode, this command may fail
    for(int retry = 0; retry < SLEEP_RETRY; retry++)
    {
        ret = m_io->call_82();
        if(ret >= 0)
        {
            break;
        }

        //100ms
        usleep(SLEEP_RETRY_TIME);
    }

    if(ret < 0)
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool SiSTouchAdapter::callCalibration()
{
    int ret = 0;
    //if the chip is in sleep mode, this command may fail

    for(int retry = 0; retry < SLEEP_RETRY; retry++)
    {
        ret = m_io->call_87();
        if(ret >= 0)
        {
            break;
        }

        //100ms
        usleep(SLEEP_RETRY_TIME);
    }

    if(ret < 0)
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool SiSTouchAdapter::isCalibrationDone()
{
    int addr[1] = {0};
    int data[1] = {0};
    int mask[1] = {0};

    addr[0] = getCalibrationStateAddr();
    data[0] = 0;
    mask[0] = 0;

    if(m_io->call_88(addr, data, mask, 1) < 0)
    {
        return false;
    }

    data[0] = invert_endian(data[0]);

    if((data[0] & getCalibrationStateMask()) == getCalibrationStateDoneFlag())
    {
        return true;
    }

    return false;
}

int SiSTouchAdapter::doReset(int time, bool noquery)
{

    int ret = SUCCESS;

    if ( m_io->stop_driver() < 0 )
    {
        printf( "stop driver fail\n" );
        return ERROR_STOP_DRIVER;
    }

    bool result = callReset();
    bool calibrationDone = false;

    if(result)
    {
        printf("Request Reset ack correctly.\n");
        printf("Waiting %d secs to complete reset.\n", time);

        for(int retry = 0; retry < time; retry++)
        {
            printf("%d ", retry+1);
            fflush(stdout);

            if(!noquery)
            {
                if(isCalibrationDone())
                {
                    calibrationDone = true;
                    break;
                }
            }

            //1s
            usleep(QUERY_CALIBRATION_SLEEP_TIME);
        }

        printf("\n");

        if(noquery)
        {
            calibrationDone = true;
        }

        if(!calibrationDone)
        {
            printf("Reset is not finished in time.\n");
        }
    }
    else
    {
        ret = ERROR_IN_CALIBRAION;
        printf("Request calibration not ack correctly.\n");
    }

    if ( m_io->start_driver() < 0 )
    {
        printf( "start driver fail\n" );

        if(ret == SUCCESS)
        {
            if(calibrationDone)
            {
                ret = ERROR_START_DRIVER;
            }
            else
            {
                ret = ERROR_CALIBRAION_TIMEOUT;
            }
        }
    }

    return ret;
}

int SiSTouchAdapter::doCalibraion(int time, bool noquery)
{
    int ret = SUCCESS;

    if ( m_io->stop_driver() < 0 )
    {
        printf( "stop driver fail\n" );
        return ERROR_STOP_DRIVER;
    }

    bool result = callCalibration();
    bool calibrationDone = false;

    // [20150304]
    if(m_io->getChipNum() >= 7)
	{
	    noquery = true;
	}
	
    if(result)
    {
        printf("Request calibration ack correctly.\n");
        printf("Waiting %d secs to complete calibration.\n", time);

        for(int retry = 0; retry < time; retry++)
        {
            printf("%d ", retry+1);
            fflush(stdout);

            if(!noquery)
            {
                if(isCalibrationDone())
                {
                    calibrationDone = true;
                    break;
                }
            }

            //1s
            usleep(QUERY_CALIBRATION_SLEEP_TIME);
        }

        printf("\n");

        if(noquery)
        {
            calibrationDone = true;
        }

        if(!calibrationDone)
        {
            printf("Calibration is not finished in time.\n");
        }
    }
    else
    {
        ret = ERROR_IN_CALIBRAION;
        printf("Request calibration not ack correctly.\n");
    }

    if ( m_io->start_driver() < 0 )
    {
        printf( "start driver fail\n" );

        if(ret == SUCCESS)
        {
            if(calibrationDone)
            {
                ret = ERROR_START_DRIVER;
            }
            else
            {
                ret = ERROR_CALIBRAION_TIMEOUT;
            }
        }
    }

    return ret;
}

int SiSTouchAdapter::doUpdateCalibrationData(OpenShortData* data)
{
	int ret = SUCCESS;

    if ( m_io->stop_driver() < 0 )
    {
        printf( "stop driver fail\n" );
        return ERROR_STOP_DRIVER;
    }

	ret = updateCalibrationFlow(data);

    if ( m_io->start_driver() < 0 )
    {
        printf( "start driver fail\n" );

        if(ret == SUCCESS)
        {
            ret = ERROR_START_DRIVER;
        }
    }

    return ret;
}

int SiSTouchAdapter::doUpdate( FILE* file, bool update_bootloader, bool update_bootloader_auto, bool reserve_RODATA, bool update_parameter, bool force_update, bool jump_check  )
{
    if(!file)
    {
        printf("null file descriptor\n");
        return ERROR_NULL_FILE;
    }

    int buf[MAX_BUF_LENGTH] = {0};

    int file_length = getFileLength(file);

    /* int read_Length = */ read_file_to_buffer( file, 0, MAX_BUF_LENGTH, buf );

    // int index = 0;

    int ret = SUCCESS;

    bool result = false;

    bool isNewBootloader = false;

    if( !jump_check )
    {
        if( ! checkFileBufIsValid( file_length, buf, MAX_BUF_LENGTH ))
        {
            return ERROR_INVALID_FILE;
        }
    }

    prepaerBuffer(buf, MAX_BUF_LENGTH);

    if ( m_io->stop_driver() < 0 )
    {
        printf( "stop driver fail\n" );
        return ERROR_STOP_DRIVER;
    }

    if(jump_check)
    {
        printf("skip parameter validataion.\n");
        isNewBootloader = true;
        ret = SUCCESS;
    }
    else
    {
        ret = parameterCheck(update_bootloader, update_bootloader_auto, isNewBootloader, reserve_RODATA, update_parameter, force_update, buf, file_length );
    }

    if(ret == SUCCESS)
    {
        result = updateFlow(buf, MAX_BUF_LENGTH, file_length, update_bootloader, isNewBootloader, reserve_RODATA, update_parameter);
        if(!result)
        {
            ret = ERROR_IN_UPDATEFLOW;
        }
    }

    if ( m_io->start_driver() < 0 )
    {
        printf( "start driver fail\n" );

        if(ret == SUCCESS)
        {
            ret = ERROR_START_DRIVER;
        }
    }

    return ret;
}

int SiSTouchAdapter::setActiveDeviceName(char* devName, int devNameLen)
{
	return SiSTouchIO::setActiveDeviceName(devName, devNameLen);
}

SiSTouchAdapter* SiSTouchAdapter::prepareAdapter(const Parameter & param)
{

    int con = param.con;
    int max_retry = param.max_retry;
    int retry_delay = param.delay;
    int verbose = param.verbose;
    int ioDelay = param.io_Delay;
    int changeDiagMode = param.changeDiagMode;
    int changePWRMode = param.changePWRMode;

	// force to only detect hidraw
	int detectHidrawFlag = 1; //param.m_detectHidrawFlag;

	if(con == CON_AUTO)
	{
		con = SiSTouchIO::getDeviceType(max_retry, retry_delay, verbose, ioDelay, changeDiagMode, changePWRMode, detectHidrawFlag);
	}

	printf("con=%d\n\n", con);

	// only detect hidraw
	if(con == SiSTouchIO::CON_AEGIS_MULTI_FOR_NEW_DRIVER)
	{
		return new AegisMultiSiSTouchAdapter(max_retry, retry_delay, verbose, ioDelay, changeDiagMode, changePWRMode, detectHidrawFlag);
	}
    else
    {
            //no-op
    }

    printf("not suitable sis touch device\n");
    return 0;
}

float SiSTouchAdapter::transferRawDataToVoltage(int rawData)
{
    return (float)rawData / MAX_RAW_DATA * MAX_VOLTAGE;
}

float SiSTouchAdapter::transferRawDataToCycle(int rawData)
{
    return rawData * CLK_DIV * CLK_SCALE;
}

int SiSTouchAdapter::getDeviceOpenShortData(OpenShortData *data)
{

    bool result = false;
    int ret = SUCCESS;

    if(m_io->stop_driver() < 0)
    {
        printf( "stop driver fail\n" );
        return ERROR_STOP_DRIVER;
    }

    //get width/height, target voltage
    result = getDeviceInfo(data);
    if(!result)
    {
        ret = ERROR_IN_READ_DATA;
    }

    //get voltage/cycle/loop
    if(result)
    {
        result = getDeviceVoltageData(data);
        if(!result)
        {
            ret = ERROR_IN_READ_DATA;
        }
    }

    if ( m_io->start_driver() < 0 )
    {
        printf( "start driver fail\n" );

        if(ret == SUCCESS)
        {
            ret = ERROR_START_DRIVER;
        }
    }

    return ret;
}

bool SiSTouchAdapter::checkContinueNG(OpenShortConfig* config, OpenShortResult *result, int x, int y)
{
    int x_begin = (x == 0) ? x : x - 1;
    int x_end = (x == result->getWidth() - 1) ? x : x + 1;
    int y_begin = (y == 0) ? y : y - 1;
    int y_end = (y == result->getHeight() - 1) ? y : y + 1;

    int ngInARow = 0;
    for(int i = x_begin; i <= x_end; i++)
    {
        for(int j = y_begin; j <= y_end; j++)
        {
            if ( result->getPointResult(i, j) != POINT_PASS )
            {
                ngInARow++;
            }
        }
    }

    if ( ngInARow > config->secondConfig.maxContinueNg )
    {
        return true;
    }
    else
    {
        return false;
    }
}

void SiSTouchAdapter::checkSecondThreshold(OpenShortConfig* config, OpenShortResult *result)
{

	if ( result->getSecondNG()== 0 || result->getPointNG() > config->secondConfig.maxAllowNg || result->getPointNG() != result->getSecondNG() )
		return;

#if 0
	int ngInARow;
	for(int i = 0; i < result->getWidth(); i++)
    {
        ngInARow = 0;
        for(int j = 0; j < result->getHeight(); j++)
        {
            if ( result->getPointResult(i, j) != POINT_PASS )
            {
                if( ++ngInARow > config->secondConfig.maxContinueNg )
                    return;
            }
            else
            {
                ngInARow = 0;
            }

        }
    }

	for(int j = 0; j < result->getHeight(); j++)
    {
        ngInARow = 0;
        for(int i = 0; i < result->getWidth(); i++)
        {
            if ( result->getPointResult(i, j) != POINT_PASS )
            {
                if( ++ngInARow > config->secondConfig.maxContinueNg)
                    return;
            }
            else
            {
                ngInARow = 0;
            }

        }
    }
#else
	for(int i = 0; i < result->getWidth(); i++)
    {
        for(int j = 0; j < result->getHeight(); j++)
        {
            if ( result->getPointResult(i, j) != POINT_PASS )
            {
                if( checkContinueNG(config, result, i, j) )
                    return;
            }
        }
    }
#endif
    result->setPointNG(0);

	for(int i = 0; i < result->getWidth(); i++)
	{
		for(int j = 0; j < result->getHeight(); j ++)
		{
			result->setPointResult( i, j, SUCCESS );
		}
	}

    return;
}

int SiSTouchAdapter::doOpenShort(OpenShortData* deviceData, OpenShortConfig* config, OpenShortResult *result, bool bCheckSecondThreshold)
{
    int ret = 0;

    result->setWH(deviceData->getWidth(), deviceData->getHeight());

    ret = testLineOpenShort(config, deviceData, result);
    if(ret != SUCCESS)
    {
        return ret;
    }

    // MutualShortExtraCheck, begin
    ret = checkMutualShortExtra(config, deviceData, result);
    if(ret != SUCCESS)
    {
        return ret;
    }
    // MutualShortExtraCheck, end

    // MutualNearFarCheck, begin
    ret = checkMutualNearFar(config, deviceData, result);
    if(ret != SUCCESS)
    {
        return ret;
    }
    // MutualNearFarCheck, end

    ret = testPointOpenShort(config, deviceData, result);
    if(ret != SUCCESS)
    {
        return ret;
    }

	if ( bCheckSecondThreshold == true && config->secondConfig.enableSecondThreshold == 1 )
		checkSecondThreshold(config, result);

    return ret;
}

int SiSTouchAdapter::doOpenShort(OpenShortData* deviceData, OpenShortData* sampleData, OpenShortConfig* config, OpenShortResult *result, bool bCheckSecondThreshold)
{
    int ret = 0;

    result->setWH(deviceData->getWidth(), deviceData->getHeight());

    ret = testLineOpenShort(config, deviceData, sampleData, result);
    if(ret != SUCCESS)
    {
        return ret;
    }

    // MutualShortExtraCheck, begin
    ret = checkMutualShortExtra(config, deviceData, result);
    if(ret != SUCCESS)
    {
        return ret;
    }
    // MutualShortExtraCheck, end

    // MutualNearFarCheck, begin
    ret = checkMutualNearFar(config, deviceData, result);
    if(ret != SUCCESS)
    {
        return ret;
    }
    // MutualNearFarCheck, end

    ret = testPointOpenShort(config, deviceData, sampleData, result);
    if(ret != SUCCESS)
    {
        return ret;
    }

	if ( bCheckSecondThreshold == true && config->secondConfig.enableSecondThreshold == 1 )
		checkSecondThreshold(config, result);

    return ret;
}

int SiSTouchAdapter::setIODelay(int delayms)
{
    return m_io->setIODelay(delayms);
}

bool SiSTouchAdapter::doDetectIsMulti()
{
    return false;
}

int SiSTouchAdapter::doDetectSlaveNum()
{
    return 0;
}

int SiSTouchAdapter::doDumpROM(int * buf)
{
    int ret = SUCCESS;

    if(m_io->stop_driver() < 0)
    {
        printf( "stop driver fail\n" );
        return ERROR_STOP_DRIVER;
    }

    this->read_from_address(0XA0000000, buf, MAX_BUF_LENGTH);

    if ( m_io->start_driver() < 0 )
    {
        printf( "start driver fail\n" );

        if(ret == SUCCESS)
        {
            ret = ERROR_START_DRIVER;
        }
    }

    return ret;
}

int SiSTouchAdapter::doSoftReset()
{
    int ret = SUCCESS;

    if(m_io->stop_driver() < 0)
    {
        printf( "stop driver fail\n" );
        return ERROR_STOP_DRIVER;
    }

    bool result = callReset();

    if (result) {
        if ( m_io->start_driver() < 0 )
        {
            printf( "start driver fail\n" );

            if(ret == SUCCESS)
            {
                ret = ERROR_START_DRIVER;
            }
        }
    } else {
        printf( "reset fail, the chip may be in sleep mode\n" );
    }

    return ret;
}

int SiSTouchAdapter::doCheckITO()
{
    int ret = SUCCESS;
    int buf = 0;

    if(m_io->stop_driver() < 0)
    {
        printf( "stop driver fail\n" );
        return ERROR_STOP_DRIVER;
    }

    this->read_from_address(0x5A000003, &buf, 1);
    printf("buf[0x5A000003] = 0x%x (%3d)\n", buf, buf);

    if ( m_io->start_driver() < 0 )
    {
        printf( "start driver fail\n" );

        if(ret == SUCCESS)
        {
            ret = ERROR_START_DRIVER;
        }
    }

    char tmp = ((buf>>16)&0xff);
    printf("tmp = 0x%x (%3d)\n", tmp, tmp);


    int itoInfo = (int)( tmp ^ 0x03 );

    if ( itoInfo >= 0 && itoInfo <= 3 )
    {
        printf("If GND is 0 and NC is 1, ITO info is %d\n", itoInfo);
    }
    else
    {
        printf( "The FW not support auto-detect ITO pin !\n" );
    }

    return ret;
}
