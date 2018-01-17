#include "attributewriter_819_r128k.h"

#include "sislog.h"
#include "spexception/spexception.h"
#include "spexception/siscoreexception.h"
#include "spbase/sisproceduredefine.h"

#include "addressprovider/819/addressprovider_819_r128k.h"

//#include "CptFactoryFacade.h"

using namespace std;
using namespace SiS::Procedure;
using namespace SiS;

#define EXCEPTION_TITLE "AttributeWriter_819_r128k Exception : "
#define TAG "AttributeWriter_819_r128k"

AttributeWriter_819_r128k::AttributeWriter_819_r128k(SiSDeviceIO* m_sisDeviceIO) :
    AttributeWriter_817_r64k(m_sisDeviceIO)
{

}

AttributeWriter_819_r128k::AttributeWriter_819_r128k(unsigned char* binBuf, int binSize) :
    AttributeWriter_817_r64k(binBuf, binSize)
{

}

AttributeWriter_819_r128k::~AttributeWriter_819_r128k()
{

}

IAddressProvider*
AttributeWriter_819_r128k::getAddressProvider()
{
    if( m_addressProvider != 0 )
    {
        return m_addressProvider;
    }
    else
    {
        m_addressProvider = new AddressProvider_819_r128k();
        return m_addressProvider;
    }
}

std::string
AttributeWriter_819_r128k::getAddressProviderTableName()
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
AttributeWriter_819_r128k::mappingChipAddress(int chipIndex)
{
    if( chipIndex == CI_MASTER )
    {
        return 0;
    }
    else
    {
        /* 0x11 ~ 0x17 : 0x21 ~ 0x27 : ...*/
        int quotient = (chipIndex - CI_SLAVE_0) / 7;
        int remain = (chipIndex - CI_SLAVE_0) % 7;
        int chipAddress = (quotient + 1) * 0x10 + (remain + 1);

        SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "mappingChipAddress : chipIndex=%d -> chipAddress=0x%x", chipIndex, chipAddress );
        return chipAddress;
    }
}
