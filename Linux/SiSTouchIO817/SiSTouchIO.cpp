/*
 * SiSTouchIO.cpp
 *
 *  Created on: Mar 25, 2010
 *      Author: suzin
 */

#include "SiSTouchIO.h"
#include "SiSLogger.h"
#include "SiSTouchDeviceName.h"
#include <errno.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>

#include "SisTouchFinder.h"

static const unsigned short crc16tab[256]=
{
    0x0000,0x1021,0x2042,0x3063,0x4084,0x50a5,0x60c6,0x70e7,
    0x8108,0x9129,0xa14a,0xb16b,0xc18c,0xd1ad,0xe1ce,0xf1ef,
    0x1231,0x0210,0x3273,0x2252,0x52b5,0x4294,0x72f7,0x62d6,
    0x9339,0x8318,0xb37b,0xa35a,0xd3bd,0xc39c,0xf3ff,0xe3de,
    0x2462,0x3443,0x0420,0x1401,0x64e6,0x74c7,0x44a4,0x5485,
    0xa56a,0xb54b,0x8528,0x9509,0xe5ee,0xf5cf,0xc5ac,0xd58d,
    0x3653,0x2672,0x1611,0x0630,0x76d7,0x66f6,0x5695,0x46b4,
    0xb75b,0xa77a,0x9719,0x8738,0xf7df,0xe7fe,0xd79d,0xc7bc,
    0x48c4,0x58e5,0x6886,0x78a7,0x0840,0x1861,0x2802,0x3823,
    0xc9cc,0xd9ed,0xe98e,0xf9af,0x8948,0x9969,0xa90a,0xb92b,
    0x5af5,0x4ad4,0x7ab7,0x6a96,0x1a71,0x0a50,0x3a33,0x2a12,
    0xdbfd,0xcbdc,0xfbbf,0xeb9e,0x9b79,0x8b58,0xbb3b,0xab1a,
    0x6ca6,0x7c87,0x4ce4,0x5cc5,0x2c22,0x3c03,0x0c60,0x1c41,
    0xedae,0xfd8f,0xcdec,0xddcd,0xad2a,0xbd0b,0x8d68,0x9d49,
    0x7e97,0x6eb6,0x5ed5,0x4ef4,0x3e13,0x2e32,0x1e51,0x0e70,
    0xff9f,0xefbe,0xdfdd,0xcffc,0xbf1b,0xaf3a,0x9f59,0x8f78,
    0x9188,0x81a9,0xb1ca,0xa1eb,0xd10c,0xc12d,0xf14e,0xe16f,
    0x1080,0x00a1,0x30c2,0x20e3,0x5004,0x4025,0x7046,0x6067,
    0x83b9,0x9398,0xa3fb,0xb3da,0xc33d,0xd31c,0xe37f,0xf35e,
    0x02b1,0x1290,0x22f3,0x32d2,0x4235,0x5214,0x6277,0x7256,
    0xb5ea,0xa5cb,0x95a8,0x8589,0xf56e,0xe54f,0xd52c,0xc50d,
    0x34e2,0x24c3,0x14a0,0x0481,0x7466,0x6447,0x5424,0x4405,
    0xa7db,0xb7fa,0x8799,0x97b8,0xe75f,0xf77e,0xc71d,0xd73c,
    0x26d3,0x36f2,0x0691,0x16b0,0x6657,0x7676,0x4615,0x5634,
    0xd94c,0xc96d,0xf90e,0xe92f,0x99c8,0x89e9,0xb98a,0xa9ab,
    0x5844,0x4865,0x7806,0x6827,0x18c0,0x08e1,0x3882,0x28a3,
    0xcb7d,0xdb5c,0xeb3f,0xfb1e,0x8bf9,0x9bd8,0xabbb,0xbb9a,
    0x4a75,0x5a54,0x6a37,0x7a16,0x0af1,0x1ad0,0x2ab3,0x3a92,
    0xfd2e,0xed0f,0xdd6c,0xcd4d,0xbdaa,0xad8b,0x9de8,0x8dc9,
    0x7c26,0x6c07,0x5c64,0x4c45,0x3ca2,0x2c83,0x1ce0,0x0cc1,
    0xef1f,0xff3e,0xcf5d,0xdf7c,0xaf9b,0xbfba,0x8fd9,0x9ff8,
    0x6e17,0x7e36,0x4e55,0x5e74,0x2e93,0x3eb2,0x0ed1,0x1ef0
};
//20120925
static const unsigned short wrong_crc16tab[256]=
{
    0x0001,0x1022,0x2043,0x3064,0x4085,0x50a6,0x60c7,0x70e8,
    0x8109,0x912a,0xa14b,0xb16c,0xc18d,0xd1ae,0xe1cf,0xf1e0,
    0x1232,0x0211,0x3274,0x2253,0x52b6,0x4295,0x72f8,0x62d7,
    0x933a,0x8319,0xb37c,0xa35b,0xd3be,0xc39d,0xf3f0,0xe3df,
    0x2463,0x3444,0x0421,0x1402,0x64e7,0x74c8,0x44a5,0x5486,
    0xa56b,0xb54c,0x8529,0x950a,0xe5ef,0xf5c0,0xc5ad,0xd58e,
    0x3654,0x2673,0x1612,0x0631,0x76d8,0x66f7,0x5696,0x46b5,
    0xb75c,0xa77b,0x971a,0x8739,0xf7d0,0xe7ff,0xd79e,0xc7bd,
    0x48c5,0x58e6,0x6887,0x78a8,0x0841,0x1862,0x2803,0x3824,
    0xc9cd,0xd9ee,0xe98f,0xf9a0,0x8949,0x996a,0xa90b,0xb92c,
    0x5af6,0x4ad5,0x7ab8,0x6a97,0x1a72,0x0a51,0x3a34,0x2a13,
    0xdbfe,0xcbdd,0xfbb0,0xeb9f,0x9b7a,0x8b59,0xbb3c,0xab1b,
    0x6ca7,0x7c88,0x4ce5,0x5cc6,0x2c23,0x3c04,0x0c61,0x1c42,
    0xedaf,0xfd80,0xcded,0xddce,0xad2b,0xbd0c,0x8d69,0x9d4a,
    0x7e98,0x6eb7,0x5ed6,0x4ef5,0x3e14,0x2e33,0x1e52,0x0e71,
    0xff90,0xefbf,0xdfde,0xcffd,0xbf1c,0xaf3b,0x9f5a,0x8f79,
    0x9189,0x81aa,0xb1cb,0xa1ec,0xd10d,0xc12e,0xf14f,0xe160,
    0x1081,0x00a2,0x30c3,0x20e4,0x5005,0x4026,0x7047,0x6068,
    0x83ba,0x9399,0xa3fc,0xb3db,0xc33e,0xd31d,0xe370,0xf35f,
    0x02b2,0x1291,0x22f4,0x32d3,0x4236,0x5213,0x6278,0x7257,
    0xb5eb,0xa5cc,0x95a9,0x858a,0xf56f,0xe540,0xd52d,0xc50e,
    0x34e3,0x24c4,0x14a1,0x0482,0x7467,0x6448,0x5425,0x4406,
    0xa7dc,0xb7fb,0x879a,0x97b9,0xe750,0xf77f,0xc71e,0xd73d,
    0x26d4,0x36f3,0x0692,0x16b1,0x6658,0x7677,0x4616,0x5635,
    0xd94d,0xc96e,0xf90f,0xe920,0x99c9,0x89ea,0xb98b,0xa9ac,
    0x5845,0x4866,0x7807,0x6828,0x18c1,0x08e2,0x3883,0x28a4,
    0xcb7e,0xdb5d,0xeb30,0xfb1f,0x8bfa,0x9bd9,0xabbc,0xbb9b,
    0x4a76,0x5a55,0x6a38,0x7a17,0x0af2,0x1ad1,0x2ab4,0x3a93,
    0xfd2f,0xed00,0xdd6d,0xcd4e,0xbdab,0xad8c,0x9de9,0x8dca,
    0x7c27,0x6c08,0x5c65,0x4c46,0x3ca3,0x2c84,0x1ce1,0x0cc2,
    0xef10,0xff3f,0xcf5e,0xdf7d,0xaf9c,0xbfbb,0x8fda,0x9ff9,
    0x6e18,0x7e37,0x4e56,0x5e75,0x2e94,0x3eb3,0x0ed2,0x1ef1
};

//for custom device name/info setting
char SiSTouchIO::m_activeDeviceName[]="";

bool SiSTouchIO::m_isCustomDevice = false;
int SiSTouchIO::m_customDeviceType = CON_NONE;	//CON_NONE
char SiSTouchIO::m_customDeviceName[]="";

void SiSTouchIO::AppendCRC( char data )
{
    m_crc = (m_crc<<8) ^ crc16tab[((m_crc>>8) ^ data)&0x00FF];
}
//20120925
void SiSTouchIO::AppendWrongCRC( char data )
{
    //m_crc = (m_crc<<8) ^ wrong_crc16tab[((m_crc>>8) ^ data)&0x00FF];
	m_crc = rand() % 255;
}
//

int
SiSTouchIO::make_83_buffer( int base, int pack_num )
{
    int buf_idx = 0;
    unsigned char temp;

    m_crc = 0;

    m_buffer[ buf_idx++ ] = 0x83;

    if ( m_add_bytecount )
        m_buffer[ buf_idx++ ] = 8;

    for ( int i = 3; i >= 0; i-- )
    {
        temp = ( base >> (8*i) ) & 0xff;
        AppendCRC( temp );
        m_buffer[ buf_idx++ ] = temp;
    }

    for ( int i = 1; i >= 0; i-- )
    {
        temp = ( pack_num >> (8*i) ) & 0xff;
        AppendCRC( temp );
        m_buffer[ buf_idx++ ] = temp;
    }

    for ( int i = 1; i >= 0; i-- )
    {
        temp = ( m_crc >> (8*i) ) & 0xff;
        m_buffer[ buf_idx++ ] = temp;
    }

    if ( m_verbose ) dbg_print_buffer_hex( m_buffer, buf_idx );

    return buf_idx;
}

int
SiSTouchIO::make_84_buffer( const int* data, int size )
{
    int buf_idx = 0;
    char temp;

    m_crc = 0;

    m_buffer[ buf_idx++ ] = 0x84;

    if ( m_add_bytecount )
        m_buffer[ buf_idx++ ] = 14;

    for ( int k = 0; k < size; k++ )
    {
        //for ( int i = 3; i >= 0; i-- )
        for ( int i = 0; i < 4; i++ )
        {
            temp = ( data[k] >> (8*i) ) & 0xff;
            AppendCRC( temp );
            m_buffer[ buf_idx++ ] = temp;
        }
    }

    for ( int i = 1; i >= 0; i-- )
    {
        temp = ( m_crc >> (8*i) ) & 0xff;
        m_buffer[ buf_idx++ ] = temp;
    }

    if ( m_verbose ) dbg_print_buffer_hex( m_buffer, buf_idx );

    return buf_idx;
}


int SiSTouchIO::make_85_buffer( unsigned char data )
{
    int buf_idx = 0;
    m_buffer[buf_idx++] = 0x85;
    if ( m_add_bytecount )
        m_buffer[ buf_idx++ ] = 1;
    m_buffer[buf_idx++] = data;

    if ( m_verbose ) dbg_print_buffer_hex( m_buffer, buf_idx );

    return buf_idx;
}

int
SiSTouchIO::make_86_buffer( int addr )
{
    int buf_idx = 0;
    unsigned char temp;

    m_crc = 0;

    m_buffer[ buf_idx++ ] = 0x86;

    if ( m_add_bytecount )
        m_buffer[ buf_idx++ ] = 6;

    for ( int i = 3; i >= 0; i-- )
    {
        temp = ( addr >> (8*i) ) & 0xff;
        AppendCRC( temp );
        m_buffer[ buf_idx++ ] = temp;
    }

    for ( int i = 1; i >= 0; i-- )
    {
        temp = ( m_crc >> (8*i) ) & 0xff;
        m_buffer[ buf_idx++ ] = temp;
    }

    if ( m_verbose ) dbg_print_buffer_hex( m_buffer, buf_idx );

    return buf_idx;
}

int
SiSTouchIO::make_88_buffer( int *addr, int *data, int *mask, int number )
{
    int buf_idx = 0;
    unsigned char temp;

    m_crc = 0;

    m_buffer[ buf_idx++ ] = 0x88;

    if ( m_add_bytecount )
        m_buffer[ buf_idx++ ] = 14;

    for ( int i = 3; i >= 0; i-- )
    {
        temp = ( addr[0] >> (8*i) ) & 0xff;
        AppendCRC( temp );
        m_buffer[ buf_idx++ ] = temp;
    }

    for( int i = 3; i >= 0; i-- )
    {
        temp = ( data[0] >> (8*i) ) & 0xff;
        AppendCRC( temp );
        m_buffer[ buf_idx++ ] = temp;
    }

    for ( int i = 3; i >= 0; i-- )
    {
        temp = ( mask[0] >> (8*i) ) & 0xff;
        AppendCRC( temp );
        m_buffer[ buf_idx++ ] = temp;
    }

    for ( int i = 1; i >= 0; i-- )
    {
        temp = ( m_crc >> (8*i) ) & 0xff;
        m_buffer[ buf_idx++ ] = temp;
    }

    if ( m_verbose ) dbg_print_buffer_hex( m_buffer, buf_idx );

    return buf_idx;
}


int
SiSTouchIO::make_90_buffer( unsigned char powermode )
{
    int buf_idx = 0;
    unsigned char temp;

    m_crc = 0;

    m_buffer[ buf_idx++ ] = 0x90;

    if ( m_add_bytecount )
        m_buffer[ buf_idx++ ] = 3;



    temp = powermode;

    AppendCRC( temp );
    m_buffer[ buf_idx++ ] = temp;


    for ( int i = 1; i >= 0; i-- )
    {
        temp = ( m_crc >> (8*i) ) & 0xff;
        m_buffer[ buf_idx++ ] = temp;
    }

    if ( m_verbose ) dbg_print_buffer_hex( m_buffer, buf_idx );



    return buf_idx;
}

int
SiSTouchIO::make_command_buffer( int cmd, int size, unsigned char *data, int addcrc )
{
    int buf_idx = 0;
    unsigned char temp;

    m_crc = 0;

    m_buffer[ buf_idx++ ] = cmd;

    if ( m_add_bytecount )
    {
        if(addcrc)
        {
            m_buffer[ buf_idx++ ] = size + 2;
        }
        else
        {
            m_buffer[ buf_idx++ ] = size;
        }
    }

    for( int i = 0; i < size; i++)
    {
        if(buf_idx >= static_cast<int>(get_MAX_SIZE()))
        {
            if ( m_verbose ) LOGE("Data exceed buffer size truncate");
            break;
        }
        temp = data[i] & 0xff;
        AppendCRC( temp );
        m_buffer[ buf_idx++ ] = temp;
    }

    if(addcrc)
    {
        for ( int i = 1; i >= 0; i-- )
        {
            if(buf_idx >= static_cast<int>(get_MAX_SIZE()))
            {
                if ( m_verbose ) LOGE("Data exceed buffer size truncate");
                break;
            }
            temp = ( m_crc >> (8*i) ) & 0xff;
            m_buffer[ buf_idx++ ] = temp;
        }
    }


    if ( m_verbose ) dbg_print_buffer_hex( m_buffer, buf_idx );



    return buf_idx;
}

void
SiSTouchIO::set_retry( int max_retry, int delay )
{
    m_retry = max_retry;
    m_delay = delay;
}

int
SiSTouchIO::setVerbose(int verbose)
{
    m_verbose = verbose;
    LOGI( "Verbose set to %d", m_verbose );
    return m_verbose;
}

int
SiSTouchIO::getVerbose()
{
    return m_verbose;
}

int
SiSTouchIO::setIODelay(int delayms)
{
    if(delayms >= 0)
    {
        m_IODelayms = delayms;
    }
    else
    {
        m_IODelayms = 0;
    }
    LOGI( "IODelay set to %d ms",  m_IODelayms );
    return m_IODelayms;
}

int
SiSTouchIO::getIODelay()
{
    return m_IODelayms;
}

//20130904 for multi device - begin
int
SiSTouchIO::device_Exist() 
{
#ifdef VIA_IOCTL
    struct stat s;
    if(stat( m_activeDeviceName , &s ) == 0)
    {
        if( S_ISBLK( s.st_mode ) || S_ISCHR( s.st_mode ))
        {
            return 1;
        }
    }
    return 0;
#else
    return 0;
#endif

}

int
SiSTouchIO::setActiveDeviceName(char* devName, int devNameLen)
{
	if (devNameLen+strlen("/dev/") > DEVNAME_MAX_LEN) {
		return ERROR;
	}

	sprintf(m_activeDeviceName, "/dev/%s", devName);
	return device_Exist();	//1:find device, 0:not found
}

char*
SiSTouchIO::getActiveDeviceName()
{
	if ( strlen(m_activeDeviceName) > 0 )
		return m_activeDeviceName;
	else
		return NULL;
}

int
SiSTouchIO::getDeviceType(int max_retry, int retry_delay, int verbose, int ioDelay, int changeDiagMode, int changePWRMode)
{
    return getDeviceType(max_retry, retry_delay, verbose, ioDelay, changeDiagMode, changePWRMode, 0);
}

int
SiSTouchIO::getDeviceType(int max_retry, int retry_delay, int verbose, int ioDelay, int changeDiagMode, int changePWRMode, int detectHidrawFlag)
{
	int con = SiSTouchIO::CON_NONE;

	if(detectHidrawFlag == 1)
	{
	
	// only detect hidraw

		printf("Only detect hidraw\n");
	
        SisTouchFinder sisTouchFinder;
        const char* deviceName = sisTouchFinder.autoDetectDevicePath();
	
        if(deviceName != 0)
        {
            if(sisTouchFinder.getDeviceType() == SisTouchFinder::USB_817)
            {
                con = CON_AEGIS_MULTI_FOR_NEW_DRIVER;
            }

            strcpy(m_activeDeviceName, deviceName);
			delete deviceName;

            printf("find activeDeviceName=%s, ", m_activeDeviceName);
            return con;
        }
    }

	return con;
}
//20130904 for multi device - end

//20130410 for custom device name/type setting
bool SiSTouchIO::isCustomDevice()
{
	return m_isCustomDevice;
}

void
SiSTouchIO::setCustomDevice(bool isCustom, int devType, char devName[], int devNameLen)
{
	m_isCustomDevice = isCustom;

	if (m_isCustomDevice == false) {
		m_customDeviceType = -2;
	}
	else if(devType > 0){
		if (devNameLen > DEVNAME_MAX_LEN-1) {
			LOGI("Error: device Name > %d, len=%d \n", DEVNAME_MAX_LEN-1, devNameLen);
			devNameLen = DEVNAME_MAX_LEN-1;
		}
		m_customDeviceType = devType;
		strncpy(m_customDeviceName, devName, devNameLen);
		m_customDeviceName[devNameLen]='\0';
		strcpy(m_activeDeviceName, m_customDeviceName);
	}
	else {
		LOGI( "setCustomDevice Error : devType = %d", __FILE__, devType);
	}
	LOGI("%s result(%d, %d, %s, %d)\n", __FUNCTION__, m_isCustomDevice, m_customDeviceType, m_customDeviceName, devNameLen);

}

char *SiSTouchIO::getCustomDeviceName()
{
	return m_customDeviceName;
}

//end 20130410

void SiSTouchIO::dbg_print_buffer_hex( const unsigned char* data, int size )
{
    LOGI( "size = %d", size );

    int strSize = get_MAX_SIZE() * 5;

    char * str = new char[strSize];

    if ( size > 0  && size <= static_cast<int>(get_MAX_SIZE()))
    {

        sprintf(str, "%02x", data[0]);

        for( int i = 1; i < size; i++ )
        {
            sprintf(str, "%s %02x", str, data[i]);

        }

        LOGI( "%s", str );


    }
    else if (size > static_cast<int>(get_MAX_SIZE()))
    {

        sprintf(str, "%02x", data[0]);

        for( int i = 1; i < static_cast<int>(get_MAX_SIZE()); i++ )
        {
            sprintf(str, "%s %02x", str, data[i]);
        }

        LOGI( "========================Too long return packets=======================" );
        LOGI( "The first %d bytes:", get_MAX_SIZE());
        LOGI( "%s", str );

    }

    delete[] str;
}

void SiSTouchIO::dbg_print_buffer_hex( int cmd, const unsigned char* data, int size )
{
    LOGI( "CMD %02x: ", cmd );
    dbg_print_buffer_hex( data, size );
}

int SiSTouchIO::parse_syscall_return_value( int ret , int type)
{
    int temp = 0;
#ifdef FOR_ANDROID_X86
    temp = -ret;
#else
    temp = errno;
#endif

    if ( m_verbose )
    {
        LOGE( "function type: %d", type);
        LOGE( "syscall error code: %d", temp );
    }


    if((type & NOT_SIS_IO_FLAG) != 0)
    {
        if ( temp == ENOSYS )
        {
            return ERROR_SYSCALL;
        }
#ifdef VIA_IOCTL
        else if ( temp == EACCES )
        {
            return ERROR_ACCESS;
        }
        else if( temp == ENOENT)
        {
            return ERROR_ENTRY_NOT_EXIST;
        }
        else if( temp == ENODEV)
        {
            return ERROR_DEVICE_NOT_EXIST;
        }
#endif
        else
        {
            return -temp;
        }
    }
    else
    {

        if ( temp == ENOSYS )
        {
            return ERROR_SYSCALL;
        }
        else if((type & ONLY_RETURN_ZERO_FLAG) != 0)
        {
            return ERROR_SYSCALL;
        }
        else
        {
            return -temp;
        }
    }
}

// [20150304]
int SiSTouchIO::getChipNum()
{
	return this->m_chipNum;
}

// [20150304]
void SiSTouchIO::setChipNum(int chipNum)
{
	this->m_chipNum = chipNum;
}


SiSTouchIO::~SiSTouchIO()
{
#ifdef VIA_IOCTL
    if(m_fd >= 0)
    {
        close(m_fd);
        m_fd = -1;
    }
#endif
}



