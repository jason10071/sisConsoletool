#include "ctbaseparameter.h"

#include "sislog.h"
#include "ctexception/ctexception.h"
#include "shellcommand/shellcommand.h"

using namespace CT;
using namespace CT::CTBP;
using namespace SiS;
using namespace SiS::Procedure;

#define EXCEPTION_TITLE "CTBaseParameter Exception : "
#define TAG "CTBaseParameter"

CTBaseParameter::CTBaseParameter() :
    SiSArgument(),
    m_help(false),
    m_debug(false),
    m_logPath(""),
    m_deviceID(""),
    m_ctlReportToOs(true),
    m_osDeviceType(DT_INVALID),
    m_osDeviceTypeUserDefined(false),
    m_osDeviceInterface(DI_INVALID),
    m_osDeviceInterfaceUserDefined(false),
    m_slaveNum(0),
    m_slaveNumUserDefined(false),
    m_specificChip(0),
    m_specificChipUserDefined(false),
    m_waitResetSec(4), // sec
    m_ioInterval(-1), // ms
    m_ioIntervalOf81(-1) // ms
{

}

CTBaseParameter::~CTBaseParameter()
{

}

void
CTBaseParameter::showHelp()
{
    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s", ARG_HELP.c_str() );
    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "\t#show help\n" );

    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s", ARG_DEBUG.c_str() );
    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "\t#enable debug log\n" );

    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s=<FILE>", ARG_LOG_PATH.c_str() );
    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "\t@<FILE>=[*.txt]" );
    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "\t#set log path\n" );

    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s=<VALUE>", ARG_DEVICE_ID.c_str() );
    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "\t#open a SiS device with indicated device ID\n" );

    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s", ARG_DIS_CTL_REPORT_TO_OS.c_str() );
    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "\t#disable CTL report to OS\n" );

    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s=<OPTION>", ARG_DEVICE_TYPE.c_str() );
    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "\t@<OPTION>=[817|819]" );
    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "\t#forced to set (OS) device type\n" );

    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s=<OPTION>", ARG_DEVICE_INTERFACE.c_str() );
    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "\t@<OPTION>=[%s|%s|%s|%s]", DEVICE_INTERFACE_I2C,
                    DEVICE_INTERFACE_HID_I2C, DEVICE_INTERFACE_HID_USB, DEVICE_INTERFACE_DUMMY );
    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "\t#forced to set (OS) device interface\n" );

    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s=<VALUE>", ARG_SLAVE_NUM.c_str() );
    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "\t#forced to set slave num\n" );

    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s=<VALUE>", ARG_SPECIFIC_CHIP.c_str() );
    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "\t#forced to set specific chip index" );
    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "\t#master set as 0, slave0 set as 1, slave1 set as 2\n" );

    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s=<VALUE>", ARG_WAIT_RESET_SEC.c_str() );
    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "\t#set wait device reset time (sec)\n" );

    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s=<VALUE>", ARG_IO_INTERVAL.c_str() );
    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "\t#set IO Interval (ms), It means delay between write and read\n" );

    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s=<VALUE>", ARG_IO_INTERVAL_OF_81.c_str() );
    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "\t#set IO IntervalOf81 (ms), It means delay between write and read of CMD_81\n" );
}

void
CTBaseParameter::parseArgument()
{
    for(int idx = 0; idx < size(); idx++)
    {
        if( contains(idx) )
        {
            ArgumentExpression* argumentExpression = find(idx);

            /* show name(value) of arg */
            if( argumentExpression->getAttribute() == ArgumentExpression::ARGUMENT_BOOL ||
                    argumentExpression->getAttribute() == ArgumentExpression::ARGUMENT_UNKNOW )
            {
                SIS_LOG_I(SiSLog::getOwnerSiS(), TAG,
                          "[arg%d: %s (%d)] ",
                          argumentExpression->getIndex(),
                          argumentExpression->getName().c_str(),
                          argumentExpression->getAttribute() );
            }
            else
            {
                SIS_LOG_I(SiSLog::getOwnerSiS(), TAG,
                          "[arg%d: %s=%s (%d)] ",
                          argumentExpression->getIndex(),
                          argumentExpression->getName().c_str(),
                          argumentExpression->getValue().c_str(),
                          argumentExpression->getAttribute() );
            }

            /* check if legal arg */
            if( !isLegalArgument(argumentExpression) )
            {
                std::string msg = EXCEPTION_TITLE;
                char errorMsg[1024] = "";
                sprintf(errorMsg, "unknow arg\n");
                msg.append(errorMsg);
                throw CTException( msg );
            }

            /* set arg */
            if( !setArgument(argumentExpression) )
            {
                std::string msg = EXCEPTION_TITLE;
                char errorMsg[1024] = "";
                sprintf(errorMsg, "Set FAIL (unknow arg)\n");
                msg.append(errorMsg);
                throw CTException( msg );
            }
        }
    }

    /* chech if has argument conflict */
    if( hasArgumentConflict() )
    {
        std::string msg = EXCEPTION_TITLE;
        char errorMsg[1024] = "";
        sprintf(errorMsg, "Argument Conflict\n");
        msg.append(errorMsg);
        throw CTException( msg );
    }

    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "");
}

bool
CTBaseParameter::isLegalArgument(ArgumentExpression* argumentExpression)
{
    if( argumentExpression->getName().compare( ARG_HELP ) == 0  &&
            argumentExpression->getAttribute() == ArgumentExpression::ARGUMENT_BOOL )
    {
        return true;
    }
    else if( argumentExpression->getName().compare( ARG_DEBUG ) == 0  &&
             argumentExpression->getAttribute() == ArgumentExpression::ARGUMENT_BOOL )
    {
        return true;
    }
    else if( argumentExpression->getName().compare( ARG_LOG_PATH ) == 0  &&
             (argumentExpression->getAttribute() == ArgumentExpression::ARGUMENT_VALUE ||
              argumentExpression->getAttribute() == ArgumentExpression::ARGUMENT_FILE))
    {
        return true;
    }
    else if( argumentExpression->getName().compare( ARG_DEVICE_ID ) == 0  &&
             argumentExpression->getAttribute() == ArgumentExpression::ARGUMENT_VALUE )
    {
        return true;
    }
    else if( argumentExpression->getName().compare( ARG_DIS_CTL_REPORT_TO_OS ) == 0  &&
             argumentExpression->getAttribute() == ArgumentExpression::ARGUMENT_BOOL )
    {
        return true;
    }
    else if( argumentExpression->getName().compare( ARG_DEVICE_TYPE ) == 0  &&
             (argumentExpression->getAttribute() == ArgumentExpression::ARGUMENT_VALUE ||
              argumentExpression->getAttribute() == ArgumentExpression::ARGUMENT_OPTION) )
    {
        return true;
    }
    else if( argumentExpression->getName().compare( ARG_DEVICE_INTERFACE ) == 0  &&
             (argumentExpression->getAttribute() == ArgumentExpression::ARGUMENT_VALUE ||
              argumentExpression->getAttribute() == ArgumentExpression::ARGUMENT_OPTION) )
    {
        return true;
    }
    else if( argumentExpression->getName().compare( ARG_SLAVE_NUM ) == 0  &&
             argumentExpression->getAttribute() == ArgumentExpression::ARGUMENT_VALUE )
    {
        return true;
    }
    else if( argumentExpression->getName().compare( ARG_SPECIFIC_CHIP ) == 0 &&
             argumentExpression->getAttribute() == ArgumentExpression::ARGUMENT_VALUE )
    {
        return true;
    }
    else if( argumentExpression->getName().compare( ARG_WAIT_RESET_SEC ) == 0 &&
             argumentExpression->getAttribute() == ArgumentExpression::ARGUMENT_VALUE )
    {
        return true;
    }
    else if( argumentExpression->getName().compare( ARG_IO_INTERVAL ) == 0 &&
             argumentExpression->getAttribute() == ArgumentExpression::ARGUMENT_VALUE )
    {
        return true;
    }
    else if( argumentExpression->getName().compare( ARG_IO_INTERVAL_OF_81 ) == 0 &&
             argumentExpression->getAttribute() == ArgumentExpression::ARGUMENT_VALUE )
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool
CTBaseParameter::setArgument(ArgumentExpression* argumentExpression)
{
    if( contains(argumentExpression->getName()) )
    {
        if( argumentExpression->getName().compare( ARG_HELP ) == 0 )
        {
            if( argumentExpression->getValueToBool() )
            {
                this->m_help = true;
                SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "set help : true");
            }
            else
            {
                this->m_help = false;
                SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "set help : false");
            }
            return true;
        }
        else if( argumentExpression->getName().compare( ARG_DEBUG ) == 0 )
        {
            if( argumentExpression->getValueToBool() )
            {
                this->m_debug = true;
                SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "set debug : true");
            }
            else
            {
                this->m_debug = false;
                SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "set debug : false");
            }
            return true;
        }
        else if( argumentExpression->getName().compare( ARG_LOG_PATH ) == 0 )
        {
            this->m_logPath = argumentExpression->getValue();
            SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "set logPath : %s", m_logPath.c_str());
            return true;
        }
        else if( argumentExpression->getName().compare( ARG_DEVICE_ID ) == 0 )
        {
            std::string deviceID = argumentExpression->getValue();
			
            /* convert hidraw* to device path */
            std::string hidrawStr = "hidraw";
			std::string hidrawNum = argumentExpression->getValue();
            if( hidrawNum.substr(0, hidrawStr.size()) == hidrawStr ) // if startswith "hidraw"
            {
                /* find ls_sys_class_hidraw_hidrawN */
                std::string cmd_ls_sys_class_hidraw_hidrawN = "ls -l /sys/class/hidraw/"; // " ls -l /sys/class/hidraw/hidraw* "
                cmd_ls_sys_class_hidraw_hidrawN.append( hidrawNum );
                SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "cmd : %s", cmd_ls_sys_class_hidraw_hidrawN.c_str() );
                std::string ls_sys_class_hidraw_hidrawN = ShellCommand::exec( cmd_ls_sys_class_hidraw_hidrawN.c_str() );
                SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "output : %s", ls_sys_class_hidraw_hidrawN.c_str() );

                /* parse to deviceID */
                std::string keywordStr1 = "-> ../../";
				std::string keywordStr2 = "/hidraw/hidraw";
                if( ls_sys_class_hidraw_hidrawN.find(keywordStr1) != std::string::npos &&
                    ls_sys_class_hidraw_hidrawN.find(keywordStr2) != std::string::npos )
                {
                    std::string tmpDeviceID = ls_sys_class_hidraw_hidrawN;
                    tmpDeviceID = tmpDeviceID.substr( tmpDeviceID.find(keywordStr1) + keywordStr1.size(), tmpDeviceID.size());
                    tmpDeviceID = tmpDeviceID.substr(0, tmpDeviceID.find(keywordStr2));
                    tmpDeviceID = tmpDeviceID.substr(0, tmpDeviceID.find_last_of("/")); // remove device (0000:0000:0000.0000)

                    deviceID = "/sys/";
                    deviceID.append(tmpDeviceID);
                }
            }

            /* assign deviceID */
            this->m_deviceID = deviceID;
            SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "set deviceID : \"%s\"", m_deviceID.c_str());
            return true;
        }
        if( argumentExpression->getName().compare( ARG_DIS_CTL_REPORT_TO_OS ) == 0 )
        {
            if( !argumentExpression->getValueToBool() )
            {
                this->m_ctlReportToOs = true;
                SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "set ctlReportToOs : true");
            }
            else
            {
                this->m_ctlReportToOs = false;
                SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "set ctlReportToOs : false");
            }
            return true;
        }
        else if( argumentExpression->getName().compare( ARG_DEVICE_TYPE ) == 0 )
        {
            if( argumentExpression->getValue().compare( "817" ) == 0 )
            {
                this->m_osDeviceType = DT_817;
                this->m_osDeviceTypeUserDefined = true;
                SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "forced to set (OS) deviceType (UserDefined) : DT_817");
                return true;
            }
            else if( argumentExpression->getValue().compare( "819" ) == 0 )
            {
                this->m_osDeviceType = DT_819;
                this->m_osDeviceTypeUserDefined = true;
                SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "forced to set (OS) deviceType (UserDefined) : DT_819");
                return true;
            }
            else
            {
                std::string msg = EXCEPTION_TITLE;
                char errorMsg[1024] = "";
                sprintf(errorMsg, "forced to set (OS) deviceType FAIL (unknow type : %s)\n",
                        argumentExpression->getValue().c_str());
                msg.append(errorMsg);
                throw CTException( msg );
            }
        }
        else if( argumentExpression->getName().compare( ARG_DEVICE_INTERFACE ) == 0 )
        {
            if( argumentExpression->getValue().compare( DEVICE_INTERFACE_I2C ) == 0 )
            {
                this->m_osDeviceInterface = DI_I2C;
                this->m_osDeviceInterfaceUserDefined = true;
                SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "forced to set (OS) deviceInterface (UserDefined) : DI_I2C");
                return true;
            }
            else if( argumentExpression->getValue().compare( DEVICE_INTERFACE_HID_I2C ) == 0 )
            {
                this->m_osDeviceInterface = DI_HID_I2C;
                this->m_osDeviceInterfaceUserDefined = true;
                SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "forced to set (OS) deviceInterface (UserDefined) : DI_HID_I2C");
                return true;
            }
            else if( argumentExpression->getValue().compare( DEVICE_INTERFACE_HID_USB ) == 0 )
            {
                this->m_osDeviceInterface = DI_HID_USB;
                this->m_osDeviceInterfaceUserDefined = true;
                SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "forced to set (OS) deviceInterface (UserDefined) : DI_HID_USB");
                return true;
            }
            else if( argumentExpression->getValue().compare( DEVICE_INTERFACE_DUMMY ) == 0 )
            {
                this->m_osDeviceInterface = DI_DUMMY;
                this->m_osDeviceInterfaceUserDefined = true;
                SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "forced to set (OS) deviceInterface (UserDefined) : DI_DUMMY");
                return true;
            }
            else
            {
                std::string msg = EXCEPTION_TITLE;
                char errorMsg[1024] = "";
                sprintf(errorMsg, "forced to set (OS) deviceInterface FAIL (unknow type : %s)\n",
                        argumentExpression->getValue().c_str());
                msg.append(errorMsg);
                throw CTException( msg );
            }
        }
        else if( argumentExpression->getName().compare( ARG_SLAVE_NUM ) == 0 )
        {
            this->m_slaveNum = argumentExpression->getValueToInt();
            this->m_slaveNumUserDefined = true;

            if( m_slaveNum >= 0 )
            {
                SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "set slaveNum (UserDefined) : %d", m_slaveNum);
                return true;
            }
            else
            {
                std::string msg = EXCEPTION_TITLE;
                char errorMsg[1024] = "";
                sprintf(errorMsg, "set slaveNum FAIL : can't be %d\n", m_slaveNum);
                msg.append(errorMsg);
                throw CTException( msg );
            }
        }
        else if( argumentExpression->getName().compare( ARG_SPECIFIC_CHIP ) == 0 )
        {
            this->m_specificChip = argumentExpression->getValueToInt();
            this->m_specificChipUserDefined = true;

            if( m_specificChip >= 0 )
            {
                SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "set specificChip (UserDefined) : %d", m_specificChip);
                return true;
            }
            else
            {
                std::string msg = EXCEPTION_TITLE;
                char errorMsg[1024] = "";
                sprintf(errorMsg, "set specificChip FAIL : can't be %d\n", m_specificChip);
                msg.append(errorMsg);
                throw CTException( msg );
            }
            return true;
        }
        else if( argumentExpression->getName().compare( ARG_WAIT_RESET_SEC ) == 0 )
        {
            this->m_waitResetSec = argumentExpression->getValueToInt();

            if( m_waitResetSec >= 1 )
            {
                SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "set wait device reset time (sec) : %d", m_waitResetSec);
                return true;
            }
            else
            {
                std::string msg = EXCEPTION_TITLE;
                char errorMsg[1024] = "";
                sprintf(errorMsg, "set wait device reset time (sec) FAIL : can't be %d\n", m_waitResetSec);
                msg.append(errorMsg);
                throw CTException( msg );
            }
            return true;
        }
        else if( argumentExpression->getName().compare( ARG_IO_INTERVAL ) == 0 )
        {
            this->m_ioInterval = argumentExpression->getValueToInt();

            if( m_ioInterval >= 0 )
            {
                SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "set IO Interval (ms) : %d", m_ioInterval);
                return true;
            }
            else
            {
                std::string msg = EXCEPTION_TITLE;
                char errorMsg[1024] = "";
                sprintf(errorMsg, "set IO Interval (ms) FAIL : can't be %d\n", m_ioInterval);
                msg.append(errorMsg);
                throw CTException( msg );
            }
            return true;
        }
        else if( argumentExpression->getName().compare( ARG_IO_INTERVAL_OF_81 ) == 0 )
        {
            this->m_ioIntervalOf81 = argumentExpression->getValueToInt();

            if( m_ioIntervalOf81 >= 0 )
            {
                SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "set IO IntervalOf81 (ms) : %d", m_ioIntervalOf81);
                return true;
            }
            else
            {
                std::string msg = EXCEPTION_TITLE;
                char errorMsg[1024] = "";
                sprintf(errorMsg, "set IO IntervalOf81 (ms) FAIL : can't be %d\n", m_ioIntervalOf81);
                msg.append(errorMsg);
                throw CTException( msg );
            }
            return true;
        }
    }

    return false;
}

bool
CTBaseParameter::hasArgumentConflict()
{
    /* no conflict */
    return false;
}

bool
CTBaseParameter::getHelp()
{
    return this->m_help;
}

bool
CTBaseParameter::getDebug()
{
    return this->m_debug;
}

std::string
CTBaseParameter::getLogPath()
{
    return this->m_logPath;
}

std::string
CTBaseParameter::getDeviceID()
{
    return this->m_deviceID;
}

bool
CTBaseParameter::getCtlReportToOs()
{
    return this->m_ctlReportToOs;
}

DeviceType
CTBaseParameter::getOSDeviceType()
{
    return this->m_osDeviceType;
}

bool
CTBaseParameter::getOSDeviceTypeUserDefined()
{
    return this->m_osDeviceTypeUserDefined;
}

DeviceInterface
CTBaseParameter::getOSDeviceInterface()
{
    return this->m_osDeviceInterface;
}

bool
CTBaseParameter::getOSDeviceInterfaceUserDefined()
{
    return this->m_osDeviceInterfaceUserDefined;
}

int
CTBaseParameter::getSlaveNum()
{
    return this->m_slaveNum;
}

bool
CTBaseParameter::getSlaveNumUserDefined()
{
    return this->m_slaveNumUserDefined;
}

int
CTBaseParameter::getSpecificChip()
{
    return this->m_specificChip;
}

bool
CTBaseParameter::getSpecificChipUserDefined()
{
    return this->m_specificChipUserDefined;
}

int
CTBaseParameter::getWaitResetSec()
{
    return this->m_waitResetSec;
}

int
CTBaseParameter::getIOInterval()
{
    return this->m_ioInterval;
}

int
CTBaseParameter::getIOIntervalOf81()
{
    return this->m_ioIntervalOf81;
}

void
CTBaseParameter::setHelp(bool help)
{
    this->m_help = help;
}

void
CTBaseParameter::setDebug(bool debug)
{
    this->m_debug = debug;
}

void
CTBaseParameter::setLogPath(std::string logPath)
{
    this->m_logPath = logPath;
}

void
CTBaseParameter::setDeviceID(std::string deviceID)
{
    this->m_deviceID = deviceID;
}

void
CTBaseParameter::setCtlReportToOs(bool ctlReportToOs)
{
    this->m_ctlReportToOs = ctlReportToOs;
}

void
CTBaseParameter::setOSDeviceType(DeviceType osDeviceType)
{
    this->m_osDeviceType = osDeviceType;
}

void
CTBaseParameter::setOSDeviceInterface(DeviceInterface osDeviceInterface)
{
    this->m_osDeviceInterface = osDeviceInterface;
}

void
CTBaseParameter::setSlaveNum(int slaveNum)
{
    this->m_slaveNum = slaveNum;
}

void
CTBaseParameter::setSpecificChip(int specificChip)
{
    this->m_specificChip = specificChip;
}

void
CTBaseParameter::setWaitResetSec(int waitResetSec)
{
    this->m_waitResetSec = waitResetSec;
}

void
CTBaseParameter::setIOInterval(int ioInterval)
{
    this->m_ioInterval = ioInterval;
}

void
CTBaseParameter::setIOIntervalOf81(int ioIntervalOf81)
{
    this->m_ioIntervalOf81 = ioIntervalOf81;
}
