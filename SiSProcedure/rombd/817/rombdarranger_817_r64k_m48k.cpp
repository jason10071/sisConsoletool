#include "rombdarranger_817_r64k_m48k.h"

#include "sislog.h"
#include "spexception/spexception.h"
#include "spexception/siscoreexception.h"
#include "spbase/sisproceduredefine.h"

#include "rombd/817/romsegmentbd_817_r64k_m48k.h"

using namespace std;
using namespace SiS::Procedure;
using namespace SiS;

#define EXCEPTION_TITLE "RomBDArranger_817_r64k_m48k Exception : "

RomBDArranger_817_r64k_m48k::RomBDArranger_817_r64k_m48k(SiSDeviceIO* sisDeviceIO, IRomSegmentBD::BurnDump burnDump, RomBDPolicy romBDPolicy) :
    RomBDArranger_817_r64k(sisDeviceIO, burnDump, romBDPolicy)
{

}

RomBDArranger_817_r64k_m48k::~RomBDArranger_817_r64k_m48k()
{

}

void
RomBDArranger_817_r64k_m48k::createRomLayout()
{

}

IRomSegmentBD*
RomBDArranger_817_r64k_m48k::generateRomSegmentBD(IRomSegmentBD::BurnDump burnDump, IRomSegmentBD::RomSegment romSegment)
{
    return new RomSegmentBD_817_r64k_m48k(m_sisDeviceIO, burnDump, romSegment);
}

void
RomBDArranger_817_r64k_m48k::preparePolicyComplete()
{
    m_romSegmentBDQueue.push_back( generateRomSegmentBD(m_burnDump, IRomSegmentBD::RS_MAIN_CODE_1) );
    m_romSegmentBDQueue.push_back( generateRomSegmentBD(m_burnDump, IRomSegmentBD::RS_MAIN_CODE_2) );
    m_romSegmentBDQueue.push_back( generateRomSegmentBD(m_burnDump, IRomSegmentBD::RS_MAIN_COMBINE_INFO_REG_DEF_THQA_DEV_RPT) );

    m_romSegmentBDQueue.push_back( generateRomSegmentBD(m_burnDump, IRomSegmentBD::RS_UPDATE_CODE) );
    m_romSegmentBDQueue.push_back( generateRomSegmentBD(m_burnDump, IRomSegmentBD::RS_BOOT_CODE) );

    m_romSegmentBDQueue.push_back( generateRomSegmentBD(m_burnDump, IRomSegmentBD::RS_RODATA_CODE) );
}

//void
//RomBDArranger_817_r64k_m48k::preparePolicyBootloader()
//{

//}

void
RomBDArranger_817_r64k_m48k::preparePolicyMain()
{
    m_romSegmentBDQueue.push_back( generateRomSegmentBD(m_burnDump, IRomSegmentBD::RS_MAIN_CODE_1) );
    m_romSegmentBDQueue.push_back( generateRomSegmentBD(m_burnDump, IRomSegmentBD::RS_MAIN_CODE_2) );
    m_romSegmentBDQueue.push_back( generateRomSegmentBD(m_burnDump, IRomSegmentBD::RS_MAIN_COMBINE_INFO_REG_DEF_THQA_DEV_RPT) );
    m_romSegmentBDQueue.push_back( generateRomSegmentBD(m_burnDump, IRomSegmentBD::RS_RODATA_CODE) );
}

//void
//RomBDArranger_817_r64k_m48k::preparePolicyMainWithReservePartial()
//{

//}

//void
//RomBDArranger_817_r64k_m48k::preparePolicyMainPartial()
//{

//}

//void
//RomBDArranger_817_r64k_m48k::exec(unsigned char* binBuffer, int binSize)
//{
//    prepare();
//}
