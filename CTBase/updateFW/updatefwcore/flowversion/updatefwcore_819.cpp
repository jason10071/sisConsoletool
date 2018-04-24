#include "updatefwcore_819.h"

#include "sislog.h"
#include "ctexception/ctexception.h"
#include "spexception/spexception.h"
#include "binloader/binloader.h"

using namespace CT;
using namespace SiS;
using namespace SiS::Procedure;

#define EXCEPTION_TITLE "UpdateFWCore_819 Exception : "
#define TAG "UpdateFWCore_819"

UpdateFWCore_819::UpdateFWCore_819(UpdateFWParameter* updateFWParameter) :
    UpdateFWCore_general(updateFWParameter)
{

}

UpdateFWCore_819::~UpdateFWCore_819()
{

}

void
UpdateFWCore_819::showMasterInfo()
{
    UpdateFWCore_general::showMasterInfo();

    showProductID(CI_MASTER); /* 16. ProductID */
    showUpdateMark(CI_MASTER); /* 17. UpdateMark */
    showLastUpdateMark(CI_MASTER); /* 18. LastUpdateMark */
    showPriorLastUpdateMark(CI_MASTER); /* 19. PriorLastUpdateMark */
}

void
UpdateFWCore_819::prepareBinMasterRef()
{
    UpdateFWCore_general::prepareBinMasterRef();

    fetchProductID(RS_BIN, CI_MASTER); /* 16. ProductID */
    fetchUpdateMark(RS_BIN, CI_MASTER); /* 17. UpdateMark */
    fetchLastUpdateMark(RS_BIN, CI_MASTER); /* 18. LastUpdateMark */
    fetchPriorLastUpdateMark(RS_BIN, CI_MASTER); /* 19. PriorLastUpdateMark */
}

void
UpdateFWCore_819::prepareXramMasterRef()
{
    UpdateFWCore_general::prepareXramMasterRef();

    /* it needs to be rocorded no matter the fw was broken or not */
    fetchProductID(RS_XRAM, CI_MASTER); /* 16. ProductID */
    fetchUpdateMark(RS_XRAM, CI_MASTER); /* 17. UpdateMark */
    fetchLastUpdateMark(RS_XRAM, CI_MASTER); /* 18. LastUpdateMark */
    fetchPriorLastUpdateMark(RS_XRAM, CI_MASTER); /* 19. PriorLastUpdateMark */
}

void
UpdateFWCore_819::showSlaveInfo(int chipIndex)
{
    UpdateFWCore_general::showSlaveInfo(chipIndex);

    showProductID(chipIndex); /* 16. ProductID */
    showUpdateMark(chipIndex); /* 17. UpdateMark */
    showLastUpdateMark(chipIndex); /* 18. LastUpdateMark */
    showPriorLastUpdateMark(chipIndex); /* 19. PriorLastUpdateMark */
}

void
UpdateFWCore_819::prepareBinSlaveRef(int chipIndex)
{
    UpdateFWCore_general::prepareBinSlaveRef(chipIndex);

    fetchProductID(RS_BIN, chipIndex); /* 16. ProductID */
    fetchUpdateMark(RS_BIN, chipIndex); /* 17. UpdateMark */
    fetchLastUpdateMark(RS_BIN, chipIndex); /* 18. LastUpdateMark */
    fetchPriorLastUpdateMark(RS_BIN, chipIndex); /* 19. PriorLastUpdateMark */
}

void
UpdateFWCore_819::prepareXramSlaveRef(int chipIndex)
{
    UpdateFWCore_general::prepareXramSlaveRef(chipIndex);

    /* it needs to be rocorded no matter the fw was broken or not */
    fetchProductID(RS_XRAM, chipIndex); /* 16. ProductID */
    fetchUpdateMark(RS_XRAM, chipIndex); /* 17. UpdateMark */
    fetchLastUpdateMark(RS_XRAM, chipIndex); /* 18. LastUpdateMark */
    fetchPriorLastUpdateMark(RS_XRAM, chipIndex); /* 19. PriorLastUpdateMark */
}

void
UpdateFWCore_819::modifyUpdateStamp(int chipIndex)
{
    // todo
    UpdateFWReference* xramRef = getUpdateFWReference(RS_XRAM, chipIndex);
    UpdateFWReference* binRef = getUpdateFWReference(RS_BIN, chipIndex);

    /* switch SiSProcedure by Bin FWSizeType */
    m_sisProcedure->switchSiSProcedure( binRef->getFWSizeType() );

    /* write update mark */
    const short updateMarkLength = 2;
    unsigned char *updateMark = new unsigned char[updateMarkLength-1];
    updateMark[0] = 0x65;
    updateMark[1] = 0x96;
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
     SerialData* now = ISiSProcedure::getTimestamp();
     m_sisProcedure->writeLastTime( RS_BIN, chipIndex, now );
     m_sisProcedure->writeLastUpdateMark( RS_BIN, chipIndex, xramRef->getUpdateMark() );
     m_sisProcedure->writeLastID( RS_BIN, chipIndex, xramRef->getFirmwareID() );
     delete now;

    /* write prior last infomation */
    m_sisProcedure->writePriorLastTime( RS_BIN, chipIndex, xramRef->getLastTime() );
    m_sisProcedure->writePriorLastUpdateMark( RS_BIN, chipIndex, xramRef->getLastUpdateMark() );
    m_sisProcedure->writePriorLastID( RS_BIN, chipIndex, xramRef->getLastID() );
}

void
UpdateFWCore_819::checkMasterBinProductID()
{
    SIS_LOG_W(SiSLog::getOwnerSiS(), TAG, "%s checkMasterBinProductID : disable this function", SYMBOL_CTL_FLOW);
    return;
	
    if( m_updateFWParameter->getJumpCheckProductID() || m_updateFWParameter->getJumpCheckAll() )
    {
        SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s checkMasterBinProductID : Jump", SYMBOL_CTL_FLOW);
        return;
    }

    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s checkMasterBinProductID : ", SYMBOL_CTL_FLOW);

//    for(std::map<ChipIndexKey, BinWrap*>::iterator it = m_updateFWParameter->getBinWrapMap().begin();
//        it != m_updateFWParameter->getBinWrapMap().end();
//        ++it)
//    {
        int chipIndex = CI_MASTER;

        std::string binProductID = getUpdateFWReference(RS_BIN, chipIndex)->getProductID()->getSerial();
        std::string _124kProductID = getUpdateFWReference(RS_XRAM, chipIndex)->getProductID()->getSerial();

        if( binProductID.compare(_124kProductID) == 0 )
        {
            ; //continue;
        }
        else
        {
            std::string msg = EXCEPTION_TITLE;
            char errorMsg[1024] = "";
            sprintf(errorMsg, "checkMasterBinProductID error ! %s : binProductID (%s), _124kProductID (%s)",
                    ISiSProcedure::getCIStr(chipIndex).c_str(),
                    binProductID.c_str(),
                    _124kProductID.c_str() );
            msg.append(errorMsg);
            throw CTException( msg );
        }
//    }

    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s checkMasterBinProductID : all match : Yes", SYMBOL_CTL_FLOW);
}

void
UpdateFWCore_819::checkAllBinProductID()
{
    SIS_LOG_W(SiSLog::getOwnerSiS(), TAG, "%s checkAllBinProductID : disable this function", SYMBOL_CTL_FLOW);
    return;

    if( m_updateFWParameter->getJumpCheckProductID() || m_updateFWParameter->getJumpCheckAll() )
    {
        SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s checkAllBinProductID : Jump", SYMBOL_CTL_FLOW);
        return;
    }

    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s checkAllBinProductID : ", SYMBOL_CTL_FLOW);

    for(std::map<ChipIndexKey, BinWrap*>::iterator it = m_updateFWParameter->getBinWrapMap().begin();
        it != m_updateFWParameter->getBinWrapMap().end();
        ++it)
    {
        int chipIndex = it->first;

        std::string binProductID = getUpdateFWReference(RS_BIN, chipIndex)->getProductID()->getSerial();
        std::string _124kProductID = getUpdateFWReference(RS_XRAM, chipIndex)->getProductID()->getSerial();

        if( binProductID.compare(_124kProductID) == 0 )
        {
            continue;
        }
        else
        {
            std::string msg = EXCEPTION_TITLE;
            char errorMsg[1024] = "";
            sprintf(errorMsg, "checkAllBinProductID error ! %s : binProductID (%s), _124kProductID (%s)",
                    ISiSProcedure::getCIStr(chipIndex).c_str(),
                    binProductID.c_str(),
                    _124kProductID.c_str() );
            msg.append(errorMsg);
            throw CTException( msg );
        }
    }

    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s checkAllBinProductID : all match : Yes", SYMBOL_CTL_FLOW);
}

bool
UpdateFWCore_819::ifNeedRestructureBootloader()
{
    // not implement
    return false;
}

bool
UpdateFWCore_819::ifNeedRestructureBootloader(int chipIndex)
{
    // not implement
    return false;
}

void
UpdateFWCore_819::restructureBootloader()
{
    // not implement
}

void
UpdateFWCore_819::restructureBootloader(int chipIndex)
{
    // not implement
}
