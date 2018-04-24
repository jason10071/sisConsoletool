#include "updatefwcore_general.h"

#include "sislog.h"
#include "ctexception/ctexception.h"
#include "spexception/spexception.h"
#include "binloader/binloader.h"

using namespace CT;
using namespace SiS;
using namespace SiS::Procedure;

#define EXCEPTION_TITLE "UpdateFWCore_general Exception : "
#define TAG "UpdateFWCore_general"

UpdateFWCore_general::UpdateFWCore_general(UpdateFWParameter* updateFWParameter) :
    IUpdateFWCore(updateFWParameter)
{

}

UpdateFWCore_general::~UpdateFWCore_general()
{

}

CTExitCode
UpdateFWCore_general::exec()
{
    /* prepare All bin */
    prepareAllBin();

    /* prepare Bin Master Ref */
    prepareBinRef(CI_MASTER);

    /* prepare Xram Master Ref */
    prepareXramRef(CI_MASTER);

    /* prepare Rom Master Ref */
    prepareRomRef(CI_MASTER);

    /* show Master Info */
    showInfo(CI_MASTER);
    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "");

    /* prepare Bin Slave Ref */
    for(int chipIndex = CI_SLAVE_0; chipIndex < m_updateFWParameter->getBinWrapMap().size(); chipIndex++)
    {
        prepareBinRef(chipIndex);
    }

    /* check */
    checkFullBinCount();
    checkAllBinSize();
    checkAllBinDeviceType();
    checkAllBinInterfaceID();
    checkAllBinSelectiveID();
    checkMasterBinProductID();
    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "");

    /* confirm update */
    confirmUpdate();

    /* modify Update Stamp */
    if( !m_updateFWParameter->getDisableStamp() )
    {
        modifyUpdateStamp(CI_MASTER);
    }

    /* MasterRecovery if need */
    if( ifNeedMasterRecovery() )
    {
        if( ifNeedRestructureBootloader(CI_MASTER) )
        {
            restructureBootloader(CI_MASTER);
            resetDevice();
        }

        masterRecovery();
        resetDevice();
    }

    /* prepare Xram Slave Ref */
    for(int chipIndex = CI_SLAVE_0; chipIndex < m_updateFWParameter->getBinWrapMap().size(); chipIndex++)
    {
        prepareXramRef(chipIndex);
    }

    /* prepare Rom Slave Ref */
    for(int chipIndex = CI_SLAVE_0; chipIndex < m_updateFWParameter->getBinWrapMap().size(); chipIndex++)
    {
        prepareRomRef(chipIndex);
    }

    /* show Slave Info */
    for(int chipIndex = CI_SLAVE_0; chipIndex < m_updateFWParameter->getBinWrapMap().size(); chipIndex++)
    {
        showInfo(chipIndex);
    }

    /* check */
    checkAllBinProductID();
    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "");

    /* modify Update Stamp */
    if( !m_updateFWParameter->getDisableStamp() )
    {
        for(int chipIndex = CI_SLAVE_0; chipIndex < m_updateFWParameter->getBinWrapMap().size(); chipIndex++)
        {
            modifyUpdateStamp(chipIndex);
        }
    }

    /* RestructureBootloader if need */
    if( ifNeedRestructureBootloader() )
    {
        restructureBootloader();
    }

    /* do UpdateFW */
    doUpdateFW();

    return CT_EXIT_PASS;
}

void
UpdateFWCore_general::prepareBinRef(int chipIndex)
{
    if( chipIndex == CI_MASTER )
    {
        prepareBinMasterRef();
    }
    else
    {
        prepareBinSlaveRef(chipIndex);
    }
}

void
UpdateFWCore_general::prepareXramRef(int chipIndex)
{
    if( chipIndex == CI_MASTER )
    {
        prepareXramMasterRef();
    }
    else
    {
        prepareXramSlaveRef(chipIndex);
    }
}

void
UpdateFWCore_general::prepareRomRef(int chipIndex)
{
    if( chipIndex == CI_MASTER )
    {
        prepareRomMasterRef();
    }
    else
    {
        prepareRomSlaveRef(chipIndex);
    }
}

void
UpdateFWCore_general::showInfo(int chipIndex)
{
    if( chipIndex == CI_MASTER )
    {
        showMasterInfo();
    }
    else
    {
        showSlaveInfo(chipIndex);
    }
}

void
UpdateFWCore_general::prepareBinMasterRef()
{
    /* switch SiSProcedure by OS DeviceType */
    m_sisProcedure->switchSiSProcedure( m_ctBaseParameter->getOSDeviceType() );

    /* init Bin Master reference */
    insertReference(RS_BIN, CI_MASTER, generateReference());
    UpdateFWReference* binMasterRef = getUpdateFWReference(RS_BIN, CI_MASTER);

    fetchDeviceType(RS_BIN, CI_MASTER); /* 1. DeviceType */

    /* switch SiSProcedure by Bin Master DeviceType */
    m_sisProcedure->switchSiSProcedure( binMasterRef->getDeviceType() );

    fetchBroken(RS_BIN, CI_MASTER); /* 2. Broken */
    fetchFWSizeType(RS_BIN, CI_MASTER, binMasterRef->isBroken()); /* 3. FWSizeType */

    /* switch SiSProcedure by Bin Master FWSizeType */
    m_sisProcedure->switchSiSProcedure( binMasterRef->getFWSizeType() );

    fetchInterfaceID(RS_BIN, CI_MASTER); /* 4. InterfaceID */
    fetchSlaveNum(RS_BIN); /* 5. SlaveNum */
    fetchMultiChipSelectiveID(RS_BIN, CI_MASTER); /* 6. MultiChipSelectiveID */
    fetchCalibrationFlag(RS_BIN, CI_MASTER); /* 7. CalibrationFlag */
    fetchFirmwareID(RS_BIN, CI_MASTER); /* 8. FirmwareID */
    fetchBootloaderID(RS_BIN, CI_MASTER); /* 9. BootloaderID */
    fetchBootCodeCRC(RS_BIN, CI_MASTER); /* 10. BootCodeCRC */
    fetchMainCodeCRC(RS_BIN, CI_MASTER); /* 11. MainCodeCRC */
    fetchLastID(RS_BIN, CI_MASTER); /* 12. LastID */
    fetchLastTime(RS_BIN, CI_MASTER); /* 13. LastTime */
    fetchPriorLastID(RS_BIN, CI_MASTER); /* 14. PriorLastID */
    fetchPriorLastTime(RS_BIN, CI_MASTER); /* 15. PriorLastTime */
}

void
UpdateFWCore_general::prepareXramMasterRef()
{
    /* switch SiSProcedure by OS DeviceType */
    m_sisProcedure->switchSiSProcedure( m_ctBaseParameter->getOSDeviceType() );

    /* init Xram Master reference */
    insertReference(RS_XRAM, CI_MASTER, generateReference());
    CTBaseReference* xramMasterRef = getUpdateFWReference(RS_XRAM, CI_MASTER);

    fetchDeviceType(RS_XRAM, CI_MASTER); /* 1. DeviceType */

    /* switch SiSProcedure by Xram Master DeviceType */
    m_sisProcedure->switchSiSProcedure( xramMasterRef->getDeviceType() );

    fetchBroken(RS_XRAM, CI_MASTER); /* 2. Broken */
    fetchFWSizeType(RS_XRAM, CI_MASTER, xramMasterRef->isBroken()); /* 3. FWSizeType */

    /* switch SiSProcedure by Xram Master FWSizeType */
    m_sisProcedure->switchSiSProcedure( xramMasterRef->getFWSizeType() );

    /* check mainCodeReliable */
    bool mainCodeReliable = false;
    if( !xramMasterRef->isBroken() )
    {
        fetchInterfaceID(RS_XRAM, CI_MASTER); /* 4. InterfaceID */

        if( !xramMasterRef->isDummy() )
        {
            mainCodeReliable = true; // not broken and dummy
        }
        else
        {
            SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "!!! Master (chipIndex=0) FW InterfaceID : Dummy");
        }
    }
    else
    {
        SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "!!! Master (chipIndex=0) FW Broken");
    }

    if( mainCodeReliable )
    {
        fetchSlaveNum(RS_XRAM); /* 5. SlaveNum */
        fetchMultiChipSelectiveID(RS_XRAM, CI_MASTER); /* 6. MultiChipSelectiveID */
        fetchCalibrationFlag(RS_XRAM, CI_MASTER); /* 7. CalibrationFlag */
        fetchMainCodeCRC(RS_XRAM, CI_MASTER); /* 11. MainCodeCRC */
    }
    else
    {
        SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "!!! MainCode is unreliable");
        /* using bin instead */

        /* 5. SlaveNum */
        if( m_ctBaseParameter->getSlaveNumUserDefined() )
        {
            xramMasterRef->setSlaveNum( m_ctBaseParameter->getSlaveNum() );
            SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "user had forced to set SlaveNum : %d", xramMasterRef->getSlaveNum() );
        }
        else
        {
            xramMasterRef->setSlaveNum( getUpdateFWReference(RS_BIN, CI_MASTER)->getSlaveNum() );
            SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "!!! using (BIN) SlaveNum instead of (XRAM) : %d", xramMasterRef->getSlaveNum());
        }  
    }

    /* it needs to be rocorded no matter the fw was broken or not */
    fetchFirmwareID(RS_XRAM, CI_MASTER); /* 8. FirmwareID */
    fetchLastID(RS_XRAM, CI_MASTER);/* 12. LastID */
    fetchLastTime(RS_XRAM, CI_MASTER); /* 13. LastTime */
    fetchPriorLastID(RS_XRAM, CI_MASTER);/* 14. PriorLastID */
    fetchPriorLastTime(RS_XRAM, CI_MASTER);/* 15. PriorLastTime */

    /* it is still reliable from boot code */
    fetchBootloaderID(RS_XRAM, CI_MASTER); /* 9. BootloaderID */
    fetchBootCodeCRC(RS_XRAM, CI_MASTER); /* 10. BootCodeCRC */
}

void
UpdateFWCore_general::prepareRomMasterRef()
{
    /* switch SiSProcedure by Xram Master FWSizeType */
    m_sisProcedure->switchSiSProcedure( getUpdateFWReference(RS_XRAM, CI_MASTER)->getFWSizeType() );

    /* init Rom Master reference */
    insertReference(RS_ROM, CI_MASTER, generateReference());
    UpdateFWReference* romMasterRef = getUpdateFWReference(RS_ROM, CI_MASTER);

    /* 3. FWSizeType */
    romMasterRef->setFWSizeType( m_sisProcedure->readRomFWSizeType(CI_MASTER) );
}

void
UpdateFWCore_general::showMasterInfo()
{
    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "Master (chipIndex=0):\n");

    showDeviceType(CI_MASTER); /* 1. DeviceType */
    showBroken(CI_MASTER); /* 2. Broken */
    showFWSizeType(CI_MASTER); /* 3. FWSizeType */
    showInterfaceID(CI_MASTER); /* 4. InterfaceID */
    showSlaveNum(); /* 5. SlaveNum */
    showMultiChipSelectiveID(CI_MASTER); /* 6. MultiChipSelectiveID */
    showCalibrationFlag(CI_MASTER); /* 7. CalibrationFlag */
    showFirmwareID(CI_MASTER); /* 8. FirmwareID */
    showBootloaderID(CI_MASTER); /* 9. BootloaderID */
    showBootCodeCRC(CI_MASTER); /* 10. BootCodeCRC */
    showMainCodeCRC(CI_MASTER); /* 11. MainCodeCRC */
    showLastID(CI_MASTER); /* 12. LastID */
    showLastTime(CI_MASTER); /* 13. LastTime */
    showPriorLastID(CI_MASTER); /* 14. PriorLastID */
    showPriorLastTime(CI_MASTER); /* 15. PriorLastTime */
}

void
UpdateFWCore_general::prepareBinSlaveRef(int chipIndex)
{
    if( chipIndex == CI_MASTER )
    {
        std::string msg = EXCEPTION_TITLE;
        char errorMsg[1024] = "";
        sprintf(errorMsg, "prepareBinSlaveRef error ! chipIndex == CI_MASTER");
        msg.append(errorMsg);
        throw CTException( msg );
    }

    /* switch SiSProcedure by OS DeviceType */
    m_sisProcedure->switchSiSProcedure( m_ctBaseParameter->getOSDeviceType() );

    /* init Bin Slave reference */
    insertReference(RS_BIN, chipIndex, generateReference());
    UpdateFWReference* binSlaveRef = getUpdateFWReference(RS_BIN, chipIndex);

    fetchDeviceType(RS_BIN, chipIndex); /* 1. DeviceType */

    /* switch SiSProcedure by Bin Slave DeviceType */
    m_sisProcedure->switchSiSProcedure( binSlaveRef->getDeviceType() );

    fetchBroken(RS_BIN, chipIndex); /* 2. Broken */
    fetchFWSizeType(RS_BIN, chipIndex, binSlaveRef->isBroken()); /* 3. FWSizeType */

    /* switch SiSProcedure by Bin Slave FWSizeType */
    m_sisProcedure->switchSiSProcedure( binSlaveRef->getFWSizeType() );

    fetchInterfaceID(RS_BIN, chipIndex); /* 4. InterfaceID */
    fetchMultiChipSelectiveID(RS_BIN, chipIndex); /* 6. MultiChipSelectiveID */
    fetchCalibrationFlag(RS_BIN, chipIndex); /* 7. CalibrationFlag */
    fetchFirmwareID(RS_BIN, chipIndex); /* 8. FirmwareID */
    fetchBootloaderID(RS_BIN, chipIndex); /* 9. BootloaderID */
    fetchBootCodeCRC(RS_BIN, chipIndex); /* 10. BootCodeCRC */
    fetchMainCodeCRC(RS_BIN, chipIndex); /* 11. MainCodeCRC */
    fetchLastID(RS_BIN, chipIndex); /* 12. LastID */
    fetchLastTime(RS_BIN, chipIndex); /* 13. LastTime */
    fetchPriorLastID(RS_BIN, chipIndex); /* 14. PriorLastID */
    fetchPriorLastTime(RS_BIN, chipIndex); /* 15. PriorLastTime */
}

void
UpdateFWCore_general::prepareXramSlaveRef(int chipIndex)
{
    if( chipIndex == CI_MASTER )
    {
        std::string msg = EXCEPTION_TITLE;
        char errorMsg[1024] = "";
        sprintf(errorMsg, "prepareXramSlaveRef error ! chipIndex == CI_MASTER");
        msg.append(errorMsg);
        throw CTException( msg );
    }

    /* switch SiSProcedure by OS DeviceType */
    m_sisProcedure->switchSiSProcedure( m_ctBaseParameter->getOSDeviceType() );

    /* init Xram Slave reference */
    insertReference(RS_XRAM, chipIndex, generateReference());
    CTBaseReference* xramSlaveRef = getUpdateFWReference(RS_XRAM, chipIndex);

    fetchDeviceType(RS_XRAM, chipIndex); /* 1. DeviceType */

    /* switch SiSProcedure by Xram Slave DeviceType */
    m_sisProcedure->switchSiSProcedure( xramSlaveRef->getDeviceType() );

    fetchBroken(RS_XRAM, chipIndex); /* 2. Broken */
    fetchFWSizeType(RS_XRAM, chipIndex, xramSlaveRef->isBroken()); /* 3. FWSizeType */

    /* switch SiSProcedure by Xram Slave FWSizeType */
    m_sisProcedure->switchSiSProcedure( xramSlaveRef->getFWSizeType() );

    /* check mainCodeReliable */
    bool mainCodeReliable = false;
    if( !xramSlaveRef->isBroken() )
    {
        fetchInterfaceID(RS_XRAM, chipIndex); /* 4. InterfaceID */

        if( !xramSlaveRef->isDummy() )
        {
            mainCodeReliable = true; // not broken nor dummy
        }
        else
        {
            SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "!!! %s FW InterfaceID : Dummy",
                            ISiSProcedure::getCIStr(chipIndex).c_str());
        }
    }
    else
    {
        SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "!!! %s FW Broken",
                        ISiSProcedure::getCIStr(chipIndex).c_str());
    }

    if( mainCodeReliable )
    {
        fetchMultiChipSelectiveID(RS_XRAM, chipIndex); /* 6. MultiChipSelectiveID */
        fetchCalibrationFlag(RS_XRAM, chipIndex); /* 7. CalibrationFlag */        
        fetchMainCodeCRC(RS_XRAM, chipIndex); /* 11. MainCodeCRC */        
    }
    else
    {
        SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "!!! MainCode is unreliable");
        /* using bin instead */
    }
    /* it needs to be rocorded no matter the fw was broken or not */
    fetchFirmwareID(RS_XRAM, chipIndex); /* 8. FirmwareID */
    fetchLastID(RS_XRAM, chipIndex);/* 12. LastID */
    fetchLastTime(RS_XRAM, chipIndex); /* 13. LastTime */
    fetchPriorLastID(RS_XRAM, chipIndex);/* 14. PriorLastID */
    fetchPriorLastTime(RS_XRAM, chipIndex);/* 15. PriorLastTime */

    /* it is still reliable from boot code */
    fetchBootloaderID(RS_XRAM, chipIndex); /* 9. BootloaderID */
    fetchBootCodeCRC(RS_XRAM, chipIndex); /* 10. BootCodeCRC */
}

void
UpdateFWCore_general::prepareRomSlaveRef(int chipIndex)
{
    if( chipIndex == CI_MASTER )
    {
        std::string msg = EXCEPTION_TITLE;
        char errorMsg[1024] = "";
        sprintf(errorMsg, "prepareRomSlaveRef error ! chipIndex == CI_MASTER");
        msg.append(errorMsg);
        throw CTException( msg );
    }

    /* switch SiSProcedure by Xram Slave FWSizeType */
    m_sisProcedure->switchSiSProcedure( getUpdateFWReference(RS_XRAM, chipIndex)->getFWSizeType() );

    /* init Rom Slave reference */
    insertReference(RS_ROM, chipIndex, generateReference());
    UpdateFWReference* romSlaveRef = getUpdateFWReference(RS_ROM, chipIndex);

    /* 3. FWSizeType */
    romSlaveRef->setFWSizeType( m_sisProcedure->readRomFWSizeType(chipIndex) );
}

void
UpdateFWCore_general::showSlaveInfo(int chipIndex)
{
    if( chipIndex == CI_MASTER )
    {
        std::string msg = EXCEPTION_TITLE;
        char errorMsg[1024] = "";
        sprintf(errorMsg, "showSlaveInfo error ! chipIndex == CI_MASTER");
        msg.append(errorMsg);
        throw CTException( msg );
    }

    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s :\n", ISiSProcedure::getCIStr(chipIndex).c_str());

    showDeviceType(chipIndex); /* 1. DeviceType */
    showBroken(chipIndex); /* 2. Broken */
    showFWSizeType(chipIndex); /* 3. FWSizeType */
    showInterfaceID(chipIndex); /* 4. InterfaceID */
    showMultiChipSelectiveID(chipIndex); /* 6. MultiChipSelectiveID */
    showCalibrationFlag(chipIndex); /* 7. CalibrationFlag */
    showFirmwareID(chipIndex); /* 8. FirmwareID */
    showBootloaderID(chipIndex); /* 9. BootloaderID */
    showBootCodeCRC(chipIndex); /* 10. BootCodeCRC */
    showMainCodeCRC(chipIndex); /* 11. MainCodeCRC */
    showLastID(chipIndex); /* 12. LastID */
    showLastTime(chipIndex); /* 13. LastTime */
    showPriorLastID(chipIndex); /* 14. PriorLastID */
    showPriorLastTime(chipIndex); /* 15. PriorLastTime */
}

void
UpdateFWCore_general::modifyUpdateStamp(int chipIndex)
{
    UpdateFWReference* xramRef = getUpdateFWReference(RS_XRAM, chipIndex);
    UpdateFWReference* binRef = getUpdateFWReference(RS_BIN, chipIndex);

    /* switch SiSProcedure by Bin FWSizeType */
    m_sisProcedure->switchSiSProcedure( binRef->getFWSizeType() );

    /* write update mark */
    const short updateMarkLength = 4;
    unsigned char *updateMark = new unsigned char[updateMarkLength-1];
    updateMark[0] = 0x50;
    updateMark[1] = 0x38;
    updateMark[2] = 0x31;
    updateMark[3] = 0x30;
    SerialData *updateMarkSerial = new SerialData(updateMark, updateMarkLength);
    m_sisProcedure->writeUpdateMark( RS_BIN, chipIndex, updateMarkSerial );

    /* write update bootloader infomation */
    const short isUpdateBootloaderInfoLength = 2;
    unsigned char *isUpdateBootloaderInfo = new unsigned char[isUpdateBootloaderInfoLength-1];
    if( xramRef->isBroken() || ifNeedUpdateBootloader(chipIndex) || ifNeedRestructureBootloader(chipIndex) )
    {
        isUpdateBootloaderInfo[0] = 'u';    //update
        isUpdateBootloaderInfo[1] = 'b';    //bootloader
    }
    else
    {
        isUpdateBootloaderInfo[0] = 'n';    //no update
        isUpdateBootloaderInfo[1] = 'b';    //bootloader
    }
    SerialData *isUpdateBootloaderInfoSerial = new SerialData(isUpdateBootloaderInfo, isUpdateBootloaderInfoLength);
    m_sisProcedure->writeIsUpdateBootloaderInfo( RS_BIN, chipIndex, isUpdateBootloaderInfoSerial );

    /* write last infomation */
    m_sisProcedure->writeLastID( RS_BIN, chipIndex, xramRef->getFirmwareID() );
    SerialData* now = ISiSProcedure::getTimestamp();
    m_sisProcedure->writeLastTime( RS_BIN, chipIndex, now );
    delete now;

    /* write prior last infomation */
    m_sisProcedure->writePriorLastID( RS_BIN, chipIndex, xramRef->getLastID() );
    m_sisProcedure->writePriorLastTime( RS_BIN, chipIndex, xramRef->getLastTime() );
}

void
UpdateFWCore_general::checkFullBinCount()
{
    /* compare XRAM, using BIN insted of XRAM if dummy */

    int binCount = m_updateFWParameter->getBinWrapMap().size();
    int chipCount = 1 + getUpdateFWReference(RS_XRAM, CI_MASTER)->getSlaveNum();
    bool ret = (binCount == chipCount) ? true : false;

    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s checkFullBinCount : binCount(%d) full match chipCount(%d) : %s", SYMBOL_CTL_FLOW,
                    binCount, chipCount, ret == true ? "Yes" : "No");

    if( !ret )
    {
        std::string msg = EXCEPTION_TITLE;
        char errorMsg[1024] = "";
        sprintf(errorMsg, "checkFullBinCount error ! binCount is not full match chipCount");
        msg.append(errorMsg);
        throw CTException( msg );
    }
}

void
UpdateFWCore_general::checkAllBinSize()
{
    if( m_updateFWParameter->getJumpCheckBinSize() || m_updateFWParameter->getJumpCheckAll() )
    {
        SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s checkAllBinSize : Jump", SYMBOL_CTL_FLOW);
        return;
    }

    /* compare to BIN itself */
    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s checkAllBinSize : ", SYMBOL_CTL_FLOW);

    for(std::map<ChipIndexKey, BinWrap*>::iterator it = m_updateFWParameter->getBinWrapMap().begin();
        it != m_updateFWParameter->getBinWrapMap().end();
        ++it)
    {
        int chipIndex = it->first;
        BinWrap* binWrap = it->second;
        FWSizeType binFWSizeType = getUpdateFWReference(RS_BIN, chipIndex)->getFWSizeType();

        /* check bin size if match FWSizeType */
        if( (binFWSizeType == FST_817_R64K || binFWSizeType == FST_817_R64K_M48K || binFWSizeType == FST_R64K)
                && binWrap->getSize() != _64K )
        {
            std::string msg = EXCEPTION_TITLE;
            char errorMsg[1024] = "";
            sprintf(errorMsg, "checkAllBinSize error ! %s bin size (%d) not match FWSizeType (%s)",
                    ISiSProcedure::getCIStr(chipIndex).c_str(),
                    binWrap->getSize(),
                    ISiSProcedure::getFSTStr(binFWSizeType).c_str());
            msg.append(errorMsg);
            throw CTException( msg );
        }
        else if( (binFWSizeType == FST_817_R128K || binFWSizeType == FST_819_R128K || binFWSizeType == FST_R128K)
                 && binWrap->getSize() != _128K )
        {
            std::string msg = EXCEPTION_TITLE;
            char errorMsg[1024] = "";
            sprintf(errorMsg, "checkAllBinSize error ! %s bin size (%d) not match FWSizeType (%s)",
                    ISiSProcedure::getCIStr(chipIndex).c_str(),
                    binWrap->getSize(),
                    ISiSProcedure::getFSTStr(binFWSizeType).c_str());
            msg.append(errorMsg);
            throw CTException( msg );
        }
        else if( binWrap->getSize() != _64K && binWrap->getSize() != _128K )
        {
            std::string msg = EXCEPTION_TITLE;
            char errorMsg[1024] = "";
            sprintf(errorMsg, "checkAllBinSize error ! %s size=%d, is not 64k/128k : ",
                    ISiSProcedure::getCIStr(chipIndex).c_str(),
                    binWrap->getSize() );
            msg.append(errorMsg);
            throw CTException( msg );
        }

        /* check if all bin are the same FWSizeType */
        if( m_updateFWParameter->getBinWrapMap().size() > 1 &&
                binFWSizeType != getUpdateFWReference(RS_BIN, CI_MASTER)->getFWSizeType() )
        {
            std::string msg = EXCEPTION_TITLE;
            char errorMsg[1024] = "";
            sprintf(errorMsg, "checkAllBinSize error ! has different FWSizeType : %s (%s), %s (%s)",
                    ISiSProcedure::getCIStr(CI_MASTER).c_str(),
                    ISiSProcedure::getFSTStr(getUpdateFWReference(RS_BIN, CI_MASTER)->getFWSizeType()).c_str(),
                    ISiSProcedure::getCIStr(chipIndex).c_str(),
                    ISiSProcedure::getFSTStr(binFWSizeType).c_str() );
            msg.append(errorMsg);
            throw CTException( msg );
        }
    }

    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s checkAllBinSize : all are the same FWSizeType : Yes", SYMBOL_CTL_FLOW);
}

void
UpdateFWCore_general::checkAllBinDeviceType()
{
    if( m_updateFWParameter->getJumpCheckDeviceType() || m_updateFWParameter->getJumpCheckAll() )
    {
        SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s checkAllBinDeviceType : Jump", SYMBOL_CTL_FLOW);
        return;
    }

    /* compare to OS */
    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s checkAllBinDeviceType : ", SYMBOL_CTL_FLOW);

    DeviceType osDeviceType = m_updateFWParameter->getOSDeviceType();

    for(std::map<ChipIndexKey, BinWrap*>::iterator it = m_updateFWParameter->getBinWrapMap().begin();
        it != m_updateFWParameter->getBinWrapMap().end();
        ++it)
    {
        int chipIndex = it->first;

        DeviceType binDeviceType = getUpdateFWReference(RS_BIN, chipIndex)->getDeviceType();

        if( osDeviceType != binDeviceType )
        {
            std::string msg = EXCEPTION_TITLE;
            char errorMsg[1024] = "";
            sprintf(errorMsg, "checkAllBinDeviceType error ! OS : %s, %s : %s",
                    ISiSProcedure::getDTStr(osDeviceType).c_str(),
                    ISiSProcedure::getCIStr(chipIndex).c_str(),
                    ISiSProcedure::getDTStr(binDeviceType).c_str() );
            msg.append(errorMsg);
            throw CTException( msg );
        }
    }

    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s checkAllBinDeviceType : all are the same (OS) DeviceType (%s) : Yes", SYMBOL_CTL_FLOW,
                    ISiSProcedure::getDTStr(osDeviceType).c_str());
}

void
UpdateFWCore_general::checkAllBinInterfaceID()
{
    if( m_updateFWParameter->getJumpCheckInterfaceID() || m_updateFWParameter->getJumpCheckAll() )
    {
        SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s checkAllBinInterfaceID : Jump", SYMBOL_CTL_FLOW);
        return;
    }

    /* compare to OS */
    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s checkAllBinInterfaceID : ", SYMBOL_CTL_FLOW);

    DeviceInterface osDeviceInterface = m_updateFWParameter->getOSDeviceInterface();

    if( osDeviceInterface == DI_I2C || osDeviceInterface == DI_HID_I2C || osDeviceInterface == DI_HID_USB || osDeviceInterface == DI_SPI )
    {
        /* ignore Dummy (Unknow) */

        for(std::map<ChipIndexKey, BinWrap*>::iterator it = m_updateFWParameter->getBinWrapMap().begin();
            it != m_updateFWParameter->getBinWrapMap().end();
            ++it)
        {
            int chipIndex = it->first;

            unsigned char binInterfaceID = getUpdateFWReference(RS_BIN, chipIndex)->getInterfaceID();
            DeviceType binDeviceType = getUpdateFWReference(RS_BIN, chipIndex)->getDeviceType();

            if( chipIndex == CI_MASTER )
            {
                /* Master */                
                if( m_sisProcedure->readSubProtocol()==DT_BRIDGE )
                {
                    if( osDeviceInterface == DI_HID_USB &&
                            (binInterfaceID == INTERFACE_ID_I2C || binInterfaceID == INTERFACE_ID_USB_I2C_819) )
                    {
                        /* bridge : bin is I2C, but OS is USB */
                        continue;
                    }
                    else
                    {
                        std::string msg = EXCEPTION_TITLE;
                        char errorMsg[1024] = "";
                        sprintf(errorMsg, "checkAllBinInterfaceID error ! bridge compare to (OS) %s, %s : 0x%x (I2C:01, USB:02, SPI:03, USB+I2C:04, Dummy:dd)",
                                ISiSProcedure::getDIStr(osDeviceInterface).c_str(),
                                ISiSProcedure::getCIStr(chipIndex).c_str(),
                                binInterfaceID );
                        msg.append(errorMsg);
                        throw CTException( msg );
                    }

                }
                else if( binDeviceType == DT_817 )
                {
                    if( binInterfaceID == INTERFACE_ID_I2C &&
                            (osDeviceInterface == DI_I2C || osDeviceInterface == DI_HID_I2C) )
                    {
                        /* I2C */
                        continue;
                    }
                    else if( binInterfaceID == INTERFACE_ID_USB && osDeviceInterface == DI_HID_USB )
                    {
                        /* USB*/
                        continue;
                    }
                    else
                    {
                        std::string msg = EXCEPTION_TITLE;
                        char errorMsg[1024] = "";
                        sprintf(errorMsg, "checkAllBinInterfaceID error ! compare to (OS) %s, %s : 0x%x (I2C:01, USB:02, SPI:03, USB+I2C:04, Dummy:dd)",
                                ISiSProcedure::getDIStr(osDeviceInterface).c_str(),
                                ISiSProcedure::getCIStr(chipIndex).c_str(),
                                binInterfaceID );
                        msg.append(errorMsg);
                        throw CTException( msg );
                    }
                }
                else //if( binDeviceType == DT_819 )
                {
                    if( binInterfaceID == INTERFACE_ID_I2C &&
                            (osDeviceInterface == DI_I2C || osDeviceInterface == DI_HID_I2C) )
                    {
                        /* I2C */
                        continue;
                    }
                    else if( binInterfaceID == INTERFACE_ID_USB && osDeviceInterface == DI_HID_USB )
                    {
                        /* USB*/
                        continue;
                    }
                    else if( binInterfaceID == INTERFACE_ID_SPI_819 && osDeviceInterface == DI_SPI )
                    {
                        /* SPI*/
                        continue;
                    }
                    else if( binInterfaceID == INTERFACE_ID_USB_I2C_819 &&
                             (osDeviceInterface == DI_I2C || osDeviceInterface == DI_HID_I2C || osDeviceInterface == DI_HID_USB) )
                    {
                        /* USB+I2C*/
                        continue;
                    }
                    else
                    {
                        std::string msg = EXCEPTION_TITLE;
                        char errorMsg[1024] = "";
                        sprintf(errorMsg, "checkAllBinInterfaceID error ! compare to (OS) %s, %s : 0x%x (I2C:01, USB:02, SPI:03, USB+I2C:04, Dummy:dd)",
                                ISiSProcedure::getDIStr(osDeviceInterface).c_str(),
                                ISiSProcedure::getCIStr(chipIndex).c_str(),
                                binInterfaceID );
                        msg.append(errorMsg);
                        throw CTException( msg );
                    }
                }
            }
            else
            {
                /* Slave */
                if( binDeviceType == DT_817 && binInterfaceID == INTERFACE_ID_I2C )
                {
                    /* I2C */
                    continue;
                }
                else if( binDeviceType == DT_819 && (binInterfaceID == INTERFACE_ID_I2C || binInterfaceID == INTERFACE_ID_SPI_819) )
                {
                    /* I2C or SPI */
                    continue;
                }
                else
                {
                    if( binDeviceType == DT_817 )
                    {
                        std::string msg = EXCEPTION_TITLE;
                        char errorMsg[1024] = "";
                        sprintf(errorMsg, "checkAllBinInterfaceID error ! compare to (I2C), %s : 0x%x (I2C:01, USB:02, SPI:03, USB+I2C:04, Dummy:dd)",
                                SiSProcedure::getCIStr(chipIndex).c_str(),
                                binInterfaceID );
                        msg.append(errorMsg);
                        throw CTException( msg );
                    }
                    else //if( binDeviceType == DT_819 )
                    {
                        std::string msg = EXCEPTION_TITLE;
                        char errorMsg[1024] = "";
                        sprintf(errorMsg, "checkAllBinInterfaceID error ! compare to (I2C|SPI), %s : 0x%x (I2C:01, USB:02, SPI:03, USB+I2C:04, Dummy:dd)",
                                ISiSProcedure::getCIStr(chipIndex).c_str(),
                                binInterfaceID );
                        msg.append(errorMsg);
                        throw CTException( msg );
                    }
                }
            }
        }
    }

    if( m_updateFWParameter->getOSDeviceType() == DT_817 )
    {
        SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s checkAllBinInterfaceID : Master (%s), Slave (I2C): Yes", SYMBOL_CTL_FLOW,
                        ISiSProcedure::getDIStr(osDeviceInterface).c_str());
    }
    else if( m_updateFWParameter->getOSDeviceType() == DT_819 )
    {
        SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s checkAllBinInterfaceID : Master (%s), Slave (I2C/SPI): Yes", SYMBOL_CTL_FLOW,
                        ISiSProcedure::getDIStr(osDeviceInterface).c_str());
    }
}

void
UpdateFWCore_general::checkAllBinSelectiveID()
{
    if( m_updateFWParameter->getJumpCheckSelectiveID() || m_updateFWParameter->getJumpCheckAll() )
    {
        SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s checkAllBinSelectiveID : Jump", SYMBOL_CTL_FLOW);
        return;
    }

    /* compare to BIN itself and chipIndex */
    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s checkAllBinSelectiveID : ", SYMBOL_CTL_FLOW);

    for(std::map<ChipIndexKey, BinWrap*>::iterator it = m_updateFWParameter->getBinWrapMap().begin();
        it != m_updateFWParameter->getBinWrapMap().end();
        ++it)
    {
        int chipIndex = it->first;

        unsigned char binSelectiveID = getUpdateFWReference(RS_BIN, chipIndex)->getMultiChipSelectiveID();
        unsigned char binInterfaceID = getUpdateFWReference(RS_BIN, chipIndex)->getInterfaceID();

        if( chipIndex == CI_MASTER )
        {
            if( binInterfaceID == INTERFACE_ID_I2C && binSelectiveID == NON_MULTI_DEVICE_FLAG )
            {
                continue;
            }
            else if( binInterfaceID == INTERFACE_ID_USB &&
                     (binSelectiveID == MASTER_DEVICE_FLAG ||
                      binSelectiveID == BRIDGE_DEVICE_FLAG ||
                      binSelectiveID == NON_MULTI_DEVICE_FLAG) )
            {
                continue;
            }
            else if( binInterfaceID == INTERFACE_ID_USB_I2C_819 &&
                     (binSelectiveID == NON_MULTI_DEVICE_FLAG ||
                      binSelectiveID == MASTER_DEVICE_FLAG ||
                      binSelectiveID == BRIDGE_DEVICE_FLAG ||
                      binSelectiveID == NON_MULTI_DEVICE_FLAG) )
            {
                continue;
            }
            else
            {
                std::string msg = EXCEPTION_TITLE;
                char errorMsg[1024] = "";
                sprintf(errorMsg, "checkAllBinSelectiveID error ! %s : SelectiveID (0x%x), InterfaceID (0x%x)",
                        ISiSProcedure::getCIStr(chipIndex).c_str(),
                        binSelectiveID,
                        binInterfaceID );
                msg.append(errorMsg);
                throw CTException( msg );
            }
        }
        else
        {
            IAttributeReader* attributeReader = m_sisProcedure->generateAttributeReader();
            int chipAddress = attributeReader->mappingChipAddress(chipIndex);
            delete attributeReader;

            if( binSelectiveID == chipAddress )
            {
                continue;
            }
            else
            {
                std::string msg = EXCEPTION_TITLE;
                char errorMsg[1024] = "";
                sprintf(errorMsg, "checkAllBinSelectiveID error ! %s : SelectiveID (0x%x), InterfaceID (0x%x)",
                        ISiSProcedure::getCIStr(chipIndex).c_str(),
                        binSelectiveID,
                        binInterfaceID );
                msg.append(errorMsg);
                throw CTException( msg );
            }

        }
    }

    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s checkAllBinSelectiveID : all match : Yes", SYMBOL_CTL_FLOW);
}

void
UpdateFWCore_general::checkMasterBinProductID()
{
    // 817 do nothing
}

void
UpdateFWCore_general::checkAllBinProductID()
{
    // 817 do nothing
}

bool
UpdateFWCore_general::ifNeedMasterRecovery()
{
    bool masterBroken = getUpdateFWReference(RS_XRAM, CI_MASTER)->isBroken();

    if( masterBroken )
    {
        SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s ifNeedMasterRecovery : Yes", SYMBOL_CTL_FLOW);
        return true;
    }

    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s ifNeedMasterRecovery : No", SYMBOL_CTL_FLOW);
    return false;
}

bool
UpdateFWCore_general::ifNeedRestructureBootloader()
{
    for(int chipIndex = CI_MASTER; chipIndex < m_updateFWParameter->getBinWrapMap().size(); chipIndex++)
    {
        if( ifNeedRestructureBootloader(chipIndex) )
        {
            SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s ifNeedRestructureBootloader : Yes", SYMBOL_CTL_FLOW);
            return true;
        }
    }

    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s ifNeedRestructureBootloader : No", SYMBOL_CTL_FLOW);
    return false;
}

bool
UpdateFWCore_general::ifNeedRestructureBootloader(int chipIndex)
{
    if( chipIndex == CI_MASTER && getUpdateFWReference(RS_XRAM, CI_MASTER)->getIsRecoveryDone() )
    {
        return false; // Master had done MasterRecovery
    }

    FWSizeType binFWSizeType = getUpdateFWReference(RS_BIN, chipIndex)->getFWSizeType();
    FWSizeType xramFWSizeType = getUpdateFWReference(RS_XRAM, chipIndex)->getFWSizeType();
    FWSizeType romFWSizeType = getUpdateFWReference(RS_ROM, chipIndex)->getFWSizeType();

    if( ((binFWSizeType == FST_817_R64K || binFWSizeType == FST_817_R64K_M48K) && xramFWSizeType == FST_817_R128K) ||
            (binFWSizeType == FST_817_R128K && (xramFWSizeType == FST_817_R64K || xramFWSizeType == FST_817_R64K_M48K)) )
    {
        /* check 128k to 64k */
        if( binFWSizeType == FST_817_R128K && romFWSizeType == FST_R64K )
        {
            std::string msg = EXCEPTION_TITLE;
            char errorMsg[1024] = "";
            sprintf(errorMsg, "ifNeedRestructureBootloader error ! %s : BIN (FST_817_R128K) can't burn to ROM (FST_R64K)",
                    ISiSProcedure::getCIStr(chipIndex).c_str() );
            msg.append(errorMsg);
            throw CTException( msg );
        }
        else if( !m_updateFWParameter->getUpdateBootloader() && !m_updateFWParameter->getUpdateBootloaderAuto())
        {
            std::string msg = EXCEPTION_TITLE;
            char errorMsg[1024] = "";
            sprintf(errorMsg, "%s : Need Restructure Bootloader ! Please update bootloader with -b /-ba parameter",
                    ISiSProcedure::getCIStr(chipIndex).c_str());
            msg.append(errorMsg);
            throw CTException( msg );
        }

        return true;
    }

    return false;
}

bool
UpdateFWCore_general::ifNeedUpdateBootloader(int chipIndex)
{
    UpdateFWReference* xramRef = getUpdateFWReference(RS_XRAM, chipIndex);
    UpdateFWReference* binRef = getUpdateFWReference(RS_BIN, chipIndex);
    bool needUpdateBootloader = false;

    if( m_updateFWParameter->getUpdateBootloader() )
    {
        SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s : force to update bootloader with -b parameter", ISiSProcedure::getCIStr(chipIndex).c_str());
        needUpdateBootloader = true;
    }
    else if( !m_sisProcedure->compareData(xramRef->getBootloaderID()->getData(), xramRef->getBootloaderID()->getSize(),
                                        binRef->getBootloaderID()->getData(), binRef->getBootloaderID()->getSize()) )
    {
        if( !m_updateFWParameter->getUpdateBootloaderAuto() )
        {
            std::string msg = EXCEPTION_TITLE;
            char errorMsg[1024] = "";
            sprintf(errorMsg, "%s : different BootloaderID ! Please update bootloader with -b /-ba parameter",
                    ISiSProcedure::getCIStr(chipIndex).c_str());
            msg.append(errorMsg);
            throw CTException( msg );
        }

        SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s : different BootloaderID. Automatically update bootloader with -ba parameter",
                        ISiSProcedure::getCIStr(chipIndex).c_str());
        needUpdateBootloader = true;
    }
    else if( !m_sisProcedure->compareData(xramRef->getBootCodeCRC()->getData(), xramRef->getBootCodeCRC()->getSize(),
                                        binRef->getBootCodeCRC()->getData(), binRef->getBootCodeCRC()->getSize()) )
    {
        if( !m_updateFWParameter->getUpdateBootloaderAuto() )
        {
            std::string msg = EXCEPTION_TITLE;
            char errorMsg[1024] = "";
            sprintf(errorMsg, "%s : different BootCodeCRC ! Please update bootloader with -b /-ba parameter",
                    ISiSProcedure::getCIStr(chipIndex).c_str());
            msg.append(errorMsg);
            throw CTException( msg );
        }

        SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s : different BootCodeCRC. Automatically update bootloader with -ba parameter",
                        ISiSProcedure::getCIStr(chipIndex).c_str());
        needUpdateBootloader = true;
    }
    else
    {
        SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s : bootloader are the same", ISiSProcedure::getCIStr(chipIndex).c_str());
    }

    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s ifNeedUpdateBootloader, %s : %s", SYMBOL_CTL_FLOW,
                    ISiSProcedure::getCIStr(chipIndex).c_str(),
                    needUpdateBootloader == true ? "Yes" : "No" );

    return needUpdateBootloader;
}

void
UpdateFWCore_general::confirmUpdate()
{
    if( !m_updateFWParameter->getConfirmUpdate() )
    {
        SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "Not need to confirm update");
        return;
    }

    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "confirm update [Y/N(y/n)] :");

    char ch = getchar();
    if( ch != 'Y' && ch != 'y' )
    {
        std::string msg = EXCEPTION_TITLE;
        char errorMsg[1024] = "";
        sprintf(errorMsg, "user did not confirm to update ! stop !");
        msg.append(errorMsg);
        throw CTException( msg );
    }
}

void
UpdateFWCore_general::clearBootFlag(int chipIndex)
{
    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s clearBootFlag : %s ", SYMBOL_DO_UPDATE, ISiSProcedure::getCIStr(chipIndex).c_str());

    UpdateFWReference* binMasterRef = getUpdateFWReference(RS_BIN, chipIndex);
//    UpdateFWReference* xramMasterRef = getUpdateFWReference(RS_XRAM, chipIndex);

    /* switch SiSProcedure by Bin FWSizeType */
    m_sisProcedure->switchSiSProcedure( binMasterRef->getFWSizeType() );

    RomBurnFlag romBurnFlag;
    romBurnFlag.setFlag(RomBurnFlag::UR_UPDATE, RomBurnFlag::RF_CLEAR_BOOT_FLAG, true);
    romBurnFlag.setFlag(RomBurnFlag::UR_RESERVE, RomBurnFlag::RF_RODATA_CODE, true);
    m_sisProcedure->burn(romBurnFlag, chipIndex);
}

void
UpdateFWCore_general::masterRecovery()
{
    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s masterRecovery : start", SYMBOL_DO_UPDATE);

    doUpdateFW(CI_MASTER);

    /* update changed state */
    getUpdateFWReference(RS_XRAM, CI_MASTER)->setIsRecoveryDone(true);
    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s masterRecovery : done", SYMBOL_DO_UPDATE);
}

void
UpdateFWCore_general::restructureBootloader()
{
    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s restructureBootloader : start", SYMBOL_DO_UPDATE);

    for(int chipIndex = CI_SLAVE_0; chipIndex < m_updateFWParameter->getBinWrapMap().size(); chipIndex++)
    {
        restructureBootloader(chipIndex);
    }

    if( !getUpdateFWReference(RS_XRAM, CI_MASTER)->getIsRecoveryDone() )
    {
        restructureBootloader(CI_MASTER);
        resetDevice();
        masterRecovery();
    }

    resetDevice();

    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s restructureBootloader : done", SYMBOL_DO_UPDATE);
}

void
UpdateFWCore_general::restructureBootloader(int chipIndex)
{
    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s restructureBootloader : %s ", SYMBOL_DO_UPDATE, ISiSProcedure::getCIStr(chipIndex).c_str());

    jumpBootloader(chipIndex);

    clearBootFlag(chipIndex);

    UpdateFWReference* binMasterRef = getUpdateFWReference(RS_BIN, chipIndex);
    UpdateFWReference* xramMasterRef = getUpdateFWReference(RS_XRAM, chipIndex);

    /* switch SiSProcedure by Bin FWSizeType */
    m_sisProcedure->switchSiSProcedure( binMasterRef->getFWSizeType() );

    RomBurnFlag romBurnFlag;
    romBurnFlag.setFlag(RomBurnFlag::UR_UPDATE, RomBurnFlag::RF_BOOTLOADER, true);
    m_sisProcedure->burn(romBurnFlag, chipIndex);

    /* update changed state */
    xramMasterRef->setIsRestructureBootloaderDone(true);
}

void
UpdateFWCore_general::doUpdateFW()
{
    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s doUpdateFW : start", SYMBOL_DO_UPDATE);
    bool hasDo = false;

    for(int chipIndex = CI_SLAVE_0; chipIndex < m_updateFWParameter->getBinWrapMap().size(); chipIndex++)
    {
        doUpdateFW(chipIndex);
        hasDo = true;
    }

    if( !getUpdateFWReference(RS_XRAM, CI_MASTER)->getIsRecoveryDone() )
    {
        masterRecovery();
        hasDo = true;
    }

    if( hasDo )
    {
        if( m_updateFWParameter->getDisableSoftReset() )
        {
            SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "! DisableSoftReset");
        }
        else
        {
            resetDevice(); // if hasDo
        }
    }

    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s doUpdateFW : done", SYMBOL_DO_UPDATE);
}

void
UpdateFWCore_general::doUpdateFW(int chipIndex)
{
    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s doUpdateFW : %s ", SYMBOL_DO_UPDATE, ISiSProcedure::getCIStr(chipIndex).c_str());

    UpdateFWReference* binMasterRef = getUpdateFWReference(RS_BIN, chipIndex);
    UpdateFWReference* xramMasterRef = getUpdateFWReference(RS_XRAM, chipIndex);

    /* switch SiSProcedure by Bin FWSizeType */
    m_sisProcedure->switchSiSProcedure( binMasterRef->getFWSizeType() );

    RomBurnFlag romBurnFlag;
    if( !xramMasterRef->getIsRestructureBootloaderDone() )
    {
        if( ifNeedUpdateBootloader(chipIndex) )
        {
            romBurnFlag.setFlag(RomBurnFlag::UR_UPDATE, RomBurnFlag::RF_BOOTLOADER, true);
        }
    }
	
    jumpBootloader(chipIndex);

    clearBootFlag(chipIndex); /* after check ifNeedUpdateBootloader */
    romBurnFlag.setFlag(RomBurnFlag::UR_UPDATE, RomBurnFlag::RF_MAIN_CODE, true);
    romBurnFlag.setFlag(RomBurnFlag::UR_UPDATE, RomBurnFlag::RF_RODATA_CODE, true);
    romBurnFlag.setFlag(RomBurnFlag::UR_UPDATE, RomBurnFlag::RF_FW_INFO, true);
    romBurnFlag.setFlag(RomBurnFlag::UR_UPDATE, RomBurnFlag::RF_REG_MEM, true);
    romBurnFlag.setFlag(RomBurnFlag::UR_UPDATE, RomBurnFlag::RF_DEF_MEM, true);
    romBurnFlag.setFlag(RomBurnFlag::UR_UPDATE, RomBurnFlag::RF_THQA, true);
    romBurnFlag.setFlag(RomBurnFlag::UR_UPDATE, RomBurnFlag::RF_USB_DEV_DESC, true);
    romBurnFlag.setFlag(RomBurnFlag::UR_UPDATE, RomBurnFlag::RF_USB_RPT_DESC, true);
    m_sisProcedure->burn(romBurnFlag, chipIndex);
}
