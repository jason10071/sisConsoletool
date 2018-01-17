#include "isisdeviceio_819_usb.h"

/* C */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include "sislog.h"
#include "sisdeviceio/sisdeviceexception.h"

using namespace SiS;
using namespace SiS::DeviceIO;

#define TAG "ISiSDeviceIO_819_usb"

ISiSDeviceIO_819_usb::ISiSDeviceIO_819_usb() :
    ISiSDeviceIO()
{

}

ISiSDeviceIO_819_usb::~ISiSDeviceIO_819_usb()
{

}

int
ISiSDeviceIO_819_usb::burnToROM(unsigned int address, int size, unsigned char* binBuffer, int binSize, int chipAddress)
{
    int dataLength = DATA_84_UNIT_832;
    unsigned char* tempBuf = new unsigned char[dataLength];
    memset(tempBuf, 0x0, sizeof(unsigned char) * dataLength);

    int ret = -1;
    int sendLegnth = 0;

    while ( sendLegnth < size )
    {
        unsigned int blockSize = size > sendLegnth + __16K ? __16K : size - sendLegnth;
        sendLegnth += blockSize;

        if ( blockSize % PAGE_SIZE != 0 ) 
        {
            delete[] tempBuf;
				
            std::string msg = "";
            char errorMsg[1024] = "";
            sprintf(errorMsg, "burnToROM(), blockSize (%d) is not a multiple of %d", blockSize, PAGE_SIZE);
            msg.append(errorMsg);
            throw SiSDeviceException(msg, -1);
            return -1;
        }

        int pageNum = blockSize / PAGE_SIZE;
        int addressBase = address / PAGE_SIZE;

        for(int blockRetry = 0; blockRetry < m_sisIOAttribute->getBlockRetry(); blockRetry++)
        {
            SIS_LOG_I( SiSLog::getOwnerSiS(), TAG, "blockRetry : %d of max %d", blockRetry+1, m_sisIOAttribute->getBlockRetry() );
            SIS_LOG_I( SiSLog::getOwnerSiS(), TAG, "Write to address = %08x, pageNum=%d ", address, pageNum );

            ret = -1;
            int pageIdx = 0;
            int pageAddr = addressBase;
            int appendPage = 0;

            /* cmd_84 */
            while(pageIdx < pageNum)
            {
                for(int i = 0; i < PAGE_SIZE; i++)
                {
                    tempBuf[appendPage * PAGE_SIZE + i] = binBuffer[address + pageIdx * PAGE_SIZE + i];
                }

                if( appendPage == 2 || (pageIdx == pageNum -1) )  // max 3 page
                {
                    printf(".");
                    fflush(stdout);

                    int tmpSize = (appendPage + 1) * PAGE_SIZE;

                    ret = call84( pageAddr, tempBuf,  tmpSize, chipAddress );

                    if ( ret < 0 )
                    {
                        break;
                    }

                    pageAddr += (appendPage + 1);
                    //memset(tempBuf, 0x0, sizeof(unsigned char) * dataLength);
                }

                pageIdx ++;
                appendPage = pageIdx % 3;
            }

            printf( "\n" );

            if ( ret < 0 )
            {
                SIS_LOG_E(SiSLog::getOwnerSiS(), TAG, "Write(0x84) fail" );			
                continue;
            }

            /* cmd_81 */
            ret = call81( addressBase,  pageNum, chipAddress );

            //SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "wait for Query(0x81)");
            //usleep(100 * 1000);

            if ( ret < 0 )
            {
                SIS_LOG_E(SiSLog::getOwnerSiS(), TAG, "Query(0x81) fail");
                continue;
            }
            else
            {
                SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "0x81 success");
                break;
            }
        }

        if( ret < 0 )
        {
            SIS_LOG_E(SiSLog::getOwnerSiS(), TAG, "Write block fail : out of retry");
            break;
        }

        address += blockSize;
    }

    delete[] tempBuf;
    return ( ret < 0 ) ? ret : 1;
}

int
ISiSDeviceIO_819_usb::getMaxBufferSize()
{
    return MAX_BUFFER_SIZE_819_USB;
}

int
ISiSDeviceIO_819_usb::getReportIdIndex(bool isOut)
{
    if(isOut)
    {
        return 0;
    }
	else
    {
        return 0;
    }
}

int
ISiSDeviceIO_819_usb::getReportId(int size, bool isOut)
{
    if( size <= REPORT_ID_21_SIZE )
    {
        return REPORT_ID_21;
    }
    else if( size > REPORT_ID_21_SIZE && size <= REPORT_ID_25_SIZE )
    {
        return REPORT_ID_25;
    }
    else if( size > REPORT_ID_25_SIZE && size <= REPORT_ID_29_SIZE )
    {
        return REPORT_ID_29;
    }
    else if( size > REPORT_ID_29_SIZE && size <= REPORT_ID_2D_SIZE )
    {
        return REPORT_ID_2D;
    }

    return REPORT_ID_2D;
}

int
ISiSDeviceIO_819_usb::getBufferSizeOfReportId(int reportId)
{
    if( reportId == REPORT_ID_21 )
    {
        return REPORT_ID_21_SIZE;
    }
    else if( reportId == REPORT_ID_25 )
    {
        return REPORT_ID_25_SIZE;
    }
    else if( reportId == REPORT_ID_29 )
    {
        return REPORT_ID_29_SIZE;
    }
    else if( reportId == REPORT_ID_2D )
    {
        return REPORT_ID_2D_SIZE;
    }

    return REPORT_ID_21_SIZE;
}

int
ISiSDeviceIO_819_usb::call85(int data, int chipAddress)
{
    int ret = -1;

    SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "call85() : ");
    SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "data=%08x", data);

    int len = makeBuffer85( data, chipAddress );
    int cmd = m_buffer[3];
    int reportId = m_buffer[0];

    ret = ioCommand( cmd, m_buffer, getBufferSizeOfReportId(reportId), REPORT_ID_21_SIZE );

    if( ret < 0 )
    {
        return ret;
    }
    else
    {
        ret = readSimpleAck(ret, chipAddress);
    }

    return ret;
}

int
ISiSDeviceIO_819_usb::call86(unsigned int address, unsigned char* data, int size, int chipAddress)
{
    int ret = -1;

    SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "call86() : ");
    SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "address=0x%08x, size=0x%02x", address, size);
		
    int len = makeBuffer86( address, size, chipAddress );
    int cmd = m_buffer[3];
    int reportId = m_buffer[0];

    int readSize = formatData86Unit(size) == DATA_86_UNIT_832 ? REPORT_ID_2D_SIZE : REPORT_ID_21_SIZE;
	
    ret = ioCommand( cmd, m_buffer, getBufferSizeOfReportId(reportId), readSize );

    if( ret < 0 )
    {
        return ret;
    }
    else
    {
        int dataStartIdx = getData86StartIndex(chipAddress);
        unsigned int dataSize = ret - dataStartIdx;

        ret = readSimpleAck( ret, chipAddress );

        if ( ret == ACK_BEEF )
        {
                dataSize = ( dataSize > formatData86Unit(size) ) ? formatData86Unit(size) : dataSize;

                for (unsigned int i = 0; i < dataSize && i < static_cast<unsigned int>(size); i++)
                {
                    data[i] = m_buffer[i + dataStartIdx];
                }
        }
    }

    return ret;
}

int
ISiSDeviceIO_819_usb::call82()
{
    int ret = -1;

    SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "call82() : ");

    int len = makeBuffer82();
    int cmd = m_buffer[3];
    int reportId = m_buffer[0];

    ret = ioCommand( cmd, m_buffer, getBufferSizeOfReportId(reportId), 0 );

    if( ret < 0 )
    {
        return ret;
    }
    else
    {
        //ret = readSimpleAck(ret, MASTER_ADDRESS);
    }

    return ret;
}

int 
ISiSDeviceIO_819_usb::call84(unsigned int pageAddress, unsigned char* data, int size, int chipAddress)
{
    int ret = -1;

    SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "call84() : ");
    SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "pageAddress=0x%08x, size=0x%08x", pageAddress, size);

    int len = makeBuffer84( pageAddress, data,  size, chipAddress );
    int cmd = m_buffer[3];
    int reportId = m_buffer[0];

    ret = ioCommand( cmd, m_buffer, getBufferSizeOfReportId(reportId), REPORT_ID_21_SIZE );

    if( ret < 0 )
    {
        return ret;
    }
    else
    {
        ret = readSimpleAck(ret, chipAddress);

        if( ret == ACK_BEEF)
        {
            ret = comparePageAddrAck(pageAddress, chipAddress);
        }
    }

    return ret;
}

int 
ISiSDeviceIO_819_usb::call81(unsigned int pageStartAddress, unsigned int pageLength, int chipAddress)
{
    int ret = -1;

    SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "call81() : ");
    SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "pageStartAddress=0x%08x, pageLength=0x%08x", pageStartAddress, pageLength);

    int len = makeBuffer81( pageStartAddress, pageLength, chipAddress );
    int cmd = m_buffer[3];
    int reportId = m_buffer[0];

    ret = ioCommand( cmd, m_buffer, getBufferSizeOfReportId(reportId), REPORT_ID_21_SIZE );

    if( ret < 0 )
    {
        return ret;
    }
    else
    {
        ret = readSimpleAck(ret, chipAddress);
    }

    return ret;
}

