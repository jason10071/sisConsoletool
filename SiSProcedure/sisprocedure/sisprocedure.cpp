#include "sisprocedure.h"
#include "sislog.h"
#include "spexception/spexception.h"

using namespace SiS::DeviceIO;
using namespace SiS::Procedure;

#define EXCEPTION_TITLE "ISiSProcedure Exception : "
#define TAG "ISiSProcedure"

SiSProcedure::SiSProcedure() :
    ISiSProcedure(),
    m_iSiSProcedure(0),
    m_sisProcedure_817_r64k(0),
    m_sisProcedure_817_r64k_m48k(0),
    m_sisProcedure_817_r128k(0),
    m_sisProcedure_819_r128k(0)
{
    this->m_sisDeviceIO = new SiSDeviceIO();
    this->m_iSiSProcedure = getSisProcedure_817_r64k();
}

SiSProcedure::~SiSProcedure()
{
    if( m_sisProcedure_817_r64k != 0 )
    {
        delete m_sisProcedure_817_r64k;
        m_sisProcedure_817_r64k = 0;
    }

    if( m_sisProcedure_817_r64k_m48k != 0 )
    {
        delete m_sisProcedure_817_r64k_m48k;
        m_sisProcedure_817_r64k_m48k = 0;
    }

    if( m_sisProcedure_817_r128k != 0 )
    {
        delete m_sisProcedure_817_r128k;
        m_sisProcedure_817_r128k = 0;
    }

    if( m_sisProcedure_819_r128k != 0 )
    {
        delete m_sisProcedure_819_r128k;
        m_sisProcedure_819_r128k = 0;
    }

    if( m_sisDeviceIO != 0 )
    {
        delete m_sisDeviceIO;
        m_sisDeviceIO = 0;
    }
}

void
SiSProcedure::switchSiSProcedure(DeviceType deviceType)
{
    switch (deviceType) {
    case DT_817:
        this->m_iSiSProcedure = getSisProcedure_817_r64k();
        SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "switchSiSProcedure : 817_r64k" );
        break;
    case DT_819:
        this->m_iSiSProcedure = getSisProcedure_819_r128k();
        SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "switchSiSProcedure : 819_r128k" );
        break;
    case DT_INVALID:
    default:
        std::string msg = EXCEPTION_TITLE;
        char errorMsg[1024] = "";
        sprintf(errorMsg, "switchSiSProcedure FAIL : invalid (unknow) deviceType (%d)", (int) deviceType);
        msg.append(errorMsg);
        throw SPException( msg );
    }

    this->m_iSiSProcedure->setBinWrapMap(this->m_binWrapMap);
    this->m_iSiSProcedure->setCtlReportToOs(this->m_ctlReportToOsFlag);
}

void
SiSProcedure::switchSiSProcedure(FWSizeType fwSizeType)
{
    switch (fwSizeType) {
    case FST_817_R64K:
        this->m_iSiSProcedure = getSisProcedure_817_r64k();
        SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "switchSiSProcedure : 817_r64k" );
        break;
    case FST_817_R64K_M48K:
        this->m_iSiSProcedure = getSisProcedure_817_r64k_m48k();
        SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "switchSiSProcedure : 817_r64k_m48k" );
        break;
    case FST_817_R128K:
        this->m_iSiSProcedure = getSisProcedure_817_r128k();
        SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "switchSiSProcedure : 817_r128k" );
        break;
    case FST_819_R128K:
        this->m_iSiSProcedure = getSisProcedure_819_r128k();
        SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "switchSiSProcedure : 819_r128k" );
        break;
    case DT_INVALID:
    default:
        std::string msg = EXCEPTION_TITLE;
        char errorMsg[1024] = "";
        sprintf(errorMsg, "switchSiSProcedure FAIL : invalid (unknow) fwSizeType (%d)", (int) fwSizeType);
        msg.append(errorMsg);
        throw SPException( msg );
    }

    this->m_iSiSProcedure->setBinWrapMap(this->m_binWrapMap);
    this->m_iSiSProcedure->setCtlReportToOs(this->m_ctlReportToOsFlag);
}

void
SiSProcedure::setSiSCore(SiSDeviceIO* sisDeviceIO)
{
    std::string msg = EXCEPTION_TITLE;
    char errorMsg[1024] = "";
    sprintf(errorMsg, "SiSProcedure::setSiSCore : not implement !)");
    msg.append(errorMsg);
    throw SPException( msg );
}

void
SiSProcedure::setBinWrapMap(std::map<ChipIndexKey, BinWrap*> binWrapMap)
{
    this->m_binWrapMap = binWrapMap;
    this->m_iSiSProcedure->setBinWrapMap(m_binWrapMap);
}

void
SiSProcedure::setCtlReportToOs(bool ctlReportToOsFlag)
{
    this->m_ctlReportToOsFlag = ctlReportToOsFlag;
    this->m_iSiSProcedure->setCtlReportToOs(m_ctlReportToOsFlag);
}

IAttributeReader*
SiSProcedure::generateAttributeReader(BinWrap* binWrap)
{
    return this->m_iSiSProcedure->generateAttributeReader(binWrap);
}

IAttributeWriter*
SiSProcedure::generateAttributeWriter(BinWrap* binWrap)
{
    return this->m_iSiSProcedure->generateAttributeWriter(binWrap);
}

IRomBDArranger*
SiSProcedure::generateRomBDArranger(IRomSegmentBD::BurnDump burnDump, IRomBDArranger::RomBDPolicy romBDPolicy)
{
    return this->m_iSiSProcedure->generateRomBDArranger(burnDump, romBDPolicy);
}

void
SiSProcedure::disableCtlReportToOs()
{
    this->m_iSiSProcedure->disableCtlReportToOs();
}

void
SiSProcedure::enableCtlReportToOs()
{
    this->m_iSiSProcedure->enableCtlReportToOs();
}

void
SiSProcedure::jumpBootloader(int chipIndex)
{
    this->m_iSiSProcedure->jumpBootloader(chipIndex);
}

void
SiSProcedure::softReset()
{
    this->m_iSiSProcedure->softReset();
}

void
SiSProcedure::reCalibration(int waitResetSec)
{
    this->m_iSiSProcedure->reCalibration(waitResetSec);
}

bool
SiSProcedure::readBroken(ReferenceSource rs, int chipIndex)
{
    return this->m_iSiSProcedure->readBroken(rs, chipIndex);
}

FWSizeType
SiSProcedure::readFWSizeType(ReferenceSource rs, int chipIndex, bool isBroken)
{
    return this->m_iSiSProcedure->readFWSizeType(rs, chipIndex, isBroken);
}

unsigned char
SiSProcedure::readInterfaceID(ReferenceSource rs, int chipIndex)
{
    return this->m_iSiSProcedure->readInterfaceID(rs, chipIndex);
}

int
SiSProcedure::readSlaveNum(ReferenceSource rs)
{
    return this->m_iSiSProcedure->readSlaveNum(rs);
}

unsigned char
SiSProcedure::readMultiChipSelectiveID(ReferenceSource rs, int chipIndex)
{
    return this->m_iSiSProcedure->readMultiChipSelectiveID(rs, chipIndex);
}

bool
SiSProcedure::readCalibrationFlag(ReferenceSource rs, int chipIndex)
{
    return this->m_iSiSProcedure->readCalibrationFlag(rs, chipIndex);
}

SerialData*
SiSProcedure::readFirmwareID(ReferenceSource rs, int chipIndex)
{
    return this->m_iSiSProcedure->readFirmwareID(rs, chipIndex);
}

SerialData*
SiSProcedure::readBootloaderID(ReferenceSource rs, int chipIndex)
{
    return this->m_iSiSProcedure->readBootloaderID(rs, chipIndex);
}

SerialData*
SiSProcedure::readBootCodeCRC(ReferenceSource rs, int chipIndex)
{
    return this->m_iSiSProcedure->readBootCodeCRC(rs, chipIndex);
}

SerialData*
SiSProcedure::readMainCodeCRC(ReferenceSource rs, int chipIndex)
{
    return this->m_iSiSProcedure->readMainCodeCRC(rs, chipIndex);
}

SerialData*
SiSProcedure::readLastID(ReferenceSource rs, int chipIndex)
{
    return this->m_iSiSProcedure->readLastID(rs, chipIndex);
}

SerialData*
SiSProcedure::readLastTime(ReferenceSource rs, int chipIndex)
{
    return this->m_iSiSProcedure->readLastTime(rs, chipIndex);
}

SerialData*
SiSProcedure::readPriorLastID(ReferenceSource rs, int chipIndex)
{
    return this->m_iSiSProcedure->readPriorLastID(rs, chipIndex);
}

SerialData*
SiSProcedure::readPriorLastTime(ReferenceSource rs, int chipIndex)
{
    return this->m_iSiSProcedure->readPriorLastTime(rs, chipIndex);
}

SerialData*
SiSProcedure::readProductID(ReferenceSource rs, int chipIndex)
{
    return this->m_iSiSProcedure->readProductID(rs, chipIndex);
}

SerialData*
SiSProcedure::readTaskID(ReferenceSource rs, int chipIndex)
{
    return this->m_iSiSProcedure->readTaskID(rs, chipIndex);
}

FwVersion
SiSProcedure::readFwVersion(ReferenceSource rs, int chipIndex)
{
    return this->m_iSiSProcedure->readFwVersion(rs, chipIndex);
}

FWSizeType
SiSProcedure::readRomFWSizeType(int chipIndex)
{
    return this->m_iSiSProcedure->readRomFWSizeType(chipIndex);
}

SerialData*
SiSProcedure::readXramAttribute(std::string key, int chipIndex)
{
    return this->m_iSiSProcedure->readXramAttribute(key, chipIndex);
}

SerialData*
SiSProcedure::readXramAddress(unsigned int address, int dataSize, int chipIndex)
{
    return this->m_iSiSProcedure->readXramAddress(address, dataSize, chipIndex);
}

SerialData* 
SiSProcedure::readNoneSiSCmdViaBridge(SerialData * readcommand, size_t len, const int transitionID)
{
    return this->m_iSiSProcedure->readNoneSiSCmdViaBridge(readcommand, len, transitionID);
}

SerialData*
SiSProcedure::readUpdateMark(ReferenceSource rs, int chipIndex)
{
    return this->m_iSiSProcedure->readUpdateMark(rs, chipIndex);
}

SerialData*
SiSProcedure::readLastUpdateMark(ReferenceSource rs, int chipIndex)
{
    return this->m_iSiSProcedure->readLastUpdateMark(rs, chipIndex);
}

SerialData*
SiSProcedure::readPriorLastUpdateMark(ReferenceSource rs, int chipIndex)
{
    return this->m_iSiSProcedure->readPriorLastUpdateMark(rs, chipIndex);
}

void
SiSProcedure::writeUpdateMark(ReferenceSource rs, int chipIndex, SerialData* serialData)
{
    this->m_iSiSProcedure->writeUpdateMark(rs, chipIndex, serialData);
}

void
SiSProcedure::writeIsUpdateBootloaderInfo(ReferenceSource rs, int chipIndex, SerialData* serialData)
{
    this->m_iSiSProcedure->writeIsUpdateBootloaderInfo(rs, chipIndex, serialData);
}

void
SiSProcedure::writeLastUpdateMark(ReferenceSource rs, int chipIndex, SerialData* serialData)
{
    this->m_iSiSProcedure->writeLastUpdateMark(rs, chipIndex, serialData);
}

void
SiSProcedure::writePriorLastUpdateMark(ReferenceSource rs, int chipIndex, SerialData* serialData)
{
    this->m_iSiSProcedure->writePriorLastUpdateMark(rs, chipIndex, serialData);
}

void
SiSProcedure::writeLastID(ReferenceSource rs, int chipIndex, SerialData* serialData)
{
    this->m_iSiSProcedure->writeLastID(rs, chipIndex, serialData);
}

void
SiSProcedure::writeLastTime(ReferenceSource rs, int chipIndex, SerialData* serialData)
{
    this->m_iSiSProcedure->writeLastTime(rs, chipIndex, serialData);
}

void
SiSProcedure::writePriorLastID(ReferenceSource rs, int chipIndex, SerialData* serialData)
{
    this->m_iSiSProcedure->writePriorLastID(rs, chipIndex, serialData);
}

void
SiSProcedure::writePriorLastTime(ReferenceSource rs, int chipIndex, SerialData* serialData)
{
    this->m_iSiSProcedure->writePriorLastTime(rs, chipIndex, serialData);
}

void SiSProcedure::writeNoneSiSCmdViaBridge( 
		SerialData * data, const int transitionID)
{
	this->m_iSiSProcedure->writeNoneSiSCmdViaBridge(data, transitionID);
}

	void
SiSProcedure::burn(RomBurnFlag romBurnFlag, int chipIndex)
{
    return this->m_iSiSProcedure->burn(romBurnFlag, chipIndex);
}

void
SiSProcedure::clearCalibration(int chipIndex)
{
    return this->m_iSiSProcedure->clearCalibration(chipIndex);
}

IRomBDArranger::RomBDPolicy
SiSProcedure::getPolicy(RomBurnFlag romBurnFlag)
{
    return this->m_iSiSProcedure->getPolicy(romBurnFlag);
}

SiSProcedure_817_r64k*
SiSProcedure::getSisProcedure_817_r64k()
{
    if( this->m_sisProcedure_817_r64k == 0 )
    {
        this->m_sisProcedure_817_r64k = new SiSProcedure_817_r64k();
        this->m_sisProcedure_817_r64k->setSiSCore(this->m_sisDeviceIO);
    }

    return this->m_sisProcedure_817_r64k;
}

SiSProcedure_817_r64k_m48k*
SiSProcedure::getSisProcedure_817_r64k_m48k()
{
    if( this->m_sisProcedure_817_r64k_m48k == 0 )
    {
        this->m_sisProcedure_817_r64k_m48k = new SiSProcedure_817_r64k_m48k();
        this->m_sisProcedure_817_r64k_m48k->setSiSCore(this->m_sisDeviceIO);
    }

    return this->m_sisProcedure_817_r64k_m48k;
}

SiSProcedure_817_r128k*
SiSProcedure::getSisProcedure_817_r128k()
{
    if( this->m_sisProcedure_817_r128k == 0 )
    {
        this->m_sisProcedure_817_r128k = new SiSProcedure_817_r128k();
        this->m_sisProcedure_817_r128k->setSiSCore(this->m_sisDeviceIO);
    }

    return this->m_sisProcedure_817_r128k;
}

SiSProcedure_819_r128k*
SiSProcedure::getSisProcedure_819_r128k()
{
    if( this->m_sisProcedure_819_r128k == 0 )
    {
        this->m_sisProcedure_819_r128k = new SiSProcedure_819_r128k();
        this->m_sisProcedure_819_r128k->setSiSCore(this->m_sisDeviceIO);
    }

    return this->m_sisProcedure_819_r128k;
}
