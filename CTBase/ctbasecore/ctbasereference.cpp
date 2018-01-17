#include "ctbasereference.h"
#include "addressprovider/addresstable/attributekeydefine.h"

CTBaseReference::CTBaseReference() :
    m_deviceType(DT_INVALID),
    m_broken(DEFAULT_BROKEN),
    m_fwSizeType(FST_INVALID),
    m_interfaceID(DEFAULT_BYTE_DATA),
    m_multiChipSelectiveID(DEFAULT_BYTE_DATA),
    m_slaveNum(0),
    m_calibrationFlag(DEFAULT_CALIBRATION_FLAG),
    m_firmwareID(0),
    m_bootloaderID(0),
    m_bootCodeCRC(0),
    m_mainCodeCRC(0),
    m_lastID(0),
    m_lastTime(0),
    m_priorLastID(0),
    m_priorLastTime(0),
    m_taskID(0)
{
    /* default init : value 'de' */
    m_firmwareID = new SerialData(EXPECT_SIZE_FA_FIRMWARE_ID_XRAM);
    m_bootloaderID = new SerialData(EXPECT_SIZE_FA_BOOTLOADER_ID_XRAM);
    m_bootCodeCRC = new SerialData(EXPECT_SIZE_FA_BOOT_CODE_CRC_XRAM);
    m_mainCodeCRC = new SerialData(EXPECT_SIZE_FA_MAIN_CODE_CRC_XRAM);
    m_lastID = new SerialData(EXPECT_SIZE_FA_LAST_ID_XRAM);
    m_lastTime = new SerialData(EXPECT_SIZE_FA_LAST_TIME_XRAM);
    m_priorLastID = new SerialData(EXPECT_SIZE_FA_PRIOR_LAST_ID_XRAM);
    m_priorLastTime = new SerialData(EXPECT_SIZE_FA_PRIOR_LAST_TIME_XRAM);
    m_taskID = new SerialData(EXPECT_SIZE_FA_TASK_ID_XRAM);

    /* default init : Fw version */
    m_fwVersion.major = 0;
    m_fwVersion.minor = 0;
}

CTBaseReference::~CTBaseReference()
{
    if( this->m_firmwareID != 0 ) { delete this->m_firmwareID; }
    if( this->m_bootloaderID != 0 ) { delete this->m_bootloaderID; }
    if( this->m_bootCodeCRC != 0 ) { delete this->m_bootCodeCRC; }
    if( this->m_mainCodeCRC != 0 ) { delete this->m_mainCodeCRC; }
    if( this->m_lastID != 0 ) { delete this->m_lastID; }
    if( this->m_lastTime != 0 ) { delete this->m_lastTime; }
    if( this->m_priorLastID != 0 ) { delete this->m_priorLastID; }
    if( this->m_priorLastTime != 0 ) { delete this->m_priorLastTime; }
    if( this->m_taskID != 0 ) { delete this->m_taskID; }
}

bool
CTBaseReference::isDummy()
{
    if( this->m_deviceType == DT_817 )
    {
        /* 817 */
        if( this->m_interfaceID == INTERFACE_ID_I2C || this->m_interfaceID == INTERFACE_ID_USB )
        {
            return false;
        }
        else if( this->m_interfaceID == INTERFACE_ID_DUMMY_817 )
        {
            return true;
        }
        else
        {
            return true;
        }
    }
    else if( this->m_deviceType == DT_819 )
    {
        /* 819 */
        if( this->m_interfaceID == INTERFACE_ID_I2C || this->m_interfaceID == INTERFACE_ID_USB || this->m_interfaceID == INTERFACE_ID_SPI_819 )
        {
            return false;
        }
        else if( this->m_interfaceID == INTERFACE_ID_DUMMY_819 )
        {
            return true;
        }
        else
        {
            return true;
        }
    }
    else
    {
        return true;
    }
}

DeviceType
CTBaseReference::getDeviceType()
{
    return this->m_deviceType;
}

bool
CTBaseReference::isBroken()
{
    return this->m_broken;
}

FWSizeType
CTBaseReference::getFWSizeType()
{
    return this->m_fwSizeType;
}

unsigned char
CTBaseReference::getInterfaceID()
{
    return this->m_interfaceID;
}

unsigned char
CTBaseReference::getMultiChipSelectiveID()
{
    return this->m_multiChipSelectiveID;
}

int
CTBaseReference::getSlaveNum()
{
    return this->m_slaveNum;
}

bool
CTBaseReference::getCalibrationFlag()
{
    return this->m_calibrationFlag;
}

SerialData*
CTBaseReference::getFirmwareID()
{
    return this->m_firmwareID;
}

SerialData*
CTBaseReference::getBootloaderID()
{
    return this->m_bootloaderID;
}

SerialData*
CTBaseReference::getBootCodeCRC()
{
    return this->m_bootCodeCRC;
}

SerialData*
CTBaseReference::getMainCodeCRC()
{
    return this->m_mainCodeCRC;
}

SerialData*
CTBaseReference::getLastID()
{
    return this->m_lastID;
}

SerialData*
CTBaseReference::getLastTime()
{
    return this->m_lastTime;
}

SerialData*
CTBaseReference::getPriorLastID()
{
    return this->m_priorLastID;
}

SerialData*
CTBaseReference::getPriorLastTime()
{
    return this->m_priorLastTime;
}

SerialData*
CTBaseReference::getTaskID()
{
    return this->m_taskID;
}

FwVersion
CTBaseReference::getFwVersion()
{
    return this->m_fwVersion;
}

void
CTBaseReference::setDeviceType(DeviceType deviceType)
{
    this->m_deviceType = deviceType;
}

void
CTBaseReference::setBroken(bool broken)
{
    this->m_broken = broken;
}

void
CTBaseReference::setFWSizeType(FWSizeType fwSizeType)
{
    this->m_fwSizeType = fwSizeType;
}

void
CTBaseReference::setInterfaceID(unsigned char interfaceID)
{
    this->m_interfaceID = interfaceID;
}

void
CTBaseReference::setMultiChipSelectiveID(unsigned char multiChipSelectiveID)
{
    this->m_multiChipSelectiveID = multiChipSelectiveID;
}

void
CTBaseReference::setSlaveNum(int slaveNum)
{
    this->m_slaveNum = slaveNum;
}

void
CTBaseReference::setCalibrationFlag(bool calibrationFlag)
{
    this->m_calibrationFlag = calibrationFlag;
}

void
CTBaseReference::setFirmwareID(SerialData* firmwareID)
{
    if( this->m_firmwareID != 0 ) { delete this->m_firmwareID; }

    this->m_firmwareID = firmwareID;
}

void
CTBaseReference::setBootloaderID(SerialData* bootloaderID)
{
    if( this->m_bootloaderID != 0 ) { delete this->m_bootloaderID; }

    this->m_bootloaderID = bootloaderID;
}

void
CTBaseReference::setBootCodeCRC(SerialData* bootCodeCRC)
{
    if( this->m_bootCodeCRC != 0 ) { delete this->m_bootCodeCRC; }

    this->m_bootCodeCRC = bootCodeCRC;
}

void
CTBaseReference::setMainCodeCRC(SerialData* mainCodeCRC)
{
    if( this->m_mainCodeCRC != 0 ) { delete this->m_mainCodeCRC; }

    this->m_mainCodeCRC = mainCodeCRC;
}

void
CTBaseReference::setLastID(SerialData* lastID)
{
    if( this->m_lastID != 0 ) { delete this->m_lastID; }

    this->m_lastID = lastID;
}

void
CTBaseReference::setLastTime(SerialData* lastTime)
{
    if( this->m_lastTime != 0 ) { delete this->m_lastTime; }

    this->m_lastTime = lastTime;
}

void
CTBaseReference::setPriorLastID(SerialData* priorLastID)
{
    if( this->m_priorLastID != 0 ) { delete this->m_priorLastID; }

    this->m_priorLastID = priorLastID;
}

void
CTBaseReference::setPriorLastTime(SerialData* priorLastTime)
{
    if( this->m_priorLastTime != 0 ) { delete this->m_priorLastTime; }

    this->m_priorLastTime = priorLastTime;
}

void
CTBaseReference::setTaskID(SerialData* taskID)
{
    if( this->m_taskID != 0 ) { delete this->m_taskID; }

    this->m_taskID = taskID;
}

void
CTBaseReference::setFwVersion(FwVersion fwVersion)
{
    this->m_fwVersion = fwVersion;
}

