#include "getfirmwareinfocore_autotool.h"

#include "sislog.h"
#include "ctexception/ctexception.h"
#include "spexception/spexception.h"

#include "sisresult/sisresult.h"

#define M_BROKEN        "M_BROKEN"
#define PID             "PID"
#define VID             "VID"
#define FW_VER          "FW_VER"
#define TASK_ID         "TASK_ID"
#define SLAVE_NUM       "SLAVE_NUM"

using namespace CT;
using namespace SiS;
using namespace SiS::Procedure;

#define EXCEPTION_TITLE "GetFirmwareInfoCore_autotool Exception : "
#define TAG "GetFirmwareInfoCore_autotool"

GetFirmwareInfoCore_autotool::GetFirmwareInfoCore_autotool(GetFirmwareInfoParameter* getFirmwareInfoParameter) :
    GetFirmwareInfoCore_general(getFirmwareInfoParameter)
{

}

GetFirmwareInfoCore_autotool::~GetFirmwareInfoCore_autotool()
{

}

CTExitCode
GetFirmwareInfoCore_autotool::exec()
{
    /* prepareBaseXramMasterRef */
    prepareBaseXramMasterRef(false); // continue even if broken

    prepareXramMasterRef();

    /* PID */
    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s", SiSResult::getAttribute(PID, m_sisProcedure->readPID()).c_str() );

    /* VID */
    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s", SiSResult::getAttribute(VID, m_sisProcedure->readVID()).c_str() );

    /* M_BROKEN */
    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s",
              SiSResult::getAttribute(M_BROKEN, this->getGetFirmwareInfoReference(RS_XRAM, CI_MASTER)->isBroken() == true ? "YES" : "NO").c_str() );

    if( !this->getGetFirmwareInfoReference(RS_XRAM, CI_MASTER)->isBroken() )
    {
        /* FW_VER */
        string fwVerStr = "";
        char msg[10] = "";
        sprintf(msg, "%d.%d", this->getGetFirmwareInfoReference(RS_XRAM, CI_MASTER)->getFwVersion().major,
                this->getGetFirmwareInfoReference(RS_XRAM, CI_MASTER)->getFwVersion().minor);
        fwVerStr.append(msg);
        SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s", SiSResult::getAttribute(FW_VER, fwVerStr).c_str() );

        /* TASK_ID */
        string taskIDStr = "\"";
        taskIDStr.append(this->getGetFirmwareInfoReference(RS_XRAM, CI_MASTER)->getTaskID()->getSerial());
        taskIDStr.append("\"");
        SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s", SiSResult::getAttribute(TASK_ID, taskIDStr).c_str() );

        /* SLAVE_NUM */
        string slaveNum = "";
        char msg1[10] = "";
        sprintf(msg1, "%d", this->getGetFirmwareInfoReference(RS_XRAM, CI_MASTER)->getSlaveNum());
        slaveNum.append(msg1);
        SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s", SiSResult::getAttribute(SLAVE_NUM, slaveNum).c_str() );
    }

    return CT_EXIT_PASS;
}

void
GetFirmwareInfoCore_autotool::prepareXramMasterRef()
{
    /* switch SiSProcedure by OS DeviceType */
    m_sisProcedure->switchSiSProcedure( m_ctBaseParameter->getOSDeviceType() );

    /* init reference */
    insertReference(RS_XRAM, CI_MASTER, generateReference());

    fetchFwVersion(RS_XRAM, CI_MASTER);
    fetchTaskID(RS_XRAM, CI_MASTER);
}
