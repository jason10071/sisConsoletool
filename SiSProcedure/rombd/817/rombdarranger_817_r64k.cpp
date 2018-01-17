#include "rombdarranger_817_r64k.h"

#include "sislog.h"
#include "spexception/spexception.h"
#include "spexception/siscoreexception.h"
#include "spbase/sisproceduredefine.h"

#include "rombd/817/romsegmentbd_817_r64k.h"

using namespace std;
using namespace SiS::Procedure;
using namespace SiS;

#define EXCEPTION_TITLE "RomBDArranger_817_r64k Exception : "

RomBDArranger_817_r64k::RomBDArranger_817_r64k(SiSDeviceIO* sisDeviceIO, IRomSegmentBD::BurnDump burnDump, RomBDPolicy romBDPolicy) :
    IRomBDArranger(sisDeviceIO, burnDump, romBDPolicy)
{

}

RomBDArranger_817_r64k::~RomBDArranger_817_r64k()
{

}

void
RomBDArranger_817_r64k::createRomLayout()
{

}

IRomSegmentBD*
RomBDArranger_817_r64k::generateRomSegmentBD(IRomSegmentBD::BurnDump burnDump, IRomSegmentBD::RomSegment romSegment)
{
    return new RomSegmentBD_817_r64k(m_sisDeviceIO, burnDump, romSegment);
}

void
RomBDArranger_817_r64k::prepareClearBootFlag()
{
    IRomSegmentBD* romSegmentBD = generateRomSegmentBD(m_burnDump, IRomSegmentBD::RS_CLEAR_BOOT_FLAG);

//    if( std::find(m_mainReservePartial.begin(), m_mainReservePartial.end(), IRomSegmentBD::RS_RODATA_CODE)
//            != m_mainReservePartial.end() )
//    {
//        romSegmentBD->setClearBootFlagWithReserveROData(true);
//    }

    m_romSegmentBDQueue.push_back( romSegmentBD );
}

void
RomBDArranger_817_r64k::prepareClearCalibrationFlag()
{
    IRomSegmentBD* romSegmentBD = generateRomSegmentBD(m_burnDump, IRomSegmentBD::RS_CLEAR_CALIBRATION_FLAG);

    m_romSegmentBDQueue.push_back( romSegmentBD );
}

void
RomBDArranger_817_r64k::preparePolicyComplete()
{
    m_romSegmentBDQueue.push_back( generateRomSegmentBD(m_burnDump, IRomSegmentBD::RS_MAIN_CODE_1) );
    m_romSegmentBDQueue.push_back( generateRomSegmentBD(m_burnDump, IRomSegmentBD::RS_MAIN_COMBINE_INFO_REG_DEF_THQA_DEV_RPT) );

    m_romSegmentBDQueue.push_back( generateRomSegmentBD(m_burnDump, IRomSegmentBD::RS_UPDATE_CODE) );
    m_romSegmentBDQueue.push_back( generateRomSegmentBD(m_burnDump, IRomSegmentBD::RS_BOOT_CODE) );

    m_romSegmentBDQueue.push_back( generateRomSegmentBD(m_burnDump, IRomSegmentBD::RS_RODATA_CODE) );
}

void
RomBDArranger_817_r64k::preparePolicyBootloader()
{
    m_romSegmentBDQueue.push_back( generateRomSegmentBD(m_burnDump, IRomSegmentBD::RS_UPDATE_CODE) );
    m_romSegmentBDQueue.push_back( generateRomSegmentBD(m_burnDump, IRomSegmentBD::RS_BOOT_CODE) );
}

void
RomBDArranger_817_r64k::preparePolicyMain()
{
    m_romSegmentBDQueue.push_back( generateRomSegmentBD(m_burnDump, IRomSegmentBD::RS_MAIN_CODE_1) );
    m_romSegmentBDQueue.push_back( generateRomSegmentBD(m_burnDump, IRomSegmentBD::RS_MAIN_COMBINE_INFO_REG_DEF_THQA_DEV_RPT) );
    m_romSegmentBDQueue.push_back( generateRomSegmentBD(m_burnDump, IRomSegmentBD::RS_RODATA_CODE) );
}

void
RomBDArranger_817_r64k::preparePolicyMainWithReservePartial()
{

}

void
RomBDArranger_817_r64k::preparePolicyMainPartial()
{

}

void
RomBDArranger_817_r64k::exec(unsigned char* binBuffer, int binSize)
{
    prepare();

    for(std::list<IRomSegmentBD*>::iterator it = m_romSegmentBDQueue.begin(); it != m_romSegmentBDQueue.end(); ++it)
    {
        IRomSegmentBD* romSegmentBD = *it;
        romSegmentBD->setChipIndex(this->m_chipIndex);
        romSegmentBD->exec(binBuffer, binSize);
    }
}
