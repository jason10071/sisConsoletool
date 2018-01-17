#include "irombdarranger.h"

#include "sislog.h"
#include "spexception/spexception.h"

using namespace SiS::DeviceIO;
using namespace SiS::Procedure;


#define EXCEPTION_TITLE "IRomBDArranger Exception : "
#define TAG "IRomBDArranger"

IRomBDArranger::IRomBDArranger(SiSDeviceIO* sisDeviceIO, IRomSegmentBD::BurnDump burnDump, RomBDPolicy romBDPolicy) :
    m_sisDeviceIO(sisDeviceIO),
    m_burnDump(burnDump),
    m_romBDPolicy(romBDPolicy),
    m_chipIndex(CI_MASTER)
{
    createRomLayout();
}

IRomBDArranger::~IRomBDArranger()
{
    for(std::list<IRomSegmentBD*>::iterator it = m_romSegmentBDQueue.begin(); it != m_romSegmentBDQueue.end(); ++it)
    {
        IRomSegmentBD* romSegmentBD = *it;
        delete romSegmentBD;
        romSegmentBD = 0;
    }
    m_romSegmentBDQueue.clear();
}

void
IRomBDArranger::setChipIndex(int chipIndex)
{
    this->m_chipIndex = chipIndex;
}

void
IRomBDArranger::addMainReservePartial(IRomSegmentBD::RomSegment romSegment)
{
    m_mainReservePartial.push_back(romSegment);
}

void
IRomBDArranger::addMainPartial(IRomSegmentBD::RomSegment romSegment)
{
    m_mainPartial.push_back(romSegment);
}

void
IRomBDArranger::createRomLayout()
{
    // do nothing
}

bool
IRomBDArranger::hasRomSegment(IRomSegmentBD::RomSegment romSegment)
{
    return false;
//    return (std::find(m_romLayout.begin(), m_romLayout.end(), romSegment) != m_romLayout.end());
}

void
IRomBDArranger::prepare()
{
    if( m_burnDump == IRomSegmentBD::BD_BURN_TO_ROM )
    {
        SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "BURN TO ROM : " );

    }
    else if( m_burnDump == IRomSegmentBD::BD_DUMP_FROM_XRAM )
    {
        SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "DUMP FROM XRAM : " );
    }

    switch (m_romBDPolicy)
    {
    case RBDP_CLEAR_BOOT_FLAG:
        SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "Policy - CLEAR_BOOT_FLAG" );
        prepareClearBootFlag();
        break;
    case RBDP_CLEAR_CALIBRATION_FLAG:
        SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "Policy - CLEAR_CALIBRATION_FLAG" );
        prepareClearCalibrationFlag();
        break;
    case RBDP_COMPLETE:
        SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "Policy - COMPLETE" );
        preparePolicyComplete();
        break;
    case RBDP_BOOTLOADER:
        SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "Policy - BOOTLOADER" );
        preparePolicyBootloader();
        break;
    case RBDP_MAIN:
        SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "Policy - MAIN" );
        preparePolicyMain();
        break;
    case RBDP_MAIN_WITH_RESERVE_PARTIAL:
        SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "Policy - MAIN_WITH_RESERVE_PARTIAL" );
        preparePolicyMainWithReservePartial();
        break;
    case RBDP_MAIN_PARTIAL:
        SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "Policy - MAIN_PARTIAL" );
        preparePolicyMainPartial();
        break;
    default:
        break;
    }
}

