#include "attributewriter_817_r64k.h"

#include "sislog.h"
#include "spexception/spexception.h"
#include "spexception/siscoreexception.h"
#include "spbase/sisproceduredefine.h"

#include "addressprovider/817/addressprovider_817_r64k.h"

//#include "CptFactoryFacade.h"

using namespace std;
using namespace SiS::Procedure;
using namespace SiS;

#define EXCEPTION_TITLE "AttributeWriter_817_r64k Exception : "
#define TAG "AttributeWriter_817_r64k"

AttributeWriter_817_r64k::AttributeWriter_817_r64k(SiSDeviceIO* m_sisDeviceIO) :
    IAttributeWriter(m_sisDeviceIO)
{

}

AttributeWriter_817_r64k::AttributeWriter_817_r64k(unsigned char* binBuf, int binSize) :
    IAttributeWriter(binBuf, binSize)
{

}

AttributeWriter_817_r64k::~AttributeWriter_817_r64k()
{

}

IAddressProvider*
AttributeWriter_817_r64k::getAddressProvider()
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
AttributeWriter_817_r64k::getAddressProviderTableName()
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
AttributeWriter_817_r64k::mappingChipAddress(int chipIndex)
{
    if( chipIndex == CI_MASTER )
    {
        return 0;
    }
    else
    {
        int chipAddress = 5 + (chipIndex - (int) CI_SLAVE_0);
        SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "mappingChipAddress : chipIndex=%d -> chipAddress=0x%x", chipIndex, chipAddress );
        return chipAddress;
    }
}

void
AttributeWriter_817_r64k::writeAddressFromSiSCore(unsigned int address, unsigned char* dataBuf, int dataSize)
{
    SIS_LOG_D(SiSLog::getOwnerSiS(), TAG,
              "write (%s) : address=0x%08x, dataSize=0x%08x",
              getAddressProviderTableName().c_str(), address, dataSize );

    SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "Not Implement !" );
}



