#include "isisdeviceio.h"

#include "sislog.h"
#include "sisdeviceio/sisdeviceexception.h"

#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>

using namespace SiS;
using namespace SiS::DeviceIO;

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

#define TAG "ISiSDeviceIO"

ISiSDeviceIO::ISiSDeviceIO() :
    m_sisDeviceMgr(0),
    m_sisIOAttribute(0),
    m_fd(-1),
    m_crc(0),
    m_buffer(0),
    m_userConnectType(SiSDeviceAttribute::CON_UNKNOW)
{

}

ISiSDeviceIO::~ISiSDeviceIO()
{
    if(m_fd >= 0)
    {
        close(m_fd);
        m_fd = -1;
    }
}

void
ISiSDeviceIO::appendCRC( char data )
{
    m_crc = (m_crc<<8) ^ crc16tab[((m_crc>>8) ^ data)&0x00FF];
}

std::string
ISiSDeviceIO::deviceNameOpened()
{
    SiSDeviceAttribute* opened = m_sisDeviceMgr->getOpened();
    return opened->getDeviceName();
}

SiSDeviceMgr* 
ISiSDeviceIO::getSiSDeviceMgr()
{
    return m_sisDeviceMgr;
}

SiSIOAttribute*
ISiSDeviceIO::getSiSIOAttribute()
{
    return m_sisIOAttribute;
}

void
ISiSDeviceIO::setUserConnectType(SiSDeviceAttribute::ConnectType userConnectType)
{
    m_userConnectType = userConnectType;
}

void 
ISiSDeviceIO::boot(std::string deviceName)
{
    if( m_sisDeviceMgr->detectBySiSDeviceNode(deviceName) ) /* detect "/dev/sis_*" */
    {

    }
    else if( m_sisDeviceMgr->detectByHidrawName(deviceName) ) /* detect "/dev/hidraw*" */
    {

    }
    else if( false ) /* detect "other (to do)" */
    {
    }

    SiSDeviceAttribute* opened = m_sisDeviceMgr->getOpened();

    /* user force to set opened : connectType */
    if( m_userConnectType == SiSDeviceAttribute::CON_819_USB_HID )
    {
        opened->setConnectType( SiSDeviceAttribute::CON_819_USB_HID  );
	    SIS_LOG_W(SiSLog::getOwnerSiS(), TAG, "user force to set opened : CON_819_USB_HID" );
    }
    else if( m_userConnectType == SiSDeviceAttribute::CON_819_HID_OVER_I2C )
    {
        opened->setConnectType( SiSDeviceAttribute::CON_819_HID_OVER_I2C  );
        SIS_LOG_W(SiSLog::getOwnerSiS(), TAG, "user force to set opened : CON_819_HID_OVER_I2C" );
    }

	/* show info */
    SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "opened : %s (\"%s\")",  opened->getDeviceName().c_str(), opened->getNodeName().c_str() );
    SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "CON : %d", opened->getConnectType() );
    SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "rawname : %s", opened->getRawName().c_str() );
    SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "VID : 0x%04hx", opened->getVID() );
    SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "PID : 0x%04hx", opened->getPID() );
}

void 
ISiSDeviceIO::shutdown()
{
    closeDevice();
}

int
ISiSDeviceIO::ctlReportToOs(bool isReport)
{
    int ret = -1;

    if( isReport == false )
    {
        ret = call85WithRetry(PWR_CMD_ACTIVE, MASTER_ADDRESS);

        if( ret < 0 )
        {
            return ret;
        }

        ret = call85WithRetry(ENABLE_DIAGNOSIS_MODE, MASTER_ADDRESS);

        if( ret < 0 )
        {
            return ret;
        }
    }
    else
    {
        ret = call85WithRetry(PWR_CMD_FWCTRL, MASTER_ADDRESS);

        if( ret < 0 )
        {
            return ret;
        }
		
        ret = call85WithRetry(DISABLE_DIAGNOSIS_MODE, MASTER_ADDRESS);

        if( ret < 0 )
        {
            return ret;
        }
    }

    usleep(CHANGE_85_MODE_DELAY * 1000);	// ms

    return ret;
}

int
ISiSDeviceIO::ctlSoftReset()
{
    int ret = -1;
    ret = call82WithRetry();

    return ret;
}

bool
ISiSDeviceIO::readFromAddress(unsigned int address, int size, unsigned char* buffer, int chipAddress)
{
    SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "readFromAddress() : address=0x%08x, size=0x%02x, chipAddress=%d\n", address,  size, chipAddress);

    int readLength = formatData86Unit(size);

    for (int i = 0; i < size; i += readLength)
    {
        int length = ( (size - i) >= readLength ) ? readLength : (size - i);
        int ret = -1;

        ret = call86WithRetry(address, buffer + i, length, chipAddress);

        if ( ret < 0 )
        {
            SIS_LOG_E(SiSLog::getOwnerSiS(), TAG, "Read(0x86) Fail\n" );
            return false;
        }

        address += length;
    }

    return true;
}

void 
ISiSDeviceIO::printBuffer( int cmd, const unsigned char* data, int size )
{
    SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "CMD %02x: ", cmd );
    printBuffer( data, size );
}

void
ISiSDeviceIO::printBuffer( const unsigned char* data, int size )
{
    SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "size = %d", size );

    //int strSize = getMaxBufferSize() * 5;

    std::string str = "";

    char * tmp_c = new char[5];
    memset( tmp_c, '\0', sizeof(char) * 5 );

    if ( size > 0  && size <= static_cast<int>(getMaxBufferSize()))
    {
        sprintf(tmp_c, "%02x", data[0]);
        std::string tmp_s(tmp_c);
        str.append(tmp_s);

        for( int i = 1; i < size; i++ )
        {
            sprintf(tmp_c, " %02x", data[i]);
            std::string tmp_s(tmp_c);
            str.append(tmp_s);
        }

        SIS_LOG_D(SiSLog::getOwnerSiS(), TAG,  "%s", str.c_str() );
    }
    else if (size > static_cast<int>(getMaxBufferSize()))
    {
        sprintf(tmp_c, "%02x", data[0]);
        std::string tmp_s(tmp_c);
        str.append(tmp_s);

        for( int i = 1; i < static_cast<int>(getMaxBufferSize()); i++ )
        {
            sprintf(tmp_c, " %02x", data[i]);
            std::string tmp_s(tmp_c);
            str.append(tmp_s);
        }

        SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "========================Too long return packets=======================" );
        SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "The first %d bytes:", getMaxBufferSize());
        SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "%s", str.c_str() );
    }

    delete [] tmp_c;
}

void 
ISiSDeviceIO::waitIOReady(int &fd, int timeoutSec)
{
    fd_set rfds;
    struct timeval tv;
    int retval;

    FD_ZERO(&rfds); /* clear the set */
    FD_SET(fd, &rfds); /* add our file descriptor to the set */

    SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "waitIOReady : %d(sec)", timeoutSec );

    tv.tv_sec = timeoutSec;
    tv.tv_usec = 0;

    retval = select(fd + 1, &rfds, NULL, NULL, &tv);

    if(retval == -1)
    {
        SIS_LOG_E(SiSLog::getOwnerSiS(), TAG, "! select, errno=%d (%s)", errno, strerror(errno)); /* an error accured */
    }
    else if(retval == 0)
    {
        SIS_LOG_E(SiSLog::getOwnerSiS(), TAG, "! IO timeout" ); /* a timeout occured */
    }
    else
    {
        SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "IO data ready" ); /* there was data to read */
    }
}

int
ISiSDeviceIO::ioCommand(int cmd, unsigned char * buf, int writeSize, int readSize, int timeoutMsec)
{
    int ret = -1;

    /* write */
    SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "writeData : (writeSize=%d)", writeSize);
    printBuffer( cmd, buf, writeSize );

    ret = writeData(buf, writeSize, 0 );
    SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "writeData done. ret : %d", ret );

    if(ret < 0)
    {
        SIS_LOG_E(SiSLog::getOwnerSiS(), TAG, "writeData error, errno=%d (%s)", errno, strerror(errno));

        if( errno == 110 ) // timeout
        {
             SIS_LOG_W(SiSLog::getOwnerSiS(), TAG, "! timeout, but continue to read guff");
             int tmpRet = readData(buf, readSize, 0 ); // force to read, in order to not lock FW
             printBuffer( buf, tmpRet );

             return ret;
        }
        else
        {
            return ret;
        }
    }

    if(isUsingIOCTL())
    {
        // ioctl impl timeout (default 5s) ?
    }
    else if( readSize > 0 )
    {
        /* wait IO */
        waitIOReady(m_fd, timeoutMsec);
    }

    //if(sleepms >= 0)
    {
        //usleep(sleepms * 1000);
    }

    /* read */
    if( readSize > 0 )
    {
        unsigned char expectReportId = getReportId(readSize, false);
		
        for(int i = 0; i < 3000; i++)
        {
            buf[ getReportIdIndex(false) ] = expectReportId;
	
            SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "readData : (readSize=%d)", readSize);

            ret = readData(buf, readSize, 0 );
            SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "readData done. ret : %d", ret );

            printBuffer( buf, ret );

            if(ret < 0)
            {
                SIS_LOG_E(SiSLog::getOwnerSiS(), TAG, "readData error, errno=%d (%s)", errno, strerror(errno));
                return ret;
            }
            else
            {
               if( buf[ getReportIdIndex(false) ] == expectReportId )
               {
                   break;
               }
               else
               {
                   usleep(10 * 1000);
                   continue;
               }
            }
        }
    }
    else
    {
        ret = 1;
        SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "readData ignore (readSize=%d). ret : %d", readSize, ret );
    }

    //if ( m_verbose ) dbg_print_buffer_hex( cmd, buf, ret );

    //usb return fixed 64 byte
    //if( ret >= 0)
    {
        //ret = m_buffer[1] + 1;
    }

    if( m_sisIOAttribute->getIODelay() > 0 )
    {
        usleep( m_sisIOAttribute->getIODelay() * 1000 );	// ms
    }

    return ret;
}

int
ISiSDeviceIO::call85WithRetry(int data, int chipAddress)
{
    int ret = -1;
    int retryCnt = CMD_85_RETRY_MAX;

    for(int i = 0; i <= retryCnt; i++ )
    {
        ret = call85( data, chipAddress );

        if(ret >= 0)
        {
            break;
        }

        if( i == retryCnt )
        {
            SIS_LOG_E(SiSLog::getOwnerSiS(), TAG, "out of retry CMD 85");
        }
        else
        {
            SIS_LOG_W(SiSLog::getOwnerSiS(), TAG, "retry CMD 85 : %d of %d", i+1, retryCnt);
            usleep( CMD_RETRY_DELAY * 1000 );	// ms
        }
    }

    return ret;
}

int
ISiSDeviceIO::call86WithRetry(unsigned int address, unsigned char* data, int size, int chipAddress)
{
    int ret = -1;
    int retryCnt = CMD_86_RETRY_MAX;

    for(int i = 0; i <= retryCnt; i++ )
    {
        ret = call86(address, data, size,  chipAddress);

        if(ret >= 0)
        {
            break;
        }

        if( i == retryCnt )
        {
            SIS_LOG_E(SiSLog::getOwnerSiS(), TAG, "out of retry CMD 86");
        }
        else
        {
            SIS_LOG_W(SiSLog::getOwnerSiS(), TAG, "retry CMD 86 : %d of %d", i+1, retryCnt);
            usleep( CMD_RETRY_DELAY * 1000 );	// ms
        }
    }

    return ret;
}

int
ISiSDeviceIO::call82WithRetry()
{
    int ret = -1;
    int retryCnt = CMD_82_RETRY_MAX;

    for(int i = 0; i <= retryCnt; i++ )
    {
        ret = call82();

        if(ret >= 0)
        {
            break;
        }

        if( i == retryCnt )
        {
            SIS_LOG_E(SiSLog::getOwnerSiS(), TAG, "out of retry CMD 82");
        }
        else
        {
            SIS_LOG_W(SiSLog::getOwnerSiS(), TAG, "retry CMD 82 : %d of %d", i+1, retryCnt);
            usleep( CMD_RETRY_DELAY * 1000 );	// ms
        }
    }

    return ret;
}

int
ISiSDeviceIO::call83WithRetry(unsigned int pageStartAddress, unsigned int pageLength, int chipAddress)
{
    int ret = -1;
    int retryCnt = CMD_83_RETRY_MAX;

    for(int i = 0; i <= retryCnt; i++ )
    {
        ret = call83(pageStartAddress, pageLength, chipAddress);

        if(ret >= 0)
        {
            break;
        }

        if( i == retryCnt )
        {
            SIS_LOG_E(SiSLog::getOwnerSiS(), TAG, "out of retry CMD 83");
        }
        else
        {
            SIS_LOG_W(SiSLog::getOwnerSiS(), TAG, "retry CMD 83 : %d of %d", i+1, retryCnt);
            usleep( CMD_RETRY_DELAY * 1000 );	// ms
        }
    }

    return ret;
}

