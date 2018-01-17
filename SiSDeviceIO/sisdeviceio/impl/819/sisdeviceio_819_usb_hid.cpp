#include "sisdeviceio_819_usb_hid.h"

/*=============================================================================*/
/* hid-example.c */
/*
* Hidraw Userspace Example
*
* Copyright (c) 2010 Alan Ott <alan@signal11.us>
* Copyright (c) 2010 Signal 11 Software
*
* The code may be used by anyone for any purpose,
* and can serve as a starting point for developing
* applications using hidraw.
*/
/* Linux */
#include <linux/types.h>
#include <linux/input.h>
#include <linux/hidraw.h>

/*
 * Ugly hack to work around failing compilation on systems that don't
 * yet populate new version of hidraw.h to userspace.
 *
 * If you need this, please have your distro update the kernel headers.
 */
#ifndef HIDIOCSFEATURE
#define HIDIOCSFEATURE(len)    _IOC(_IOC_WRITE|_IOC_READ, 'H', 0x06, len)
#define HIDIOCGFEATURE(len)    _IOC(_IOC_WRITE|_IOC_READ, 'H', 0x07, len)
#endif

/* Unix */
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

/* C */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
/*=============================================================================*/

#include "sislog.h"
#include "sisdeviceio/sisdeviceexception.h"

#include <errno.h>

/* cmd info */
#define SLAVE_HEADER_LENGTH 4
#define PAYLOAD_INDEX_MASTER 4
#define PAYLOAD_INDEX_SLAVE 8
#define DATA_86_START_INDEX_MASTER 0x8
#define DATA_86_START_INDEX_SLAVE 0x8

using namespace SiS;
using namespace SiS::DeviceIO;

#define TAG "SiSDeviceIO_819_usb_hid"

SiSDeviceIO_819_usb_hid::SiSDeviceIO_819_usb_hid(SiSDeviceMgr* sisDeviceMgr, SiSIOAttribute* sisIOAttribute) :
    ISiSDeviceIO_819_usb()
{
    m_sisDeviceMgr = sisDeviceMgr;
    m_sisIOAttribute = sisIOAttribute;

    /* malloc m_buffer */
    m_buffer = new unsigned char[MAX_BUFFER_SIZE_819_USB];
    initBuffer();
}

SiSDeviceIO_819_usb_hid::~SiSDeviceIO_819_usb_hid()
{
    closeDevice();
    delete [] m_buffer;
}

void
SiSDeviceIO_819_usb_hid::initBuffer()
{
    memset(m_buffer, 0x0, sizeof(unsigned char) * MAX_BUFFER_SIZE_819_USB);
}

int 
SiSDeviceIO_819_usb_hid::jumpBootloader(int chipAddress)
{
    int ret = -1;
    ret = call85WithRetry(JUMP_BOOTLOADER, chipAddress);

    return ret;
}

int 
SiSDeviceIO_819_usb_hid::openDevice()
{
    SiSDeviceAttribute* opened = m_sisDeviceMgr->getOpened();

    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "openDevice : %s",  opened->getNodeName().c_str() );

    if( opened->getNodeName().empty() )
    {
        throw SiSDeviceException("openDevice, nodeName is empty", -1);
    }

    const char* devName = opened->getNodeName().c_str();

    if(m_fd >= 0)
    {
        close(m_fd);
        m_fd = -1;
    }

    if(isUsingIOCTL())
    {
        SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "isUsingIOCTL : true");
        m_fd = open(devName, O_RDWR | O_NONBLOCK); // ioctl : O_NONBLOCK flag has no effect
    }
    else
    {
        SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "isUsingIOCTL : false");
        m_fd = open(devName, O_RDWR | O_NONBLOCK);
    }

    /* handle error */
    if (m_fd < 0) 
    {
        std::string msg = "";
        char errorMsg[1024] = "";
        sprintf(errorMsg, "openDevice, unable to open device, errno=%d (%s)", errno, strerror(errno));
        msg.append(errorMsg);
        throw SiSDeviceException(msg, -1);

        return m_fd;
    }

    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "m_fd : %d",  m_fd );

    return m_fd;
}

int 
SiSDeviceIO_819_usb_hid::closeDevice()
{
    if( m_fd < 0 )
    {
        return 0;
    }

    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "closeDevice, m_fd : %d",  m_fd);

    int ret = 0;
    if(m_fd >= 0)
    {
        ret = close(m_fd);
        m_fd = -1;
    }

    /* handle error */
    if (ret < 0) 
    {
        std::string msg = "";
        char errorMsg[1024] = "";
        sprintf(errorMsg, "closeDevice, unable to close device, errno=%d (%s)", errno, strerror(errno));
        msg.append(errorMsg);
        throw SiSDeviceException(msg, -1);

        return m_fd;
    }
	
    return ret;
}

int
SiSDeviceIO_819_usb_hid::writeData(unsigned char * buf, int size, int timeoutMsec)
{
    int ret = 0;

    if(isUsingIOCTL())
    {
        ret = ioctl(m_fd, HIDIOCSFEATURE(size), buf);
    }
    else
    {
        ret = write(m_fd, buf, size);
    }
    
    return ret;
}

int
SiSDeviceIO_819_usb_hid::readData( unsigned char * buf, int size, int timeoutMsec)
{
    int ret = 0;

    if(isUsingIOCTL())
    {
        ret = ioctl(m_fd, HIDIOCGFEATURE(size), buf);
    }
    else
    {
        ret = read(m_fd, buf, size);
    }

    return ret;
}

bool
SiSDeviceIO_819_usb_hid::isUsingIOCTL()
{
    return true;
}

int
SiSDeviceIO_819_usb_hid::readSimpleAck(int size, int chipAddress)
{
    if(chipAddress <= MASTER_ADDRESS)
    {
        return readSimpleAckMaster(size, chipAddress);
    }
    else
    {
        return readSimpleAckSlave(size, chipAddress);
    }
}

int
SiSDeviceIO_819_usb_hid::readSimpleAckMaster(int size, int chipAddress)
{
    int length = (m_buffer[1] << 8) | m_buffer[2];

    SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "readSimpleAck, length=%d", length);

    m_crc = 0;

	/* BEEF/DEAD */
    if( m_buffer[4] == 0xBE && m_buffer[5] == 0xEF )
    {
        SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "readSimpleAck success : BEEF");
        return ACK_BEEF;
    }
    else if( m_buffer[4] == 0xDE && m_buffer[5] == 0xAD )
    {
        SIS_LOG_E(SiSLog::getOwnerSiS(), TAG, "readSimpleAck error : DEAD");
        return -1;
    }

    SIS_LOG_E(SiSLog::getOwnerSiS(), TAG, "readSimpleAck error : return not recognized format");
    return -1;
}

int
SiSDeviceIO_819_usb_hid::readSimpleAckSlave(int size, int chipAddress)
{
    int length = (m_buffer[1] << 8) | m_buffer[2];

    SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "readSimpleAck, length=%d", length);

    m_crc = 0;

    /* CRC */
    for ( int i = 4; i < length && i < static_cast<int>(getMaxBufferSize()); i++ )
    {
        appendCRC( m_buffer[i] );
    }

    if ( m_buffer[3] != ( m_crc & 0xff ) )
    {
        SIS_LOG_E(SiSLog::getOwnerSiS(), TAG, "readSimpleAck error : ERROR_CRC_MISMATCH");
        return -1;
    }

    /* BEEF/DEAD */
    if( m_buffer[4] == 0xBE && m_buffer[5] == 0xEF )
    {
        SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "readSimpleAck success : BEEF");
        return ACK_BEEF;
    }
    else if( m_buffer[4] == 0xDE && m_buffer[5] == 0xAD )
    {
        SIS_LOG_E(SiSLog::getOwnerSiS(), TAG, "readSimpleAck error : DEAD");
        return -1;
    }
	
    SIS_LOG_E(SiSLog::getOwnerSiS(), TAG, "readSimpleAck error : return not recognized format");
    return -1;

}

int
SiSDeviceIO_819_usb_hid::makeCommonHeader(int cmd, int length, int chipAddress)
{
    if(chipAddress <= MASTER_ADDRESS)
    {
        return makeCommonHeaderMaster(cmd, length, chipAddress);
    }
    else
    {
        return makeCommonHeaderSlave(cmd, length, chipAddress);
    }
}

int 
SiSDeviceIO_819_usb_hid::makeCommonHeaderMaster(int cmd, int length, int chipAddress)
{
    int byteCnt = length;

    m_buffer[0] = getReportId(byteCnt);		// out report id
    m_buffer[1] = (byteCnt >> 8) & 0xff; 	// Length field (MSB)
    m_buffer[2] = (byteCnt >> 0) & 0xff;	// Length field (LSB)
    m_buffer[3] = cmd;						// cmd

    return PAYLOAD_INDEX_MASTER; // payloadIndex
}

int
SiSDeviceIO_819_usb_hid::makeCommonHeaderSlave(int cmd, int length, int chipAddress)
{
    int byteCnt = length;

    m_buffer[0] = getReportId(byteCnt);		// out report id
    m_buffer[1] = (byteCnt >> 8) & 0xff; 	// Length field (MSB)
    m_buffer[2] = (byteCnt >> 0) & 0xff;	// Length field (LSB)
    m_buffer[3] = 0xb0;						// 0xb0
    m_buffer[4] = 0x0;						// CRC
    m_buffer[5] = chipAddress;				// Slave addr
    m_buffer[6] = 0x0;						// Reserved
    m_buffer[7] = cmd;						// cmd

    return PAYLOAD_INDEX_SLAVE;
}

void
SiSDeviceIO_819_usb_hid::generateOutputCRC(int chipAddress)
{
    if(chipAddress <= MASTER_ADDRESS)
    {
        return generateOutputCRCMaster(chipAddress);
    }
    else
    {
        return generateOutputCRCSlave(chipAddress);
    }
}

void
SiSDeviceIO_819_usb_hid::generateOutputCRCMaster(int chipAddress)
{
    // 819 usb : Master CRC not needed 
}

void
SiSDeviceIO_819_usb_hid::generateOutputCRCSlave(int chipAddress)
{
    int byteCnt = (m_buffer[1] << 8) | m_buffer[2];

    m_crc = 0;

    for ( int i = 7; i < byteCnt && i < static_cast<int>(getMaxBufferSize()); i++ )
    {
        appendCRC( m_buffer[i] );
    }

    m_buffer[4] = m_crc & 0xff;
}

int
SiSDeviceIO_819_usb_hid::makeBuffer85(int data, int chipAddress)
{
    initBuffer();

    int len = 0x6;
    len = ( chipAddress == MASTER_ADDRESS ? len : len + SLAVE_HEADER_LENGTH );

    int payloadIndex = makeCommonHeader( 0x85, len , chipAddress );

    m_buffer[payloadIndex + 0] = ( data >> 8 ) & 0xff;	// Mode (MSB)
    m_buffer[payloadIndex + 1] = ( data >> 0 ) & 0xff;	// Mode (LSB)

    generateOutputCRC(chipAddress);

    return len;
}

int 
SiSDeviceIO_819_usb_hid::formatData86Unit(int size)
{
    if( size <= DATA_86_UNIT_64 )
    {
        return DATA_86_UNIT_64;
    }
    else
    {
        return DATA_86_UNIT_832;
    }

    return DATA_86_UNIT_832;
}

int
SiSDeviceIO_819_usb_hid::makeBuffer86(unsigned int address, int size, int chipAddress)
{
    initBuffer();

    int len = 0xb;
    len = ( chipAddress == MASTER_ADDRESS ? len : len + SLAVE_HEADER_LENGTH );

    int payloadIndex = makeCommonHeader( 0x86, len, chipAddress );
    
    m_buffer[payloadIndex + 0] = ( address >> 24 ) & 0xff;
    m_buffer[payloadIndex + 1] = ( address >> 16 ) & 0xff;
    m_buffer[payloadIndex + 2] = ( address >>  8 ) & 0xff;
    m_buffer[payloadIndex + 3] = ( address >>  0 ) & 0xff;
    m_buffer[payloadIndex + 4] = ( formatData86Unit(size) >> 8 ) & 0xff; // ReadLength (MSB)
    m_buffer[payloadIndex + 5] = ( formatData86Unit(size) >> 0 ) & 0xff; // ReadLength (LSB)
    m_buffer[payloadIndex + 6] = 0x03;

    generateOutputCRC(chipAddress);

    return len;
}

int 
SiSDeviceIO_819_usb_hid::getData86StartIndex(int chipAddress)
{
    if(chipAddress <= MASTER_ADDRESS)
    {
        return DATA_86_START_INDEX_MASTER;
    }
    else
    {
        return DATA_86_START_INDEX_SLAVE;
    }
}

int
SiSDeviceIO_819_usb_hid::makeBuffer82()
{
    initBuffer();

    int len = 0x4;

    int payloadIndex = makeCommonHeader( 0x82, len, MASTER_ADDRESS );

    generateOutputCRC(MASTER_ADDRESS);

    return len;
}

int
SiSDeviceIO_819_usb_hid::makeBuffer84(unsigned int pageAddress, unsigned char* data, int size, int chipAddress)
{
    initBuffer();

    int len = 0x8 + size;
    len = ( chipAddress == MASTER_ADDRESS ? len : len + SLAVE_HEADER_LENGTH );

    int payloadIndex = makeCommonHeader( 0x84, len , chipAddress );

    m_buffer[payloadIndex + 0] = ( pageAddress >> 8 ) & 0xff;	// Page addr (MSB)
    m_buffer[payloadIndex + 1] = ( pageAddress >> 0 ) & 0xff;	// Page addr (LSB)
    m_buffer[payloadIndex + 2] = 0x0; // Reserved (0x0)
    m_buffer[payloadIndex + 3] = 0x0; // Reserved (0x0)
    memcpy( m_buffer + payloadIndex + 4, data, size * sizeof(unsigned char) ); // D0 ~ D767

    generateOutputCRC(chipAddress);

    return len;
}

int
SiSDeviceIO_819_usb_hid::comparePageAddrAck(unsigned int pageAddress, int chipAddress)
{
    if(chipAddress <= MASTER_ADDRESS)
    {
        return comparePageAddrAckMaster(pageAddress, chipAddress);
    }
    else
    {
        return comparePageAddrAckSlave(pageAddress, chipAddress);
    }
}

int
SiSDeviceIO_819_usb_hid::comparePageAddrAckMaster(unsigned int pageAddress, int chipAddress)
{
    int pageAddrM = ( pageAddress >> 8 ) & 0xff;
    int pageAddrL = ( pageAddress >> 0 ) & 0xff;

    if( m_buffer[8] == pageAddrM && m_buffer[9] == pageAddrL )
    {
        SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "comparePageAddrAck success : %02x %02x", pageAddrM, pageAddrL);
        return ACK_BEEF;
    }

    SIS_LOG_E(SiSLog::getOwnerSiS(), TAG, "comparePageAddrAck error : byte(8, 9) different");
    return -1;
}

int
SiSDeviceIO_819_usb_hid::comparePageAddrAckSlave(unsigned int pageAddress, int chipAddress)
{
    int pageAddrM = ( pageAddress >> 8 ) & 0xff;
    int pageAddrL = ( pageAddress >> 0 ) & 0xff;

    if( m_buffer[8] == pageAddrM && m_buffer[9] == pageAddrL )
    {
        SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "comparePageAddrAck success : %02x %02x", pageAddrM, pageAddrL);
        return ACK_BEEF;
    }

    SIS_LOG_E(SiSLog::getOwnerSiS(), TAG, "comparePageAddrAck error : byte(8, 9) different");
    return -1;
}

int
SiSDeviceIO_819_usb_hid::makeBuffer81(unsigned int pageStartAddress, unsigned int pageLength, int chipAddress)
{
    initBuffer();

    int len = 0x8;
    len = ( chipAddress == MASTER_ADDRESS ? len : len + SLAVE_HEADER_LENGTH );

    int payloadIndex = makeCommonHeader( 0x81, len , chipAddress );

    m_buffer[payloadIndex + 0] = ( pageStartAddress >> 8 ) & 0xff;	// Page Start addr (MSB)
    m_buffer[payloadIndex + 1] = ( pageStartAddress >> 0 ) & 0xff;	// Page Start addr (LSB)
    m_buffer[payloadIndex + 2] = ( pageLength >> 8 ) & 0xff; // page Length (MSB)
    m_buffer[payloadIndex + 3] = ( pageLength >> 0 ) & 0xff; // page Length (LSB)

    generateOutputCRC(chipAddress);

    return len;
}

