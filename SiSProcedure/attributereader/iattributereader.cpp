#include "iattributereader.h"

#include "sislog.h"
#include "spexception/spexception.h"
#include "spbase/sisproceduredefine.h"

#include <string.h>

#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif // win32

using namespace SiS::DeviceIO;
using namespace SiS::Procedure;

#define EXCEPTION_TITLE "IAttributeReader Exception : "
#define TAG "IAttributeReader"

IAttributeReader::IAttributeReader(SiSDeviceIO* sisDeviceIO) :
    m_attributeSource(IAttributeReader::AS_SISCORE),
    m_sisDeviceIO(sisDeviceIO),
    m_ctlReportToOsFlag(DEFAULT_CTL_REPORT_TO_OS),
    m_binBuf(0),
    m_binSize(0),
    m_addressProvider(0),
    m_chipIndex(CI_MASTER)
{

}

IAttributeReader::IAttributeReader(unsigned char* binBuf, int binSize) :
    m_attributeSource(IAttributeReader::AS_BIN),
    m_sisDeviceIO(0),
    m_ctlReportToOsFlag(false),
    m_binBuf(binBuf),
    m_binSize(binSize),
    m_addressProvider(0),
    m_chipIndex(CI_MASTER)
{

}

IAttributeReader::~IAttributeReader()
{
    if( m_addressProvider != 0 )
    {
        delete m_addressProvider;
        m_addressProvider = 0;
    }
}

void
IAttributeReader::setCtlReportToOs(bool ctlReportToOsFlag)
{
    this->m_ctlReportToOsFlag = ctlReportToOsFlag;
}

void
IAttributeReader::setChipIndex(int chipIndex)
{
    this->m_chipIndex = chipIndex;
}

void
IAttributeReader::readAttribute(std::string key, unsigned char* &dataBuf, int &dataSize)
{
    SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "readAttribute: key \"%s\"", key.c_str());

    /* get data size */
    dataSize = getAddressProvider()->getLength(key);

    /* get address */
    unsigned int address = getAddressProvider()->getAddress(key);

    /* call readAddress */
    readAddress(address, dataBuf, dataSize);
}

void
IAttributeReader::readAddress(unsigned int address, unsigned char* &dataBuf, int dataSize)
{
    /* malloc data buf */
    dataBuf = new unsigned char[dataSize];
    memset( dataBuf, 0x0, dataSize * sizeof(unsigned char) );

    /* read attribute from source */
    if( m_attributeSource == IAttributeReader::AS_SISCORE )
    {
        SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "readAddress from SiSCore");
        readAddressFromSiSCore(address, dataBuf, dataSize);
    }
    else
    {
        SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "readAddress from Bin");
        readAddressFromBin(address, dataBuf, dataSize);
    }
}

void
IAttributeReader::readAddressFromBin(unsigned int address, unsigned char* dataBuf, int dataSize)
{
    SIS_LOG_D(SiSLog::getOwnerSiS(), TAG,
              "read (%s): address=0x%08x, dataSize=0x%08x, m_binSize=0x%08x",
              getAddressProviderTableName().c_str(), address, dataSize, m_binSize);

    /* exception */
    if( address < 0 || (address + dataSize) > m_binSize)
    {
        std::string msg = EXCEPTION_TITLE;
        char errorMsg[1024] = "";
        sprintf(errorMsg,
                "read out of range (address=0x%08x, dataSize=0x%08x, m_binSize=0x%08x)",
                address, dataSize, m_binSize);
        msg.append(errorMsg);
        throw SPException( msg );
    }

    /* assign data */
    for(int i = 0; i < dataSize; i++)
    {
        dataBuf[i] = m_binBuf[ address + i ];
    }

    /* print data */
    if( SiSLog::getOwnerSiS()->isLOG_D() )
    {
        SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "dataBuf:");
        std::string dataStr;
        for(int i = 0; i < dataSize; i++)
        {
            char bData[10] = "";
            sprintf(bData, "%02x ", dataBuf[i]);
            dataStr.append(bData);

            if( i % 16 == 15 || i == dataSize - 1)
            {
                SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "%s", dataStr.c_str());
                dataStr = "";
            }
        }
    }
}

void
IAttributeReader::checkDataSize(int expectSize, int realSize)
{
    if( expectSize != realSize )
    {
        std::string msg = EXCEPTION_TITLE;
        char errorMsg[1024] = "";
        sprintf(errorMsg, "checkDataSize FAIL, expectSize(%d) != realSize(%d)\n", expectSize, realSize);
        msg.append(errorMsg);
        throw SPException( msg );
    }
}

void
IAttributeReader::milliSleep(int msec)
{
#ifdef WIN32
    Sleep(msec);
#else
    usleep(msec * 1000);
#endif // win32
}
