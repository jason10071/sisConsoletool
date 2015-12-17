#include "AegisSiSTouchAdapter.h"
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <algorithm>
#include <cstring>

static inline unsigned char BCD (unsigned char x)
{
    return ((x / 10) << 4) | (x % 10);
}

static int getTimestamp()
{
    time_t rawtime;
    struct tm timeinfo;
    int mmddHHMM = 0;

    time (&rawtime);
    localtime_r (&rawtime, &timeinfo);
#if 0
    char tmpbuf[8];
    strftime(tmpbuf, 3, "%m", timeinfo); printf("%s",tmpbuf);
    mmddHHMM = BCD(static_cast<unsigned char>(atoi(tmpbuf)));
    strftime(tmpbuf, 3, "%d", timeinfo); printf("%s",tmpbuf);
    mmddHHMM |= BCD(static_cast<unsigned char>(atoi(tmpbuf)))<<8;
    strftime(tmpbuf, 3, "%H", timeinfo); printf("%s",tmpbuf);
    mmddHHMM |= BCD(static_cast<unsigned char>(atoi(tmpbuf)))<<16;
    strftime(tmpbuf, 3, "%M", timeinfo); printf("%s\n",tmpbuf);
    mmddHHMM |= BCD(static_cast<unsigned char>(atoi(tmpbuf)))<<24;
#else
    mmddHHMM  = BCD(static_cast<unsigned char>(timeinfo.tm_mon +1));
    mmddHHMM |= BCD(static_cast<unsigned char>(timeinfo.tm_mday))<<8;
    mmddHHMM |= BCD(static_cast<unsigned char>(timeinfo.tm_hour))<<16;
    mmddHHMM |= BCD(static_cast<unsigned char>(timeinfo.tm_min ))<<24;
#endif

    return mmddHHMM;
}

int
AegisSiSTouchAdapter::updateFlow( int* buf, int buf_length, int file_length, bool update_bootloader, bool isNewBootloader, bool reserve_RODATA, bool update_parameter, bool restart )
{


    if( isNewBootloader )
    {
        if(update_parameter)
        {
            printf( "write Parameters...\n");
            if ( !write_buffer_to_address( buf, FIRMWARE_INFO_BASE, buf_length, _4K, FIRMWARE_INFO_BASE ) )
            {
                printf( "write Parameters fail\n" );
                return 0;
            }
        }
        else
        {
#ifdef RESERVER_RO_DATA_ONLY_SUCCESS
            if(reserve_RODATA)
            {
                int RODataBuf[_8K/sizeof(int)] = {0};
                memset(RODataBuf, 0, _8K / sizeof(int) * sizeof(int));

                printf( "acquire RO data...\n");
                if( !read_from_address( DEVICE_CALIBRATION_DATA_ADDRESS, RODataBuf, _8K/sizeof(int) ) )
                {
                    printf( "cannot acquire RO data\n");
                    return 0;
                }

                memcpy( buf + CALIBRATION_BASE/sizeof(int) , RODataBuf, _8K / sizeof(int) * sizeof(int) );
            }


            int fakeBuf[_8K/sizeof(int)] = {0};
            memset(fakeBuf, 0, _8K / sizeof(int) * sizeof(int));

            printf("release RO data...\n");
            if ( !write_buffer_to_address( fakeBuf, 0, _8K/sizeof(int), _8K, CALIBRATION_BASE ) )
            {
                printf( "release RO fail\n" );
                return 0;
            }
#else
            if(reserve_RODATA)
            {
                int RODataBuf[_8K/sizeof(int)] = {0};
                memset(RODataBuf, 0, _8K / sizeof(int) * sizeof(int));

                printf( "acquire RO data...\n");
                if( !read_from_address( DEVICE_CALIBRATION_DATA_ADDRESS, RODataBuf, _8K/sizeof(int) ) )
                {
                    printf( "cannot acquire RO data\n");
                    return 0;
                }

                memcpy( buf + CALIBRATION_BASE/sizeof(int) , RODataBuf, _8K / sizeof(int) * sizeof(int) );

                buf[BOOT_FLAG_BASE / sizeof(int)] = 0x0;

                printf( "clear boot flag...\n");
                if ( !write_buffer_to_address( buf, CALIBRATION_BASE, buf_length, _8K, CALIBRATION_BASE ) )
                {
                    printf( "write FW fail\n" );
                    return 0;
                }

                buf[BOOT_FLAG_BASE / sizeof(int)] = invert_endian(BOOT_FLAG_VALUE);
            }
            else
            {

                int fakeBuf[_8K/sizeof(int)] = {0};
                memset(fakeBuf, 0, _8K / sizeof(int) * sizeof(int));

                printf("release RO data...\n");
                if ( !write_buffer_to_address( fakeBuf, 0, _8K/sizeof(int), _8K, CALIBRATION_BASE ) )
                {
                    printf( "release RO fail\n" );
                    return 0;
                }
            }
#endif


            if(update_bootloader)
            {
                printf( "write FW...\n");
                if ( !write_buffer_to_address( buf, MAINCODE_BASE, buf_length, _48K, MAINCODE_BASE ) )
                {
                    printf( "write FW fail\n" );
                    return 0;
                }


               printf( "write bootloader...\n");
                if ( !write_buffer_to_address( buf, INITIAL_BASE, buf_length, _4K, INITIAL_BASE ) )
                {
                    printf( "write FW fail\n" );
                    return 0;
                }

                printf( "write update code...\n");
                if ( !write_buffer_to_address( buf, UPDATE_BASE, buf_length, file_length - _56K -_4K, UPDATE_BASE ) )
                {
                    printf( "write FW fail\n" );
                    return 0;
                }

                printf( "write RO data...\n");
                if ( !write_buffer_to_address( buf, CALIBRATION_BASE, buf_length, _8K, CALIBRATION_BASE ) )
                {
                    printf( "write FW fail\n" );
                    return 0;
                }

            }
            else
            {
                printf( "write FW...\n");
                if ( !write_buffer_to_address( buf, MAINCODE_BASE, buf_length, _56K, MAINCODE_BASE ) )
                {
                    printf( "write FW fail\n" );
                    return 0;
                }
            }

        }
    }
    else
    {
        if(reserve_RODATA)
        {
            int RODataBuf[_8K/sizeof(int)] = {0};
            memset(RODataBuf, 0, _8K / sizeof(int) * sizeof(int));

            printf( "acquire RO data...\n");
            if( !read_from_address( DEVICE_CALIBRATION_DATA_ADDRESS, RODataBuf, _8K/sizeof(int) ) )
            {
                printf( "cannot acquire RO data\n");
                return 0;
            }

            memcpy( buf + CALIBRATION_BASE/sizeof(int) , RODataBuf, _8K / sizeof(int) * sizeof(int) );
        }


        if( update_parameter )
        {
            printf("Update parameter is not effect in old bootloader\n");
        }

        //original flow
        printf( "write FW...\n");
        if ( !write_buffer_to_address( buf, 0, buf_length, file_length, 0 ) )
        {
            printf( "write FW fail\n" );
            return 0;
        }
    }



    if(restart)
    {
        if (  m_io->call_82() < 0 )
        {
            printf( "Reset(0x82) fail\n" );
            return 0;
        }
    }

    return 1;
}

int
AegisSiSTouchAdapter::updateCalibrationFlow(OpenShortData* data)
{
	int buf[_8K/sizeof(int)] = {0};

	data->writeCalibrationToBuf((char *)buf);

	buf[(BOOT_FLAG_BASE - CALIBRATION_BASE) / sizeof(int)] = invert_endian(BOOT_FLAG_VALUE);
	buf[(CALIBRATION_FLAG_BASE - CALIBRATION_BASE) / sizeof(int)] = invert_endian(CALIBRATION_FLAG_VALUE);


	printf("buf[ %d ] = %d\n", static_cast<int>((CALIBRATION_FLAG_BASE - CALIBRATION_BASE) / sizeof(int)), static_cast<int>(buf[(CALIBRATION_FLAG_BASE - CALIBRATION_BASE) / sizeof(int)]));
	printf("write RO data...\n");

	if ( !write_buffer_to_address( buf, 0, _8K/sizeof(int), _8K, CALIBRATION_BASE ) )
	{
		printf( "write RO fail\n" );
		return ERROR_IN_UPDATEFLOW;
	}
	else
	{
		printf( "write RO success, reset ...\n" );

		if (  m_io->call_82() < 0 )
        {
            printf( "Reset(0x82) fail\n" );
            return ERROR_IN_UPDATEFLOW;
        }

		return SUCCESS;
	}

}

int
AegisSiSTouchAdapter::eraseCalibrationFlag(bool checkBootLoader)
{
    if(checkBootLoader)
	{
		int bootLoaderId[BOOTLOADER_ID_LENGTH];

		printf("reading boot loader info...\n");
		bool result = read_from_address(getBootloaderIdAddr(), bootLoaderId, BOOTLOADER_ID_LENGTH);

		if(result)
		{
			printf("reading boot loader info : success\n");

			int deviceBootloaderId = invert_endian(bootLoaderId[0]);

	        int main = (deviceBootloaderId >> 16) & 0xffff;
	        int sub = deviceBootloaderId & 0xffff;

			printf("current boot loader version : %d.%d\n", main, sub);

			if(main <= 0x0001 && sub < 0x0005)
			{
			    printf("boot loader version below 1.5 : not supported\n");
				return ERROR_IN_COMPARE_BOOTLOADER;
			}
		}
		else
		{
			printf("reading boot loader info : fail\n");
			return ERROR_IN_READ_DATA;
		}
	}

	int buf[_4K/sizeof(int)] = {0};

	buf[(BOOT_FLAG_BASE - CALIBRATION_BASE2) / sizeof(int)] = invert_endian(BOOT_FLAG_VALUE);

	printf("buf[ %d ] = %d\n", static_cast<int>((CALIBRATION_FLAG_BASE - CALIBRATION_BASE2) / sizeof(int)), static_cast<int>(buf[(CALIBRATION_FLAG_BASE - CALIBRATION_BASE2) / sizeof(int)]));
	printf("Erase RO data...\n");

	if ( !write_buffer_to_address( buf, 0, _4K/sizeof(int), _4K, CALIBRATION_BASE2 ) )
	{
		printf( "erase RO fail\n" );
		return ERROR_IN_UPDATEFLOW;
	}
	else
	{
		return SUCCESS;
	}

}


void
AegisSiSTouchAdapter::prepaerBuffer(int *buf, int buf_length)
{

    int index = 0;
    index = AEGIS_BASE / sizeof(int);
    if(index < buf_length)
    {
        buf[index] = invert_endian( 0x50383132 );
    }

    int device_firmware_id[FIRMWARE_ID_LENGTH] = {0};
    int getDeviceIdResult = getDeviceId(device_firmware_id, FIRMWARE_ID_LENGTH);
    if(getDeviceIdResult != SUCCESS && getDeviceIdResult != ERROR_START_DRIVER )
    {
        printf("ERROR: Fail to Get Device ID\n");
        exit(-1);
    }

    for (int i = 0; i < 3; i++) {
        buf[0xc0b0/sizeof(int)+i] = invert_endian(device_firmware_id[i]);
    }

    buf[0xc0bc/sizeof(int)] = buf[0xc0ac/sizeof(int)];

    for (int i = 0; i < 3; i++) {
        buf[0xc0a0/sizeof(int)+i] = buf[0xc004/sizeof(int)+i];
    }

    buf[0xc0ac/sizeof(int)] = getTimestamp();

}

int
AegisSiSTouchAdapter::isSiSFWFile(int* buf, int buf_length)
{

    if( buf[BUILDER_CHIPINFO_ADDRESS / sizeof(int)] != 0x73696765 || buf[BUILDER_SISMARK_ADDRESS / sizeof(int)] != 0x53695300 )
    {
        return 0;
    }
    else
    {
        return 1;
    }
}


int
AegisI2CSiSTouchAdapter::isSiSFWFile(int* buf, int buf_length)
{
    int ret = AegisSiSTouchAdapter::isSiSFWFile(buf, buf_length);
    if(ret)
    {
        unsigned int interfaceData = invert_endian(buf[INTERFACE_FLAG_BASE / sizeof(int)]);
        unsigned int interfaceId = (interfaceData >> INTERFACE_SHIFT) & INTERFACE_MASK;
        unsigned int multichip_Selective_ChipId = (interfaceData >> MULTI_SHIFT) & MULTI_MASK;

        if(interfaceId != I2C_INTERFACE || multichip_Selective_ChipId != NON_MULTI_DEVICE_FLAG)
        {
            printf("interface not match. interface : %02x, multi_select : %02x\n", interfaceId, multichip_Selective_ChipId);
            ret = 0;
        }
        else
        {
            ret = 1;
        }
    }

    return ret;
}

int
AegisUSBSiSTouchAdapter::isSiSFWFile(int* buf, int buf_length)
{
    int ret = AegisSiSTouchAdapter::isSiSFWFile(buf, buf_length);
    if(ret)
    {
        unsigned int interfaceData = invert_endian(buf[INTERFACE_FLAG_BASE / sizeof(int)]);
        unsigned int interfaceId = (interfaceData >> INTERFACE_SHIFT) & INTERFACE_MASK;
        unsigned int multichip_Selective_ChipId = (interfaceData >> MULTI_SHIFT) & MULTI_MASK;

        if(interfaceId != I2C_INTERFACE || multichip_Selective_ChipId != NON_MULTI_DEVICE_FLAG)
        {
            printf("interface not match. interface : %02x, multi_select : %02x\n", interfaceId, multichip_Selective_ChipId);
            ret = 0;
        }
        else
        {
            ret = 1;
        }
    }

    return ret;
}

int
AegisInterUSBSiSTouchAdapter::isSiSFWFile(int* buf, int buf_length)
{
    int ret = AegisSiSTouchAdapter::isSiSFWFile(buf, buf_length);
    if(ret)
    {
        unsigned int interfaceData = invert_endian(buf[INTERFACE_FLAG_BASE / sizeof(int)]);
        unsigned int interfaceId = (interfaceData >> INTERFACE_SHIFT) & INTERFACE_MASK;
        unsigned int multichip_Selective_ChipId = (interfaceData >> MULTI_SHIFT) & MULTI_MASK;

        if(interfaceId != USB_INTERFACE || multichip_Selective_ChipId == SLAVE_DEVICE0_FLAG || multichip_Selective_ChipId == SLAVE_DEVICE1_FLAG || multichip_Selective_ChipId == SLAVE_DEVICE2_FLAG)
        {
            printf("interface not match. interface : %02x, multi_select : %02x\n", interfaceId, multichip_Selective_ChipId);
            ret = 0;
        }
        else
        {
            ret = 1;
        }
    }

    return ret;
}


int
AegisSlaveSiSTouchAdapter::isSiSFWFile(int* buf, int buf_length)
{
    int ret = AegisSiSTouchAdapter::isSiSFWFile(buf, buf_length);
    if(ret)
    {
        unsigned int interfaceData = invert_endian(buf[INTERFACE_FLAG_BASE / sizeof(int)]);
        unsigned int interfaceId = (interfaceData >> INTERFACE_SHIFT) & INTERFACE_MASK;
        unsigned int multichip_Selective_ChipId = (interfaceData >> MULTI_SHIFT) & MULTI_MASK;;

        unsigned int slaveAddr = getSlaveAddr();

        if(interfaceId != I2C_INTERFACE || (multichip_Selective_ChipId != slaveAddr))
        {
            printf("interface not match. interface : %02x, multi_select : %02x\n", interfaceId, multichip_Selective_ChipId);
            ret = 0;
        }
        else
        {
            ret = 1;
        }
    }

    return ret;
}


int
AegisMultiSiSTouchAdapter::isSiSFWFile(int* buf, int buf_length)
{

    int ret = AegisSiSTouchAdapter::isSiSFWFile(buf, buf_length);
    if(ret)
    {
        unsigned int interfaceData = invert_endian(buf[INTERFACE_FLAG_BASE / sizeof(int)]);
        unsigned int interfaceId = (interfaceData >> INTERFACE_SHIFT) & INTERFACE_MASK;
        unsigned int multichip_Selective_ChipId = (interfaceData >> MULTI_SHIFT) & MULTI_MASK;

        unsigned int slaveAddr = getSlaveAddr();

		//if(multichip_Selective_ChipId == NON_MULTI_DEVICE_FLAG || ((slaveAddr > 0) && (multichip_Selective_ChipId != slaveAddr)) || ((slaveAddr <= 0) && (multichip_Selective_ChipId != MASTER_DEVICE_FLAG) && (multichip_Selective_ChipId != BRIDGE_DEVICE_FLAG) ))
        if( ((slaveAddr > 0) && (multichip_Selective_ChipId != slaveAddr)) ) // remove master == 0x00
        {
            printf("interface not match. interface : %02x, multi_select : %02x\n", interfaceId, multichip_Selective_ChipId);
            ret = 0;
        }
        else
        {
            ret = 1;
        }
    }

    return ret;
}

void
AegisMultiSiSTouchAdapter::setSlaveAddr( int slaveAddr )
{
    return ((SiSTouch_Aegis_Multi*)m_io)->setSlaveAddr(slaveAddr);
}

int
AegisMultiSiSTouchAdapter::getSlaveAddr()
{
    return ((SiSTouch_Aegis_Multi*)m_io)->getSlaveAddr();
}

int
AegisMultiSiSTouchAdapter::getStaticSlaveAddr(int idx)
{
    switch(idx)
    {
        case INDEX_MASTER:
            return MASTER_DEVICE_ADDR;
        case INDEX_SLAVE0:
            return SLAVE_DEVICE0_ADDR;
        case INDEX_SLAVE1:
            return SLAVE_DEVICE1_ADDR;
        case INDEX_SLAVE2:
            return SLAVE_DEVICE2_ADDR;
        case INDEX_SLAVE3:
            return SLAVE_DEVICE3_ADDR;
        case INDEX_SLAVE4:
            return SLAVE_DEVICE4_ADDR;
        case INDEX_SLAVE5:
            return SLAVE_DEVICE5_ADDR;
        default:
            return NON_MULTI_DEVICE_ADDR;
    }
}

static inline void wait(const int sec)
{
    printf("wait for %d secs\n", sec);
    for (int i = 0; i < sec; i++) {
        printf("...");
        fflush(stdout);
        sleep(1);
    }
    printf("\n");
}

static inline bool Is817MasterFWBroken(AegisMultiSiSTouchAdapter * adapter)
{
    return adapter->doCheckFWState()!=SiSTouchAdapter::RESULT_SAME;
}

int
AegisMultiSiSTouchAdapter::doUpdate( FILE** multi_files, int multi_Num, const UpdateFWParameter & param)
{
    bool is817MasterFWBroken = Is817MasterFWBroken(this);

    if(!multi_files)
    {
        printf("null file descriptor\n");
        return ERROR_NULL_FILE;
    }
#define NUM_OF_FILES (7)

    int multi_files_buf[NUM_OF_FILES][MAX_BUF_LENGTH] = {0};

    int multi_files_length[NUM_OF_FILES] = {0};

    for(int i=0;i<multi_Num;i++)
    {
        multi_files_length[i] = getFileLength(multi_files[i]);
    }

    // int multi_files_read_length[NUM_OF_FILES] = {0};

    for(int i=0; i<multi_Num; i++)
    {
       /* multi_files_read_length[i] = */ read_file_to_buffer( multi_files[i], 0, MAX_BUF_LENGTH, multi_files_buf[i] );
    }

    // int index = 0;

    int ret = SUCCESS;

    bool result = false;

    bool multi_update_bootloader[NUM_OF_FILES] = {0};

    bool multi_update_bootloader_auto[NUM_OF_FILES] = {0};

    bool multi_isNewBootloader[NUM_OF_FILES] = {0};

    bool multi_reserve_RODATA[NUM_OF_FILES] = {0};

    bool multi_update_parameter[NUM_OF_FILES] = {0};

    bool multi_force_update[NUM_OF_FILES] = {0};
#undef NUM_OF_FILES

    if( !param.jump_check )
    {
        for(int i=0; i<multi_Num; i++)
        {
            setSlaveAddr(getStaticSlaveAddr(i));
            if( ! checkFileBufIsValid( multi_files_length[i], multi_files_buf[i], MAX_BUF_LENGTH ))
            {
                return ERROR_INVALID_FILE;
            }
        }
    }

    for(int i=0; i<multi_Num; i++)
    {
        prepaerBuffer(multi_files_buf[i], MAX_BUF_LENGTH);
    }

    if ( m_io->stop_driver() < 0 )
    {
        printf( "stop driver fail\n" );
        return ERROR_STOP_DRIVER;
    }

    for(int i=0; i<multi_Num; i++)
    {
        if(i == 0)
        {
            printf("master:\n");
        }
        else
        {
            printf("slave %d:\n", (i-1));
        }

        multi_update_bootloader[i] = param.update_bootloader;

        multi_update_bootloader_auto[i] = param.update_bootloader_auto;

        multi_isNewBootloader[i] = false;

        multi_reserve_RODATA[i] = param.reserve_RODATA;

        multi_update_parameter[i] = param.update_parameter;

        multi_force_update[i] = param.force_update;

        setSlaveAddr(getStaticSlaveAddr(i));

        if(param.jump_check)
        {
            printf("skip parameter validataion.\n");
            multi_isNewBootloader[i] = true;
            ret = SUCCESS;
        }
        else
        {
            ret = parameterCheck(multi_update_bootloader[i], multi_update_bootloader_auto[i], multi_isNewBootloader[i], multi_reserve_RODATA[i], multi_update_parameter[i], multi_force_update[i], multi_files_buf[i], multi_files_length[i] );
        }
    }

    if(ret == SUCCESS)
    {
        if (is817MasterFWBroken)
        {
            // update master first and wait for param.wait_time
            printf("master:\n");

            setSlaveAddr(getStaticSlaveAddr(0));

            result = updateFlow(multi_files_buf[0], MAX_BUF_LENGTH, multi_files_length[0], multi_update_bootloader[0], multi_isNewBootloader[0], multi_reserve_RODATA[0], multi_update_parameter[0]);
            if(!result)
            {
                ret = ERROR_IN_UPDATEFLOW;
            }
            if(ret == SUCCESS)
            {
                // wait for master reboot
                wait(param.wait_time);

                for(int i=1; i<multi_Num; i++)
                {
                    printf("slave %d:\n", (i-1));

                    setSlaveAddr(getStaticSlaveAddr(i));

                    // must restart when finish update all slaves FW
                    result = updateFlow(multi_files_buf[i], MAX_BUF_LENGTH, multi_files_length[i], multi_update_bootloader[i], multi_isNewBootloader[i], multi_reserve_RODATA[i], multi_update_parameter[i], (i!=multi_Num-1) ? false : true);
                    if(!result)
                    {
                        ret = ERROR_IN_UPDATEFLOW;
                        break;
                    }
                }
            }
        }
        else
        {
            //update slave first
            for(int i=1; i<multi_Num; i++)
            {
                printf("slave %d:\n", (i-1));

                setSlaveAddr(getStaticSlaveAddr(i));

                if(ret == SUCCESS)
                {
                    result = updateFlow(multi_files_buf[i], MAX_BUF_LENGTH, multi_files_length[i], multi_update_bootloader[i], multi_isNewBootloader[i], multi_reserve_RODATA[i], multi_update_parameter[i], false);
                    if(!result)
                    {
                        ret = ERROR_IN_UPDATEFLOW;
                        break;
                    }
                }
            }

            if(ret == SUCCESS)
            {
                printf("master:\n");

                setSlaveAddr(getStaticSlaveAddr(0));

                result = updateFlow(multi_files_buf[0], MAX_BUF_LENGTH, multi_files_length[0], multi_update_bootloader[0], multi_isNewBootloader[0], multi_reserve_RODATA[0], multi_update_parameter[0]);
                if(!result)
                {
                    ret = ERROR_IN_UPDATEFLOW;
                }
            }
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

int
AegisMultiSiSTouchAdapter::doEraseCalibrationFlag( int multi_Num )
{
	int ret = SUCCESS;

    if ( m_io->stop_driver() < 0 )
    {
        printf( "stop driver fail\n" );
        return ERROR_STOP_DRIVER;
    }

	//erase slave first
	for(int i = multi_Num - 1; i >= 0; i--)
	{
	    if( i == 0 )
			printf("master:\n");
		else
			printf("slave %d:\n", (i-1));

		if(ret == SUCCESS)
		{
			setSlaveAddr(getStaticSlaveAddr(i));

			if( i == multi_Num - 1 )
				ret = eraseCalibrationFlag(true);
			else
				ret = eraseCalibrationFlag(false);

			if(!ret)
			{
				ret = ERROR_IN_UPDATEFLOW;
				break;
			}
		}
	}

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

int
AegisMultiSiSTouchAdapter::getMultiDeviceOpenShortData(MultiOpenShortData *data )
{
    bool result = false;
    int ret = SUCCESS;
    int chipNum = data->getChipNum();

    if(m_io->stop_driver() < 0)
    {
        printf( "stop driver fail\n" );
        return ERROR_STOP_DRIVER;
    }

    for(int i = 0;i < chipNum; i++)
    {
        int chipOrderIndex = data->getChipOrderIndex(i);

        setSlaveAddr(getStaticSlaveAddr(chipOrderIndex));

        //get width/height, target voltage
        result = getDeviceInfo(data->getData(i));
        if(!result)
        {
            ret = ERROR_IN_READ_DATA;
        }

        //get voltage/cycle/loop
        if(result)
        {
            data->getData(i)->setReadData(data->getReadData());
            result = getDeviceVoltageData(data->getData(i));
            if(!result)
            {
                ret = ERROR_IN_READ_DATA;
            }
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


bool
AegisMultiSiSTouchAdapter::checkContinueNG(OpenShortConfig* config, MultiOpenShortResult *result, int x, int y)
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


void
AegisMultiSiSTouchAdapter::checkSecondThreshold(OpenShortConfig* config, MultiOpenShortResult *result)
{
	int chipNum = result->getChipNum();


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
                if( ++ngInARow > config->secondConfig.maxContinueNg)
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

	for(int i = 0; i < chipNum; i++)
    {
	    OpenShortResult *temp = result->getResult(i);
        temp->setPointNG(0);

		for(int j = 0; j < temp->getWidth(); j++)
		{
			for(int k = 0; k < temp->getHeight(); k ++)
			{
				temp->setPointResult( j, k, SUCCESS );
			}
		}
	}




}

int
AegisMultiSiSTouchAdapter::doMultiOpenShort(MultiOpenShortData* deviceData, OpenShortConfig* config, MultiOpenShortResult *result)
{
    int ret = SUCCESS;
    int chipNum = deviceData->getChipNum();
    int resultChipNum = result->getChipNum();

    if(resultChipNum != chipNum)
    {
        result->setChipNum(chipNum);
    }

    for(int i = 0; i < chipNum; i++)
    {
        result->setChipOrderIndex(i, deviceData->getChipOrderIndex(i));
    }

    for(int i = 0; i < chipNum; i++)
    {
        ret = doOpenShort(deviceData->getData(i), config, result->getResult(i), false);

        if(ret != SUCCESS)
        {
            return ret;
        }
    }

    // MutualShortExtraCheckByTP, begin
    result->init(deviceData);
    testLineOpenShortByTP(config, deviceData, result); // do after doOpenShort of each single-chip 

    checkMutualShortExtraByTP(config, deviceData, result);
    // MutualShortExtraCheckByTP, end

    // MutualNearFarCheckByTP, begin
    checkMutualNearFarByTP(config, deviceData, result);
    // MutualNearFarCheckByTP, end

	if ( config->secondConfig.enableSecondThreshold == 1 )
		checkSecondThreshold(config, result);


    return ret;
}


int
AegisMultiSiSTouchAdapter::doMultiOpenShort(MultiOpenShortData* deviceData, MultiOpenShortData* sampleData, OpenShortConfig* config, MultiOpenShortResult *result)
{
    int ret = SUCCESS;
    int chipNum = deviceData->getChipNum();
    int resultChipNum = result->getChipNum();

    if(resultChipNum != chipNum)
    {
        result->setChipNum(chipNum);
    }

    for(int i = 0; i < chipNum; i++)
    {
        result->setChipOrderIndex(i, deviceData->getChipOrderIndex(i));
    }

    for(int i = 0;i < chipNum; i++)
    {
        ret = doOpenShort(deviceData->getData(i), sampleData->getData(i), config, result->getResult(i), false);
        if(ret != SUCCESS)
        {
            return ret;
        }
    }

    // MutualShortExtraCheckByTP, begin

    // same as no sampleData

    // MutualShortExtraCheckByTP, end

	if ( config->secondConfig.enableSecondThreshold == 1 )
		checkSecondThreshold(config, result);

    return ret;
}

// MutualShortExtraCheckByTP, begin
int
AegisMultiSiSTouchAdapter::testLineOpenShortByTP(OpenShortConfig* config, MultiOpenShortData *deviceData, MultiOpenShortResult *result)
{
    int width = deviceData->getWidth();
    int height = deviceData->getHeight();

    // float caliTime = 0;
    // float sampleTime = 0;
    float timeDiff = 0;
    float volDiff = 0;

    float tempLoop = 0;
    float tempCycle = 0;
    float tempTime = 0;
    // float tempSampleTime = 0;

    int shortNodeCount = 0;
    int nNG = 0;

    if((width + height) == 0)
    {
        return ERROR_NODATA;
    }

    int minLoop = config->lineConfig.minLoop;
    int maxLoop = config->lineConfig.maxLoop;

    std::vector<float> leftTopList, leftBottomList, rightTopList, rightBottomList, totalList;


    for ( int i = 0; i < width; i++ )
    {
        for ( int j = 0; j < height; j++ )
        {

            tempLoop = deviceData->getLoop(i, j);
            tempCycle = deviceData->getCycle(i, j);

            tempTime = tempLoop * tempCycle;
            if( tempLoop > minLoop && tempLoop < maxLoop )
            {
                totalList.push_back( tempTime );

                if ( i < (width / 2) && j < (height / 2) )
                {
                    leftTopList.push_back( tempTime );
                }
                else if ( i >= (width / 2) && j < (height / 2) )
                {
                    rightTopList.push_back( tempTime );
                }
                else if ( i < (width / 2) && j >= (height / 2) )
                {
                    leftBottomList.push_back( tempTime );
                }
                else
                {
                    rightBottomList.push_back( tempTime );
                }
            }

        }
    }


    float leftTopBaseTime, leftBottomBaseTime, rightTopBaseTime, rightBottomBaseTime, totalBaseTime;

    if ( totalList.size() == 0 )
    {
        for( int i = 0; i < width; i++ )
        {
            result->setXLineTpResult(i, LINE_SHORT_LOOP);
            nNG++;
        }

        for( int j = 0; j < height; j++ )
        {
            result->setYLineTpResult(j, LINE_SHORT_LOOP);
            nNG++;
        }
    }
    else
    {
        std::sort( totalList.begin(), totalList.end() );
        totalBaseTime = totalList[ totalList.size() / 2 ];

        if ( leftTopList.size() == 0 )
        {
            leftTopBaseTime = totalBaseTime;
        }
        else
        {
            std::sort( leftTopList.begin(), leftTopList.end() );
            leftTopBaseTime = leftTopList[ leftTopList.size() / 2 ];
        }

        if ( rightTopList.size() == 0 )
        {
            rightTopBaseTime = totalBaseTime;
        }
        else
        {
            std::sort( rightTopList.begin(), rightTopList.end() );
            rightTopBaseTime = rightTopList[ rightTopList.size() / 2 ];
        }

        if ( leftBottomList.size() == 0 )
        {
            leftBottomBaseTime = totalBaseTime;
        }
        else
        {
            std::sort( leftBottomList.begin(), leftBottomList.end() );
            leftBottomBaseTime = leftBottomList[ leftBottomList.size() / 2 ];
        }

        if ( rightBottomList.size() == 0 )
        {
            rightBottomBaseTime = totalBaseTime;
        }
        else
        {
            std::sort( rightBottomList.begin(), rightBottomList.end() );
            rightBottomBaseTime = rightBottomList[ rightBottomList.size() / 2 ];
        }


        for ( int i = 0; i < width; i++ )
        {
            shortNodeCount = 0;
            for ( int j = 0; j < height; j++ )
            {

                tempLoop = deviceData->getLoop(i, j);
                tempCycle = deviceData->getCycle(i, j);

                tempTime = tempLoop * tempCycle;

                if ( i < (width / 2) && j < (height / 2) )
                    timeDiff = getAbsDiffPercentage(tempTime , leftTopBaseTime);
                else if ( i >= (width / 2) && j < (height / 2) )
                    timeDiff = getAbsDiffPercentage(tempTime , rightTopBaseTime);
                else if ( i < (width / 2) && j >= (height / 2) )
                    timeDiff = getAbsDiffPercentage(tempTime , leftBottomBaseTime);
                else
                    timeDiff = getAbsDiffPercentage(tempTime , rightBottomBaseTime);


                if ( deviceData->getReadData() )
                	volDiff = getAbsDiffPercentage(deviceData->getBaseVoltage(i, j) , deviceData->getTargetVoltage(i, j));
				else
					volDiff = getAbsDiffPercentage(deviceData->getRawVoltage(i, j) , deviceData->getTargetVoltage(i, j));

                if ( timeDiff > config->lineConfig.calibTime || volDiff > config->lineConfig.voltage )
                {
                    result->setPointTpResult(i, j, POINT_SHORT);
                }
                else
                {
                    result->setPointTpResult(i, j, POINT_PASS);
                }


            }
        }

        for( int i = 0; i < width; i++ )
        {

            shortNodeCount = 0;
            for( int j = 0; j < height; j++ )
            {
                if(result->getPointTpResult(i, j) == POINT_SHORT)
                {
                    shortNodeCount++;
                }
            }

            if(shortNodeCount >= (config->lineConfig.outOfSpecNode / 100.0f * height))
            {
                result->setXLineTpResult(i, LINE_SHORT);
                nNG++;
            }
            else
            {
                result->setXLineTpResult(i, LINE_PASS);
            }
        }


        for( int j = 0; j < height; j++ )
        {
            shortNodeCount = 0;
            for( int i = 0; i < width; i++ )
            {
                if(result->getPointTpResult(i, j)  == POINT_SHORT)
                {
                    shortNodeCount++;
                }
            }

            if(shortNodeCount >= (config->lineConfig.outOfSpecNode /100.0f * width))
            {
                result->setYLineTpResult(j, LINE_SHORT);
                nNG++;
            }
            else
            {
                result->setYLineTpResult(j, LINE_PASS);
            }
        }

    }

    result->setLineTpNG(nNG);



    return SUCCESS;
}

float
AegisMultiSiSTouchAdapter::getAbsDiffPercentage(float numerator, float denominator)
{
    if(denominator == 0)
    {
        if(numerator == 0)
        {
            return 0;
        }
        else
        {
            return 100;
        }
    }
    else
    {
        return 100 * fabs(numerator - denominator) / denominator;
    }
}

int
AegisMultiSiSTouchAdapter::JudgeTxShortResultByTP(OpenShortConfig* config, MultiOpenShortData *deviceData, MultiOpenShortResult *result)
{
    int width = deviceData->getWidth();
    int height = deviceData->getHeight();

    if(width + height == 0)
    {
        return ERROR_NODATA;
    }

    int shortNodeCnt;
    int shortNodeRepairCnt;
    bool shortCntOverThreshold;

    int nNG = result->getLineTpNG();

    for ( int j = 0; j < height; j++ )
    { 
        shortNodeCnt = 0;
        shortNodeRepairCnt = 0;
        shortCntOverThreshold = 0;
        
        for(int i = 0; i < width; i++)
        {
            if(result->getCycleTxDiff(i, j) > config->lineExtraConfig.RXCycleDiffHighPercent) // Config switch x/y
            {
                shortNodeCnt++;
            } else if(result->getCycleTxDiff(i, j) <= -1 * config->lineExtraConfig.RXCycleDiffLowPercent // Config switch x/y
                      && ((j - 1) >= 0))
            {
                shortNodeRepairCnt++;
            }
        }

        shortCntOverThreshold = (shortNodeCnt >= (float) width * config->lineExtraConfig.cycleOutOfSpecNodePercent/100);
        if(shortCntOverThreshold)
        {
            if(result->getYLineTpResult(j) != LINE_SHORT &&
               result->getYLineTpResult(j) != LINE_SHORT_LOOP)
            {
                result->setYLineTpResult(j, LINE_EXTREME_DIF);
                nNG++;
            }
        }

        shortCntOverThreshold = (shortNodeRepairCnt >= (float) width * config->lineExtraConfig.cycleOutOfSpecNodePercent/100);
        if(shortCntOverThreshold)
        {
            if(result->getYLineTpResult(j-1) != LINE_SHORT
               && result->getYLineTpResult(j-1) != LINE_SHORT_LOOP
               && result->getYLineTpResult(j-1) != LINE_EXTREME_DIF
               && ((j - 1) >= 0) 
               )
            {
                result->setYLineTpResult(j-1, LINE_EXTREME_DIF);
                nNG++;
            }
        }
    }

    // only mark 2 line short, repeal unusual loading
    if((int)config->lineExtraConfig.detectShortOnly == 1)
    {
        for( int j = 0; j < height; j++ )
        {
            if(result->getYLineTpResult(j) == LINE_EXTREME_DIF)
            {
                if(   (result->getYLineTpResult(j-1) == LINE_EXTREME_DIF && ((j - 1) >= 0))
                   || (result->getYLineTpResult(j+1) == LINE_EXTREME_DIF && ((j + 1) < height)))
                {
                    continue;
                }
                else
                {
                    result->setYLineTpResult(j, LINE_PASS);
                    nNG--;
                }
            }
        }
    }

    result->setLineTpNG(nNG);

    return SUCCESS;
}

int
AegisMultiSiSTouchAdapter::JudgeRxShortResultByTP(OpenShortConfig* config, MultiOpenShortData *deviceData, MultiOpenShortResult *result)
{
    int width = deviceData->getWidth();
    int height = deviceData->getHeight();

    if(width + height == 0)
    {
        return ERROR_NODATA;
    }

    int shortNodeCnt;
    int shortNodeRepairCnt;
    bool shortCntOverThreshold;

    int nNG = result->getLineTpNG();

    for ( int i = 0; i < width; i++ )
    {
        shortNodeCnt = 0;
        shortNodeRepairCnt = 0;
        shortCntOverThreshold = 0;
        
        for(int j = 0; j < height; j++)
        {
            if(result->getCycleRxDiff(i, j) > config->lineExtraConfig.TXCycleDiffHighPercent) // Config switch x/y
            {
                shortNodeCnt++;
            } else if(result->getCycleRxDiff(i, j) <= -1 * config->lineExtraConfig.TXCycleDiffLowPercent // Config switch x/y
                      && ((i - 1) >= 0))
            {
                shortNodeRepairCnt++;
            }
        }

        shortCntOverThreshold = (shortNodeCnt >= (float) height * config->lineExtraConfig.cycleOutOfSpecNodePercent/100);
        if(shortCntOverThreshold)
        {
            if(result->getXLineTpResult(i) != LINE_SHORT &&
               result->getXLineTpResult(i) != LINE_SHORT_LOOP)
            {
                result->setXLineTpResult(i, LINE_EXTREME_DIF);
                nNG++;
            }
        }

        shortCntOverThreshold = (shortNodeRepairCnt >= (float) height * config->lineExtraConfig.cycleOutOfSpecNodePercent/100);
        if(shortCntOverThreshold)
        {
            if(result->getXLineTpResult(i-1) != LINE_SHORT
               && result->getXLineTpResult(i-1) != LINE_SHORT_LOOP
               && result->getXLineTpResult(i-1) != LINE_EXTREME_DIF
               && ((i - 1) >= 0) 
               )
            {
                result->setXLineTpResult(i-1, LINE_EXTREME_DIF);
                nNG++;
            }
        }
    }

    // only mark 2 line short, repeal unusual loading
    if((int)config->lineExtraConfig.detectShortOnly == 1)
    {
        for( int i = 0; i < width; i++ )
        {
            if(result->getXLineTpResult(i) == LINE_EXTREME_DIF)
            {
                if(   (result->getXLineTpResult(i-1) == LINE_EXTREME_DIF && ((i - 1) >= 0))
                   || (result->getXLineTpResult(i+1) == LINE_EXTREME_DIF && ((i + 1) < width)))
                {
                    continue;
                }
                else
                {
                    result->setXLineTpResult(i, LINE_PASS);
                    nNG--;
                }
            }
        }
    }

    result->setLineTpNG(nNG);

    return SUCCESS;
}

int
AegisMultiSiSTouchAdapter::checkMutualShortExtraByTP(OpenShortConfig* config, MultiOpenShortData *deviceData, MultiOpenShortResult *result)
{
    int ret = 0;

    if((int)config->lineExtraConfig.enableRXExtraCheck == 1) // Config switch x/y
    {
        ret = JudgeTxShortResultByTP(config, deviceData, result);
        if(ret != SUCCESS)
        {
            return ret;
        }
    }

    if((int)config->lineExtraConfig.enableTXExtraCheck == 1) // Config switch x/y
    {
        ret = JudgeRxShortResultByTP(config, deviceData, result);
        if(ret != SUCCESS)
        {
            return ret;
        }
    }

    return SUCCESS;
}
// MutualShortExtraCheckByTP, end

// MutualNearFarCheckByTP, begin
int
AegisMultiSiSTouchAdapter::checkMutualNearFarByTP(OpenShortConfig* config, MultiOpenShortData *deviceData, MultiOpenShortResult *result)
{
    result->lostSide = LOST_NONE;
    
    if(fabs(config->lineNearFarConfig.cycleDifferencePercentTx / 100.0) < 1E-3)
    {
        // (config->lineNearFarConfig.cycleDifferencePercentTx / 100.0) = 1
    } else
    {
        if((config->lineNearFarConfig.cycleDifferencePercentTx / 100.0) < 1)
        {
            checkMutualNearFarTxByTP(config, deviceData, result);
        }
    }

    if(fabs(config->lineNearFarConfig.cycleDifferencePercentRx / 100.0) < 1E-3)
    {
        // (config->lineNearFarConfig.cycleDifferencePercentRx / 100.0) = 1
    } else
    {
        if((config->lineNearFarConfig.cycleDifferencePercentRx / 100.0) < 1)
        {
            checkMutualNearFarRxByTP(config, deviceData, result);
        }
    }

    return SUCCESS;
}

void
AegisMultiSiSTouchAdapter::checkMutualNearFarTxByTP(OpenShortConfig* config, MultiOpenShortData *deviceData, MultiOpenShortResult *result)
{
    int width = deviceData->getWidth();
    int height = deviceData->getHeight();

    int left_bigger_than_right = 0;
    int right_bigger_than_left = 0;

    int normal_count = 0;
    int abnormal_count = 0;

    double val =0.0;

    for(int y = 0; y < height; y++)
    {
        int pos = y;

        int x = 0;
        int condition_count = 0;

        // TX: avg. of left n pointer
        double left = 0.0;
        x = condition_count = 0;
        while(condition_count < config->lineNearFarConfig.cycleAvgCountNumberTx)
        {
            left += deviceData->getCycle(x, y) * deviceData->getLoop(x, y);
            condition_count++;
            x++;
        }
        left /= config->lineNearFarConfig.cycleAvgCountNumberTx;

        // TX: avg. of right n pointer
        double right = 0.0;
        x = width - 1;
        condition_count = 0;
        while(condition_count < config->lineNearFarConfig.cycleAvgCountNumberTx)
        {
            right += deviceData->getCycle(x, y) * deviceData->getLoop(x, y);
            condition_count++;
            x--;
        }
        right /= config->lineNearFarConfig.cycleAvgCountNumberTx;

        val = (left - right) / std::max(left, right);
        result->setYLineTpNearFarValue(pos, val);

        if(val > 0)
        {
            left_bigger_than_right++;
        } else 
        {
            right_bigger_than_left++;
        }

        if(fabs(val) > (config->lineNearFarConfig.cycleDifferencePercentTx / 100.0))
        {
            result->setYLineTpNearFarResult(pos, LINE_NEARFAR_SHORT);
            abnormal_count++;
        } else
        {
            normal_count++;
        }
    }

    if(abnormal_count > config->lineNearFarConfig.cycleOutOfChannels)
    {
        int nNG = result->getLineTpNG();
        int nChipNG = result->getResult(0)->getLineNG();

        for(int j = 0; j < height; j++)
        {
            if(result->getYLineTpResult(j) != LINE_SHORT &&
               result->getYLineTpResult(j) != LINE_SHORT_LOOP &&
               result->getYLineTpResult(j) != LINE_EXTREME_DIF)
            {
                if(result->getYLineTpNearFarResult(j) == LINE_NEARFAR_SHORT)
                {
                    result->setYLineTpResult(j, LINE_NEARFAR_SHORT);
                    nNG++;
                    nChipNG++;
                }
            }
        }

        result->setLineTpNG(nNG);

        if(config->m_chipNum > 1) // only for TP
        {
        	result->getResult(0)->setLineNG(nChipNG);  // recode TP at chip_0
        }

        if(left_bigger_than_right > right_bigger_than_left)
        {
            result->lostSide |= LOST_LEFT;
        } else 
        {
            result->lostSide |= LOST_RIGHT;
        }
    }
}

void
AegisMultiSiSTouchAdapter::checkMutualNearFarRxByTP(OpenShortConfig* config, MultiOpenShortData *deviceData, MultiOpenShortResult *result)
{
    int width = deviceData->getWidth();
    int height = deviceData->getHeight();

    int top_bigger_than_down = 0;
    int down_bigger_than_top = 0;

    int normal_count = 0;
    int abnormal_count = 0;

    double val =0.0;

    for(int x = 0; x < width; x++)
    {
        int pos = x;

        int y = 0;
        int condition_count = 0;

        // RX: avg. of top n pointer
        double top = 0.0;
        y = condition_count = 0;
        while(condition_count < config->lineNearFarConfig.cycleAvgCountNumberRx)
        {
            top += deviceData->getCycle(x, y) * deviceData->getLoop(x, y);
            condition_count++;
            y++;
        }
        top /= config->lineNearFarConfig.cycleAvgCountNumberRx;

        // RX: avg. of down n pointer
        double down = 0.0;
        y = height - 1;
        condition_count = 0;
        while(condition_count < config->lineNearFarConfig.cycleAvgCountNumberRx)
        {
            down += deviceData->getCycle(x, y) * deviceData->getLoop(x, y);
            condition_count++;
            y--;
        }
        down /= config->lineNearFarConfig.cycleAvgCountNumberRx;

        val = (top - down) / std::max(top, down);
        result->setXLineTpNearFarValue(pos, val);

        if(val > 0)
        {
            top_bigger_than_down++;
        } else 
        {
            down_bigger_than_top++;
        }

        if(fabs(val) > (config->lineNearFarConfig.cycleDifferencePercentRx / 100.0))
        {
            result->setXLineTpNearFarResult(pos, LINE_NEARFAR_SHORT);
            abnormal_count++;
        } else
        {
            normal_count++;
        }
    }

    if(abnormal_count > config->lineNearFarConfig.cycleOutOfChannels)
    {
        int nNG = result->getLineTpNG();
        int nChipNG = result->getResult(0)->getLineNG();

        for(int i = 0; i < width; i++)
        {
            if(result->getXLineTpResult(i) != LINE_SHORT &&
               result->getXLineTpResult(i) != LINE_SHORT_LOOP &&
               result->getXLineTpResult(i) != LINE_EXTREME_DIF)
            {
                if(result->getXLineTpNearFarResult(i) == LINE_NEARFAR_SHORT)
                {
                    result->setXLineTpResult(i, LINE_NEARFAR_SHORT);
                    nNG++;
                    nChipNG++;
                }
            }
        }
        result->setLineTpNG(nNG);

        if(config->m_chipNum > 1) // only for TP
        {
        	result->getResult(0)->setLineNG(nChipNG);  // recode TP at chip_0
        }

        if(top_bigger_than_down > down_bigger_than_top)
        {
            result->lostSide |= LOST_TOP;
        } else 
        {
            result->lostSide |= LOST_DOWN;
        }
    }
}

// MutualNearFarCheckByTP, end

bool
AegisSiSTouchAdapter::loadTargetVoltage(float *targetVoltage)
{
    int buf[1] = {0};

    int ret = read_from_address(getTargetVolatgeAddress(), buf, 1);

    if(!ret)
    {
        printf("cannot load target voltage from device\n");
        return ret;
    }

    buf[0] = invert_endian(buf[0]);

    int rawTargetVoltage = (buf[0] &  TARGET_VOLTAGE_MASK) >> TARGET_VOLTAGE_SHIFT;

    *targetVoltage = transferRawDataToVoltage(rawTargetVoltage);

    return ret;
}

bool
AegisSiSTouchAdapter::loadWidthHeight(int* width, int* height)
{
    int buf[1] = {0};

    int ret = read_from_address(getWidthHeightAddress(), buf, 1);

    if(!ret)
    {
        printf("cannot load width/height from device\n");
        return ret;
    }

    buf[0] = invert_endian(buf[0]);

    *width = (buf[0] & WIDTH_MASK) >> WIDTH_SHIFT;

    *height = (buf[0] & HEIGHT_MASK) >> HEIGHT_SHIFT;

    return ret;
}

bool
AegisSiSTouchAdapter::loadBaseVoltage(float *dataBuf, int buf_width, int buf_height)
{
    int layout_width = buf_width;
    int layout_height = buf_height / MEMORY_ALIGNMENT * MEMORY_ALIGNMENT + (buf_height % MEMORY_ALIGNMENT == 0 ? 0 : MEMORY_ALIGNMENT);

    int length = layout_width * layout_height / MEMORY_ALIGNMENT;

    int* buf = new int[length];

    int ret = read_from_address(getBaseVoltageAddress(), buf, length);

    if(!ret)
    {
        printf("cannot load base voltage data from device\n");
        delete[] buf;
        return ret;
    }

    for(int i = 0; i < length; i++)
    {
        buf[i] = invert_endian(buf[i]);
    }

    for(int i = 0; i <  layout_width; i++)
    {
        for(int j = 0; j < layout_height; j++)
        {

            if( j >= buf_height)
            {
                continue;
            }

            int index = (i * layout_height + j) / MEMORY_ALIGNMENT;
            int shift = (i * layout_height + j) % MEMORY_ALIGNMENT;

            dataBuf[i * buf_height + j] = transferRawDataToVoltage((buf[index] >> (3-shift) * 8) & 0xff);

        }
    }


    delete[] buf;
    return ret;
}

bool
AegisSiSTouchAdapter::loadRawVoltage(float *dataBuf, int buf_width, int buf_height)
{
    int layout_width = buf_width;
    int layout_height = buf_height / MEMORY_ALIGNMENT * MEMORY_ALIGNMENT + (buf_height % MEMORY_ALIGNMENT == 0 ? 0 : MEMORY_ALIGNMENT);

    int length = layout_width * layout_height / MEMORY_ALIGNMENT;

    int* buf = new int[length];

    int ret = read_from_address(getRawVoltageAddress(), buf, length);

    if(!ret)
    {
        printf("cannot load raw voltage data from device\n");
        delete[] buf;
        return ret;
    }

    for(int i = 0; i < length; i++)
    {
        buf[i] = invert_endian(buf[i]);
    }

    for(int i = 0; i <  layout_width; i++)
    {
        for(int j = 0; j < layout_height; j++)
        {

            if( j >= buf_height)
            {
                continue;
            }

            int index = (i * layout_height + j) / MEMORY_ALIGNMENT;
            int shift = (i * layout_height + j) % MEMORY_ALIGNMENT;

            dataBuf[i * buf_height + j] = transferRawDataToVoltage((buf[index] >> (3-shift) * 8) & 0xff);
        }
    }


    delete[] buf;
    return ret;
}

bool
AegisSiSTouchAdapter::loadCycle(float *dataBuf, int buf_width, int buf_height)
{
    int layout_width = buf_width;
    int layout_height = buf_height / MEMORY_ALIGNMENT * MEMORY_ALIGNMENT + (buf_height % MEMORY_ALIGNMENT == 0 ? 0 : MEMORY_ALIGNMENT);

    int length = layout_width * layout_height / MEMORY_ALIGNMENT;

    int* buf = new int[length];

    int ret = read_from_address(getCycleAddress(), buf, length);

    if(!ret)
    {
        printf("cannot load cycle data from device\n");
        delete[] buf;
        return ret;
    }

    for(int i = 0; i < length; i++)
    {
        buf[i] = invert_endian(buf[i]);
    }

    for(int i = 0; i <  layout_width; i++)
    {
        for(int j = 0; j < layout_height; j++)
        {

            if( j >= buf_height)
            {
                continue;
            }

            int index = (i * layout_height + j) / MEMORY_ALIGNMENT;
            int shift = (i * layout_height + j) % MEMORY_ALIGNMENT;

            dataBuf[i * buf_height + j] = transferRawDataToCycle((buf[index] >> (3-shift) * 8) & 0xff);
        }
    }


    delete[] buf;
    return ret;
}

bool
AegisSiSTouchAdapter::loadLoop(float *dataBuf, int buf_width, int buf_height)
{
    int layout_width = buf_width;
    int layout_height = buf_height / MEMORY_ALIGNMENT * MEMORY_ALIGNMENT + (buf_height % MEMORY_ALIGNMENT == 0 ? 0 : MEMORY_ALIGNMENT);

    int length = layout_width * layout_height / MEMORY_ALIGNMENT;

    int* buf = new int[length];

    int ret = read_from_address(getLoopAddress(), buf, length);

    if(!ret)
    {
        printf("cannot load loop data from device\n");
        delete[] buf;
        return ret;
    }

    for(int i = 0; i < length; i++)
    {
        buf[i] = invert_endian(buf[i]);
    }

    for(int i = 0; i <  layout_width; i++)
    {
        for(int j = 0; j < layout_height; j++)
        {

            if( j >= buf_height)
            {
                continue;
            }

            int index = (i * layout_height + j) / MEMORY_ALIGNMENT;
            int shift = (i * layout_height + j) % MEMORY_ALIGNMENT;

            dataBuf[i * buf_height + j] = ((buf[index] >> (3-shift) * 8) & 0xff);
        }
    }

    delete[] buf;
    return ret;
}

bool
AegisSiSTouchAdapter::loadDiff(float *dataBuf, int buf_width, int buf_height)
{
    int layout_width = buf_width;
    int layout_height = buf_height / MEMORY_ALIGNMENT * MEMORY_ALIGNMENT + (buf_height % MEMORY_ALIGNMENT == 0 ? 0 : MEMORY_ALIGNMENT);

    int length = layout_width * layout_height / MEMORY_ALIGNMENT;

    int* buf = new int[length];

    int ret = read_from_address(getDiffVoltageAddress(), buf, length);

    if(!ret)
    {
        printf("cannot load diff voltage data from device\n");
        delete[] buf;
        return ret;
    }

    for(int i = 0; i < length; i++)
    {
        buf[i] = invert_endian(buf[i]);
    }

    for(int i = 0; i <  layout_width; i++)
    {
        for(int j = 0; j < layout_height; j++)
        {

            if( j >= buf_height)
            {
                continue;
            }

            int index = (i * layout_height + j) / MEMORY_ALIGNMENT;
            int shift = (i * layout_height + j) % MEMORY_ALIGNMENT;

            dataBuf[i * buf_height + j] = transferRawDataToVoltage((buf[index] >> (3-shift) * 8) & 0xff);
        }
    }


    delete[] buf;
    return ret;
}


bool
AegisSiSTouchAdapter::getDeviceInfo(OpenShortData *data)
{
    bool result = false;
    float targetVoltage = 0;
    int width = 0;
    int height = 0;

    result = loadWidthHeight(&width, &height);


    if(!result)
    {
        return result;
    }
    else
    {
        data->setWH(width, height);
    }

    result = loadTargetVoltage(&targetVoltage);

    if(!result)
    {
        return result;
    }
    else
    {
        data->setTargetVoltage(targetVoltage);
    }

    return result;
}

bool
AegisSiSTouchAdapter::getDeviceVoltageData(OpenShortData *data)
{
    bool result = false;
    float *dataBuf;
    int width = data->getWidth();
    int height = data->getHeight();

    dataBuf = new float[width * height];

    if(data->getReadData())
        result = loadBaseVoltage(dataBuf, width, height);
	else
		result = loadRawVoltage(dataBuf, width, height);

    if(result)
    {
        for(int i = 0; i < width; i++)
        {
            for(int j = 0; j < height; j++)
            {
                if(data->getReadData())
	                data->setBaseVoltage(i, j, dataBuf[i * height + j]);
				else
				    data->setRawVoltage(i, j, dataBuf[i * height + j]);
            }
        }
    }
    else
    {
        delete[] dataBuf;
        return result;
    }

    result = loadCycle(dataBuf, width, height);

    if(result)
    {
        for(int i = 0; i < width; i++)
        {
            for(int j = 0; j < height; j++)
            {
                data->setCycle(i, j, dataBuf[i * height + j]);
            }
        }
    }
    else
    {
        delete[] dataBuf;
        return result;
    }


    result = loadLoop(dataBuf, width, height);

    if(result)
    {
        for(int i = 0; i < width; i++)
        {
            for(int j = 0; j < height; j++)
            {
                data->setLoop(i, j, dataBuf[i * height + j]);
            }
        }
    }
    else
    {
        delete[] dataBuf;
        return result;
    }

    delete[] dataBuf;
    return result;
}

int
AegisSiSTouchAdapter::testPointOpenShort(OpenShortConfig* config, OpenShortData *deviceData, OpenShortResult *result)
{
    int width = deviceData->getWidth();
    int height = deviceData->getHeight();

    float diffVoltage = 0;

    int nNG = 0;
	int nSecondNG = 0;

    if(width + height == 0)
    {
        return ERROR_NODATA;
    }

    for ( int i = 0; i < width; i++ )
    {
        for ( int j = 0; j < height; j++ )
        {
            result->setPointResult(i, j, POINT_PASS);
        }
    }


    for ( int i = 0; i < width; i++ )
    {
        for ( int j = 0; j < height; j++ )
        {
            if(deviceData->getReadData())
                diffVoltage = getDiffPercentage(deviceData->getBaseVoltage(i, j), deviceData->getTargetVoltage());
			else
				diffVoltage = getDiffPercentage(deviceData->getRawVoltage(i, j), deviceData->getTargetVoltage());


            if(diffVoltage > 0)
            {
                if( diffVoltage > config->pointConfig.targetMutl )
                {
                    result->appendPointResult(i, j, POINT_OVEROPEN_VOLTAGE_ON_TARGET);
                    nNG ++;

					if ( diffVoltage < config->pointConfig.targetMutl + config->secondConfig.targetPosVol )
						nSecondNG ++;
                }
                else
                {
                    result->appendPointResult(i, j, POINT_PASS);
                }

            }
            else if(diffVoltage < 0)
            {
                if( diffVoltage < config->pointConfig.negTargetMutl )
                {
                    result->appendPointResult(i, j, POINT_OPEN_VOLTAGE_ON_TARGET);
                    nNG ++;

					if ( diffVoltage > config->pointConfig.negTargetMutl - config->secondConfig.targetNegVol )
						nSecondNG ++;
                }
                else
                {
                    result->appendPointResult(i, j, POINT_PASS);
                }
            }
            else if(diffVoltage == 0)
            {
                result->appendPointResult(i, j, POINT_PASS);
            }
        }
    }

    result->setPointNG(nNG);
	result->setSecondNG(nSecondNG);

    return SUCCESS;
}

int
AegisSiSTouchAdapter::testPointOpenShort(OpenShortConfig* config, OpenShortData *deviceData, OpenShortData *sampleData, OpenShortResult *result)
{
    int width = deviceData->getWidth();
    int height = deviceData->getHeight();

    int nNG = 0;
	int nSecondNG = 0;

    float diffVoltage = 0;
    float diffVoltageWithSample = 0;
    float caliTime = 0;
    float sampleTime = 0;
    float diffTime = 0;


    if(deviceData->getWidth() != sampleData->getWidth() || deviceData->getHeight() != sampleData->getHeight())
    {
        return ERROR_NONCOOPERATE_DATA;
    }

    if(width + height == 0)
    {
        return ERROR_NODATA;
    }

    for ( int i = 0; i < width; i++ )
    {
        for ( int j = 0; j < height; j++ )
        {
            result->setPointResult(i, j, POINT_PASS);
        }
    }

    for ( int i = 0; i < width; i++ )
    {
        for ( int j = 0; j < height; j++ )
        {
            int originalNg = nNG;
			int originalSecondNg = nSecondNG;

            if(deviceData->getReadData())
            {
            	diffVoltage = getDiffPercentage(deviceData->getBaseVoltage(i, j), deviceData->getTargetVoltage());
	            diffVoltageWithSample = getDiffPercentage(deviceData->getBaseVoltage(i, j), sampleData->getBaseVoltage(i, j));
            }
			else
			{
				diffVoltage = getDiffPercentage(deviceData->getRawVoltage(i, j), deviceData->getTargetVoltage());
	            diffVoltageWithSample = getDiffPercentage(deviceData->getRawVoltage(i, j), sampleData->getBaseVoltage(i, j));
			}

            caliTime = deviceData->getCycle(i, j) * deviceData->getLoop(i, j);
            sampleTime = sampleData->getCycle(i, j) * sampleData->getLoop(i, j);

            diffTime = getDiffPercentage(caliTime, sampleTime);

            if(diffVoltageWithSample > 0)
            {
                if( diffVoltageWithSample > config->pointConfig.mutl )
                {
                    result->appendPointResult(i, j, POINT_OVEROPEN_VOLTAGE_ON_SAMPLE);
                    nNG ++;

					if ( diffVoltageWithSample < config->pointConfig.mutl + config->secondConfig.goldenPosVol )
						nSecondNG ++;
                }
                else
                {
                    result->appendPointResult(i, j, POINT_PASS);
                }
            }
            else if(diffVoltageWithSample < 0)
            {
                if( diffVoltageWithSample < config->pointConfig.negMutl )
                {
                    result->appendPointResult(i, j, POINT_OPEN_VOLTAGE_ON_SAMPLE);
                    nNG ++;

					if ( diffVoltageWithSample > config->pointConfig.negMutl - config->secondConfig.goldenNegVol )
						nSecondNG ++;
                }
                else
                {
                    result->appendPointResult(i, j, POINT_PASS);
                }
            }
            else if(diffVoltageWithSample == 0)
            {
                result->appendPointResult(i, j, POINT_PASS);
            }

            if(diffTime > 0)
            {
                if( diffTime > config->pointConfig.calib )
                {
                    result->appendPointResult(i, j, POINT_OVEROPEN_TIME_ON_SAMPLE);
                    nNG ++;

					if ( diffTime < config->pointConfig.calib + config->secondConfig.goldenPosCalib )
						nSecondNG ++;
                }
                else
                {
                    result->appendPointResult(i, j, POINT_PASS);
                }
            }
            else if(diffTime < 0)
            {
                if( diffTime < config->pointConfig.negCalib )
                {
                    result->appendPointResult(i, j, POINT_OPEN_TIME_ON_SAMPLE);
                    nNG ++;

					if ( diffTime > config->pointConfig.negCalib - config->secondConfig.goldenNegCalib)
						nSecondNG ++;
                }
                else
                {
                    result->appendPointResult(i, j, POINT_PASS);
                }
            }

            else if(diffTime == 0)
            {
                result->appendPointResult(i, j, POINT_PASS);
            }

            if(diffVoltage > 0)
            {
                if( diffVoltage > config->pointConfig.targetMutl)
                {
                    result->appendPointResult(i, j, POINT_OVEROPEN_VOLTAGE_ON_TARGET);
                    nNG ++;

					if ( diffVoltage < config->pointConfig.targetMutl + config->secondConfig.targetPosVol )
						nSecondNG ++;
                }
                else
                {
                    result->appendPointResult(i, j, POINT_PASS);
                }

            }
            else if(diffVoltage < 0)
            {
                if( diffVoltage < config->pointConfig.negTargetMutl )
                {
                    result->appendPointResult(i, j, POINT_OPEN_VOLTAGE_ON_TARGET );
                    nNG ++;

					if ( diffVoltage > config->pointConfig.negTargetMutl - config->secondConfig.targetNegVol )
						nSecondNG ++;
                }
                else
                {
                    result->appendPointResult(i, j, POINT_PASS);
                }
            }
            else if(diffVoltage == 0)
            {
                result->appendPointResult(i, j, POINT_PASS);
            }

			if ( nNG > originalNg )
                   nNG = originalNg + 1;

            if ( nSecondNG > originalSecondNg )
                nSecondNG = originalSecondNg + 1;
        }
    }

    result->setPointNG(nNG);
	result->setSecondNG(nSecondNG);

    return SUCCESS;
}

int
AegisSiSTouchAdapter::testLineOpenShort(OpenShortConfig* config, OpenShortData *deviceData, OpenShortResult *result)
{

    int width = deviceData->getWidth();
    int height = deviceData->getHeight();

    // float caliTime = 0;
    // float sampleTime = 0;
    float timeDiff = 0;
    float volDiff = 0;

    float tempLoop = 0;
    float tempCycle = 0;
    float tempTime = 0;
    // float tempSampleTime = 0;

    int shortNodeCount = 0;
    int nNG = 0;

    if((width + height) == 0)
    {
        return ERROR_NODATA;
    }

    int minLoop = config->lineConfig.minLoop;
    int maxLoop = config->lineConfig.maxLoop;

    std::vector<float> leftTopList, leftBottomList, rightTopList, rightBottomList, totalList;


    for ( int i = 0; i < width; i++ )
    {
        for ( int j = 0; j < height; j++ )
        {

            tempLoop = deviceData->getLoop(i, j);
            tempCycle = deviceData->getCycle(i, j);

            tempTime = tempLoop * tempCycle;
            if( tempLoop > minLoop && tempLoop < maxLoop )
            {
                totalList.push_back( tempTime );

                if ( i < (width / 2) && j < (height / 2) )
                {
                    leftTopList.push_back( tempTime );
                }
                else if ( i >= (width / 2) && j < (height / 2) )
                {
                    rightTopList.push_back( tempTime );
                }
                else if ( i < (width / 2) && j >= (height / 2) )
                {
                    leftBottomList.push_back( tempTime );
                }
                else
                {
                    rightBottomList.push_back( tempTime );
                }
            }

        }
    }


    float leftTopBaseTime, leftBottomBaseTime, rightTopBaseTime, rightBottomBaseTime, totalBaseTime;

    if ( totalList.size() == 0 )
    {
        for( int i = 0; i < width; i++ )
        {
            result->setXLineResult(i, LINE_SHORT_LOOP);
            nNG++;
        }

        for( int j = 0; j < height; j++ )
        {
            result->setYLineResult(j, LINE_SHORT_LOOP);
            nNG++;
        }
    }
    else
    {
        std::sort( totalList.begin(), totalList.end() );
        totalBaseTime = totalList[ totalList.size() / 2 ];

        if ( leftTopList.size() == 0 )
        {
            leftTopBaseTime = totalBaseTime;
        }
        else
        {
            std::sort( leftTopList.begin(), leftTopList.end() );
            leftTopBaseTime = leftTopList[ leftTopList.size() / 2 ];
        }

        if ( rightTopList.size() == 0 )
        {
            rightTopBaseTime = totalBaseTime;
        }
        else
        {
            std::sort( rightTopList.begin(), rightTopList.end() );
            rightTopBaseTime = rightTopList[ rightTopList.size() / 2 ];
        }

        if ( leftBottomList.size() == 0 )
        {
            leftBottomBaseTime = totalBaseTime;
        }
        else
        {
            std::sort( leftBottomList.begin(), leftBottomList.end() );
            leftBottomBaseTime = leftBottomList[ leftBottomList.size() / 2 ];
        }

        if ( rightBottomList.size() == 0 )
        {
            rightBottomBaseTime = totalBaseTime;
        }
        else
        {
            std::sort( rightBottomList.begin(), rightBottomList.end() );
            rightBottomBaseTime = rightBottomList[ rightBottomList.size() / 2 ];
        }


        for ( int i = 0; i < width; i++ )
        {
            shortNodeCount = 0;
            for ( int j = 0; j < height; j++ )
            {

                tempLoop = deviceData->getLoop(i, j);
                tempCycle = deviceData->getCycle(i, j);

                tempTime = tempLoop * tempCycle;

                if ( i < (width / 2) && j < (height / 2) )
                    timeDiff = getAbsDiffPercentage(tempTime , leftTopBaseTime);
                else if ( i >= (width / 2) && j < (height / 2) )
                    timeDiff = getAbsDiffPercentage(tempTime , rightTopBaseTime);
                else if ( i < (width / 2) && j >= (height / 2) )
                    timeDiff = getAbsDiffPercentage(tempTime , leftBottomBaseTime);
                else
                    timeDiff = getAbsDiffPercentage(tempTime , rightBottomBaseTime);


                if ( deviceData->getReadData() )
                	volDiff = getAbsDiffPercentage(deviceData->getBaseVoltage(i, j) , deviceData->getTargetVoltage());
				else
					volDiff = getAbsDiffPercentage(deviceData->getRawVoltage(i, j) , deviceData->getTargetVoltage());

                if ( timeDiff > config->lineConfig.calibTime || volDiff > config->lineConfig.voltage )
                {
                    result->setPointResult(i, j, POINT_SHORT);
                }
                else
                {
                    result->setPointResult(i, j, POINT_PASS);
                }


            }
        }

        for( int i = 0; i < width; i++ )
        {

            shortNodeCount = 0;
            for( int j = 0; j < height; j++ )
            {
                if(result->getPointResult(i, j) == POINT_SHORT)
                {
                    shortNodeCount++;
                }
            }

            if(shortNodeCount >= (config->lineConfig.outOfSpecNode / 100.0f * height))
            {
                result->setXLineResult(i, LINE_SHORT);
                nNG++;
            }
            else
            {
                result->setXLineResult(i, LINE_PASS);
            }
        }


        for( int j = 0; j < height; j++ )
        {
            shortNodeCount = 0;
            for( int i = 0; i < width; i++ )
            {
                if(result->getPointResult(i, j)  == POINT_SHORT)
                {
                    shortNodeCount++;
                }
            }

            if(shortNodeCount >= (config->lineConfig.outOfSpecNode /100.0f * width))
            {
                result->setYLineResult(j, LINE_SHORT);
                nNG++;
            }
            else
            {
                result->setYLineResult(j, LINE_PASS);
            }
        }

    }

    result->setLineNG(nNG);



    return SUCCESS;
}

int
AegisSiSTouchAdapter::testLineOpenShort(OpenShortConfig* config, OpenShortData *deviceData, OpenShortData *sampleData, OpenShortResult *result)
{
    int width = deviceData->getWidth();
    int height = deviceData->getHeight();

    float caliTime = 0;
    float sampleTime = 0;
    float timeDiff = 0;
    float volDiff = 0;

    int  shortNodeCount = 0;
    int nNG = 0;

    if(deviceData->getWidth() != sampleData->getWidth() || deviceData->getHeight() != sampleData->getHeight())
    {
        return ERROR_NONCOOPERATE_DATA;
    }





    if((width + height) == 0)
    {
        return ERROR_NODATA;
    }

    for ( int i = 0; i < width; i++ )
    {
        for ( int j = 0; j < height; j++ )
        {
            caliTime = deviceData->getCycle(i, j) * deviceData->getLoop(i, j);
            sampleTime = sampleData->getCycle(i, j) * sampleData->getLoop(i, j);

            timeDiff = getAbsDiffPercentage(caliTime , sampleTime);

			if ( deviceData->getReadData() )
	            volDiff = getAbsDiffPercentage(deviceData->getBaseVoltage(i, j) , sampleData->getBaseVoltage(i, j));
			else
				volDiff = getAbsDiffPercentage(deviceData->getRawVoltage(i, j) , sampleData->getBaseVoltage(i, j));

            if ( timeDiff > config->lineConfig.calibTime || volDiff > config->lineConfig.voltage )
            {
                result->setPointResult(i, j, POINT_SHORT);
            }
            else
            {
                result->setPointResult(i, j, POINT_PASS);
            }

        }
    }

    for( int i = 0; i < width; i++ )
    {
        shortNodeCount = 0;
        for( int j = 0; j < height; j++ )
        {
            if(result->getPointResult(i, j)  == POINT_SHORT)
            {
                shortNodeCount++;
            }
        }

        if(shortNodeCount >= (config->lineConfig.outOfSpecNode / 100.0f * height))
        {
            result->setXLineResult(i, LINE_SHORT);
            nNG++;
        }
        else
        {
            result->setXLineResult(i, LINE_PASS);
        }
    }


    for( int j = 0; j < height; j++ )
    {
        shortNodeCount = 0;
        for( int i = 0; i < width; i++ )
        {
            if(result->getPointResult(i, j)  == POINT_SHORT)
            {
                shortNodeCount++;
            }
        }

        if(shortNodeCount >= (config->lineConfig.outOfSpecNode / 100.0f  * width))
        {
            result->setYLineResult(j, LINE_SHORT);
            nNG++;
        }
        else
        {
            result->setYLineResult(j, LINE_PASS);
        }
    }

    result->setLineNG(nNG);

    return SUCCESS;
}

// MutualShortExtraCheck, begin
// multi-chip: Tx -> RX
int
AegisSiSTouchAdapter::calculateCycleTxDiff(OpenShortConfig* config, OpenShortData *deviceData, OpenShortResult *result)
{
    int width = deviceData->getWidth();
    int height = deviceData->getHeight();

    if(width + height == 0)
    {
        return ERROR_NODATA;
    }

    for ( int i = 0; i < width; i++ )
    {
        for ( int j = 0; j < height; j++ )
        {
            if(j == 0) 
            {
                result->setCycleTxDiff(i, j, 0);
            } else 
            {
                float thisCaliTime = deviceData->getCycle(i, j) * deviceData->getLoop(i, j);
                float preCaliTime = deviceData->getCycle(i, j-1) * deviceData->getLoop(i, j-1);
                float cycleTxDiff = (thisCaliTime - preCaliTime) / thisCaliTime * 100;
                result->setCycleTxDiff(i, j, cycleTxDiff);
            }
        }
    }

    return SUCCESS;
}

// multi-chip: RX -> Tx
int
AegisSiSTouchAdapter::calculateCycleRxDiff(OpenShortConfig* config, OpenShortData *deviceData, OpenShortResult *result)
{
    int width = deviceData->getWidth();
    int height = deviceData->getHeight();

    if(width + height == 0)
    {
        return ERROR_NODATA;
    }

    for ( int i = 0; i < width; i++ )
    {
        for ( int j = 0; j < height; j++ )
        {
            if(i == 0) 
            {
                result->setCycleRxDiff(i, j, 0);
            } else 
            {
                float thisCaliTime = deviceData->getCycle(i, j) * deviceData->getLoop(i, j);
                float preCaliTime = deviceData->getCycle(i-1, j) * deviceData->getLoop(i-1, j);
                float cycleRxDiff = (thisCaliTime - preCaliTime) / thisCaliTime * 100;
                result->setCycleRxDiff(i, j, cycleRxDiff);
            }
        }
    }

    return SUCCESS;
}

// multi-chip: Tx -> RX
int
AegisSiSTouchAdapter::JudgeTxShortResult(OpenShortConfig* config, OpenShortData *deviceData, OpenShortResult *result)
{
    int width = deviceData->getWidth();
    int height = deviceData->getHeight();

    if(width + height == 0)
    {
        return ERROR_NODATA;
    }

    int shortNodeCnt;
    int shortNodeRepairCnt;
    bool shortCntOverThreshold;

    int nNG = result->getLineNG();

    for ( int j = 0; j < height; j++ ) // multi-chip: height -> RX
    { 
        shortNodeCnt = 0;
        shortNodeRepairCnt = 0;
        shortCntOverThreshold = 0;
        
        for(int i = 0; i < width; i++)
        {
            if(result->getCycleTxDiff(i, j) > config->lineExtraConfig.TXCycleDiffHighPercent)
            {
                result->setTXPointShortResult(i, j, POINT_SHORT);
                shortNodeCnt++;
            } else if(result->getCycleTxDiff(i, j) <= -1 * config->lineExtraConfig.TXCycleDiffLowPercent
                      && ((j - 1) >= 0))
            {
                result->setTXPointShortResult(i, j-1, POINT_SHORT);
                shortNodeRepairCnt++;
            }
        }

        shortCntOverThreshold = (shortNodeCnt >= (float) width * config->lineExtraConfig.cycleOutOfSpecNodePercent/100);
        if(shortCntOverThreshold)
        {
            if(result->getYLineResult(j) != LINE_SHORT &&
               result->getYLineResult(j) != LINE_SHORT_LOOP)
            {
                result->setYLineResult(j, LINE_EXTREME_DIF);
                nNG++;
            }
        }

        shortCntOverThreshold = (shortNodeRepairCnt >= (float) width * config->lineExtraConfig.cycleOutOfSpecNodePercent/100);
        if(shortCntOverThreshold)
        {
            if(result->getYLineResult(j-1) != LINE_SHORT
               && result->getYLineResult(j-1) != LINE_SHORT_LOOP
               && result->getYLineResult(j-1) != LINE_EXTREME_DIF
               && ((j - 1) >= 0) 
               )
            {
                result->setYLineResult(j-1, LINE_EXTREME_DIF);
                nNG++;
            }
        }
    }

    // only mark 2 line short, repeal unusual loading
    if((int)config->lineExtraConfig.detectShortOnly == 1)
    {
        for( int j = 0; j < height; j++ )
        {
            if(result->getYLineResult(j) == LINE_EXTREME_DIF)
            {
                if(   (result->getYLineResult(j-1) == LINE_EXTREME_DIF && ((j - 1) >= 0))
                   || (result->getYLineResult(j+1) == LINE_EXTREME_DIF && ((j + 1) < height)))
                {
                    continue;
                }
                else
                {
                    result->setYLineResult(j, LINE_PASS);
                    nNG--;
                }
            }
        }
    }

    result->setLineNG(nNG);

    return SUCCESS;
}

// multi-chip: Rx -> TX
int
AegisSiSTouchAdapter::JudgeRxShortResult(OpenShortConfig* config, OpenShortData *deviceData, OpenShortResult *result)
{
    int width = deviceData->getWidth();
    int height = deviceData->getHeight();

    if(width + height == 0)
    {
        return ERROR_NODATA;
    }

    int shortNodeCnt;
    int shortNodeRepairCnt;
    bool shortCntOverThreshold;

    int nNG = result->getLineNG();

    for ( int i = 0; i < width; i++ ) // multi-chip: width -> TX
    {
        shortNodeCnt = 0;
        shortNodeRepairCnt = 0;
        shortCntOverThreshold = 0;
        
        for(int j = 0; j < height; j++)
        {
            if(result->getCycleRxDiff(i, j) > config->lineExtraConfig.RXCycleDiffHighPercent)
            {
                result->setRXPointShortResult(i, j, POINT_SHORT);
                shortNodeCnt++;
            } else if(result->getCycleRxDiff(i, j) <= -1 * config->lineExtraConfig.RXCycleDiffLowPercent
                      && ((i - 1) >= 0))
            {
                result->setRXPointShortResult(i-1, j, POINT_SHORT);
                shortNodeRepairCnt++;
            }
        }

        shortCntOverThreshold = (shortNodeCnt >= (float) height * config->lineExtraConfig.cycleOutOfSpecNodePercent/100);
        if(shortCntOverThreshold)
        {
            if(result->getXLineResult(i) != LINE_SHORT &&
               result->getXLineResult(i) != LINE_SHORT_LOOP)
            {
                result->setXLineResult(i, LINE_EXTREME_DIF);
                nNG++;
            }
        }

        shortCntOverThreshold = (shortNodeRepairCnt >= (float) height * config->lineExtraConfig.cycleOutOfSpecNodePercent/100);
        if(shortCntOverThreshold)
        {
            if(result->getXLineResult(i-1) != LINE_SHORT
               && result->getXLineResult(i-1) != LINE_SHORT_LOOP
               && result->getXLineResult(i-1) != LINE_EXTREME_DIF
               && ((i - 1) >= 0) 
               )
            {
                result->setXLineResult(i-1, LINE_EXTREME_DIF);
                nNG++;
            }
        }
    }

    // only mark 2 line short, repeal unusual loading
    if((int)config->lineExtraConfig.detectShortOnly == 1)
    {
        for( int i = 0; i < width; i++ )
        {
            if(result->getXLineResult(i) == LINE_EXTREME_DIF)
            {
                if(   (result->getXLineResult(i-1) == LINE_EXTREME_DIF && ((i - 1) >= 0))
                   || (result->getXLineResult(i+1) == LINE_EXTREME_DIF && ((i + 1) < width)))
                {
                    continue;
                }
                else
                {
                    result->setXLineResult(i, LINE_PASS);
                    nNG--;
                }
            }
        }
    }

    result->setLineNG(nNG);

    return SUCCESS;
}

int
AegisSiSTouchAdapter::checkMutualShortExtra(OpenShortConfig* config, OpenShortData* deviceData, OpenShortResult *result)
{
    int ret = 0;

    ret = calculateCycleTxDiff(config, deviceData, result);
    if(ret != SUCCESS)
    {
        return ret;
    }

    ret = calculateCycleRxDiff(config, deviceData, result);
    if(ret != SUCCESS)
    {
        return ret;
    }

    if((int)config->lineExtraConfig.enableTXExtraCheck == 1)
    {
        ret = JudgeTxShortResult(config, deviceData, result);
        if(ret != SUCCESS)
        {
            return ret;
        }
    }

    if((int)config->lineExtraConfig.enableRXExtraCheck == 1)
    {
        ret = JudgeRxShortResult(config, deviceData, result);
        if(ret != SUCCESS)
        {
            return ret;
        }
    }

    return SUCCESS;
}
// MutualShortExtraCheck, end

// MutualNearFarCheck, begin
int
AegisSiSTouchAdapter::checkMutualNearFar(OpenShortConfig* config, OpenShortData* deviceData, OpenShortResult *result)
{
    result->lostSide = LOST_NONE;
    
    if(fabs(config->lineNearFarConfig.cycleDifferencePercentTx / 100.0) < 1E-3)
    {
        // (config->lineNearFarConfig.cycleDifferencePercentTx / 100.0) = 1
    } else
    {
        if((config->lineNearFarConfig.cycleDifferencePercentTx / 100.0) < 1)
        {
            checkMutualNearFarTx(config, deviceData, result);
        }
    }

    if(fabs(config->lineNearFarConfig.cycleDifferencePercentRx / 100.0) < 1E-3)
    {
        // (config->lineNearFarConfig.cycleDifferencePercentRx / 100.0) = 1
    } else
    {
        if((config->lineNearFarConfig.cycleDifferencePercentRx / 100.0) < 1)
        {
            checkMutualNearFarRx(config, deviceData, result);
        }
    }

    return SUCCESS;
}

void
AegisSiSTouchAdapter::checkMutualNearFarTx(OpenShortConfig* config, OpenShortData* deviceData, OpenShortResult *result)
{
    int width = deviceData->getWidth();
    int height = deviceData->getHeight();

    int left_bigger_than_right = 0;
    int right_bigger_than_left = 0;

    int normal_count = 0;
    int abnormal_count = 0;

    double val =0.0;

    for(int y = 0; y < height; y++)
    {
        int pos = y;

        int x = 0;
        int condition_count = 0;

        // TX: avg. of left n pointer
        double left = 0.0;
        x = condition_count = 0;
        while(condition_count < config->lineNearFarConfig.cycleAvgCountNumberTx)
        {
            left += deviceData->getCycle(x, y) * deviceData->getLoop(x, y);
            condition_count++;
            x++;
        }
        left /= config->lineNearFarConfig.cycleAvgCountNumberTx;

        // TX: avg. of right n pointer
        double right = 0.0;
        x = width - 1;
        condition_count = 0;
        while(condition_count < config->lineNearFarConfig.cycleAvgCountNumberTx)
        {
            right += deviceData->getCycle(x, y) * deviceData->getLoop(x, y);
            condition_count++;
            x--;
        }
        right /= config->lineNearFarConfig.cycleAvgCountNumberTx;

        val = (left - right) / std::max(left, right);
        result->setYLineNearFarValue(pos, val);

        if(val > 0)
        {
            left_bigger_than_right++;
        } else 
        {
            right_bigger_than_left++;
        }

        if(fabs(val) > (config->lineNearFarConfig.cycleDifferencePercentTx / 100.0))
        {
            result->setYLineNearFarResult(pos, LINE_NEARFAR_SHORT);
            abnormal_count++;
        } else
        {
            normal_count++;
        }
    }

    if(abnormal_count > config->lineNearFarConfig.cycleOutOfChannels)
    {
        int nNG = result->getLineNG();
        for(int j = 0; j < height; j++)
        {
            if(result->getYLineResult(j) != LINE_SHORT &&
               result->getYLineResult(j) != LINE_SHORT_LOOP &&
               result->getYLineResult(j) != LINE_EXTREME_DIF)
            {
                if(result->getYLineNearFarResult(j) == LINE_NEARFAR_SHORT)
                {
                    result->setYLineResult(j, LINE_NEARFAR_SHORT);
                    nNG++;
                }
            }
        }

        if(config->m_chipNum == 1) // only for TP
        {
        	result->setLineNG(nNG);
        }

        if(left_bigger_than_right > right_bigger_than_left)
        {
            result->lostSide |= LOST_LEFT;
        } else 
        {
            result->lostSide |= LOST_RIGHT;
        }
    }
}

void
AegisSiSTouchAdapter::checkMutualNearFarRx(OpenShortConfig* config, OpenShortData* deviceData, OpenShortResult *result)
{
    int width = deviceData->getWidth();
    int height = deviceData->getHeight();

    int top_bigger_than_down = 0;
    int down_bigger_than_top = 0;

    int normal_count = 0;
    int abnormal_count = 0;

    double val =0.0;

    for(int x = 0; x < width; x++)
    {
        int pos = x;

        int y = 0;
        int condition_count = 0;

        // RX: avg. of top n pointer
        double top = 0.0;
        y = condition_count = 0;
        while(condition_count < config->lineNearFarConfig.cycleAvgCountNumberRx)
        {
            top += deviceData->getCycle(x, y) * deviceData->getLoop(x, y);
            condition_count++;
            y++;
        }
        top /= config->lineNearFarConfig.cycleAvgCountNumberRx;

        // RX: avg. of down n pointer
        double down = 0.0;
        y = height - 1;
        condition_count = 0;
        while(condition_count < config->lineNearFarConfig.cycleAvgCountNumberRx)
        {
            down += deviceData->getCycle(x, y) * deviceData->getLoop(x, y);
            condition_count++;
            y--;
        }
        down /= config->lineNearFarConfig.cycleAvgCountNumberRx;

        val = (top - down) / std::max(top, down);
        result->setXLineNearFarValue(pos, val);

        if(val > 0)
        {
            top_bigger_than_down++;
        } else 
        {
            down_bigger_than_top++;
        }

        if(fabs(val) > (config->lineNearFarConfig.cycleDifferencePercentRx / 100.0))
        {
            result->setXLineNearFarResult(pos, LINE_NEARFAR_SHORT);
            abnormal_count++;
        } else
        {
            normal_count++;
        }
    }

    if(abnormal_count > config->lineNearFarConfig.cycleOutOfChannels)
    {
        int nNG = result->getLineNG();
        for(int i = 0; i < width; i++)
        {
            if(result->getXLineResult(i) != LINE_SHORT &&
               result->getXLineResult(i) != LINE_SHORT_LOOP &&
               result->getXLineResult(i) != LINE_EXTREME_DIF)
            {
                if(result->getXLineNearFarResult(i) == LINE_NEARFAR_SHORT)
                {
                    result->setXLineResult(i, LINE_NEARFAR_SHORT);
                    nNG++;
                }

            }
        }

        if(config->m_chipNum == 1) // only for TP
        {
        	result->setLineNG(nNG);
        }

        if(top_bigger_than_down > down_bigger_than_top)
        {
            result->lostSide |= LOST_TOP;
        } else 
        {
            result->lostSide |= LOST_DOWN;
        }
    }
}

// MutualNearFarCheck, end

float
AegisSiSTouchAdapter::getDiffPercentage(float numerator, float denominator)
{
    if(denominator == 0)
    {
        if(numerator == 0)
        {
            return 0;
        }
        else
        {
            return 100;
        }
    }
    else
    {
        return 100 * (numerator - denominator) / denominator;
    }
}

float
AegisSiSTouchAdapter::getAbsDiffPercentage(float numerator, float denominator)
{
    if(denominator == 0)
    {
        if(numerator == 0)
        {
            return 0;
        }
        else
        {
            return 100;
        }
    }
    else
    {
        return 100 * fabs(numerator - denominator) / denominator;
    }
}

int
AegisSiSTouchAdapter::parameterCheck(bool& update_bootloader, bool& update_bootloader_auto, bool &isNewBootloader, bool& reserve_RODATA, bool &update_parameter, bool force_update, int *buf, int file_Length)
{

    int firmwareInfo[FIRMWARE_INFO_LENGTH] = {0};

    int bootInfo[BOOT_INFO_LENGTH] = {0};

    int mainCode[MAIN_CODE_LENGTH] = {0};

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
        index = (getMainCodeCrcBase() - getFirmwareInfoBase()) / sizeof(int);
        printf("current maincode crc: %08x\n", invert_endian(firmwareInfo[index]));
        index = (getBootloaderCrcBase() - getFirmwareInfoBase()) / sizeof(int);
        printf("current bootloader crc: %08x\n", invert_endian(firmwareInfo[index]));
    }
    else
    {
        printf("reading firmware info : fail\n");
    }

    index = getFirmwareIdBase() / sizeof(int);
    printf("new firmware id: %08x, %08x ,%08x\n", invert_endian(buf[index]), invert_endian(buf[index + 1]), invert_endian(buf[index + 2]));
    index = getBootloaderIdBase() / sizeof(int);
    printf("new bootloader id: %08x\n", invert_endian(buf[index]));
    index = getMainCodeCrcBase() / sizeof(int);
    printf("new maincode crc: %08x\n", invert_endian(buf[index]));
    index = getBootloaderCrcBase() / sizeof(int);
    printf("new bootloader crc: %08x\n", invert_endian(buf[index]));


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

    index = (getBootFlagBase() - getBootFlagBase()) / sizeof(int);
    unsigned int bootFlag = static_cast<unsigned>(invert_endian(bootInfo[index]));
    index = (getBootloaderIdBase() - getFirmwareInfoBase()) / sizeof(int);
    int deviceBootloaderId = invert_endian(firmwareInfo[index]);
    index = getBootloaderIdBase() / sizeof(int);
    int fileBootloaderId = invert_endian(buf[index]);
    int main = (deviceBootloaderId >>16) & 0xffff;
    int sub = deviceBootloaderId & 0xffff;

    index = getMainCodeCrcBase() / sizeof(int);
    unsigned int mainCodeCrc = static_cast<unsigned int>(invert_endian(buf[index]));
    index = (getMainCodeCrcBase() - getFirmwareInfoBase()) / sizeof(int);
    unsigned int deviceMainCodeCrc = static_cast<unsigned int>(invert_endian(firmwareInfo[index]));
    index = getBootloaderCrcBase() / sizeof(int);
    int bootLoaderCrc = invert_endian(buf[index]);
    index = (getBootloaderCrcBase() - getFirmwareInfoBase()) / sizeof(int);
    int deviceBootLoaderCrc= invert_endian(firmwareInfo[index]);


    if(bootFlag == getBootFlagValue())
    {
        if((main == 0xffff && sub == 0xffff) || (main < 0x0001) || (main == 0x0001 && sub <= 0x0001))
        {
            isNewBootloader = false;
            printf("Old bootloader. Only can update whole fw.\n");
            update_bootloader = true;
            if(update_parameter)
            {
                printf("Disable only update parameters\n");
                update_parameter = false;
            }
        }
        else
        {
            isNewBootloader = true;
            if(!update_bootloader)
            {
                if(deviceBootloaderId != fileBootloaderId)
                {
                    if (update_bootloader_auto) {
                        printf("Different bootloader id. Automatically update with -ba parameter.\n");
                        update_bootloader = true;
                    }
                    else
                    {
                        if(force_update)
                        {
                            printf("Different bootloader id. Force updating with -b parameter.\n");
                            update_bootloader = true;
                            if(update_parameter)
                            {
                                printf("Disable only update parameters\n");
                                update_parameter = false;
                            }
                        }
                        else
                        {
                            printf("Different bootloader id. Please update with -b parameter.\n");
                            return ERROR_IN_COMPARE_BOOTLOADER;
                        }
                    }
                }
                else
                {
                    if(bootLoaderCrc != deviceBootLoaderCrc)
                    {
                    	if (update_bootloader_auto) {
							printf("Different bootloader id. Automatically update with -ba parameter.\n");
							update_bootloader = true;
						}
						else
						{
							if(force_update)
							{
								printf("Different bootloader crc. Force updating with -b parameter.\n");
								update_bootloader = true;
								if(update_parameter)
								{
									printf("Disable only update parameters\n");
									update_parameter = false;
								}
							}
							else
							{
								printf("Different bootloader crc. Please update with -b parameter.\n");
								return ERROR_IN_COMPARE_BOOTLOADER;
							}
						}
                    }
                }
            }
        }
    }
    else
    {
        isNewBootloader = true;
        printf("Broken fw. take new flow\n");
        if(update_parameter)
        {
            printf("Disable only update parameters\n");
            update_parameter = false;
        }

#ifdef RESERVER_RO_DATA_ONLY_SUCCESS
        if(reserve_RODATA)
        {
            printf("Invalide RO data, drop RO data\n");
            reserve_RODATA = false;
        }
#endif
    }


    if(update_parameter)
    {
        if(mainCodeCrc != deviceMainCodeCrc)
        {
            if(force_update)
            {
                printf("Different main code crc. Disable only update parameters\n");
                update_parameter = false;
            }
            else
            {
                printf("Different main code crc. Cannot only update parameters.\n");
                return ERROR_IN_COMPARE_MAINCODE;
            }
        }
        else
        {
            if(mainCodeCrc == 0xffffffff)
            {
                printf("The is no information in crc = 0xffffffff. Compare main code.\n");
                printf("reading main code\n");

                for(unsigned int i = 0; i < MAIN_CODE_LENGTH; i+= (0x0500 / sizeof(int)))
                {
                    result = read_from_address(MAINCODE_ADDRESS + i  * sizeof(int), mainCode + i, READ_LENGTH);
                    if(!result)
                    {
                        break;
                    }
                }

                if(result)
                {
                    printf("reading main code : success\n");
                }
                else
                {
                    printf("reading main code : fail\n");
                }

                for(unsigned int i = 0; i < MAIN_CODE_LENGTH; i+= (0x0500 / sizeof(int)))
                {
                    for(int j = 0; j < READ_LENGTH; j++)
                    {
                       if(mainCode[i+j] != buf[(MAINCODE_BASE / sizeof(int)) + i+j])
                        {
                            if(force_update)
                            {
                                printf("Different main code. Disable only update parameters\n");
                                update_parameter = false;
                                break;
                            }
                            else
                            {
                                printf("Different main code. Cannot only update parameters.\n");
                                return ERROR_IN_COMPARE_MAINCODE;
                            }
                        }
                    }

                    if(!update_parameter)
                    {
                        break;
                    }
                }
            }
        }
    }

    if(reserve_RODATA)
    {

		index = (getCalibrationFlagBase() - getBootFlagBase()) / sizeof(int);
		unsigned int calibrationFlag = static_cast<unsigned int>(invert_endian(bootInfo[index]));
		if(calibrationFlag != getCalibrationFlagValue())
		{
			printf("Invalid RO data, drop RO data\n");
			reserve_RODATA = false;
		}
    }

    return SUCCESS;
}

bool
AegisMultiSiSTouchAdapter::doDetectIsMulti()
{
    int oldSlaveAddr = ((SiSTouch_Aegis_Multi*)m_io)->getSlaveAddr();

    bool isMultiDevice = false;
    int ret = 0;
    bool result = false;

    ((SiSTouch_Aegis_Multi*)m_io)->setSlaveAddr( MASTER_DEVICE_ADDR );

    ret = m_io->stop_driver();


    if( ret < 0 )
    {
        ((SiSTouch_Aegis_Multi*)m_io)->setSlaveAddr( oldSlaveAddr );
        printf("[SiSTouch][AegisMultiSiSTouchAdapter] stop_driver fail, isMultiDevice=%d\n", isMultiDevice);
        return isMultiDevice;
    }

    int data[1] = {0};

    result = read_from_address(INTERFACE_FLAG_ADDR, data, 1);

    if( result )
    {
        unsigned int interfaceData = invert_endian(data[0]);
        int multiSelect =
                (interfaceData >> MULTI_SHIFT)
                        & MULTI_MASK;

        if (( multiSelect == MASTER_DEVICE_FLAG )|| ( multiSelect == BRIDGE_DEVICE_FLAG ))
        {
            isMultiDevice = true;
        }
    }

    m_io->start_driver();

    ((SiSTouch_Aegis_Multi*)m_io)->setSlaveAddr( oldSlaveAddr );
    return isMultiDevice;
}

int
AegisMultiSiSTouchAdapter::doDetectSlaveNum()
{
    int oldSlaveAddr = ((SiSTouch_Aegis_Multi*)m_io)->getSlaveAddr();

    int slaveNum = DEFALUT_SLAVE_NUM;
    int ret = 0;
    bool result = false;

    ((SiSTouch_Aegis_Multi*)m_io)->setSlaveAddr( MASTER_DEVICE_ADDR );

    ret = m_io->stop_driver();
    if( ret < 0 )
    {
        ((SiSTouch_Aegis_Multi*)m_io)->setSlaveAddr( oldSlaveAddr );
        printf("[SiSTouch][AegisMultiSiSTouchAdapter::doDetectSlaveNum] stop_driver fail, slaveNum=%d\n", slaveNum);
        return slaveNum;
    }

    int data[2] = {0};

    result = read_from_address(INTERFACE_FLAG_ADDR, data, 2);

    if( result )
    {
        bool isMultiDevice = false;

        unsigned int interfaceData = invert_endian(data[0]);
        int multiSelect = (interfaceData >> MULTI_SHIFT) & MULTI_MASK;

        if (( multiSelect == MASTER_DEVICE_FLAG )|| ( multiSelect == BRIDGE_DEVICE_FLAG ))
        {
            isMultiDevice = true;
        }

        if(isMultiDevice)
        {
            unsigned int slaveNumData = invert_endian(data[1]);
            int num = (slaveNumData >> SLAVE_NUM_SHIFT) & SLAVE_NUM_MASK;
            slaveNum = ( num >= INDEX_MASTER && num <= INDEX_SLAVE5 ) ? num : 1;
        }
        else
        {
            slaveNum = 0;
        }

    }

    m_io->start_driver();

    ((SiSTouch_Aegis_Multi*)m_io)->setSlaveAddr( oldSlaveAddr );

    m_io->setChipNum(slaveNum + 1); // [20150304]
    return slaveNum;
}

bool
AegisMultiSiSTouchAdapter::ioStaticDetectIsMulti(int max_retry, int retryDelay, int verbose, int ioDelay, int changeDiagMode, int changePWRMode)
{
    AegisMultiSiSTouchAdapter localAdapter(max_retry, retryDelay, verbose, ioDelay, changeDiagMode, changePWRMode) ;

    bool isMulti = localAdapter.doDetectIsMulti();
    return isMulti;

}
