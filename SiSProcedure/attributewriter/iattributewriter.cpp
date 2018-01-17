#include "iattributewriter.h"

#include "sislog.h"
#include "spexception/spexception.h"
#include "spbase/sisproceduredefine.h"

using namespace SiS::Procedure;

#define EXCEPTION_TITLE "IAttributeWriter Exception : "
#define TAG "IAttributeWriter"

IAttributeWriter::IAttributeWriter(SiSDeviceIO* sisDeviceIO) :
    m_attributeSource(IAttributeReader::AS_SISCORE),
    m_sisDeviceIO(sisDeviceIO),
    m_ctlReportToOsFlag(DEFAULT_CTL_REPORT_TO_OS),
    m_binBuf(0),
    m_binSize(0),
    m_addressProvider(0),
    m_chipIndex(CI_MASTER)
{

}

IAttributeWriter::IAttributeWriter(unsigned char* binBuf, int binSize) :
    m_attributeSource(IAttributeReader::AS_BIN),
    m_sisDeviceIO(0),
    m_ctlReportToOsFlag(false),
    m_binBuf(binBuf),
    m_binSize(binSize),
    m_addressProvider(0),
    m_chipIndex(CI_MASTER)
{

}

IAttributeWriter::~IAttributeWriter()
{
    if( m_addressProvider != 0 )
    {
        delete m_addressProvider;
        m_addressProvider = 0;
    }
}

void
IAttributeWriter::setCtlReportToOs(bool ctlReportToOsFlag)
{
    this->m_ctlReportToOsFlag = ctlReportToOsFlag;
}

void
IAttributeWriter::setChipIndex(int chipIndex)
{
    this->m_chipIndex = chipIndex;
}

void
IAttributeWriter::writeAttribute(std::string key, unsigned char* dataBuf, int dataSize)
{
    SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "writeAttribute: key \"%s\"", key.c_str() );

    /* get data size */
    int expectSize = getAddressProvider()->getLength(key);

    /* check if match size */
    IAttributeReader::checkDataSize(expectSize, dataSize);

    /* get address */
    unsigned int address = getAddressProvider()->getAddress(key);

    /* call writeAddress */
    writeAddress(address, dataBuf, dataSize);
}

void
IAttributeWriter::writeAddress(unsigned int address, unsigned char* dataBuf, int dataSize)
{
    /* write attribute from source */
    if( m_attributeSource == IAttributeReader::AS_SISCORE )
    {
        SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "writeAddress from SiSCore" );
        writeAddressFromSiSCore(address, dataBuf, dataSize);
    }
    else
    {
        SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "writeAddress from Bin" );
        writeAddressFromBin(address, dataBuf, dataSize);
    }
}

void
IAttributeWriter::writeAddressFromBin(unsigned int address, unsigned char* dataBuf, int dataSize)
{
    SIS_LOG_D(SiSLog::getOwnerSiS(), TAG,
              "write (%s): address=0x%08x, dataSize=0x%08x, m_binSize=0x%08x",
              getAddressProviderTableName().c_str(), address, dataSize, m_binSize );

    /* exception */
    if( address < 0 || (address + dataSize) > m_binSize)
    {
        std::string msg = EXCEPTION_TITLE;
        char errorMsg[1024] = "";
        sprintf(errorMsg,
                "write out of range (address=0x%08x, dataSize=0x%08x, m_binSize=0x%08x)",
                address, dataSize, m_binSize);
        msg.append(errorMsg);
        throw SPException( msg );
    }

    /* assign data */
    for(int i = 0; i < dataSize; i++)
    {
        m_binBuf[ address + i ] = dataBuf[i];
    }

    /* print data */
    if( SiSLog::getOwnerSiS()->isLOG_D() )
    {
        SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "write succeed, binBuf address(%0x08x) :", address );
        std::string dataStr;
        for(int i = 0; i < dataSize; i++)
        {
            char bData[10] = "";
            sprintf(bData, "%02x ", m_binBuf[ address + i ]);
            dataStr.append(bData);

            if( i % 16 == 15 || i == dataSize - 1)
            {
                SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "%s", dataStr.c_str() );
                dataStr = "";
            }
        }
    }
}

