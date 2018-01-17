#include "updatefwcore.h"

#include "sislog.h"
#include "ctexception/ctexception.h"
#include "binloader/binloader.h"

using namespace CT;
using namespace SiS;
using namespace SiS::Procedure;

#define EXCEPTION_TITLE "UpdateFWCore Exception : "
#define TAG "UpdateFWCore"

UpdateFWCore::UpdateFWCore(UpdateFWParameter* updateFWParameter) :
    IUpdateFWCore(updateFWParameter),
    m_iUpdateFWCore(0),
    m_updateFWCore_general(0),
    m_updateFWCore_819(0),
    m_updateFWCore_generalSingle(0),
    m_updateFWCore_819Single(0)
{
    this->m_sisProcedure = new SiSProcedure();
    this->m_iUpdateFWCore = getUpdateFWCore_general();

    /* force to set OSDeviceInterface */
    if( updateFWParameter->getOSDeviceInterfaceUserDefined() && updateFWParameter->getOSDeviceInterface() == DI_HID_USB )
    {
        m_sisProcedure->getSiSDeviceIO()->setUserConnectType(SiSDeviceAttribute::CON_819_USB_HID);
    }
    else if( updateFWParameter->getOSDeviceInterfaceUserDefined() && updateFWParameter->getOSDeviceInterface() == DI_HID_I2C )
    {
        m_sisProcedure->getSiSDeviceIO()->setUserConnectType(SiSDeviceAttribute::CON_819_HID_OVER_I2C);
    }
}

UpdateFWCore::~UpdateFWCore()
{
    if( m_updateFWCore_general != 0 )
    {
        delete m_updateFWCore_general;
        m_updateFWCore_general = 0;
    }

    if( m_updateFWCore_819 != 0 )
    {
        delete m_updateFWCore_819;
        m_updateFWCore_819 = 0;
    }

    if( m_updateFWCore_generalSingle != 0 )
    {
        delete m_updateFWCore_generalSingle;
        m_updateFWCore_general = 0;
    }

    if( m_updateFWCore_819Single != 0 )
    {
        delete m_updateFWCore_819Single;
        m_updateFWCore_819 = 0;
    }

    if( m_sisProcedure != 0 )
    {
        delete m_sisProcedure;
        m_sisProcedure = 0;
    }
}

UpdateFWCore_general*
UpdateFWCore::getUpdateFWCore_general()
{
    if( this->m_updateFWCore_general == 0 )
    {
        this->m_updateFWCore_general = new UpdateFWCore_general(this->m_updateFWParameter);
        this->m_updateFWCore_general->setSiSProcedure(m_sisProcedure);
    }

    return this->m_updateFWCore_general;
}

UpdateFWCore_819*
UpdateFWCore::getUpdateFWCore_819()
{
    if( this->m_updateFWCore_819 == 0 )
    {
        this->m_updateFWCore_819 = new UpdateFWCore_819(this->m_updateFWParameter);
        this->m_updateFWCore_819->setSiSProcedure(m_sisProcedure);
    }

    return this->m_updateFWCore_819;
}

UpdateFWCore_generalSingle*
UpdateFWCore::getUpdateFWCore_generalSingle()
{
    if( this->m_updateFWCore_generalSingle == 0 )
    {
        this->m_updateFWCore_generalSingle = new UpdateFWCore_generalSingle(this->m_updateFWParameter);
        this->m_updateFWCore_generalSingle->setSiSProcedure(m_sisProcedure);
    }

    return this->m_updateFWCore_generalSingle;
}

UpdateFWCore_819Single*
UpdateFWCore::getUpdateFWCore_819Single()
{
    if( this->m_updateFWCore_819Single == 0 )
    {
        this->m_updateFWCore_819Single = new UpdateFWCore_819Single(this->m_updateFWParameter);
        this->m_updateFWCore_819Single->setSiSProcedure(m_sisProcedure);
    }

    return this->m_updateFWCore_819Single;
}

void
UpdateFWCore::switchFlowVersion(DeviceType deviceType)
{
    switch (deviceType) {
    case DT_817:
        if( m_updateFWParameter->getSpecificChipUserDefined() == true )
        {
            this->m_iUpdateFWCore = getUpdateFWCore_generalSingle();
            SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "switchFlowVersion : generalSingle");
        }
        else
        {
            this->m_iUpdateFWCore = getUpdateFWCore_general();
            SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "switchFlowVersion : general");
        }
        break;
    case DT_819:
        if( m_updateFWParameter->getSpecificChipUserDefined() == true )
        {
            this->m_iUpdateFWCore = getUpdateFWCore_819Single();
            SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "switchFlowVersion : 819Single");
            break;
        }
        else
        {
            this->m_iUpdateFWCore = getUpdateFWCore_819();
            SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "switchFlowVersion : 819");
            break;
        }
    case DT_INVALID:
    default:
        std::string msg = EXCEPTION_TITLE;
        char errorMsg[1024] = "";
        sprintf(errorMsg, "switchFlowVersion FAIL : invalid (unknow) deviceType (%d)", (int) deviceType);
        msg.append(errorMsg);
        throw CTException( msg );
    }
}

void
UpdateFWCore::setSiSProcedure(SiSProcedure* sisProcedure)
{
    std::string msg = EXCEPTION_TITLE;
    char errorMsg[1024] = "";
    sprintf(errorMsg, "UpdateFWCore::setSiSProcedure : only set by itself !");
    msg.append(errorMsg);
    throw CTException( msg );
}

void
UpdateFWCore::init()
{
    /* set atttibute */
    m_sisProcedure->setBlockRetry( this->m_updateFWParameter->getBlockRetry() );
    m_sisProcedure->setAllRetry( this->m_updateFWParameter->getAllRetry() );
    m_sisProcedure->setRamSize( this->m_updateFWParameter->getRamSize() );

    CTBaseCore::init();

    if( this->m_updateFWParameter->getSpecificChipUserDefined() == true )
    {
        if( this->m_updateFWParameter->getBinWrapMap().size() > 1 )
        {
            std::string msg = EXCEPTION_TITLE;
            char errorMsg[1024] = "";
            sprintf(errorMsg, "UpdateFWCore::init : specific chip only allow one bin file !");
            msg.append(errorMsg);
            throw CTException( msg );
        }
        else if( this->m_updateFWParameter->getBinWrapMap().size() < 1 )
        {
            std::string msg = EXCEPTION_TITLE;
            char errorMsg[1024] = "";
            sprintf(errorMsg, "UpdateFWCore::init : not set bin file !");
            msg.append(errorMsg);
            throw CTException( msg );
        }

        /* re-assign bin */
        std::string binFileName = "";
        for(std::map<ChipIndexKey, BinWrap*>::iterator it = this->m_updateFWParameter->getBinWrapMap().begin();
            it != this->m_updateFWParameter->getBinWrapMap().end();
            ++it)
        {
            BinWrap* binWrap = it->second;
            binFileName = binWrap->getName();
            break;
        }
        this->m_updateFWParameter->clearBinWrapMap();
        this->m_updateFWParameter->setBinWrapMap( this->m_updateFWParameter->getSpecificChip(),
                                                  new BinWrap(binFileName, this->m_updateFWParameter->getSpecificChip()) );
    }

    /* auto switch FlowVersion by OS DeviceType */
    switchFlowVersion( this->m_updateFWParameter->getOSDeviceType() );
}

CTExitCode
UpdateFWCore::exec()
{
    return this->m_iUpdateFWCore->exec();
}

void
UpdateFWCore::destroy()
{
    if( m_updateFWParameter->getDisableSoftReset() )
    {
        SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "! DisableSoftReset, can't do enableCtlReportToOs");
    }
    else
    {
        enableCtlReportToOs();
    }
}



