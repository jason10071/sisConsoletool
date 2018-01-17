#include "sisprocedure_817_r128k.h"

#include "sislog.h"
#include "spexception/spexception.h"

#include "attributereader/817/attributereader_817_r128k.h"
#include "attributewriter/817/attributewriter_817_r128k.h"
#include "rombd/817/rombdarranger_817_r128k.h"

//#include "CptFactoryFacade.h"

using namespace std;
using namespace SiS::Procedure;
using namespace SiS;

#define EXCEPTION_TITLE "SiSProcedure_817_r128k Exception : "

SiSProcedure_817_r128k::SiSProcedure_817_r128k() :
    SiSProcedure_817_r64k()
{

}

SiSProcedure_817_r128k::~SiSProcedure_817_r128k()
{

}

IAttributeReader*
SiSProcedure_817_r128k::generateAttributeReader(BinWrap* binWrap)
{
    if( binWrap == 0 )
    {
        if( m_sisDeviceIO == 0 )
        {
            std::string msg = EXCEPTION_TITLE;
            char errorMsg[1024] = "";
            sprintf(errorMsg, "generateAttributeReader() : no sisCore");
            msg.append(errorMsg);
            throw SPException( msg );
        }

        return new AttributeReader_817_r128k(m_sisDeviceIO);
    }
    else
    {
        return new AttributeReader_817_r128k(binWrap->getBuf(), binWrap->getSize());
    }
}

IAttributeWriter*
SiSProcedure_817_r128k::generateAttributeWriter(BinWrap* binWrap)
{
    if( binWrap == 0 )
    {
        if( m_sisDeviceIO == 0 )
        {
            std::string msg = EXCEPTION_TITLE;
            char errorMsg[1024] = "";
            sprintf(errorMsg, "generateAttributeWriter() : no sisCore");
            msg.append(errorMsg);
            throw SPException( msg );
        }

        return new AttributeWriter_817_r128k(m_sisDeviceIO);
    }
    else
    {
        return new AttributeWriter_817_r128k(binWrap->getBuf(), binWrap->getSize());
    }
}

IRomBDArranger*
SiSProcedure_817_r128k::generateRomBDArranger(IRomSegmentBD::BurnDump burnDump, IRomBDArranger::RomBDPolicy romBDPolicy)
{
    return new RomBDArranger_817_r128k(m_sisDeviceIO, burnDump, romBDPolicy);
}
