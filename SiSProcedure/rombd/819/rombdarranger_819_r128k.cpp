#include "rombdarranger_819_r128k.h"

#include "sislog.h"
#include "spexception/spexception.h"
#include "spexception/siscoreexception.h"
#include "spbase/sisproceduredefine.h"

#include "rombd/819/romsegmentbd_819_r128k.h"

using namespace std;
using namespace SiS::Procedure;
using namespace SiS;

#define EXCEPTION_TITLE "RomBDArranger_819_r128k Exception : "

RomBDArranger_819_r128k::RomBDArranger_819_r128k(SiSDeviceIO* sisDeviceIO, IRomSegmentBD::BurnDump burnDump, RomBDPolicy romBDPolicy) :
    RomBDArranger_817_r64k(sisDeviceIO, burnDump, romBDPolicy)
{

}

RomBDArranger_819_r128k::~RomBDArranger_819_r128k()
{

}

void
RomBDArranger_819_r128k::createRomLayout()
{

}

IRomSegmentBD*
RomBDArranger_819_r128k::generateRomSegmentBD(IRomSegmentBD::BurnDump burnDump, IRomSegmentBD::RomSegment romSegment)
{
    return new RomSegmentBD_819_r128k(m_sisDeviceIO, burnDump, romSegment);
}


void
RomBDArranger_819_r128k::preparePolicyComplete()
{
    m_romSegmentBDQueue.push_back( generateRomSegmentBD(m_burnDump, IRomSegmentBD::RS_MAIN_CODE_1) );
    m_romSegmentBDQueue.push_back( generateRomSegmentBD(m_burnDump, IRomSegmentBD::RS_MAIN_CODE_2) );
    m_romSegmentBDQueue.push_back( generateRomSegmentBD(m_burnDump, IRomSegmentBD::RS_MAIN_COMBINE_INFO_REG_DEF_THQA_DEV_RPT) );

    m_romSegmentBDQueue.push_back( generateRomSegmentBD(m_burnDump, IRomSegmentBD::RS_BOOT_CODE) );

    m_romSegmentBDQueue.push_back( generateRomSegmentBD(m_burnDump, IRomSegmentBD::RS_RODATA_CODE) );

    m_romSegmentBDQueue.push_back( generateRomSegmentBD(m_burnDump, IRomSegmentBD::RS_REMAIN_SECTION) );
}

void
RomBDArranger_819_r128k::preparePolicyBootloader()
{
    m_romSegmentBDQueue.push_back( generateRomSegmentBD(m_burnDump, IRomSegmentBD::RS_BOOT_CODE) );
}

void
RomBDArranger_819_r128k::preparePolicyMain()
{
    m_romSegmentBDQueue.push_back( generateRomSegmentBD(m_burnDump, IRomSegmentBD::RS_MAIN_CODE_1) );
    m_romSegmentBDQueue.push_back( generateRomSegmentBD(m_burnDump, IRomSegmentBD::RS_MAIN_CODE_2) );
    m_romSegmentBDQueue.push_back( generateRomSegmentBD(m_burnDump, IRomSegmentBD::RS_MAIN_COMBINE_INFO_REG_DEF_THQA_DEV_RPT) );
    m_romSegmentBDQueue.push_back( generateRomSegmentBD(m_burnDump, IRomSegmentBD::RS_RODATA_CODE) );

    m_romSegmentBDQueue.push_back( generateRomSegmentBD(m_burnDump, IRomSegmentBD::RS_REMAIN_SECTION) );
}

//void
//RomBDArranger_819_r128k::preparePolicyMainWithReservePartial()
//{

//}

//void
//RomBDArranger_819_r128k::preparePolicyMainPartial()
//{

//}

//void
//RomBDArranger_819_r128k::exec(unsigned char* binBuffer, int binSize)
//{
//    prepare();
//}
