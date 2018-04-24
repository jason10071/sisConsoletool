#include "ctbasecore.h"

#include "sislog.h"
#include "ctexception/ctexception.h"
#include "sisprocedure/sisprocedure.h"

using namespace CT;
using namespace SiS;
using namespace SiS::Procedure;

#define EXCEPTION_TITLE "CTBaseCore Exception : "
#define TAG "CTBaseCore"

CTBaseCore::CTBaseCore(CTBaseParameter* ctBaseParameter) :
    m_ctBaseParameter(ctBaseParameter),
    m_sisProcedure(0)
{

}

CTBaseCore::~CTBaseCore()
{
    for(std::map<ChipIndexKey, CTBaseReference*>::iterator it = m_referenceMap[RS_BIN].begin();
        it != m_referenceMap[RS_BIN].end(); it++)
    {
        delete it->second;
    }
    m_referenceMap[RS_BIN].clear();

    for(std::map<ChipIndexKey, CTBaseReference*>::iterator it = m_referenceMap[RS_ROM].begin();
        it != m_referenceMap[RS_ROM].end(); it++)
    {
        delete it->second;
    }
    m_referenceMap[RS_ROM].clear();

    for(std::map<ChipIndexKey, CTBaseReference*>::iterator it = m_referenceMap[RS_XRAM].begin();
        it != m_referenceMap[RS_XRAM].end(); it++)
    {
        delete it->second;
    }
    m_referenceMap[RS_XRAM].clear();
}

void
CTBaseCore::setSiSProcedure(SiSProcedure* sisProcedure)
{
    this->m_sisProcedure = sisProcedure;
}

CTExitCode
CTBaseCore::undoException(std::string exceptionTitle)
{
    std::string msg = exceptionTitle;
    char errorMsg[1024] = "";
    sprintf(errorMsg, "UNDO !");
    msg.append(errorMsg);
    throw CTException( msg );

    return CT_EXIT_AP_FLOW_ERROR;
}

void
CTBaseCore::init()
{
    /* set atttibute */
    m_sisProcedure->setIOInterval( m_ctBaseParameter->getIOInterval() );
    m_sisProcedure->setIOIntervalOf81( m_ctBaseParameter->getIOIntervalOf81() );

    /* openDevice */
    openDevice();

    /* fetch OS Device Type */
    fetchOSDeviceType();

    /* fetch OS Device Interface */
    fetchOSDeviceInterface();

    /* switch SiSProcedure by OS DeviceType */
    m_sisProcedure->switchSiSProcedure( m_ctBaseParameter->getOSDeviceType() );

    /* disable CtlReportToOs */
    disableCtlReportToOs();
}

void
CTBaseCore::destroy()
{
    enableCtlReportToOs();
}

CTBaseReference*
CTBaseCore::generateReference()
{
    return new CTBaseReference();
}

void
CTBaseCore::insertReference(ReferenceSource rs, int chipIndex, CTBaseReference* ctBaseReference)
{
    if( m_referenceMap[rs].find(chipIndex) != m_referenceMap[rs].end() )
    {
        return; // not replace
    }
    else
    {
        m_referenceMap[rs][chipIndex] = ctBaseReference;
    }
}

CTBaseReference*
CTBaseCore::getCTBaseReference(ReferenceSource rs, int chipIndex)
{
    if( m_referenceMap[rs].find(chipIndex) == m_referenceMap[rs].end() )
    {
        std::string msg = EXCEPTION_TITLE;
        char errorMsg[1024] = "";
        sprintf(errorMsg, "can't find CTBaseReference : rs=%d, chipIndex=%d\n", rs, chipIndex);
        msg.append(errorMsg);
        throw CTException( msg );
    }

    return m_referenceMap[rs].find(chipIndex)->second;
}

void
CTBaseCore::openDevice()
{
    /* openDevice */
    std::string opened = m_sisProcedure->openDevice( m_ctBaseParameter->getDeviceID() );
    m_ctBaseParameter->setDeviceID( opened );
    SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "set opened deviceID : %s", opened.c_str() );

}

void
CTBaseCore::disableCtlReportToOs()
{
    if( !m_ctBaseParameter->getCtlReportToOs() )
    {
        SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "SWITCH-OFF: CTL_REPORT_TO_OS, NO(IsReport)" );
        return;
    }

    m_sisProcedure->disableCtlReportToOs();
}

void
CTBaseCore::enableCtlReportToOs()
{
    if( !m_ctBaseParameter->getCtlReportToOs() )
    {
        SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "SWITCH-OFF: CTL_REPORT_TO_OS, YES(IsReport)" );
        return;
    }

    m_sisProcedure->enableCtlReportToOs();
}

void
CTBaseCore::jumpBootloader(int chipIndex)
{
    if( m_sisProcedure->readSubProtocol()== DT_BRIDGE )
    {
        SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "bypass jumpBootloader because of the test via the bridge" );
        return;
    }
    else if( m_ctBaseParameter->getOSDeviceInterface() == DI_I2C ||
             m_ctBaseParameter->getOSDeviceInterface() == DI_HID_I2C )
    {
        SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "bypass jumpBootloader because of the interface is I2C" );
        return;
    }
    else
    {
        m_sisProcedure->jumpBootloader(chipIndex);
    }
}

void
CTBaseCore::resetDevice()
{
    m_sisProcedure->resetDevice( m_ctBaseParameter->getWaitResetSec() );
}

void
CTBaseCore::prepareBaseXramMasterRef(bool stopIfBroken)
{   
    /* switch SiSProcedure by OS DeviceType */
    m_sisProcedure->switchSiSProcedure( m_ctBaseParameter->getOSDeviceType() );

    /* init Xram master reference */
    insertReference(RS_XRAM, CI_MASTER, generateReference());
    CTBaseReference* xramMasterRef = getCTBaseReference(RS_XRAM, CI_MASTER);

    fetchDeviceType(RS_XRAM, CI_MASTER); /* 1. DeviceType */
    fetchBroken(RS_XRAM, CI_MASTER); /* 2. Broken */

    /* check mainCodeReliable */
    if( xramMasterRef->isBroken() && stopIfBroken )
    {
        SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "!!! Master (chipIndex=0) FW Broken" );

        /* stop if Master FW Broken */
        std::string msg = EXCEPTION_TITLE;
        char errorMsg[1024] = "";
        sprintf(errorMsg, "prepareBaseXramMasterRef : Master FW Broken !\n");
        msg.append(errorMsg);
        throw CTException( msg );
    }
    else if( xramMasterRef->isBroken() )
    {
        SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "!!! Master (chipIndex=0) FW Broken, but continue" );
    }

    fetchSlaveNum(RS_XRAM); /* 5. SlaveNum */
}

void
CTBaseCore::fetchOSDeviceType()
{
    /* user had forced to set (OS) device type */
    if( m_ctBaseParameter->getOSDeviceTypeUserDefined() )
    {
        SIS_LOG_I(SiSLog::getOwnerSiS(), TAG,
                  "user had forced to set (OS) device type : %s",
                  ISiSProcedure::getDTStr(m_ctBaseParameter->getOSDeviceType()).c_str() );
        return;
    }
    else
    {
        m_ctBaseParameter->setOSDeviceType( m_sisProcedure->readOSDeviceType() );
    }
}

void
CTBaseCore::fetchOSDeviceInterface()
{
    /* user had forced to set (OS) device interface */
    if( m_ctBaseParameter->getOSDeviceInterfaceUserDefined() )
    {
        SIS_LOG_I(SiSLog::getOwnerSiS(), TAG,
                  "user had forced to set (OS) device interface : %s",
                  ISiSProcedure::getDIStr(m_ctBaseParameter->getOSDeviceInterface()).c_str() );
        return;
    }
    else
    {
        m_ctBaseParameter->setOSDeviceInterface( m_sisProcedure->readOSDeviceInterface() );
    }
}

void
CTBaseCore::fetchDeviceType(ReferenceSource rs, int chipIndex)
{
    switch (rs) {
    case RS_BIN:
        getCTBaseReference(rs, chipIndex)->setDeviceType( m_sisProcedure->readBinDeviceType(chipIndex) );
        break;
    case RS_XRAM:
    case RS_ROM:
    default:
        getCTBaseReference(rs, chipIndex)->setDeviceType( m_ctBaseParameter->getOSDeviceType() );
        break;
    }
}

void
CTBaseCore::fetchBroken(ReferenceSource rs, int chipIndex)
{
    getCTBaseReference(rs, chipIndex)->setBroken( m_sisProcedure->readBroken(rs, chipIndex) );
}

void
CTBaseCore::fetchFWSizeType(ReferenceSource rs, int chipIndex, bool isBroken)
{
    getCTBaseReference(rs, chipIndex)->setFWSizeType( m_sisProcedure->readFWSizeType(rs, chipIndex, isBroken) );
}

void
CTBaseCore::fetchInterfaceID(ReferenceSource rs, int chipIndex)
{
    getCTBaseReference(rs, chipIndex)->setInterfaceID( m_sisProcedure->readInterfaceID(rs, chipIndex) );
}

void
CTBaseCore::fetchSlaveNum(ReferenceSource rs)
{
    if( m_ctBaseParameter->getSlaveNumUserDefined() && rs != RS_BIN )
    {
        SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "user had forced to set SlaveNum : %d", m_ctBaseParameter->getSlaveNum() );
        getCTBaseReference(rs, CI_MASTER)->setSlaveNum( m_ctBaseParameter->getSlaveNum() );
    }
    else
    {
        getCTBaseReference(rs, CI_MASTER)->setSlaveNum( m_sisProcedure->readSlaveNum(rs) );
    }
}

void
CTBaseCore::fetchMultiChipSelectiveID(ReferenceSource rs, int chipIndex)
{
    getCTBaseReference(rs, chipIndex)->setMultiChipSelectiveID( m_sisProcedure->readMultiChipSelectiveID(rs, chipIndex) );
}

void
CTBaseCore::fetchCalibrationFlag(ReferenceSource rs, int chipIndex)
{
    getCTBaseReference(rs, chipIndex)->setCalibrationFlag( m_sisProcedure->readCalibrationFlag(rs, chipIndex) );
}

void
CTBaseCore::fetchFirmwareID(ReferenceSource rs, int chipIndex)
{
    getCTBaseReference(rs, chipIndex)->setFirmwareID( m_sisProcedure->readFirmwareID(rs, chipIndex) );
}

void
CTBaseCore::fetchBootloaderID(ReferenceSource rs, int chipIndex)
{
    getCTBaseReference(rs, chipIndex)->setBootloaderID( m_sisProcedure->readBootloaderID(rs, chipIndex) );
}

void
CTBaseCore::fetchBootCodeCRC(ReferenceSource rs, int chipIndex)
{
    getCTBaseReference(rs, chipIndex)->setBootCodeCRC( m_sisProcedure->readBootCodeCRC(rs, chipIndex) );
}

void
CTBaseCore::fetchMainCodeCRC(ReferenceSource rs, int chipIndex)
{
    getCTBaseReference(rs, chipIndex)->setMainCodeCRC( m_sisProcedure->readMainCodeCRC(rs, chipIndex) );
}

void
CTBaseCore::fetchLastID(ReferenceSource rs, int chipIndex)
{
    getCTBaseReference(rs, chipIndex)->setLastID( m_sisProcedure->readLastID(rs, chipIndex) );
}

void
CTBaseCore::fetchLastTime(ReferenceSource rs, int chipIndex)
{
    getCTBaseReference(rs, chipIndex)->setLastTime( m_sisProcedure->readLastTime(rs, chipIndex) );
}

void
CTBaseCore::fetchPriorLastID(ReferenceSource rs, int chipIndex)
{
    getCTBaseReference(rs, chipIndex)->setPriorLastID( m_sisProcedure->readPriorLastID(rs, chipIndex) );
}

void
CTBaseCore::fetchPriorLastTime(ReferenceSource rs, int chipIndex)
{
    getCTBaseReference(rs, chipIndex)->setPriorLastTime( m_sisProcedure->readPriorLastTime(rs, chipIndex) );
}

void
CTBaseCore::fetchProductID(ReferenceSource rs, int chipIndex)
{
    getCTBaseReference(rs, chipIndex)->setProductID( m_sisProcedure->readProductID(rs, chipIndex) );
}

void
CTBaseCore::fetchTaskID(ReferenceSource rs, int chipIndex)
{
    getCTBaseReference(rs, chipIndex)->setTaskID( m_sisProcedure->readTaskID(rs, chipIndex) );
}

void
CTBaseCore::fetchFwVersion(ReferenceSource rs, int chipIndex)
{
    getCTBaseReference(rs, chipIndex)->setFwVersion( m_sisProcedure->readFwVersion(rs, chipIndex) );
}

void
CTBaseCore::fetchUpdateMark(ReferenceSource rs, int chipIndex)
{
    getCTBaseReference(rs, chipIndex)->setUpdateMark( m_sisProcedure->readUpdateMark(rs, chipIndex) );
}

void
CTBaseCore::fetchLastUpdateMark(ReferenceSource rs, int chipIndex)
{
    getCTBaseReference(rs, chipIndex)->setLastUpdateMark( m_sisProcedure->readLastUpdateMark(rs, chipIndex) );
}

void
CTBaseCore::fetchPriorLastUpdateMark(ReferenceSource rs, int chipIndex)
{
    getCTBaseReference(rs, chipIndex)->setPriorLastUpdateMark( m_sisProcedure->readPriorLastUpdateMark(rs, chipIndex) );
}

void
CTBaseCore::showDeviceType(int chipIndex, ReferenceSource rs)
{
    switch (rs)
    {
    case RS_BIN:
    case RS_ROM:
    case RS_XRAM:
        SIS_LOG_I(SiSLog::getOwnerSiS(), TAG,
                  "DeviceType : %s (%s)",
                  ISiSProcedure::getRSStr(rs).c_str(),
                  ISiSProcedure::getDTStr(getCTBaseReference(rs, chipIndex)->getDeviceType()).c_str() );
        break;
    case RS_COUNT:
    default:
        SIS_LOG_I(SiSLog::getOwnerSiS(), TAG,
                  "DeviceType :\n\t%s  (%s),\n\t%s (%s)",
                  ISiSProcedure::getRSStr(RS_BIN).c_str(),
                  ISiSProcedure::getDTStr(getCTBaseReference(RS_BIN, chipIndex)->getDeviceType()).c_str(),
                  ISiSProcedure::getRSStr(RS_XRAM).c_str(),
                  ISiSProcedure::getDTStr(getCTBaseReference(RS_XRAM, chipIndex)->getDeviceType()).c_str() );
        break;
    }
}

void
CTBaseCore::showBroken(int chipIndex, ReferenceSource rs)
{
    switch (rs)
    {
    case RS_BIN:
    case RS_ROM:
    case RS_XRAM:
        SIS_LOG_I(SiSLog::getOwnerSiS(), TAG,
                  "Broken : %s (%s)",
                  ISiSProcedure::getRSStr(rs).c_str(),
                  getCTBaseReference(rs, chipIndex)->isBroken() == true ? "Yes" : "No" );
        break;
    case RS_COUNT:
    default:
        SIS_LOG_I(SiSLog::getOwnerSiS(), TAG,
                  "Broken :\n\t%s  (%s),\n\t%s (%s)",
                  ISiSProcedure::getRSStr(RS_BIN).c_str(),
                  getCTBaseReference(RS_BIN, chipIndex)->isBroken() == true ? "Yes" : "No",
                  ISiSProcedure::getRSStr(RS_XRAM).c_str(),
                  getCTBaseReference(RS_XRAM, chipIndex)->isBroken() == true ? "Yes" : "No"  );
        break;
    }
}

void
CTBaseCore::showFWSizeType(int chipIndex, ReferenceSource rs)
{
    switch (rs)
    {
    case RS_BIN:
    case RS_ROM:
    case RS_XRAM:
        SIS_LOG_I(SiSLog::getOwnerSiS(), TAG,
                  "FWSizeType : %s (%s)",
                  ISiSProcedure::getRSStr(rs).c_str(),
                  ISiSProcedure::getFSTStr(getCTBaseReference(rs, chipIndex)->getFWSizeType()).c_str()  );
        break;
    case RS_COUNT:
    default:
        SIS_LOG_I(SiSLog::getOwnerSiS(), TAG,
                  "FWSizeType :\n\t%s  (%s),\n\t%s (%s),\n\t%s  (%s)",
                  ISiSProcedure::getRSStr(RS_BIN).c_str(),
                  ISiSProcedure::getFSTStr(getCTBaseReference(RS_BIN, chipIndex)->getFWSizeType()).c_str(),
                  ISiSProcedure::getRSStr(RS_XRAM).c_str(),
                  ISiSProcedure::getFSTStr(getCTBaseReference(RS_XRAM, chipIndex)->getFWSizeType()).c_str(),
                  ISiSProcedure::getRSStr(RS_ROM).c_str(),
                  ISiSProcedure::getFSTStr(getCTBaseReference(RS_ROM, chipIndex)->getFWSizeType()).c_str()  );
        break;
    }
}

void
CTBaseCore::showInterfaceID(int chipIndex, ReferenceSource rs)
{
    switch (rs)
    {
    case RS_BIN:
    case RS_ROM:
    case RS_XRAM:
        SIS_LOG_I(SiSLog::getOwnerSiS(), TAG,
                  "InterfaceID : %s (0x%x)",
                  ISiSProcedure::getRSStr(rs).c_str(),
                  getCTBaseReference(rs, chipIndex)->getInterfaceID() );
        break;
    case RS_COUNT:
    default:
        SIS_LOG_I(SiSLog::getOwnerSiS(), TAG,
                  "InterfaceID :\n\t%s  (0x%x),\n\t%s (0x%x)",
                  ISiSProcedure::getRSStr(RS_BIN).c_str(),
                  getCTBaseReference(RS_BIN, chipIndex)->getInterfaceID(),
                  ISiSProcedure::getRSStr(RS_XRAM).c_str(),
                  getCTBaseReference(RS_XRAM, chipIndex)->getInterfaceID() );
        break;
    }
}

void
CTBaseCore::showSlaveNum(ReferenceSource rs)
{
    switch (rs)
    {
    case RS_BIN:
    case RS_ROM:
    case RS_XRAM:
        SIS_LOG_I(SiSLog::getOwnerSiS(), TAG,
                  "SlaveNum : %s (%d)",
                  ISiSProcedure::getRSStr(rs).c_str(),
                  getCTBaseReference(rs, CI_MASTER)->getSlaveNum() );
        break;
    case RS_COUNT:
    default:
        SIS_LOG_I(SiSLog::getOwnerSiS(), TAG,
                  "SlaveNum :\n\t%s  (%d),\n\t%s (%d)",
                  ISiSProcedure::getRSStr(RS_BIN).c_str(),
                  getCTBaseReference(RS_BIN, CI_MASTER)->getSlaveNum(),
                  ISiSProcedure::getRSStr(RS_XRAM).c_str(),
                  getCTBaseReference(RS_XRAM, CI_MASTER)->getSlaveNum() );
        break;
    }
}

void
CTBaseCore::showMultiChipSelectiveID(int chipIndex, ReferenceSource rs)
{
    switch (rs)
    {
    case RS_BIN:
    case RS_ROM:
    case RS_XRAM:
        SIS_LOG_I(SiSLog::getOwnerSiS(), TAG,
                  "MultiChipSelectiveID : %s (0x%x)",
                  ISiSProcedure::getRSStr(rs).c_str(),
                  getCTBaseReference(rs, chipIndex)->getMultiChipSelectiveID() );
        break;
    case RS_COUNT:
    default:
        SIS_LOG_I(SiSLog::getOwnerSiS(), TAG,
                  "MultiChipSelectiveID :\n\t%s  (0x%x),\n\t%s (0x%x)",
                  ISiSProcedure::getRSStr(RS_BIN).c_str(),
                  getCTBaseReference(RS_BIN, chipIndex)->getMultiChipSelectiveID(),
                  ISiSProcedure::getRSStr(RS_XRAM).c_str(),
                  getCTBaseReference(RS_XRAM, chipIndex)->getMultiChipSelectiveID() );
        break;
    }
}

void
CTBaseCore::showCalibrationFlag(int chipIndex, ReferenceSource rs)
{
    switch (rs)
    {
    case RS_BIN:
    case RS_ROM:
    case RS_XRAM:
        SIS_LOG_I(SiSLog::getOwnerSiS(), TAG,
                  "CalibrationFlag : %s (%s)",
                  ISiSProcedure::getRSStr(rs).c_str(),
                  getCTBaseReference(rs, chipIndex)->getCalibrationFlag() == true ? "Yes" : "No" );
        break;
    case RS_COUNT:
    default:
        SIS_LOG_I(SiSLog::getOwnerSiS(), TAG,
                  "CalibrationFlag :\n\t%s  (%s),\n\t%s (%s)",
                  ISiSProcedure::getRSStr(RS_BIN).c_str(),
                  getCTBaseReference(RS_BIN, chipIndex)->getCalibrationFlag() == true ? "Yes" : "No",
                  ISiSProcedure::getRSStr(RS_XRAM).c_str(),
                  getCTBaseReference(RS_XRAM, chipIndex)->getCalibrationFlag() == true ? "Yes" : "No" );
        break;
    }
}

void
CTBaseCore::showFirmwareID(int chipIndex, ReferenceSource rs)
{
    switch (rs)
    {
    case RS_BIN:
    case RS_ROM:
    case RS_XRAM:
        SIS_LOG_I(SiSLog::getOwnerSiS(), TAG,
                  "FirmwareID : %s (%s)",
                  ISiSProcedure::getRSStr(rs).c_str(),
                  getCTBaseReference(rs, chipIndex)->getFirmwareID()->getSerial().c_str() );
        break;
    case RS_COUNT:
    default:
        SIS_LOG_I(SiSLog::getOwnerSiS(), TAG,
                  "FirmwareID :\n\t%s  (%s),\n\t%s (%s)",
                  ISiSProcedure::getRSStr(RS_BIN).c_str(),
                  getCTBaseReference(RS_BIN, chipIndex)->getFirmwareID()->getSerial().c_str(),
                  ISiSProcedure::getRSStr(RS_XRAM).c_str(),
                  getCTBaseReference(RS_XRAM, chipIndex)->getFirmwareID()->getSerial().c_str() );
        break;
    }
}

void
CTBaseCore::showBootloaderID(int chipIndex, ReferenceSource rs)
{
    switch (rs)
    {
    case RS_BIN:
    case RS_ROM:
    case RS_XRAM:
        SIS_LOG_I(SiSLog::getOwnerSiS(), TAG,
                  "BootloaderID : %s (%s)",
                  ISiSProcedure::getRSStr(rs).c_str(),
                  getCTBaseReference(rs, chipIndex)->getBootloaderID()->getSerial().c_str() );
        break;
    case RS_COUNT:
    default:
        SIS_LOG_I(SiSLog::getOwnerSiS(), TAG,
                  "BootloaderID :\n\t%s  (%s),\n\t%s (%s)",
                  ISiSProcedure::getRSStr(RS_BIN).c_str(),
                  getCTBaseReference(RS_BIN, chipIndex)->getBootloaderID()->getSerial().c_str(),
                  ISiSProcedure::getRSStr(RS_XRAM).c_str(),
                  getCTBaseReference(RS_XRAM, chipIndex)->getBootloaderID()->getSerial().c_str() );
        break;
    }
}

void
CTBaseCore::showBootCodeCRC(int chipIndex, ReferenceSource rs)
{
    switch (rs)
    {
    case RS_BIN:
    case RS_ROM:
    case RS_XRAM:
        SIS_LOG_I(SiSLog::getOwnerSiS(), TAG,
                  "BootCodeCRC : %s (%s)",
                  ISiSProcedure::getRSStr(rs).c_str(),
                  getCTBaseReference(rs, chipIndex)->getBootCodeCRC()->getSerial().c_str() );
        break;
    case RS_COUNT:
    default:
        SIS_LOG_I(SiSLog::getOwnerSiS(), TAG,
                  "BootCodeCRC :\n\t%s  (%s),\n\t%s (%s)",
                  ISiSProcedure::getRSStr(RS_BIN).c_str(),
                  getCTBaseReference(RS_BIN, chipIndex)->getBootCodeCRC()->getSerial().c_str(),
                  ISiSProcedure::getRSStr(RS_XRAM).c_str(),
                  getCTBaseReference(RS_XRAM, chipIndex)->getBootCodeCRC()->getSerial().c_str() );
        break;
    }
}

void
CTBaseCore::showMainCodeCRC(int chipIndex, ReferenceSource rs)
{
    switch (rs)
    {
    case RS_BIN:
    case RS_ROM:
    case RS_XRAM:
        SIS_LOG_I(SiSLog::getOwnerSiS(), TAG,
                  "MainCodeCRC : %s (%s)",
                  ISiSProcedure::getRSStr(rs).c_str(),
                  getCTBaseReference(rs, chipIndex)->getMainCodeCRC()->getSerial().c_str() );
        break;
    case RS_COUNT:
    default:
        SIS_LOG_I(SiSLog::getOwnerSiS(), TAG,
                  "MainCodeCRC :\n\t%s  (%s),\n\t%s (%s)",
                  ISiSProcedure::getRSStr(RS_BIN).c_str(),
                  getCTBaseReference(RS_BIN, chipIndex)->getMainCodeCRC()->getSerial().c_str(),
                  ISiSProcedure::getRSStr(RS_XRAM).c_str(),
                  getCTBaseReference(RS_XRAM, chipIndex)->getMainCodeCRC()->getSerial().c_str() );
        break;
    }
}

void
CTBaseCore::showLastID(int chipIndex, ReferenceSource rs)
{
    switch (rs)
    {
    case RS_BIN:
    case RS_ROM:
    case RS_XRAM:
        SIS_LOG_I(SiSLog::getOwnerSiS(), TAG,
                  "LastID : %s (%s)",
                  ISiSProcedure::getRSStr(rs).c_str(),
                  getCTBaseReference(rs, chipIndex)->getLastID()->getSerial().c_str() );
        break;
    case RS_COUNT:
    default:
        SIS_LOG_I(SiSLog::getOwnerSiS(), TAG,
                  "LastID :\n\t%s  (%s),\n\t%s (%s)",
                  ISiSProcedure::getRSStr(RS_BIN).c_str(),
                  getCTBaseReference(RS_BIN, chipIndex)->getLastID()->getSerial().c_str(),
                  ISiSProcedure::getRSStr(RS_XRAM).c_str(),
                  getCTBaseReference(RS_XRAM, chipIndex)->getLastID()->getSerial().c_str() );
        break;
    }
}

void
CTBaseCore::showLastTime(int chipIndex, ReferenceSource rs)
{
    switch (rs)
    {
    case RS_BIN:
    case RS_ROM:
    case RS_XRAM:
        SIS_LOG_I(SiSLog::getOwnerSiS(), TAG,
                  "LastTime : %s (%s)",
                  ISiSProcedure::getRSStr(rs).c_str(),
                  getCTBaseReference(rs, chipIndex)->getLastTime()->getSerial().c_str() );
        break;
    case RS_COUNT:
    default:
        SIS_LOG_I(SiSLog::getOwnerSiS(), TAG,
                  "LastTime :\n\t%s  (%s),\n\t%s (%s)",
                  ISiSProcedure::getRSStr(RS_BIN).c_str(),
                  getCTBaseReference(RS_BIN, chipIndex)->getLastTime()->getSerial().c_str(),
                  ISiSProcedure::getRSStr(RS_XRAM).c_str(),
                  getCTBaseReference(RS_XRAM, chipIndex)->getLastTime()->getSerial().c_str() );
        break;
    }
}

void
CTBaseCore::showPriorLastID(int chipIndex, ReferenceSource rs)
{
    switch (rs)
    {
    case RS_BIN:
    case RS_ROM:
    case RS_XRAM:
        SIS_LOG_I(SiSLog::getOwnerSiS(), TAG,
                  "PriorLastID : %s (%s)",
                  ISiSProcedure::getRSStr(rs).c_str(),
                  getCTBaseReference(rs, chipIndex)->getPriorLastID()->getSerial().c_str() );
        break;
    case RS_COUNT:
    default:
        SIS_LOG_I(SiSLog::getOwnerSiS(), TAG,
                  "PriorLastID :\n\t%s  (%s),\n\t%s (%s)",
                  ISiSProcedure::getRSStr(RS_BIN).c_str(),
                  getCTBaseReference(RS_BIN, chipIndex)->getPriorLastID()->getSerial().c_str(),
                  ISiSProcedure::getRSStr(RS_XRAM).c_str(),
                  getCTBaseReference(RS_XRAM, chipIndex)->getPriorLastID()->getSerial().c_str() );
        break;
    }
}

void
CTBaseCore::showPriorLastTime(int chipIndex, ReferenceSource rs)
{
    switch (rs)
    {
    case RS_BIN:
    case RS_ROM:
    case RS_XRAM:
        SIS_LOG_I(SiSLog::getOwnerSiS(), TAG,
                  "PriorLastTime : %s (%s)",
                  ISiSProcedure::getRSStr(rs).c_str(),
                  getCTBaseReference(rs, chipIndex)->getPriorLastTime()->getSerial().c_str() );
        break;
    case RS_COUNT:
    default:
        SIS_LOG_I(SiSLog::getOwnerSiS(), TAG,
                  "PriorLastTime :\n\t%s  (%s),\n\t%s (%s)",
                  ISiSProcedure::getRSStr(RS_BIN).c_str(),
                  getCTBaseReference(RS_BIN, chipIndex)->getPriorLastTime()->getSerial().c_str(),
                  ISiSProcedure::getRSStr(RS_XRAM).c_str(),
                  getCTBaseReference(RS_XRAM, chipIndex)->getPriorLastTime()->getSerial().c_str() );
        break;
    }
}

void
CTBaseCore::showProductID(int chipIndex, ReferenceSource rs)
{
    switch (rs)
    {
    case RS_BIN:
    case RS_ROM:
    case RS_XRAM:
        SIS_LOG_I(SiSLog::getOwnerSiS(), TAG,
                  "ProductID : %s (%s)",
                  ISiSProcedure::getRSStr(rs).c_str(),
                  getCTBaseReference(rs, chipIndex)->getProductID()->getSerial().c_str() );
        break;
    case RS_COUNT:
    default:
        SIS_LOG_I(SiSLog::getOwnerSiS(), TAG,
                  "ProductID :\n\t%s  (%s),\n\t%s (%s)",
                  ISiSProcedure::getRSStr(RS_BIN).c_str(),
                  getCTBaseReference(RS_BIN, chipIndex)->getProductID()->getSerial().c_str(),
                  ISiSProcedure::getRSStr(RS_XRAM).c_str(),
                  getCTBaseReference(RS_XRAM, chipIndex)->getProductID()->getSerial().c_str() );
        break;
    }
}

void
CTBaseCore::showUpdateMark(int chipIndex, ReferenceSource rs)
{
    switch (rs)
    {
    case RS_BIN:
    case RS_ROM:
    case RS_XRAM:
        SIS_LOG_I(SiSLog::getOwnerSiS(), TAG,
                  "UpdateMark : %s (%s)",
                  ISiSProcedure::getRSStr(rs).c_str(),
                  getCTBaseReference(rs, chipIndex)->getUpdateMark()->getSerial().c_str() );
        break;
    case RS_COUNT:
    default:
        SIS_LOG_I(SiSLog::getOwnerSiS(), TAG,
                  "UpdateMark :\n\t%s  (%s),\n\t%s (%s)",
                  ISiSProcedure::getRSStr(RS_BIN).c_str(),
                  getCTBaseReference(RS_BIN, chipIndex)->getUpdateMark()->getSerial().c_str(),
                  ISiSProcedure::getRSStr(RS_XRAM).c_str(),
                  getCTBaseReference(RS_XRAM, chipIndex)->getUpdateMark()->getSerial().c_str() );
        break;
    }
}


void
CTBaseCore::showLastUpdateMark(int chipIndex, ReferenceSource rs)
{
    switch (rs)
    {
    case RS_BIN:
    case RS_ROM:
    case RS_XRAM:
        SIS_LOG_I(SiSLog::getOwnerSiS(), TAG,
                  "LastUpdateMark : %s (%s)",
                  ISiSProcedure::getRSStr(rs).c_str(),
                  getCTBaseReference(rs, chipIndex)->getLastUpdateMark()->getSerial().c_str() );
        break;
    case RS_COUNT:
    default:
        SIS_LOG_I(SiSLog::getOwnerSiS(), TAG,
                  "LastUpdateMark :\n\t%s  (%s),\n\t%s (%s)",
                  ISiSProcedure::getRSStr(RS_BIN).c_str(),
                  getCTBaseReference(RS_BIN, chipIndex)->getLastUpdateMark()->getSerial().c_str(),
                  ISiSProcedure::getRSStr(RS_XRAM).c_str(),
                  getCTBaseReference(RS_XRAM, chipIndex)->getLastUpdateMark()->getSerial().c_str() );
        break;
    }
}

void
CTBaseCore::showPriorLastUpdateMark(int chipIndex, ReferenceSource rs)
{
    switch (rs)
    {
    case RS_BIN:
    case RS_ROM:
    case RS_XRAM:
        SIS_LOG_I(SiSLog::getOwnerSiS(), TAG,
                  "PriorLastUpdateMark : %s (%s)",
                  ISiSProcedure::getRSStr(rs).c_str(),
                  getCTBaseReference(rs, chipIndex)->getPriorLastUpdateMark()->getSerial().c_str() );
        break;
    case RS_COUNT:
    default:
        SIS_LOG_I(SiSLog::getOwnerSiS(), TAG,
                  "PriorLastUpdateMark :\n\t%s  (%s),\n\t%s (%s)",
                  ISiSProcedure::getRSStr(RS_BIN).c_str(),
                  getCTBaseReference(RS_BIN, chipIndex)->getPriorLastUpdateMark()->getSerial().c_str(),
                  ISiSProcedure::getRSStr(RS_XRAM).c_str(),
                  getCTBaseReference(RS_XRAM, chipIndex)->getPriorLastUpdateMark()->getSerial().c_str() );
        break;
    }
}
