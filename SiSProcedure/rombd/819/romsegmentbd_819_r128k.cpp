#include "romsegmentbd_819_r128k.h"

#include "sislog.h"
#include "spexception/spexception.h"
#include "spexception/siscoreexception.h"
#include "spbase/sisproceduredefine.h"

#include "addressprovider/819/addressprovider_819_r128k.h"
#include "attributereader/819/attributereader_819_r128k.h"

//#include "CptFactoryFacade.h"

using namespace std;
using namespace SiS::Procedure;
using namespace SiS;

#define EXCEPTION_TITLE "RomSegmentBD_819_r128k Exception : "
#define TAG "RomSegmentBD_819_r128k"

RomSegmentBD_819_r128k::RomSegmentBD_819_r128k(SiSDeviceIO* sisDeviceIO, BurnDump burnDump, RomSegment romSegment) :
    RomSegmentBD_817_r64k(sisDeviceIO, burnDump, romSegment)
{

}

RomSegmentBD_819_r128k::~RomSegmentBD_819_r128k()
{

}

IAddressProvider*
RomSegmentBD_819_r128k::getAddressProvider()
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
RomSegmentBD_819_r128k::getAddressProviderTableName()
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
RomSegmentBD_819_r128k::generateAttributeReader()
{
    return new AttributeReader_819_r128k(m_sisDeviceIO);
}

int
RomSegmentBD_819_r128k::getMaxBlockSize()
{
    return _16K;
}

int
RomSegmentBD_819_r128k::mappingChipAddress(int chipIndex)
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
