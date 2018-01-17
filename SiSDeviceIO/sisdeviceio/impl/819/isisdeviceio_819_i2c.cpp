#include "isisdeviceio_819_i2c.h"

/* C */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include "sislog.h"
#include "sisdeviceio/sisdeviceexception.h"

using namespace SiS;
using namespace SiS::DeviceIO;

#define TAG "ISiSDeviceIO_819_i2c"

ISiSDeviceIO_819_i2c::ISiSDeviceIO_819_i2c() :
    ISiSDeviceIO()
{

}

ISiSDeviceIO_819_i2c::~ISiSDeviceIO_819_i2c()
{

}

int
ISiSDeviceIO_819_i2c::burnToROM(unsigned int address, int size, unsigned char* binBuffer, int binSize, int chipAddress)
{
    int dataLength = DATA_84_UNIT_64;
    unsigned char* tempBuf = new unsigned char[dataLength];
    memset(tempBuf, 0x0, sizeof(unsigned char) * dataLength);

    int ret = -1;
    int sendLegnth = 0;

    while ( sendLegnth < size )
    {
        unsigned int blockSize = size > sendLegnth + __8K ? __8K : size - sendLegnth;
        sendLegnth += blockSize;

        int pageNum = blockSize / dataLength + (blockSize % dataLength == 0 ? 0 : 1 );
        int addressBase = address;

        for(int blockRetry = 0; blockRetry < m_sisIOAttribute->getBlockRetry(); blockRetry++)
        {
            SIS_LOG_I( SiSLog::getOwnerSiS(), TAG, "blockRetry : %d of max %d", blockRetry+1, m_sisIOAttribute->getBlockRetry() );
            SIS_LOG_I( SiSLog::getOwnerSiS(), TAG, "Write to address = %08x, pageNum=%d ", address, pageNum );

            /* cmd_83 */
            ret = call83WithRetry( addressBase, pageNum, chipAddress ) ;
            if ( ret < 0 )
            {
                SIS_LOG_E(SiSLog::getOwnerSiS(), TAG, "Write Header(0x83) fail" );	
                continue;
            }

            ret = -1;
            int pageIdx = 0;

            /* cmd_84 */
            while(pageIdx < pageNum)
            {
                for(int i = 0; i < dataLength; i++)
                {
                    int idx = pageIdx * dataLength + i;
                    tempBuf[i] = ( (idx < blockSize) && (idx + addressBase < binSize ) ) ? binBuffer[idx + addressBase] : 0;
                }

                /* inverse int */
                for(int idx = 0; idx < dataLength / sizeof(int); idx++)
                {
                    unsigned char tmp0 = tempBuf[ (idx * sizeof(int)) + 0];
                    unsigned char tmp1 = tempBuf[ (idx * sizeof(int)) + 1];
                    tempBuf[ (idx * sizeof(int)) + 0] = tempBuf[ (idx * sizeof(int)) + 3];
                    tempBuf[ (idx * sizeof(int)) + 1] = tempBuf[ (idx * sizeof(int)) + 2];
                    tempBuf[ (idx * sizeof(int)) + 2] = tmp1;
                    tempBuf[ (idx * sizeof(int)) + 3] = tmp0;
                }

				int remaining = blockSize - pageIdx * dataLength;
                int tmpSize = remaining < dataLength ? remaining : dataLength;

                printf(".");
                fflush(stdout);
				
                ret = call84( 0, tempBuf,  tmpSize, chipAddress );
				
                if ( ret < 0 )
                {
                    break;
                }

                pageIdx ++;
            }

            printf( "\n" );

            if ( ret < 0 )
            {
                SIS_LOG_E(SiSLog::getOwnerSiS(), TAG, "Write(0x84) fail" );			
                continue;
            }

            /* cmd_81 */
            ret = call81( 0,  0, chipAddress );

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
ISiSDeviceIO_819_i2c::getMaxBufferSize()
{
    return MAX_BUFFER_SIZE_819_I2C;
}

int
ISiSDeviceIO_819_i2c::getReportIdIndex(bool isOut)
{
    if(isOut)
    {
        return 4;
    }
	else
    {
        return 2;
    }
}

int
ISiSDeviceIO_819_i2c::getReportId(int size, bool isOut)
{
    if(isOut)
    {
        return REPORT_ID_OUT;
    }
	else
    {
        return REPORT_ID_IN;
    }
}

int
ISiSDeviceIO_819_i2c::getBufferSizeOfReportId(int reportId)
{
    if( reportId == REPORT_ID_OUT )
    {
        return REPORT_ID_OUT_SIZE;
    }
    else if( reportId == REPORT_ID_IN )
    {
        return REPORT_ID_IN_SIZE;
    }

    return REPORT_ID_OUT_SIZE;
}

int
ISiSDeviceIO_819_i2c::call85(int data, int chipAddress)
{
    int ret = -1;

    SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "call85() : ");
    SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "data=%08x", data);

    int len = makeBuffer85( data, chipAddress );
    int cmd = m_buffer[6];
    int reportId = m_buffer[4];

    ret = ioCommand( cmd, m_buffer, len, REPORT_ID_IN_SIZE);

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
ISiSDeviceIO_819_i2c::call86(unsigned int address, unsigned char* data, int size, int chipAddress)
{
    int ret = -1;

    SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "call86() : ");
    SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "address=0x%08x, size=0x%02x", address, size);
		
    int len = makeBuffer86( address, size, chipAddress );
    int cmd = m_buffer[6];
    int reportId = m_buffer[4];

    ret = ioCommand( cmd, m_buffer, len, REPORT_ID_IN_SIZE );

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
ISiSDeviceIO_819_i2c::call82()
{
    int ret = -1;

    SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "call82() : ");

    int len = makeBuffer82();
    int cmd = m_buffer[6];
    int reportId = m_buffer[4];

    ret = ioCommand( cmd, m_buffer, len, 0 );

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
ISiSDeviceIO_819_i2c::call84(unsigned int pageAddress, unsigned char* data, int size, int chipAddress)
{
    int ret = -1;

    SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "call84() : ");
    SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "data=%08x,%08x,%08x,..., size=0x%08x", data[0], data[1], data[2], size);

    int len = makeBuffer84( pageAddress, data,  size, chipAddress );
    int cmd = m_buffer[6];
    int reportId = m_buffer[4];

    ret = ioCommand( cmd, m_buffer, len, REPORT_ID_IN_SIZE );

    if( ret < 0 )
    {
        return ret;
    }
    else
    {
        ret = readSimpleAck(ret, chipAddress);

        //if( ret == ACK_BEEF)
        //{
            //ret = comparePageAddrAck(pageAddress, chipAddress);
        //}
    }

    return ret;
}

int 
ISiSDeviceIO_819_i2c::call81(unsigned int pageStartAddress, unsigned int pageLength, int chipAddress)
{
    int ret = -1;

    SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "call81() : ");
    //SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "pageStartAddress=0x%08x, pageLength=0x%08x", pageStartAddress, pageLength);

    int len = makeBuffer81( pageStartAddress, pageLength, chipAddress );
    int cmd = m_buffer[6];
    int reportId = m_buffer[4];

    ret = ioCommand( cmd, m_buffer, len, REPORT_ID_IN_SIZE );

    if( m_sisIOAttribute->getIODelayOf81() > 0 )
    {
        usleep( m_sisIOAttribute->getIODelayOf81() * 1000 );	// ms
    }

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
ISiSDeviceIO_819_i2c::call83(unsigned int pageStartAddress, unsigned int pageLength, int chipAddress)
{
    int ret = -1;

    SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "call83() : ");
    SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "pageStartAddress=0x%08x, pageLength=0x%08x", pageStartAddress, pageLength);

    int len = makeBuffer83( pageStartAddress, pageLength, chipAddress );
    int cmd = m_buffer[6];
    int reportId = m_buffer[4];

    ret = ioCommand( cmd, m_buffer, len, REPORT_ID_IN_SIZE );

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


