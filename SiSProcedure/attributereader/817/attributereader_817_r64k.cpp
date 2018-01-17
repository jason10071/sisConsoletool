#include "attributereader_817_r64k.h"

#include "sislog.h"
#include "spexception/spexception.h"
#include "spexception/siscoreexception.h"
#include "spbase/sisproceduredefine.h"

#include "addressprovider/817/addressprovider_817_r64k.h"

//#include "CptFactoryFacade.h"

using namespace std;
using namespace SiS::Procedure;
using namespace SiS;

#define EXCEPTION_TITLE "AttributeReader_817_r64k Exception : "
#define TAG "AttributeReader_817_r64k"

AttributeReader_817_r64k::AttributeReader_817_r64k(SiSDeviceIO* sisDeviceIO) :
    IAttributeReader(sisDeviceIO)
{

}

AttributeReader_817_r64k::AttributeReader_817_r64k(unsigned char* binBuf, int binSize) :
    IAttributeReader(binBuf, binSize)
{

}

AttributeReader_817_r64k::~AttributeReader_817_r64k()
{

}

IAddressProvider*
AttributeReader_817_r64k::getAddressProvider()
{
    if( m_addressProvider != 0 )
    {
        return m_addressProvider;
    }
    else
    {
        m_addressProvider = new AddressProvider_817_r64k();
        return m_addressProvider;
    }
}

std::string
AttributeReader_817_r64k::getAddressProviderTableName()
{
    if( m_addressProvider != 0 )
    {
        return m_addressProvider->getTableName();
    }
    else
    {
        return getAddressProvider()->getTableName();
    }
}

int
AttributeReader_817_r64k::mappingChipAddress(int chipIndex)
{
    if( chipIndex == CI_MASTER )
    {
        return 0;
    }
    else
    {
        int chipAddress = 5 + (chipIndex - (int) CI_SLAVE_0);
        SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "mappingChipAddress : chipIndex=%d -> chipAddress=0x%x", chipIndex, chipAddress);
        return chipAddress;
    }
}

void
AttributeReader_817_r64k::readAddressFromSiSCore(unsigned int address, unsigned char* dataBuf, int dataSize)
{
    SIS_LOG_D(SiSLog::getOwnerSiS(), TAG,
              "read (%s) : address=0x%08x, dataSize=0x%08x",
              getAddressProviderTableName().c_str(), address, dataSize);

    /* alignment int (4 bytes) */
    int dataShift = address % 4;
    int dataSizeAlignment = dataSize + dataShift;
    unsigned int addressAlignment = address - dataShift;

    SIS_LOG_D(SiSLog::getOwnerSiS(), TAG,
              "alignment : addressAlignment=0x%08x, dataSizeAlignment=0x%08x, dataShift=0x%08x",
              addressAlignment, dataSizeAlignment, dataShift);

    /* exception */
    if( m_sisDeviceIO == 0 )
    {
        std::string msg = EXCEPTION_TITLE;
        msg.append("m_sisDeviceIO == 0");
        throw SPException( msg );
    }

    bool ret = false;

    /* alloc temp buffer */
    unsigned char* tempBuffer = new unsigned char[dataSizeAlignment];
    memset( tempBuffer, 0x0, dataSizeAlignment * sizeof(unsigned char) );

    /* read data */
    SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "chipIndex=%d, chipAddress=0x%02x", m_chipIndex, mappingChipAddress(m_chipIndex));
    try
    {
//        Component * result = 0;
//        result = SERIAL_DATA(m_sisCore, dataSizeAlignment);
//        int ret = -1;

//        if( m_ctlReportToOsFlag )
//        {
//            if( m_chipIndex == CI_MASTER )
//            {
//                ret = m_sisCore->exe<int>("READ",
//                                          ADDRESS(m_sisCore, addressAlignment),
//                                          READ_LEN(m_sisCore, dataSizeAlignment),
//                                          result);
//            }
//            else
//            {
//                ret = m_sisCore->exe<int>( "READ_S",
//                                           ADDRESS(m_sisCore, addressAlignment),
//                                           READ_LEN(m_sisCore, dataSizeAlignment),
//                                           CHIP(m_sisCore, mappingChipAddress(m_chipIndex)),
//                                           result);
//            }
//        }
//        else
//        {
//            if( m_chipIndex == CI_MASTER )
//            {
//                ret = m_sisCore->exe<int>("BURST_READ",
//                                          ADDRESS(m_sisCore, addressAlignment),
//                                          READ_LEN(m_sisCore, dataSizeAlignment),
//                                          result);
//            }
//            else
//            {
//                ret = m_sisCore->exe<int>( "BURST_READ_S",
//                                           ADDRESS(m_sisCore, addressAlignment),
//                                           READ_LEN(m_sisCore, dataSizeAlignment),
//                                           CHIP(m_sisCore, mappingChipAddress(m_chipIndex)),
//                                           result);
//            }
//        }

        ret = m_sisDeviceIO->readFromAddress(addressAlignment, dataSizeAlignment, tempBuffer, mappingChipAddress(m_chipIndex));

    }
    SISCORE_EXCEPTION_SP_HANDLE
    
    /* handle result */
    SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "handle result:");
    if ( ret )
    {
        /* assign data */
        for(int i = dataShift; i < dataSizeAlignment; i++)
        {
            dataBuf[i - dataShift] =  tempBuffer[i]; //(unsigned char) result->data(i);
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

        delete[] tempBuffer;
    }
    else
    {
        std::string msg = EXCEPTION_TITLE;
        char errorMsg[1024] = "readFromAddress return fail";
        msg.append(errorMsg);
        throw SPException( msg );
    }
		
}

