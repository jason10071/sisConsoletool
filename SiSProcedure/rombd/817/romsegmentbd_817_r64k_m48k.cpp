#include "romsegmentbd_817_r64k_m48k.h"

#include "sislog.h"
#include "spexception/spexception.h"
#include "spexception/siscoreexception.h"
#include "spbase/sisproceduredefine.h"

#include "addressprovider/817/addressprovider_817_r64k_m48k.h"
#include "attributereader/817/attributereader_817_r64k_m48k.h"

//#include "CptFactoryFacade.h"

using namespace std;
using namespace SiS::Procedure;
using namespace SiS;

#define EXCEPTION_TITLE "RomSegmentBD_817_r64k_m48k Exception : "

RomSegmentBD_817_r64k_m48k::RomSegmentBD_817_r64k_m48k(SiSDeviceIO* sisDeviceIO, BurnDump burnDump, RomSegment romSegment) :
    RomSegmentBD_817_r64k(sisDeviceIO, burnDump, romSegment)
{

}

RomSegmentBD_817_r64k_m48k::~RomSegmentBD_817_r64k_m48k()
{

}

IAddressProvider*
RomSegmentBD_817_r64k_m48k::getAddressProvider()
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
RomSegmentBD_817_r64k_m48k::getAddressProviderTableName()
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

IAttributeReader*
RomSegmentBD_817_r64k_m48k::generateAttributeReader()
{
    return new AttributeReader_817_r64k_m48k(m_sisDeviceIO);
}
