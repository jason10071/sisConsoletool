#include "attributereader_817_r64k_m48k.h"

#include "sislog.h"
#include "spexception/spexception.h"

#include "addressprovider/817/addressprovider_817_r64k_m48k.h"

using namespace SiS::Procedure;

#define EXCEPTION_TITLE "AttributeReader_817_r64k_m48k Exception : "

AttributeReader_817_r64k_m48k::AttributeReader_817_r64k_m48k(SiSDeviceIO* sisDeviceIO) :
    AttributeReader_817_r64k(sisDeviceIO)
{

}

AttributeReader_817_r64k_m48k::AttributeReader_817_r64k_m48k(unsigned char* binBuf, int binSize) :
    AttributeReader_817_r64k(binBuf, binSize)
{

}

AttributeReader_817_r64k_m48k::~AttributeReader_817_r64k_m48k()
{

}

IAddressProvider*
AttributeReader_817_r64k_m48k::getAddressProvider()
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
AttributeReader_817_r64k_m48k::getAddressProviderTableName()
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
