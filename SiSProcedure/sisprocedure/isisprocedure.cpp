#include "isisprocedure.h"

#include "sislog.h"
#include "spexception/spexception.h"
#include "spexception/siscoreexception.h"
#include "addressprovider/addresstable/attributekeydefine.h"

//#include "UnderlyingDevDefine.h"
//#include "UnderlyingDevKey.h"
//#include "CptFactoryFacade.h"
//#include "Convert.h"
//#include "HydraDefine.h"

#include <ctime>

using namespace SiS::Procedure;
using namespace std;

#define EXCEPTION_TITLE "ISiSProcedure Exception : "
#define TAG "ISiSProcedure"

ISiSProcedure::ISiSProcedure() :
    m_sisDeviceIO(0),
    m_ctlReportToOsFlag(DEFAULT_CTL_REPORT_TO_OS),
    m_ioInterval(-1),
    m_iOIntervalOf81(-1),
    m_blockRetry(-1),
    m_allRetry(-1),
    m_ramSize(-1)
{

}

ISiSProcedure::~ISiSProcedure()
{

}

SiSDeviceIO*
ISiSProcedure::getSiSDeviceIO()
{
    return this->m_sisDeviceIO;
}

void
ISiSProcedure::setSiSCore(SiSDeviceIO* sisDeviceIO)
{
    this->m_sisDeviceIO = sisDeviceIO;
}

void
ISiSProcedure::setBinWrapMap(std::map<ChipIndexKey, BinWrap*> binWrapMap)
{
    this->m_binWrapMap = binWrapMap;
}

void
ISiSProcedure::setCtlReportToOs(bool ctlReportToOsFlag)
{
    this->m_ctlReportToOsFlag = ctlReportToOsFlag;
}

std::string
ISiSProcedure::openDevice(std::string deviceID)
{
    if( m_sisDeviceIO == 0 )
    {
        std::string msg = EXCEPTION_TITLE;
        char errorMsg[1024] = "";
        sprintf(errorMsg, "openDevice() : no sisDeviceIO");
        msg.append(errorMsg);
        throw SPException( msg );
    }

    if( deviceID.empty() )
    {
        std::string msg = EXCEPTION_TITLE;
        msg.append("Not support to auto search SiS Device ! Please use \"-n=hidraw*\"");
        throw SPException( msg, SPException::SP_SELF_EXCEPTION );
    }

    try
    {
        if( deviceID.empty() )
        {
            // to do
            
            //SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "auto search SiS Device");
            //m_sisDeviceIO->boot();
        }
        else
        {
            SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "open SiS Device : %s", deviceID.c_str());
            //SSetting setting;
            //setting[PHYS_LOCATION] = deviceID;
            m_sisDeviceIO->boot(deviceID);
        }

        //-------------------------------------------------------------------------------------------------------------
        SiSIOAttribute* sisIOAttribute = m_sisDeviceIO->getSiSIOAttribute();
		
        /* set sisCore Attr : IOInterval */
        if( this->m_ioInterval >= 0 )
        {
            sisIOAttribute->setIODelay( this->m_ioInterval );
            SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "ISiSProcedure::openDevice, set m_ioInterval : %d (ms)", sisIOAttribute->getIODelay());
        }
		SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "ISiSProcedure::openDevice, get m_ioInterval : %d (ms)", sisIOAttribute->getIODelay());

        /* set sisCore Attr : IOIntervalOf81 */
        if( this->m_iOIntervalOf81 >= 0 )
        {
            sisIOAttribute->setIODelayOf81( this->m_iOIntervalOf81 );
            SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "ISiSProcedure::openDevice, set m_ioIntervalOf81 : %d (ms)", sisIOAttribute->getIODelayOf81());

        }
		SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "ISiSProcedure::openDevice, get m_ioIntervalOf81 : %d (ms)", sisIOAttribute->getIODelayOf81());


        /* set sisCore Attr : BlockRetry */
        if( this->m_blockRetry > 0 )
        {
            sisIOAttribute->setBlockRetry( this->m_blockRetry );
            SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "ISiSProcedure::openDevice, set setBlockRetry : %d", sisIOAttribute->getBlockRetry());
        }
        SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "ISiSProcedure::openDevice, get setBlockRetry : %d", sisIOAttribute->getBlockRetry());

//        /* set sisCore Attr : AllRetry */
//        if( this->m_allRetry > 0 )
//        {
//            m_sisCore->setOperationAttr(AllRetry, QString::number(this->m_allRetry).toStdString(), "BURST_WRITE");
//            SIS_LOG_D(SiSLog::getOwnerSiS(), TAG,
//                      "ISiSProcedure::openDevice, m_sisCore set m_allRetry : %s", QString::number(this->m_allRetry).toStdString().c_str() );
//        }
//        SIS_LOG_D(SiSLog::getOwnerSiS(), TAG,
//                  "ISiSProcedure::openDevice, m_sisCore get AllRetry : %s", m_sisCore->operationAttr( "BURST_WRITE", AllRetry ).c_str()  );

//        /* set sisCore Attr : RamSize */
//        if( this->m_ramSize > 0 )
//        {
//            m_sisCore->setOperationAttr(RamSize, QString::number(this->m_ramSize).toStdString(), "BURST_WRITE");
//            SIS_LOG_D(SiSLog::getOwnerSiS(), TAG,
//                      "ISiSProcedure::openDevice, m_sisCore set m_ramSize : %s", QString::number(this->m_ramSize).toStdString().c_str() );
//        }
//        SIS_LOG_D(SiSLog::getOwnerSiS(), TAG,
//                  "ISiSProcedure::openDevice, m_sisCore get RamSize : %s", m_sisCore->operationAttr( "BURST_WRITE", RamSize ).c_str() );

//        /* set sisCore Attr : READ_MODE */
//        m_sisCore->setOperationAttr(READ_MODE, Convert::toString(READ_BY_HID), "BURST_READ");
//        SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "ISiSProcedure::openDevice, m_sisCore set READ_MODE : READ_BY_HID" );
//        //-------------------------------------------------------------------------------------------------------------

        /* read deviceID */
        deviceID = m_sisDeviceIO->deviceNameOpened();
        SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "open SiS Device succeed : %s", deviceID.c_str() );

//        /* show device info */
//        SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "DEV_TYPE : %s", m_sisCore->attribute(DEV_TYPE).c_str() );
//        SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "INTERFACE_NAME : %s", m_sisCore->attribute(INTERFACE_NAME).c_str() );
    }
    SISCORE_EXCEPTION_SP_HANDLE

    return deviceID;
}

std::string
ISiSProcedure::reOpenDevice(std::string deviceIDOpened, int waitResetSec, bool isCheckDeviceBack)
{
    if( m_sisDeviceIO == 0 )
    {
        std::string msg = EXCEPTION_TITLE;
        char errorMsg[1024] = "";
        sprintf(errorMsg, "reOpenDevice() : no sisCore");
        msg.append(errorMsg);
        throw SPException( msg );
    }

    SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "re-open SiS Device : %s", deviceIDOpened.c_str() );
    std::string deviceIDReOpened = "";

    /* re-open device */
    int REOPEN_RETRY_MAX = 5;
    for(int i = 1; i <= REOPEN_RETRY_MAX; i++)
    {
        /* reset SiSCore */
        m_sisDeviceIO->shutdown();

        /* wait for device reset */
        SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "(%d of %d) wait device reset %d (sec)", i, REOPEN_RETRY_MAX, waitResetSec );
        IAttributeReader::milliSleep(waitResetSec * 1000);

        try
        {
            if( isCheckDeviceBack )
            {
                SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "isCheckDeviceBack : true" );
                deviceIDReOpened = openDevice( deviceIDOpened );
            }
            else
            {
                SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "isCheckDeviceBack : false" );
            }
            break;
        }
        catch(SPException sp)
        {
            if( i == REOPEN_RETRY_MAX )
            {
                throw SPException( sp.what() );
            }
            else
            {
                SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "%s", sp.what() );
            }
        }

        /* decrease wait time */
        waitResetSec = (waitResetSec / 2) > 1 ? (waitResetSec / 2) : 1;
    }

    /* force to wait device ready */
    int forceToWaitDeviceReadySec = 1; // sec
    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "force to wait device ready : %d (sec)", forceToWaitDeviceReadySec );
    IAttributeReader::milliSleep(forceToWaitDeviceReadySec * 1000);

    if( isCheckDeviceBack )
    {
        /* disableCtlReportToOs */
        disableCtlReportToOs();
    }

    return deviceIDReOpened;
}

void
ISiSProcedure::resetDevice(int waitResetSec, bool isCheckDeviceBack)
{
    /* softReset */
    softReset();

    /* re-open device */
    std::string deviceNameOpened = m_sisDeviceIO->deviceNameOpened();
    reOpenDevice(deviceNameOpened, waitResetSec, isCheckDeviceBack);
}

std::string
ISiSProcedure::readPID()
{
    if( m_sisDeviceIO == 0 )
    {
        std::string msg = EXCEPTION_TITLE;
        char errorMsg[1024] = "";
        sprintf(errorMsg, "readPID() : no sisCore");
        msg.append(errorMsg);
        throw SPException( msg );
    }

//    return m_sisCore->attribute(PID);
    return "PID";
}

std::string
ISiSProcedure::readVID()
{
    if( m_sisDeviceIO == 0 )
    {
        std::string msg = EXCEPTION_TITLE;
        char errorMsg[1024] = "";
        sprintf(errorMsg, "readVID() : no sisCore");
        msg.append(errorMsg);
        throw SPException( msg );
    }

//    return m_sisCore->attribute(VID);
    return "VID";
}

DeviceType
ISiSProcedure::readOSDeviceType()
{
    if( m_sisDeviceIO == 0 )
    {
        std::string msg = EXCEPTION_TITLE;
        char errorMsg[1024] = "";
        sprintf(errorMsg, "readOSDeviceType() : no sisCore");
        msg.append(errorMsg);
        throw SPException( msg );
    }

//    DeviceType deviceType = DT_INVALID;
    //DeviceType deviceType = DT_817;
    DeviceType deviceType = DT_819;

//    /* read (OS) Device Type */
//    std::string devType = m_sisCore->attribute(DEV_TYPE);

//    if( devType.compare(SIS_817) == 0 || devType.compare(AEGIS) == 0 )
//    {
//        deviceType = DT_817;
//    }
//    else
//    {
//        deviceType = DT_819;
//    }

    SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "read (OS) device type : %s", getDTStr(deviceType).c_str() );
    return deviceType;
}

DeviceType
ISiSProcedure::readSubProtocol()
{
    if( m_sisDeviceIO == 0 )
    {
        std::string msg = EXCEPTION_TITLE;
        char errorMsg[1024] = "";
        sprintf(errorMsg, "readSubProtocol() : no sisCore");
        msg.append(errorMsg);
        throw SPException( msg );
    }

    DeviceType deviceType = DT_INVALID;

//    /* read (OS) Device Type */
//    std::string devType = m_sisCore->attribute(SUB_PROTOCOL);

//    if( devType.compare(HID_USB_BRIDGE) == 0 )
//    {
//        deviceType = DT_BRIDGE;
//    }
//    else
//    {
//        deviceType = DT_INVALID;
//    }

//    SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "read sub protocol type : %s", getDTStr(deviceType).c_str() );
    return deviceType;
}

DeviceInterface
ISiSProcedure::readOSDeviceInterface()
{
    if( m_sisDeviceIO == 0 )
    {
        std::string msg = EXCEPTION_TITLE;
        char errorMsg[1024] = "";
        sprintf(errorMsg, "readOSDeviceInterface() : no sisCore");
        msg.append(errorMsg);
        throw SPException( msg );
    }

    DeviceInterface deviceInterface = DI_INVALID;

    /* read (OS) Device Interface */
//    std::string interfaceName = m_sisCore->attribute(INTERFACE_NAME);

//    if( interfaceName.compare(DEVICE_INTERFACE_I2C) == 0 )
//    {
//        deviceInterface = DI_I2C;
//    }
//    else if( interfaceName.compare(DEVICE_INTERFACE_HID_I2C) == 0 )
//    {
//        deviceInterface = DI_HID_I2C;
//    }
//    else if( interfaceName.compare(DEVICE_INTERFACE_HID_USB) == 0 )
//    {
//        deviceInterface = DI_HID_USB;
//    }
//    else if( interfaceName.compare(DEVICE_INTERFACE_DUMMY) == 0 )
//    {
//        deviceInterface = DI_DUMMY;
//    }
    SiSDeviceAttribute* opened = m_sisDeviceIO->getSiSDeviceMgr()->getOpened();
    if( opened->getConnectType() == SiSDeviceAttribute::CON_819_USB_HID )
    {
        deviceInterface = DI_HID_USB;
    }
    else if( opened->getConnectType() == SiSDeviceAttribute::CON_819_HID_OVER_I2C )
    {
        deviceInterface = DI_HID_I2C;
    }

    SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "read (OS) device interface : %s", getDIStr(deviceInterface).c_str() );
    return deviceInterface;
}

DeviceType
ISiSProcedure::readBinDeviceType(int chipIndex)
{
    SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "readBinDeviceType() : chipIndex=%d", chipIndex );

    /* prepare */
    string keyFa0x0200;
    string keyFa0x020c;
    IAttributeReader* attributeReader = 0;

    keyFa0x0200 = KEY_FA_0X0200_ROM;
    keyFa0x020c = KEY_FA_0X020C_ROM;
    attributeReader = generateAttributeReader( getBinWrap(chipIndex) );

    attributeReader->setCtlReportToOs( this->m_ctlReportToOsFlag );
    attributeReader->setChipIndex(chipIndex);

    /* read DeviceType */
    /* 0X0200Xram */
    unsigned char* dataBuf0x0200Xram = 0;
    int dataSize0x0200Xram = 0;
    attributeReader->readAttribute( keyFa0x0200, dataBuf0x0200Xram, dataSize0x0200Xram );

    /* check if address table error */
    IAttributeReader::checkDataSize(EXPECT_SIZE_FA_0X0200_XRAM, dataSize0x0200Xram);

    /* 0X020CXram */
    unsigned char* dataBuf0x020cXram = 0;
    int dataSize0x020cXram = 0;
    attributeReader->readAttribute( keyFa0x020c, dataBuf0x020cXram, dataSize0x020cXram );

    /* check if address table error */
    IAttributeReader::checkDataSize(EXPECT_SIZE_FA_0X020C_XRAM, dataSize0x020cXram);

    /* judge */
    DeviceType deviceType = DT_INVALID;

    if( dataSize0x0200Xram == EXPECT_SIZE_FA_0X0200_XRAM &&
            (dataBuf0x0200Xram[0] == 0x65 &&
             dataBuf0x0200Xram[1] == 0x67 &&
             dataBuf0x0200Xram[2] == 0x69 &&
             dataBuf0x0200Xram[3] == 0x73) &&
            (dataBuf0x020cXram[0] == 0x00 &&
             dataBuf0x020cXram[1] == 0x53 &&
             dataBuf0x020cXram[2] == 0x69 &&
             dataBuf0x020cXram[3] == 0x53) )
    {
        deviceType = DT_817;
    }
    else if( dataSize0x0200Xram == EXPECT_SIZE_FA_0X0200_XRAM &&
                 (dataBuf0x0200Xram[0] == 0x68 &&
                  dataBuf0x0200Xram[1] == 0x79 &&
                  dataBuf0x0200Xram[2] == 0x64 &&
                  dataBuf0x0200Xram[3] == 0x72 &&
                  dataBuf0x0200Xram[4] == 0x61 &&
                  dataBuf0x0200Xram[5] == 0x00) &&
                 (dataBuf0x020cXram[0] == 0x00 &&
                  dataBuf0x020cXram[1] == 0x53 &&
                  dataBuf0x020cXram[2] == 0x69 &&
                  dataBuf0x020cXram[3] == 0x53) )
    {
        deviceType = DT_819;    //65 series - hydra

    }
    else if ( dataSize0x0200Xram == EXPECT_SIZE_FA_0X0200_XRAM &&
              (dataBuf0x0200Xram[0] == 0x64 &&
               dataBuf0x0200Xram[1] == 0x72 &&
               dataBuf0x0200Xram[2] == 0x61 &&
               dataBuf0x0200Xram[3] == 0x63 &&
               dataBuf0x0200Xram[4] == 0x6f) &&
              (dataBuf0x020cXram[0] == 0x00 &&
               dataBuf0x020cXram[1] == 0x53 &&
               dataBuf0x020cXram[2] == 0x69 &&
               dataBuf0x020cXram[3] == 0x53) )
    {
        deviceType = DT_819;    //64 series - draco
    }
    else if ( dataSize0x0200Xram == EXPECT_SIZE_FA_0X0200_XRAM &&
              (dataBuf0x0200Xram[0] == 0x6c &&
               dataBuf0x0200Xram[1] == 0x69 &&
               dataBuf0x0200Xram[2] == 0x62 &&
               dataBuf0x0200Xram[3] == 0x72 &&
               dataBuf0x0200Xram[4] == 0x61) &&
              (dataBuf0x020cXram[0] == 0x00 &&
               dataBuf0x020cXram[1] == 0x53 &&
               dataBuf0x020cXram[2] == 0x69 &&
               dataBuf0x020cXram[3] == 0x53) )
    {
        deviceType = DT_819;   //67 series - libra
    }
    else if ( dataSize0x0200Xram == EXPECT_SIZE_FA_0X0200_XRAM &&
              (dataBuf0x0200Xram[0] == 0x63 &&
               dataBuf0x0200Xram[1] == 0x61 &&
               dataBuf0x0200Xram[2] == 0x6c &&
               dataBuf0x0200Xram[3] == 0x6c &&
               dataBuf0x0200Xram[4] == 0x69) &&
              (dataBuf0x020cXram[0] == 0x00 &&
               dataBuf0x020cXram[1] == 0x53 &&
               dataBuf0x020cXram[2] == 0x69 &&
               dataBuf0x020cXram[3] == 0x53) )
    {
        deviceType = DT_819;   //7701 series - calli
    }
    else
    {
        std::string msg = EXCEPTION_TITLE;
        char errorMsg[1024] = "";
        sprintf(errorMsg, "unknow deviceType !");
        msg.append(errorMsg);
        throw SPException( msg, SPException::SP_SELF_EXCEPTION );
    }

    SIS_LOG_D(SiSLog::getOwnerSiS(), TAG,
              "read %s (%s) DeviceType : %s\n",
              ISiSProcedure::getCIStr(chipIndex).c_str(),
              ISiSProcedure::getRSStr(RS_BIN).c_str(),
              ISiSProcedure::getDTStr(deviceType).c_str() );

    delete [] dataBuf0x0200Xram;
    delete [] dataBuf0x020cXram;
    delete attributeReader;
    return deviceType;
}

BinWrap*
ISiSProcedure::getBinWrap(int chipIndex)
{
    if( m_binWrapMap.find(chipIndex) == m_binWrapMap.end() )
    {
        std::string msg = EXCEPTION_TITLE;
        char errorMsg[1024] = "";
        sprintf(errorMsg, "no bin-file for %s ", ISiSProcedure::getCIStr(chipIndex).c_str());
        msg.append(errorMsg);
        throw SPException( msg, SPException::SP_SELF_EXCEPTION );
    }

    return m_binWrapMap.find(chipIndex)->second;
}

bool
ISiSProcedure::compareData(unsigned char* dataA, int sizeA, unsigned char* dataB, int sizeB)
{
    if( sizeA != sizeB )
    {
        return false;
    }

    for(int i = 0; i < sizeA; i++)
    {
        if( dataA[i] != dataB[i] )
        {
            return false;
        }
    }

    return true;
}

SerialData*
ISiSProcedure::getTimestamp()
{
    time_t t = time(0);   // get time now
    struct tm * now = localtime( &t );

    int dataSize = EXPECT_SIZE_FA_PRIOR_LAST_TIME_XRAM;
    unsigned char* dataBuf = new unsigned char[dataSize];
    memset( dataBuf, 0x0, dataSize * sizeof(unsigned char) );

    /* month/day/hour/minute (4 bytes) */
    dataBuf[0] = (now->tm_mon + 1);
    dataBuf[1] = now->tm_mday;
    dataBuf[2] = now->tm_hour;
    dataBuf[3] = now->tm_min;

    return new SerialData(dataBuf, dataSize);
}

std::string
ISiSProcedure::getFSTStr(FWSizeType fwSizeType)
{
    switch (fwSizeType) {
    case FST_817_R64K:
        return "817_R64K";
        break;
    case FST_817_R64K_M48K:
        return "817_R64K_M48K";
        break;
    case FST_817_R128K:
        return "817_R128K";
        break;
    case FST_819_R128K:
        return "819_R128K";
        break;
    case FST_R64K:
        return "R64K";
        break;
    case FST_R128K:
        return "R128K";
        break;
    case FST_INVALID:
        return "FST_INVALID";
        break;
    default:
        return "FST_UNKNOW";
        break;
    }
}

std::string
ISiSProcedure::getDTStr(DeviceType deviceType)
{
    switch (deviceType) {
    case DT_817:
        return "817";
        break;
    case DT_819:
        return "819";
        break;
    case DT_INVALID:
        return "DT_INVALID";
        break;
    default:
        return "DT_UNKNOW";
        break;
    }
}

std::string
ISiSProcedure::getDIStr(DeviceInterface deviceInterface)
{
    switch (deviceInterface) {
    case DI_I2C:
        return "I2C";
        break;
    case DI_HID_I2C:
        return "HID_I2C";
        break;
    case DI_HID_USB:
        return "HID_USB";
        break;
    case DI_SPI:
        return "SPI";
        break;
    case DI_DUMMY:
        return "DUMMY";
        break;
    case DI_INVALID:
        return "DI_INVALID";
        break;
    default:
        return "DI_UNKNOW";
        break;
    }
}

std::string
ISiSProcedure::getRSStr(ReferenceSource rs)
{
    switch (rs) {
    case RS_BIN:
        return "BIN";
        break;
    case RS_ROM:
        return "ROM";
        break;
    case RS_XRAM:
        return "XRAM";
        break;
    case RS_COUNT:
    default:
        return "RS_UNKNOW";
        break;
    }
}

std::string
ISiSProcedure::getCIStr(int chipIndex)
{
    std::string ciStr;
    char ciTmp[128] = "";

    if( chipIndex == (int) CI_MASTER )
    {
        sprintf(ciTmp, "Master" );
        //sprintf(ciTmp, "Master(chipIndex:%d)", chipIndex );
    }
    else
    {
        sprintf(ciTmp, "Slave%d", (chipIndex - (int) CI_SLAVE_0) );
        //sprintf(ciTmp, "Slave%d(chipIndex:%d)", (chipIndex - (int) CI_SLAVE_0), chipIndex );
    }
    ciStr.append(ciTmp);

    return ciStr;
}

int
ISiSProcedure::getIOInterval()
{
    return this->m_ioInterval;
}

void
ISiSProcedure::setIOInterval(int ioInterval)
{
    this->m_ioInterval = ioInterval;
}

int
ISiSProcedure::getIOIntervalOf81()
{
    return this->m_iOIntervalOf81;
}

void
ISiSProcedure::setIOIntervalOf81(int ioIntervalOf81)
{
    this->m_iOIntervalOf81 = ioIntervalOf81;
}

int
ISiSProcedure::getBlockRetry()
{
    return this->m_blockRetry;
}

void
ISiSProcedure::setBlockRetry(int blockRetry)
{
    this->m_blockRetry = blockRetry;
}

int
ISiSProcedure::getAllRetry()
{
    return this->m_allRetry;
}

void
ISiSProcedure::setAllRetry(int allRetry)
{
    this->m_allRetry = allRetry;
}

int
ISiSProcedure::getRamSize()
{
    return this->m_ramSize;
}

void
ISiSProcedure::setRamSize(int ramSize)
{
    this->m_ramSize = ramSize;
}
