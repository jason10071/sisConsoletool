#include "attributewriter_817_r64k_m48k.h"

#include "sislog.h"
#include "spexception/spexception.h"
#include "spexception/siscoreexception.h"
#include "spbase/sisproceduredefine.h"

#include "addressprovider/817/addressprovider_817_r64k_m48k.h"

//#include "CptFactoryFacade.h"

using namespace std;
using namespace SiS::Procedure;
using namespace SiS;

#define EXCEPTION_TITLE "AttributeWriter_817_r64k_m48k Exception : "

AttributeWriter_817_r64k_m48k::AttributeWriter_817_r64k_m48k(SiSDeviceIO* m_sisDeviceIO) :
    AttributeWriter_817_r64k(m_sisDeviceIO)
{

}

AttributeWriter_817_r64k_m48k::AttributeWriter_817_r64k_m48k(unsigned char* binBuf, int binSize) :
    AttributeWriter_817_r64k(binBuf, binSize)
{

}

AttributeWriter_817_r64k_m48k::~AttributeWriter_817_r64k_m48k()
{

}

IAddressProvider*
AttributeWriter_817_r64k_m48k::getAddressProvider()
{
    if( m_addressProvider != 0 )
    {
        return m_addressProvider;
    }
    else
    {
        m_addressProvider = new AddressProvider_817_r64k_m48k();
        return m_addressProvider;
    }
}

std::string
AttributeWriter_817_r64k_m48k::getAddressProviderTableName()
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
