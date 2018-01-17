#include "romsegmentbd_817_r128k.h"

#include "sislog.h"
#include "spexception/spexception.h"
#include "spexception/siscoreexception.h"
#include "spbase/sisproceduredefine.h"

#include "addressprovider/817/addressprovider_817_r128k.h"
#include "attributereader/817/attributereader_817_r128k.h"

//#include "CptFactoryFacade.h"

using namespace std;
using namespace SiS::Procedure;
using namespace SiS;

#define EXCEPTION_TITLE "RomSegmentBD_817_r128k Exception : "

RomSegmentBD_817_r128k::RomSegmentBD_817_r128k(SiSDeviceIO* sisDeviceIO, BurnDump burnDump, RomSegment romSegment) :
    RomSegmentBD_817_r64k(sisDeviceIO, burnDump, romSegment)
{

}

RomSegmentBD_817_r128k::~RomSegmentBD_817_r128k()
{

}

IAddressProvider*
RomSegmentBD_817_r128k::getAddressProvider()
{
    if( m_addressProvider != 0 )
    {
        return m_addressProvider;
    }
    else
    {
        m_addressProvider = new AddressProvider_817_r128k();
        return m_addressProvider;
    }
}

std::string
RomSegmentBD_817_r128k::getAddressProviderTableName()
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
RomSegmentBD_817_r128k::generateAttributeReader()
{
    return new AttributeReader_817_r128k(m_sisDeviceIO);
}

int
RomSegmentBD_817_r128k::getMaxBlockSize()
{
    return _8K;
}
