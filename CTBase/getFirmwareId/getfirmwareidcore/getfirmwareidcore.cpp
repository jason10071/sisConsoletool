#include "getfirmwareidcore.h"

#include "sislog.h"
#include "ctexception/ctexception.h"

using namespace CT;
using namespace SiS;
using namespace SiS::Procedure;

#define EXCEPTION_TITLE "GetFirmwareIdCore Exception : "
#define TAG "GetFirmwareIdCore"

GetFirmwareIdCore::GetFirmwareIdCore(GetFirmwareIdParameter* getFirmwareIdParameter) :
    IGetFirmwareIdCore(getFirmwareIdParameter),
    m_iGetFirmwareIdCore(0),
    m_getFirmwareIdCore_general(0),
    m_getFirmwareIdCore_generalSingle(0)
{
    this->m_sisProcedure = new SiSProcedure();
    m_iGetFirmwareIdCore = getGetFirmwareIdCore_general();

    /* force to set OSDeviceInterface */
    if( getFirmwareIdParameter->getOSDeviceInterfaceUserDefined() && getFirmwareIdParameter->getOSDeviceInterface() == DI_HID_USB )
    {
        m_sisProcedure->getSiSDeviceIO()->setUserConnectType(SiSDeviceAttribute::CON_819_USB_HID);
    }
    else if( getFirmwareIdParameter->getOSDeviceInterfaceUserDefined() && getFirmwareIdParameter->getOSDeviceInterface() == DI_HID_I2C )
    {
        m_sisProcedure->getSiSDeviceIO()->setUserConnectType(SiSDeviceAttribute::CON_819_HID_OVER_I2C);
    }
}

GetFirmwareIdCore::~GetFirmwareIdCore()
{
    if( m_getFirmwareIdCore_general != 0 )
    {
        delete m_getFirmwareIdCore_general;
        m_getFirmwareIdCore_general = 0;
    }

    if( m_getFirmwareIdCore_generalSingle != 0 )
    {
        delete m_getFirmwareIdCore_generalSingle;
        m_getFirmwareIdCore_generalSingle = 0;
    }

    if( m_sisProcedure != 0 )
    {
        delete m_sisProcedure;
        m_sisProcedure = 0;
    }
}

GetFirmwareIdCore_general*
GetFirmwareIdCore::getGetFirmwareIdCore_general()
{
    if( this->m_getFirmwareIdCore_general == 0 )
    {
        this->m_getFirmwareIdCore_general = new GetFirmwareIdCore_general(this->m_getFirmwareIdParameter);
        this->m_getFirmwareIdCore_general->setSiSProcedure(m_sisProcedure);
    }

    return this->m_getFirmwareIdCore_general;
}

GetFirmwareIdCore_generalSingle*
GetFirmwareIdCore::getGetFirmwareIdCore_generalSingle()
{
    if( this->m_getFirmwareIdCore_generalSingle == 0 )
    {
        this->m_getFirmwareIdCore_generalSingle = new GetFirmwareIdCore_generalSingle(this->m_getFirmwareIdParameter);
        this->m_getFirmwareIdCore_generalSingle->setSiSProcedure(m_sisProcedure);
    }

    return this->m_getFirmwareIdCore_generalSingle;
}

void
GetFirmwareIdCore::switchFlowVersion(DeviceType deviceType)
{
    switch (deviceType) {
    case DT_817:
    case DT_819:
        if( this->m_getFirmwareIdParameter->getSpecificChipUserDefined() )
        {
            this->m_iGetFirmwareIdCore = getGetFirmwareIdCore_generalSingle();
            SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "switchFlowVersion : generalSingle");
        }
        else
        {
            this->m_iGetFirmwareIdCore = getGetFirmwareIdCore_general();
            SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "switchFlowVersion : general");
        }
        break;
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
GetFirmwareIdCore::setSiSProcedure(SiSProcedure* sisProcedure)
{
    std::string msg = EXCEPTION_TITLE;
    char errorMsg[1024] = "";
    sprintf(errorMsg, "GetFirmwareIdCore::setSiSProcedure : only set by itself !");
    msg.append(errorMsg);
    throw CTException( msg );
}

void
GetFirmwareIdCore::init()
{
    CTBaseCore::init();

    if( this->m_getFirmwareIdParameter->getSpecificChipUserDefined() == true )
    {
        /* m_firmwareCheckIdMap */
        if( this->m_getFirmwareIdParameter->getFirmwareCheckIdMap().size() > 1 )
        {
            std::string msg = EXCEPTION_TITLE;
            char errorMsg[1024] = "";
            sprintf(errorMsg, "GetFirmwareIdCore::init : specific chip only allow one input !");
            msg.append(errorMsg);
            throw CTException( msg );
        }
        else if( this->m_getFirmwareIdParameter->getFirmwareCheckIdMap().size() == 1 )
        {
            /* re-assign input */
            std::string input = "";
            for(std::map<ChipIndexKey, std::string>::iterator it = this->m_getFirmwareIdParameter->getFirmwareCheckIdMap().begin();
                it != this->m_getFirmwareIdParameter->getFirmwareCheckIdMap().end();
                ++it)
            {
                input = it->second;
                break;
            }
            this->m_getFirmwareIdParameter->clearFirmwareCheckIdMap();
            this->m_getFirmwareIdParameter->setFirmwareCheckIdMap( this->m_getFirmwareIdParameter->getSpecificChip(), input );
        }

        /* m_binWrapMap */
        if( this->m_getFirmwareIdParameter->getBinWrapMap().size() > 1 )
        {
            std::string msg = EXCEPTION_TITLE;
            char errorMsg[1024] = "";
            sprintf(errorMsg, "GetFirmwareIdCore::init : specific chip only allow one bin file !");
            msg.append(errorMsg);
            throw CTException( msg );
        }
        else if( this->m_getFirmwareIdParameter->getBinWrapMap().size() == 1 )
        {
            /* re-assign bin */
            std::string binFileName = "";
            for(std::map<ChipIndexKey, BinWrap*>::iterator it = this->m_getFirmwareIdParameter->getBinWrapMap().begin();
                it != this->m_getFirmwareIdParameter->getBinWrapMap().end();
                ++it)
            {
                BinWrap* binWrap = it->second;
                binFileName = binWrap->getName();
                break;
            }
            this->m_getFirmwareIdParameter->clearBinWrapMap();
            this->m_getFirmwareIdParameter->setBinWrapMap( this->m_getFirmwareIdParameter->getSpecificChip(),
                                                      new BinWrap(binFileName, this->m_getFirmwareIdParameter->getSpecificChip()) );
        }
    }

    /* auto switch FlowVersion by OS DeviceType */
    switchFlowVersion( this->m_getFirmwareIdParameter->getOSDeviceType() );
}

CTExitCode
GetFirmwareIdCore::exec()
{
    return this->m_iGetFirmwareIdCore->exec();
}


