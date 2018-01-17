#ifndef CTBASEPARAMETER_H
#define CTBASEPARAMETER_H

#include "ctbasedllexportdefine.h"
#include "sisargument/sisargument.h"
#include "ctbasecore/ctbasedefine.h"
#include "spbase/sisproceduredefine.h"

using namespace SiS::Procedure;
using namespace SE;

#define EXPRESSION_KEYWORD_SLAVE_INDEX "$SLAVE_INDEX"

namespace CT
{

namespace CTBP
{

const std::string ARG_HELP = "-h";                              // m_help
const std::string ARG_DEBUG = "-debug";                         // m_debug
const std::string ARG_LOG_PATH = "-log";                        // m_logPath
const std::string ARG_DEVICE_ID = "-n";                       // m_deviceID
const std::string ARG_DIS_CTL_REPORT_TO_OS = "-dis-crto";       // m_ctlReportToOs
const std::string ARG_DEVICE_TYPE = "-dt";                      // m_osDeviceType
const std::string ARG_DEVICE_INTERFACE = "-di";                 // m_osDeviceInterface
const std::string ARG_SLAVE_NUM = "-sn";                        // m_slaveNum
const std::string ARG_SPECIFIC_CHIP = "-chip";                  // m_specificChip
const std::string ARG_WAIT_RESET_SEC = "-wrs";                  // m_waitResetSec
const std::string ARG_IO_INTERVAL = "-io-interval";             // m_ioInterval
const std::string ARG_IO_INTERVAL_OF_81 = "-io-intervalof81";   // m_ioIntervalOf81

}

class CTBASE_EXPORT CTBaseParameter : public SiSArgument
{
public:
    explicit CTBaseParameter();
    virtual ~CTBaseParameter();

    virtual void showHelp();
    virtual void parseArgument();

    /* get function */
    bool getHelp();
    bool getDebug();
    std::string getLogPath();
    std::string getDeviceID();
    bool getCtlReportToOs();
    DeviceType getOSDeviceType();
    bool getOSDeviceTypeUserDefined();
    DeviceInterface getOSDeviceInterface();
    bool getOSDeviceInterfaceUserDefined();
    bool getMasterBroken();
    FWSizeType getMasterFWSizeType();
    int getSlaveNum();
    bool getSlaveNumUserDefined();
    int getSpecificChip();
    bool getSpecificChipUserDefined();
    int getWaitResetSec();
    int getIOInterval();
    int getIOIntervalOf81();

    /* set function */
    void setHelp(bool help);
    void setDebug(bool debug);
    void setLogPath(std::string logPath);
    void setDeviceID(std::string deviceID);
    void setCtlReportToOs(bool ctlReportToOs);
    void setOSDeviceType(DeviceType osDeviceType);
    void setOSDeviceInterface(DeviceInterface osDeviceInterface);
    void setMasterBroken(bool masterBroken);
    void setMasterFWSizeType(FWSizeType masterFWSizeType);
    void setSlaveNum(int slaveNum);
    void setSpecificChip(int specificChip);
    void setWaitResetSec(int waitResetSec);
    void setIOInterval(int ioInterval);
    void setIOIntervalOf81(int ioIntervalOf81);

protected:
    virtual bool isLegalArgument(ArgumentExpression* argumentExpression);
    virtual bool setArgument(ArgumentExpression* argumentExpression);
    virtual bool hasArgumentConflict();

protected:
    bool m_help; // arg
    bool m_debug; // arg
    std::string m_logPath; // arg
    std::string m_deviceID; // arg
    bool m_ctlReportToOs; // arg

    DeviceType m_osDeviceType; // arg
    bool m_osDeviceTypeUserDefined;
    DeviceInterface m_osDeviceInterface; // arg
    bool m_osDeviceInterfaceUserDefined;
    int m_slaveNum; // arg
    bool m_slaveNumUserDefined;
    int m_specificChip; //arg
    bool m_specificChipUserDefined;

    int m_waitResetSec; // arg
    int m_ioInterval; // arg (ms)
    int m_ioIntervalOf81; // arg (ms)
};

} // CT

#endif // CTBASEPARAMETER_H
