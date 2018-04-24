#include "getfirmwareinfocore.h"

#include "sislog.h"
#include "ctexception/ctexception.h"

using namespace CT;
using namespace SiS;
using namespace SiS::Procedure;

#define EXCEPTION_TITLE "GetFirmwareInfoCore Exception : "
#define TAG "GetFirmwareInfoCore"

GetFirmwareInfoCore::GetFirmwareInfoCore(GetFirmwareInfoParameter* getFirmwareInfoParameter) :
    IGetFirmwareInfoCore(getFirmwareInfoParameter),
    m_iGetFirmwareInfoCore(0),
    m_getFirmwareInfoCore_general(0),
    m_getFirmwareInfoCore_generalSingle(0),
    m_getFirmwareInfoCore_autotool(0)
{
    this->m_sisProcedure = new SiSProcedure();
    m_iGetFirmwareInfoCore = getGetFirmwareInfoCore_general();
}

GetFirmwareInfoCore::~GetFirmwareInfoCore()
{
    if( m_getFirmwareInfoCore_general != 0 )
    {
        delete m_getFirmwareInfoCore_general;
        m_getFirmwareInfoCore_general = 0;
    }

    if( m_getFirmwareInfoCore_generalSingle != 0 )
    {
        delete m_getFirmwareInfoCore_generalSingle;
        m_getFirmwareInfoCore_generalSingle = 0;
    }

    if( m_getFirmwareInfoCore_autotool != 0 )
    {
        delete m_getFirmwareInfoCore_autotool;
        m_getFirmwareInfoCore_autotool = 0;
    }

    if( m_sisProcedure != 0 )
    {
        delete m_sisProcedure;
        m_sisProcedure = 0;
    }
}

GetFirmwareInfoCore_general*
GetFirmwareInfoCore::getGetFirmwareInfoCore_general()
{
    if( this->m_getFirmwareInfoCore_general == 0 )
    {
        this->m_getFirmwareInfoCore_general = new GetFirmwareInfoCore_general(this->m_getFirmwareInfoParameter);
        this->m_getFirmwareInfoCore_general->setSiSProcedure(m_sisProcedure);
    }

    return this->m_getFirmwareInfoCore_general;
}

GetFirmwareInfoCore_generalSingle*
GetFirmwareInfoCore::getGetFirmwareInfoCore_generalSingle()
{
    if( this->m_getFirmwareInfoCore_generalSingle == 0 )
    {
        this->m_getFirmwareInfoCore_generalSingle = new GetFirmwareInfoCore_generalSingle(this->m_getFirmwareInfoParameter);
        this->m_getFirmwareInfoCore_generalSingle->setSiSProcedure(m_sisProcedure);
    }

    return this->m_getFirmwareInfoCore_generalSingle;
}

GetFirmwareInfoCore_autotool*
GetFirmwareInfoCore::getGetFirmwareInfoCore_autotool()
{
    if( this->m_getFirmwareInfoCore_autotool == 0 )
    {
        this->m_getFirmwareInfoCore_autotool = new GetFirmwareInfoCore_autotool(this->m_getFirmwareInfoParameter);
        this->m_getFirmwareInfoCore_autotool->setSiSProcedure(m_sisProcedure);
    }

    return this->m_getFirmwareInfoCore_autotool;
}

void
GetFirmwareInfoCore::switchFlowVersion(DeviceType deviceType)
{
    switch (deviceType) {
    case DT_817:
    case DT_819:
        if( this->m_getFirmwareInfoParameter->getAutotool() )
        {
            this->m_iGetFirmwareInfoCore = getGetFirmwareInfoCore_autotool();
            SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "switchFlowVersion : autotool");
        }
        else if( this->m_getFirmwareInfoParameter->getSpecificChipUserDefined() )
        {
            this->m_iGetFirmwareInfoCore = getGetFirmwareInfoCore_generalSingle();
            SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "switchFlowVersion : generalSingle");
        }
        else
        {
            this->m_iGetFirmwareInfoCore = getGetFirmwareInfoCore_general();
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
GetFirmwareInfoCore::setSiSProcedure(SiSProcedure* sisProcedure)
{
    std::string msg = EXCEPTION_TITLE;
    char errorMsg[1024] = "";
    sprintf(errorMsg, "GetFirmwareInfoCore::setSiSProcedure : only set by itself !");
    msg.append(errorMsg);
    throw CTException( msg );
}

void
GetFirmwareInfoCore::init()
{
    CTBaseCore::init();

    /* auto switch FlowVersion by OS DeviceType */
    switchFlowVersion( this->m_getFirmwareInfoParameter->getOSDeviceType() );
}

CTExitCode
GetFirmwareInfoCore::exec()
{
    return this->m_iGetFirmwareInfoCore->exec();
}
