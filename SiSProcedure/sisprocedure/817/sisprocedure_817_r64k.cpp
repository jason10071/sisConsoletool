#include "sisprocedure_817_r64k.h"

#include "sislog.h"
#include "spexception/spexception.h"
#include "spexception/siscoreexception.h"
#include "addressprovider/addresstable/attributekeydefine.h"

#include "attributereader/817/attributereader_817_r64k.h"
#include "attributewriter/817/attributewriter_817_r64k.h"
#include "rombd/817/rombdarranger_817_r64k.h"

//#include "UnderlyingDevDefine.h"
//#include "UnderlyingDevKey.h"
//#include "CptFactoryFacade.h"

#include <stdio.h>
#include <string.h>

using namespace std;
using namespace SiS::Procedure;
using namespace SiS;

#define EXCEPTION_TITLE "SiSProcedure_817_r64k Exception : "
#define TAG "SiSProcedure_817_r64k"

SiSProcedure_817_r64k::SiSProcedure_817_r64k() :
    ISiSProcedure()
{

}

SiSProcedure_817_r64k::~SiSProcedure_817_r64k()
{

}

IAttributeReader*
SiSProcedure_817_r64k::generateAttributeReader(BinWrap* binWrap)
{
    if( binWrap == 0 )
    {
        if( m_sisDeviceIO == 0 )
        {
            std::string msg = EXCEPTION_TITLE;
            char errorMsg[1024] = "";
            sprintf(errorMsg, "generateAttributeReader() : no sisCore");
            msg.append(errorMsg);
            throw SPException( msg );
        }

        return new AttributeReader_817_r64k(m_sisDeviceIO);
    }
    else
    {
        return new AttributeReader_817_r64k(binWrap->getBuf(), binWrap->getSize());
    }
}

IAttributeWriter*
SiSProcedure_817_r64k::generateAttributeWriter(BinWrap* binWrap)
{
    if( binWrap == 0 )
    {
        if( m_sisDeviceIO == 0 )
        {
            std::string msg = EXCEPTION_TITLE;
            char errorMsg[1024] = "";
            sprintf(errorMsg, "generateAttributeWriter() : no sisCore");
            msg.append(errorMsg);
            throw SPException( msg );
        }

        return new AttributeWriter_817_r64k(m_sisDeviceIO);
    }
    else
    {
        return new AttributeWriter_817_r64k(binWrap->getBuf(), binWrap->getSize());
    }
}

IRomBDArranger*
SiSProcedure_817_r64k::generateRomBDArranger(IRomSegmentBD::BurnDump burnDump, IRomBDArranger::RomBDPolicy romBDPolicy)
{
    return new RomBDArranger_817_r64k(m_sisDeviceIO, burnDump, romBDPolicy);
}

void
SiSProcedure_817_r64k::disableCtlReportToOs()
{
    if( m_sisDeviceIO == 0 )
    {
        std::string msg = EXCEPTION_TITLE;
        char errorMsg[1024] = "";
        sprintf(errorMsg, "disableCtlReportToOs() : no sisDeviceIO");
        msg.append(errorMsg);
        throw SPException( msg );
    }

    int ret = -1;
    try
    {
        SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "CTL_REPORT_TO_OS, disable" );
        //ret = m_sisCore->exe<int>("CTL_REPORT_TO_OS", NO("IsReport"));
        ret = m_sisDeviceIO->ctlReportToOs( false );
        SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "CTL_REPORT_TO_OS, disable : (ret=%d)", ret );
    }
    SISCORE_EXCEPTION_SP_HANDLE

    if( ret != 1 )
    {
        std::string msg = EXCEPTION_TITLE;
        char errorMsg[1024] = "";
        sprintf(errorMsg, "CTL_REPORT_TO_OS, disable : FAIL (ret=%d)", ret);
        msg.append(errorMsg);
        throw SPException( msg );
    }
}

void
SiSProcedure_817_r64k::enableCtlReportToOs()
{
    if( m_sisDeviceIO == 0 )
    {
        std::string msg = EXCEPTION_TITLE;
        char errorMsg[1024] = "";
        sprintf(errorMsg, "enableCtlReportToOs() : no sisDeviceIO");
        msg.append(errorMsg);
        throw SPException( msg );
    }

    int ret = -1;
    try
    {
        SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "CTL_REPORT_TO_OS, enable" );
        //ret = m_sisCore->exe<int>("CTL_REPORT_TO_OS", YES("IsReport"));
        ret = m_sisDeviceIO->ctlReportToOs( true );
        SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "CTL_REPORT_TO_OS, enable : (ret=%d)", ret );
    }
    SISCORE_EXCEPTION_SP_HANDLE

    if( ret != 1 )
    {
        std::string msg = EXCEPTION_TITLE;
        char errorMsg[1024] = "";
        sprintf(errorMsg, "CTL_REPORT_TO_OS, enable : FAIL (ret=%d)", ret);
        msg.append(errorMsg);
        throw SPException( msg );
    }
}

void
SiSProcedure_817_r64k::softReset()
{
    if( m_sisDeviceIO == 0 )
    {
        std::string msg = EXCEPTION_TITLE;
        char errorMsg[1024] = "";
        sprintf(errorMsg, "softReset() : no sisCore");
        msg.append(errorMsg);
        throw SPException( msg );
    }

    int ret = -1;
//    try
//    {
//        SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "CTL_SOFT_RESET" );
//        ret = m_sisCore->exe<int>("CTL_SOFT_RESET");
//        SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "CTL_SOFT_RESET : (ret=%d)", ret );
//    }
//    SISCORE_EXCEPTION_SP_HANDLE

    if( ret != 1 )
    {
        std::string msg = EXCEPTION_TITLE;
        char errorMsg[1024] = "";
        sprintf(errorMsg, "CTL_SOFT_RESET : FAIL (ret=%d)", ret);
        msg.append(errorMsg);
        throw SPException( msg );
    }
}

void
SiSProcedure_817_r64k::reCalibration(int waitResetSec)
{
    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "SiSProcedure_817_r64k::reCalibration()\n" );

    if( m_sisDeviceIO == 0 )
    {
        std::string msg = EXCEPTION_TITLE;
        char errorMsg[1024] = "";
        sprintf(errorMsg, "reCalibration() : no sisCore");
        msg.append(errorMsg);
        throw SPException( msg );
    }

    int ret = -1;
//    try
//    {
//        SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "CTL_RECALIBRATION" );
//        ret = m_sisCore->exe<int>("CTL_RECALIBRATION");
//        SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "CTL_RECALIBRATION : (ret=%d)", ret );
//    }
//    SISCORE_EXCEPTION_SP_HANDLE

    if( ret != 1 )
    {
        std::string msg = EXCEPTION_TITLE;
        char errorMsg[1024] = "";
        sprintf(errorMsg, "CTL_RECALIBRATION : FAIL (ret=%d)", ret);
        msg.append(errorMsg);
        throw SPException( msg );
    }

    /* re-open device */
    std::string deviceNameOpened = m_sisDeviceIO->deviceNameOpened();
    reOpenDevice(deviceNameOpened, waitResetSec);
}

bool
SiSProcedure_817_r64k::readBroken(ReferenceSource rs, int chipIndex)
{
    SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "readBroken() : rs=%d, chipIndex=%d", (int) rs, chipIndex );

    /* prepare */
    string keyFaBootFlag;
    IAttributeReader* attributeReader = 0;

    switch (rs) {
    case RS_XRAM:
        keyFaBootFlag = KEY_FA_BOOT_FLAG_XRAM;
        attributeReader = generateAttributeReader();
        break;
    case RS_BIN:
        keyFaBootFlag = KEY_FA_BOOT_FLAG_ROM;
        attributeReader = generateAttributeReader( getBinWrap(chipIndex) );
        break;
    case RS_ROM:
    default:
        std::string msg = EXCEPTION_TITLE;
        char errorMsg[1024] = "";
        sprintf(errorMsg, "readBroken() : not support ReferenceSource %d (%s)", (int) rs, ISiSProcedure::getRSStr(rs).c_str());
        msg.append(errorMsg);
        throw SPException( msg );
        break;
    }
    attributeReader->setCtlReportToOs( this->m_ctlReportToOsFlag );
    attributeReader->setChipIndex(chipIndex);

    /* read BootFlag */
    unsigned char* dataBuf = 0;
    int dataSize = 0;
    attributeReader->readAttribute( keyFaBootFlag, dataBuf, dataSize );

    /* check if address table error */
    IAttributeReader::checkDataSize(EXPECT_SIZE_FA_BOOT_FLAG_XRAM, dataSize);

    /* judge */
    bool bootFlag = true;

    if( dataBuf[0] == 0x50 &&
            dataBuf[1] == 0x38 &&
            dataBuf[2] == 0x31 &&
            dataBuf[3] == 0x30 )
    {
        bootFlag = false;
    }
    else
    {
        bootFlag = true;
    }

    /* print data */
    SIS_LOG_D(SiSLog::getOwnerSiS(), TAG,
              "read %s (%s) Broken (BootFlag) : %s\n",
              ISiSProcedure::getCIStr(chipIndex).c_str(),
              ISiSProcedure::getRSStr(rs).c_str(),
              bootFlag == true ? "Yes" : "No" );

    delete [] dataBuf;
    delete attributeReader;
    return bootFlag;
}

FWSizeType
SiSProcedure_817_r64k::readFWSizeType(ReferenceSource rs, int chipIndex, bool isBroken)
{   
    SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "readFWSizeType() : rs=%d, chipIndex=%d, isBroken=%s", (int) rs, chipIndex, isBroken == true ? "true" : "false" );

    /* prepare */
    string keyFa0x0238;
    string keyFa0xc04c;
    IAttributeReader* attributeReader = 0;

    switch (rs) {
    case RS_XRAM:
        keyFa0x0238 = KEY_FA_0X0238_XRAM;
        keyFa0xc04c = KEY_FA_0XC04C_XRAM;
        attributeReader = generateAttributeReader();
        break;
    case RS_BIN:
        keyFa0x0238 = KEY_FA_0X0238_ROM;
        keyFa0xc04c = KEY_FA_0XC04C_ROM;
        attributeReader = generateAttributeReader( getBinWrap(chipIndex) );
        break;
    case RS_ROM:
    default:
        std::string msg = EXCEPTION_TITLE;
        char errorMsg[1024] = "";
        sprintf(errorMsg, "readFWSizeType() : not support ReferenceSource %d (%s)", (int) rs, ISiSProcedure::getRSStr(rs).c_str());
        msg.append(errorMsg);
        throw SPException( msg );
        break;
    }
    attributeReader->setCtlReportToOs( this->m_ctlReportToOsFlag );
    attributeReader->setChipIndex(chipIndex);

    /* read FWSizeType */
    /* 0X0238Xram */
    unsigned char* dataBuf0x0238Xram = 0;
    int dataSize0x0238Xram = 0;
    attributeReader->readAttribute( keyFa0x0238, dataBuf0x0238Xram, dataSize0x0238Xram );

    /* check if address table error */
    IAttributeReader::checkDataSize(EXPECT_SIZE_FA_0X0238_XRAM, dataSize0x0238Xram);

    /* 0XC04CXram */
    unsigned char* dataBuf0xc04cXram = 0;
    int dataSize0xc04cXram = 0;
    attributeReader->readAttribute( keyFa0xc04c, dataBuf0xc04cXram, dataSize0xc04cXram );

    /* check if address table error */
    IAttributeReader::checkDataSize(EXPECT_SIZE_FA_0XC04C_XRAM, dataSize0xc04cXram);

    /* judge */
    FWSizeType fwSizeType = FST_INVALID;

    if( dataBuf0x0238Xram[0] == 0x01 )
    {
        fwSizeType = FST_817_R64K;
    }
    else if( dataBuf0x0238Xram[0] == 0x02 )
    {
        fwSizeType = FST_817_R128K;
    }
    else
    {
        if( isBroken )
        {
            fwSizeType = FST_817_R64K;
        }
        if( dataBuf0xc04cXram[0] == 0x01 )
        {
            fwSizeType = FST_817_R64K_M48K;
        }
        else if( dataBuf0xc04cXram[0] == 0x02 )
        {
            fwSizeType = FST_817_R128K;
        }
        else
        {
            fwSizeType = FST_817_R64K;
        }
    }

    /* print data */
    SIS_LOG_D(SiSLog::getOwnerSiS(), TAG,
              "read %s (%s) FwSizeType : %s\n",
              ISiSProcedure::getCIStr(chipIndex).c_str(),
              ISiSProcedure::getRSStr(rs).c_str(),
              ISiSProcedure::getFSTStr(fwSizeType).c_str());

    delete [] dataBuf0x0238Xram;
    delete [] dataBuf0xc04cXram;
    delete attributeReader;
    return fwSizeType;
}

unsigned char
SiSProcedure_817_r64k::readInterfaceID(ReferenceSource rs, int chipIndex)
{
    SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "readInterfaceID() : rs=%d, chipIndex=%d", (int) rs, chipIndex);

    /* prepare */
    string keyFaInterfaceID;
    IAttributeReader* attributeReader = 0;

    switch (rs) {
    case RS_XRAM:
        keyFaInterfaceID = KEY_FA_INTERFACE_ID_XRAM;
        attributeReader = generateAttributeReader();
        break;
    case RS_BIN:
        keyFaInterfaceID = KEY_FA_INTERFACE_ID_ROM;
        attributeReader = generateAttributeReader( getBinWrap(chipIndex) );
        break;
    case RS_ROM:
    default:
        std::string msg = EXCEPTION_TITLE;
        char errorMsg[1024] = "";
        sprintf(errorMsg, "readInterfaceID() : not support ReferenceSource %d (%s)", (int) rs, ISiSProcedure::getRSStr(rs).c_str());
        msg.append(errorMsg);
        throw SPException( msg );
        break;
    }
    attributeReader->setCtlReportToOs( this->m_ctlReportToOsFlag );
    attributeReader->setChipIndex(chipIndex);

    /* read InterfaceID */
    unsigned char* dataBuf = 0;
    int dataSize = 0;
    attributeReader->readAttribute( keyFaInterfaceID, dataBuf, dataSize );

    /* check if address table error */
    IAttributeReader::checkDataSize(EXPECT_SIZE_FA_INTERFACE_ID_XRAM, dataSize);

    /* judge */
    unsigned char interfaceID = 0x0;

    interfaceID = dataBuf[0];

    /* print data */
    SIS_LOG_D(SiSLog::getOwnerSiS(), TAG,
              "read %s (%s) InterfaceID : 0x%02x\n",
              ISiSProcedure::getCIStr(chipIndex).c_str(),
              ISiSProcedure::getRSStr(rs).c_str(),
              interfaceID);

    delete [] dataBuf;
    delete attributeReader;
    return interfaceID;
}

int
SiSProcedure_817_r64k::readSlaveNum(ReferenceSource rs)
{
    SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "readSlaveNum() : rs=%d", (int) rs);

    /* prepare */
    string keyFaMultiSlaveNumber;
    IAttributeReader* attributeReader = 0;

    switch (rs) {
    case RS_XRAM:
        keyFaMultiSlaveNumber = KEY_FA_MULTI_SLAVE_NUMBER_XRAM;
        attributeReader = generateAttributeReader();
        break;
    case RS_BIN:
        keyFaMultiSlaveNumber = KEY_FA_MULTI_SLAVE_NUMBER_ROM;
        attributeReader = generateAttributeReader( getBinWrap(CI_MASTER) );
        break;
    case RS_ROM:
    default:
        std::string msg = EXCEPTION_TITLE;
        char errorMsg[1024] = "";
        sprintf(errorMsg, "readSlaveNum() : not support ReferenceSource %d (%s)", (int) rs, ISiSProcedure::getRSStr(rs).c_str());
        msg.append(errorMsg);
        throw SPException( msg );
        break;
    }
    attributeReader->setCtlReportToOs( this->m_ctlReportToOsFlag );
    attributeReader->setChipIndex(CI_MASTER);

    /* read SlaveNum */
    unsigned char* dataBuf = 0;
    int dataSize = 0;
    attributeReader->readAttribute( keyFaMultiSlaveNumber, dataBuf, dataSize );

    /* check if address table error */
    IAttributeReader::checkDataSize(EXPECT_SIZE_FA_MULTI_SLAVE_NUMBER_XRAM, dataSize);

    /* judge */
    int slaveNum = 0;

    slaveNum = dataBuf[0];

    /* print data */
    SIS_LOG_D(SiSLog::getOwnerSiS(), TAG,
              "read %s (%s) SlaveNum : %d\n",
              ISiSProcedure::getCIStr(CI_MASTER).c_str(),
              ISiSProcedure::getRSStr(rs).c_str(),
              slaveNum);

    delete [] dataBuf;
    delete attributeReader;
    return slaveNum;
}

unsigned char
SiSProcedure_817_r64k::readMultiChipSelectiveID(ReferenceSource rs, int chipIndex)
{
    SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "readMultiChipSelectiveID() : rs=%d, chipIndex=%d", (int) rs, chipIndex);

    /* prepare */
    string keyFaMultiChipSelectiveID;
    IAttributeReader* attributeReader = 0;

    switch (rs) {
    case RS_XRAM:
        keyFaMultiChipSelectiveID = KEY_FA_MULTI_CHIP_SELECTIVE_ID_XRAM;
        attributeReader = generateAttributeReader();
        break;
    case RS_BIN:
        keyFaMultiChipSelectiveID = KEY_FA_MULTI_CHIP_SELECTIVE_ID_ROM;
        attributeReader = generateAttributeReader( getBinWrap(chipIndex) );
        break;
    case RS_ROM:
    default:
        std::string msg = EXCEPTION_TITLE;
        char errorMsg[1024] = "";
        sprintf(errorMsg, "readMultiChipSelectiveID() : not support ReferenceSource %d (%s)", (int) rs, ISiSProcedure::getRSStr(rs).c_str());
        msg.append(errorMsg);
        throw SPException( msg );
        break;
    }
    attributeReader->setCtlReportToOs( this->m_ctlReportToOsFlag );
    attributeReader->setChipIndex(chipIndex);

    /* read MultiChipSelectiveID */
    unsigned char* dataBuf = 0;
    int dataSize = 0;
    attributeReader->readAttribute( keyFaMultiChipSelectiveID, dataBuf, dataSize );

    /* check if address table error */
    IAttributeReader::checkDataSize(EXPECT_SIZE_FA_MULTI_CHIP_SELECTIVE_ID_XRAM, dataSize);

    /* judge */
    unsigned char multiChipSelectiveID = 0x0;

    multiChipSelectiveID = dataBuf[0];

    /* print data */
    SIS_LOG_D(SiSLog::getOwnerSiS(), TAG,
              "read %s (%s) MultiChipSelectiveID : 0x%02x\n",
              ISiSProcedure::getCIStr(chipIndex).c_str(),
              ISiSProcedure::getRSStr(rs).c_str(),
              multiChipSelectiveID);

    delete [] dataBuf;
    delete attributeReader;
    return multiChipSelectiveID;
}

bool
SiSProcedure_817_r64k::readCalibrationFlag(ReferenceSource rs, int chipIndex)
{
    SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "readCalibrationFlag() : rs=%d, chipIndex=%d", (int) rs, chipIndex);

    /* prepare */
    string keyFaCalibrationFlag;
    IAttributeReader* attributeReader = 0;

    switch (rs) {
    case RS_XRAM:
        keyFaCalibrationFlag = KEY_FA_CALIBRATION_FLAG_XRAM;
        attributeReader = generateAttributeReader();
        break;
    case RS_BIN:
        keyFaCalibrationFlag = KEY_FA_CALIBRATION_FLAG_ROM;
        attributeReader = generateAttributeReader( getBinWrap(chipIndex) );
        break;
    case RS_ROM:
    default:
        std::string msg = EXCEPTION_TITLE;
        char errorMsg[1024] = "";
        sprintf(errorMsg, "readCalibrationFlag() : not support ReferenceSource %d (%s)", (int) rs, ISiSProcedure::getRSStr(rs).c_str());
        msg.append(errorMsg);
        throw SPException( msg );
        break;
    }
    attributeReader->setCtlReportToOs( this->m_ctlReportToOsFlag );
    attributeReader->setChipIndex(chipIndex);

    /* read Calibration Flag */
    unsigned char* dataBuf = 0;
    int dataSize = 0;
    attributeReader->readAttribute( keyFaCalibrationFlag, dataBuf, dataSize );

    /* check if address table error */
    IAttributeReader::checkDataSize(EXPECT_SIZE_FA_CALIBRATION_FLAG_XRAM, dataSize);

    /* judge */
    bool calibrationFlag = false;

    if( dataBuf[0] == 0x50 &&
            dataBuf[1] == 0x38 &&
            dataBuf[2] == 0x31 &&
            dataBuf[3] == 0x30 )
    {
        calibrationFlag = true;
    }
    else
    {
        calibrationFlag = false;
    }

    /* print data */
    SIS_LOG_D(SiSLog::getOwnerSiS(), TAG,
              "read %s (%s) calibrationFlag : %s\n",
              ISiSProcedure::getCIStr(chipIndex).c_str(),
              ISiSProcedure::getRSStr(rs).c_str(),
              calibrationFlag == true ? "Yes" : "No");

    delete [] dataBuf;
    delete attributeReader;
    return calibrationFlag;
}

SerialData*
SiSProcedure_817_r64k::readFirmwareID(ReferenceSource rs, int chipIndex)
{
    SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "readFirmwareID() : rs=%d, chipIndex=%d", (int) rs, chipIndex);

    /* prepare */
    string keyFaFirmwareID;
    IAttributeReader* attributeReader = 0;

    switch (rs) {
    case RS_XRAM:
        keyFaFirmwareID = KEY_FA_FIRMWARE_ID_XRAM;
        attributeReader = generateAttributeReader();
        break;
    case RS_BIN:
        keyFaFirmwareID = KEY_FA_FIRMWARE_ID_ROM;
        attributeReader = generateAttributeReader( getBinWrap(chipIndex) );
        break;
    case RS_ROM:
    default:
        std::string msg = EXCEPTION_TITLE;
        char errorMsg[1024] = "";
        sprintf(errorMsg, "readFirmwareID() : not support ReferenceSource %d (%s)", (int) rs, ISiSProcedure::getRSStr(rs).c_str());
        msg.append(errorMsg);
        throw SPException( msg );
        break;
    }
    attributeReader->setCtlReportToOs( this->m_ctlReportToOsFlag );
    attributeReader->setChipIndex(chipIndex);

    /* read FirmwareID */
    unsigned char* dataBuf = 0;
    int dataSize = 0;
    attributeReader->readAttribute( keyFaFirmwareID, dataBuf, dataSize );

    /* check if address table error */
//    IAttributeReader::checkDataSize(EXPECT_SIZE_FA_FIRMWARE_ID_XRAM, dataSize);
//    IAttributeReader::checkDataSize(EXPECT_SIZE_FA_FIRMWARE_ID_XRAM_819, dataSize);

    /* judge */
    unsigned char* firmwareID = new unsigned char[dataSize];
    memset( firmwareID, 0x0, dataSize * sizeof(unsigned char) );

    for(int i = 0; i < dataSize; i++)
    {
        firmwareID[i] = dataBuf[i];
    }

    /* print data */
    if( SiSLog::getOwnerSiS()->isLOG_D() )
    {
        SIS_LOG_D(SiSLog::getOwnerSiS(), TAG,
                  "read %s (%s) FirmwareID : ",
                  ISiSProcedure::getCIStr(chipIndex).c_str(),
                  ISiSProcedure::getRSStr(rs).c_str());

        std::string dataStr;
        for(int i = 0; i < dataSize; i++)
        {
            char bData[10] = "";
            sprintf(bData, "%02x ", firmwareID[i]);
            dataStr.append(bData);

            if( i % 16 == 15 || i == dataSize - 1)
            {
                SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "%s", dataStr.c_str());
                dataStr = "";
            }
        }
    }

    SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "");
    delete [] dataBuf;
    delete attributeReader;
    return new SerialData(firmwareID, dataSize);
}

SerialData*
SiSProcedure_817_r64k::readBootloaderID(ReferenceSource rs, int chipIndex)
{
    SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "readBootloaderID() : rs=%d, chipIndex=%d", (int) rs, chipIndex);

    /* prepare */
    string keyFaBootloaderID;
    IAttributeReader* attributeReader = 0;

    switch (rs) {
    case RS_XRAM:
        keyFaBootloaderID = KEY_FA_BOOTLOADER_ID_XRAM;
        attributeReader = generateAttributeReader();
        break;
    case RS_BIN:
        keyFaBootloaderID = KEY_FA_BOOTLOADER_ID_ROM;
        attributeReader = generateAttributeReader( getBinWrap(chipIndex) );
        break;
    case RS_ROM:
    default:
        std::string msg = EXCEPTION_TITLE;
        char errorMsg[1024] = "";
        sprintf(errorMsg, "readBootloaderID() : not support ReferenceSource %d (%s)", (int) rs, ISiSProcedure::getRSStr(rs).c_str());
        msg.append(errorMsg);
        throw SPException( msg );
        break;
    }
    attributeReader->setCtlReportToOs( this->m_ctlReportToOsFlag );
    attributeReader->setChipIndex(chipIndex);

    /* read BootloaderID */
    unsigned char* dataBuf = 0;
    int dataSize = 0;
    attributeReader->readAttribute( keyFaBootloaderID, dataBuf, dataSize );

    /* check if address table error */
    IAttributeReader::checkDataSize(EXPECT_SIZE_FA_BOOTLOADER_ID_XRAM, dataSize);

    /* judge */
    unsigned char* bootloaderID = new unsigned char[dataSize];
    memset( bootloaderID, 0x0, dataSize * sizeof(unsigned char) );

    for(int i = 0; i < dataSize; i++)
    {
        bootloaderID[i] = dataBuf[i];
    }

    /* print data */
    if( SiSLog::getOwnerSiS()->isLOG_D() )
    {
        SIS_LOG_D(SiSLog::getOwnerSiS(), TAG,
                  "read %s (%s) BootloaderID : ",
                  ISiSProcedure::getCIStr(chipIndex).c_str(),
                  ISiSProcedure::getRSStr(rs).c_str());

        std::string dataStr;
        for(int i = 0; i < dataSize; i++)
        {
            char bData[10] = "";
            sprintf(bData, "%02x ", bootloaderID[i]);
            dataStr.append(bData);

            if( i % 16 == 15 || i == dataSize - 1)
            {
                SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "%s", dataStr.c_str());
                dataStr = "";
            }
        }
    }

    SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "");
    delete [] dataBuf;
    delete attributeReader;
    return new SerialData(bootloaderID, dataSize);
}

SerialData*
SiSProcedure_817_r64k::readBootCodeCRC(ReferenceSource rs, int chipIndex)
{
    SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "readBootCodeCRC() : rs=%d, chipIndex=%d", (int) rs, chipIndex);

    /* prepare */
    string keyFaBootCodeCRC;
    IAttributeReader* attributeReader = 0;

    switch (rs) {
    case RS_XRAM:
        keyFaBootCodeCRC = KEY_FA_BOOT_CODE_CRC_XRAM;
        attributeReader = generateAttributeReader();
        break;
    case RS_BIN:
        keyFaBootCodeCRC = KEY_FA_BOOT_CODE_CRC_ROM;
        attributeReader = generateAttributeReader( getBinWrap(chipIndex) );
        break;
    case RS_ROM:
    default:
        std::string msg = EXCEPTION_TITLE;
        char errorMsg[1024] = "";
        sprintf(errorMsg, "readBootCodeCRC() : not support ReferenceSource %d (%s)", (int) rs, ISiSProcedure::getRSStr(rs).c_str());
        msg.append(errorMsg);
        throw SPException( msg );
        break;
    }
    attributeReader->setCtlReportToOs( this->m_ctlReportToOsFlag );
    attributeReader->setChipIndex(chipIndex);

    /* read BootCodeCRC */
    unsigned char* dataBuf = 0;
    int dataSize = 0;
    attributeReader->readAttribute( keyFaBootCodeCRC, dataBuf, dataSize );

    /* check if address table error */
    IAttributeReader::checkDataSize(EXPECT_SIZE_FA_BOOT_CODE_CRC_XRAM, dataSize);

    /* judge */
    unsigned char* bootCodeCRC = new unsigned char[dataSize];
    memset( bootCodeCRC, 0x0, dataSize * sizeof(unsigned char) );

    for(int i = 0; i < dataSize; i++)
    {
        bootCodeCRC[i] = dataBuf[i];
    }

    /* print data */
    if( SiSLog::getOwnerSiS()->isLOG_D() )
    {
        SIS_LOG_D(SiSLog::getOwnerSiS(), TAG,
                  "read %s (%s) BootCodeCRC : ",
                  ISiSProcedure::getCIStr(chipIndex).c_str(),
                  ISiSProcedure::getRSStr(rs).c_str());

        std::string dataStr;
        for(int i = 0; i < dataSize; i++)
        {
            char bData[10] = "";
            sprintf(bData, "%02x ", bootCodeCRC[i]);
            dataStr.append(bData);

            if( i % 16 == 15 || i == dataSize - 1)
            {
                SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "%s", dataStr.c_str());
                dataStr = "";
            }
        }
    }

    SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "");
    delete [] dataBuf;
    delete attributeReader;
    return new SerialData(bootCodeCRC, dataSize);
}

SerialData*
SiSProcedure_817_r64k::readMainCodeCRC(ReferenceSource rs, int chipIndex)
{
    SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "readMainCodeCRC() : rs=%d, chipIndex=%d", (int) rs, chipIndex);

    /* prepare */
    string keyFaMainCodeCRC;
    IAttributeReader* attributeReader = 0;

    switch (rs) {
    case RS_XRAM:
        keyFaMainCodeCRC = KEY_FA_MAIN_CODE_CRC_XRAM;
        attributeReader = generateAttributeReader();
        break;
    case RS_BIN:
        keyFaMainCodeCRC = KEY_FA_MAIN_CODE_CRC_ROM;
        attributeReader = generateAttributeReader( getBinWrap(chipIndex) );
        break;
    case RS_ROM:
    default:
        std::string msg = EXCEPTION_TITLE;
        char errorMsg[1024] = "";
        sprintf(errorMsg, "readMainCodeCRC() : not support ReferenceSource %d (%s)", (int) rs, ISiSProcedure::getRSStr(rs).c_str());
        msg.append(errorMsg);
        throw SPException( msg );
        break;
    }
    attributeReader->setCtlReportToOs( this->m_ctlReportToOsFlag );
    attributeReader->setChipIndex(chipIndex);

    /* read MainCodeCRC */
    unsigned char* dataBuf = 0;
    int dataSize = 0;
    attributeReader->readAttribute( keyFaMainCodeCRC, dataBuf, dataSize );

    /* check if address table error */
    IAttributeReader::checkDataSize(EXPECT_SIZE_FA_MAIN_CODE_CRC_XRAM, dataSize);

    /* judge */
    unsigned char* mainCodeCRC = new unsigned char[dataSize];
    memset( mainCodeCRC, 0x0, dataSize * sizeof(unsigned char) );

    for(int i = 0; i < dataSize; i++)
    {
        mainCodeCRC[i] = dataBuf[i];
    }

    /* print data */
    if( SiSLog::getOwnerSiS()->isLOG_D() )
    {
        SIS_LOG_D(SiSLog::getOwnerSiS(), TAG,
                  "read %s (%s) MainCodeCRC : ",
                  ISiSProcedure::getCIStr(chipIndex).c_str(),
                  ISiSProcedure::getRSStr(rs).c_str());

        std::string dataStr;
        for(int i = 0; i < dataSize; i++)
        {
            char bData[10] = "";
            sprintf(bData, "%02x ", mainCodeCRC[i]);
            dataStr.append(bData);

            if( i % 16 == 15 || i == dataSize - 1)
            {
                SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "%s", dataStr.c_str());
                dataStr = "";
            }
        }
    }

    SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "");
    delete [] dataBuf;
    delete attributeReader;
    return new SerialData(mainCodeCRC, dataSize);
}

SerialData*
SiSProcedure_817_r64k::readLastID(ReferenceSource rs, int chipIndex)
{
    SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "readLastID() : rs=%d, chipIndex=%d", (int) rs, chipIndex);

    /* prepare */
    string keyFaLastID;
    IAttributeReader* attributeReader = 0;

    switch (rs) {
    case RS_XRAM:
        keyFaLastID = KEY_FA_LAST_ID_XRAM;
        attributeReader = generateAttributeReader();
        break;
    case RS_BIN:
        keyFaLastID = KEY_FA_LAST_ID_ROM;
        attributeReader = generateAttributeReader( getBinWrap(chipIndex) );
        break;
    case RS_ROM:
    default:
        std::string msg = EXCEPTION_TITLE;
        char errorMsg[1024] = "";
        sprintf(errorMsg, "readLastID() : not support ReferenceSource %d (%s)", (int) rs, ISiSProcedure::getRSStr(rs).c_str());
        msg.append(errorMsg);
        throw SPException( msg );
        break;
    }
    attributeReader->setCtlReportToOs( this->m_ctlReportToOsFlag );
    attributeReader->setChipIndex(chipIndex);

    /* read LastID */
    unsigned char* dataBuf = 0;
    int dataSize = 0;
    attributeReader->readAttribute( keyFaLastID, dataBuf, dataSize );

    /* check if address table error */
//    IAttributeReader::checkDataSize(EXPECT_SIZE_FA_LAST_ID_XRAM, dataSize);
//    IAttributeReader::checkDataSize(EXPECT_SIZE_FA_LAST_ID_XRAM_819, dataSize);

    /* judge */
    unsigned char* lastID = new unsigned char[dataSize];
    memset( lastID, 0x0, dataSize * sizeof(unsigned char) );

    for(int i = 0; i < dataSize; i++)
    {
        lastID[i] = dataBuf[i];
    }

    /* print data */
    if( SiSLog::getOwnerSiS()->isLOG_D() )
    {
        SIS_LOG_D(SiSLog::getOwnerSiS(), TAG,
                  "read %s (%s) LastID : ",
                  ISiSProcedure::getCIStr(chipIndex).c_str(),
                  ISiSProcedure::getRSStr(rs).c_str());

        std::string dataStr;
        for(int i = 0; i < dataSize; i++)
        {
            char bData[10] = "";
            sprintf(bData, "%02x ", lastID[i]);
            dataStr.append(bData);

            if( i % 16 == 15 || i == dataSize - 1)
            {
                SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "%s", dataStr.c_str());
                dataStr = "";
            }
        }
    }

    SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "");
    delete [] dataBuf;
    delete attributeReader;
    return new SerialData(lastID, dataSize);
}

SerialData*
SiSProcedure_817_r64k::readLastTime(ReferenceSource rs, int chipIndex)
{
    SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "readLastTime() : rs=%d, chipIndex=%d", (int) rs, chipIndex);

    /* prepare */
    string keyFaLastTime;
    IAttributeReader* attributeReader = 0;

    switch (rs) {
    case RS_XRAM:
        keyFaLastTime = KEY_FA_LAST_TIME_XRAM;
        attributeReader = generateAttributeReader();
        break;
    case RS_BIN:
        keyFaLastTime = KEY_FA_LAST_TIME_ROM;
        attributeReader = generateAttributeReader( getBinWrap(chipIndex) );
        break;
    case RS_ROM:
    default:
        std::string msg = EXCEPTION_TITLE;
        char errorMsg[1024] = "";
        sprintf(errorMsg, "readLastTime() : not support ReferenceSource %d (%s)", (int) rs, ISiSProcedure::getRSStr(rs).c_str());
        msg.append(errorMsg);
        throw SPException( msg );
        break;
    }
    attributeReader->setCtlReportToOs( this->m_ctlReportToOsFlag );
    attributeReader->setChipIndex(chipIndex);

    /* read LastTime */
    unsigned char* dataBuf = 0;
    int dataSize = 0;
    attributeReader->readAttribute( keyFaLastTime, dataBuf, dataSize );

    /* check if address table error */
    IAttributeReader::checkDataSize(EXPECT_SIZE_FA_LAST_TIME_XRAM, dataSize);

    /* judge */
    unsigned char* lastTime = new unsigned char[dataSize];
    memset( lastTime, 0x0, dataSize * sizeof(unsigned char) );

    for(int i = 0; i < dataSize; i++)
    {
        lastTime[i] = dataBuf[i];
    }

    /* print data */
    if( SiSLog::getOwnerSiS()->isLOG_D() )
    {
        SIS_LOG_D(SiSLog::getOwnerSiS(), TAG,
                  "read %s (%s) LastTime : ",
                  ISiSProcedure::getCIStr(chipIndex).c_str(),
                  ISiSProcedure::getRSStr(rs).c_str());

        std::string dataStr;
        for(int i = 0; i < dataSize; i++)
        {
            char bData[10] = "";
            sprintf(bData, "%02x ", lastTime[i]);
            dataStr.append(bData);

            if( i % 16 == 15 || i == dataSize - 1)
            {
                SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "%s", dataStr.c_str());
                dataStr = "";
            }
        }
    }

    SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "");
    delete [] dataBuf;
    delete attributeReader;
    return new SerialData(lastTime, dataSize);
}

SerialData*
SiSProcedure_817_r64k::readPriorLastID(ReferenceSource rs, int chipIndex)
{
    SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "readPriorLastID() : rs=%d, chipIndex=%d", (int) rs, chipIndex);

    /* prepare */
    string keyFaPriorLastID;
    IAttributeReader* attributeReader = 0;

    switch (rs) {
    case RS_XRAM:
        keyFaPriorLastID = KEY_FA_PRIOR_LAST_ID_XRAM;
        attributeReader = generateAttributeReader();
        break;
    case RS_BIN:
        keyFaPriorLastID = KEY_FA_PRIOR_LAST_ID_ROM;
        attributeReader = generateAttributeReader( getBinWrap(chipIndex) );
        break;
    case RS_ROM:
    default:
        std::string msg = EXCEPTION_TITLE;
        char errorMsg[1024] = "";
        sprintf(errorMsg, "readPriorLastID() : not support ReferenceSource %d (%s)", (int) rs, ISiSProcedure::getRSStr(rs).c_str());
        msg.append(errorMsg);
        throw SPException( msg );
        break;
    }
    attributeReader->setCtlReportToOs( this->m_ctlReportToOsFlag );
    attributeReader->setChipIndex(chipIndex);

    /* read PriorLastID */
    unsigned char* dataBuf = 0;
    int dataSize = 0;
    attributeReader->readAttribute( keyFaPriorLastID, dataBuf, dataSize );

    /* check if address table error */
//    IAttributeReader::checkDataSize(EXPECT_SIZE_FA_PRIOR_LAST_ID_XRAM, dataSize);
//    IAttributeReader::checkDataSize(EXPECT_SIZE_FA_PRIOR_LAST_ID_XRAM_819, dataSize);

    /* judge */
    unsigned char* priorLastID = new unsigned char[dataSize];
    memset( priorLastID, 0x0, dataSize * sizeof(unsigned char) );

    for(int i = 0; i < dataSize; i++)
    {
        priorLastID[i] = dataBuf[i];
    }

    /* print data */
    if( SiSLog::getOwnerSiS()->isLOG_D() )
    {
        SIS_LOG_D(SiSLog::getOwnerSiS(), TAG,
                  "read %s (%s) PriorLastID : ",
                  ISiSProcedure::getCIStr(chipIndex).c_str(),
                  ISiSProcedure::getRSStr(rs).c_str());

        std::string dataStr;
        for(int i = 0; i < dataSize; i++)
        {
            char bData[10] = "";
            sprintf(bData, "%02x ", priorLastID[i]);
            dataStr.append(bData);

            if( i % 16 == 15 || i == dataSize - 1)
            {
                SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "%s", dataStr.c_str());
                dataStr = "";
            }
        }
    }

    SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "");
    delete [] dataBuf;
    delete attributeReader;
    return new SerialData(priorLastID, dataSize);
}

SerialData*
SiSProcedure_817_r64k::readPriorLastTime(ReferenceSource rs, int chipIndex)
{
    SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "readPriorLastTime() : rs=%d, chipIndex=%d", (int) rs, chipIndex);

    /* prepare */
    string keyFaPriorLastTime;
    IAttributeReader* attributeReader = 0;

    switch (rs) {
    case RS_XRAM:
        keyFaPriorLastTime = KEY_FA_PRIOR_LAST_TIME_XRAM;
        attributeReader = generateAttributeReader();
        break;
    case RS_BIN:
        keyFaPriorLastTime = KEY_FA_PRIOR_LAST_TIME_ROM;
        attributeReader = generateAttributeReader( getBinWrap(chipIndex) );
        break;
    case RS_ROM:
    default:
        std::string msg = EXCEPTION_TITLE;
        char errorMsg[1024] = "";
        sprintf(errorMsg, "readPriorLastTime() : not support ReferenceSource %d (%s)", (int) rs, ISiSProcedure::getRSStr(rs).c_str());
        msg.append(errorMsg);
        throw SPException( msg );
        break;
    }
    attributeReader->setCtlReportToOs( this->m_ctlReportToOsFlag );
    attributeReader->setChipIndex(chipIndex);

    /* read PriorLastTime */
    unsigned char* dataBuf = 0;
    int dataSize = 0;
    attributeReader->readAttribute( keyFaPriorLastTime, dataBuf, dataSize );

    /* check if address table error */
    IAttributeReader::checkDataSize(EXPECT_SIZE_FA_PRIOR_LAST_TIME_XRAM, dataSize);

    /* judge */
    unsigned char* priorLastTime = new unsigned char[dataSize];
    memset( priorLastTime, 0x0, dataSize * sizeof(unsigned char) );

    for(int i = 0; i < dataSize; i++)
    {
        priorLastTime[i] = dataBuf[i];
    }

    /* print data */
    if( SiSLog::getOwnerSiS()->isLOG_D() )
    {
        SIS_LOG_D(SiSLog::getOwnerSiS(), TAG,
                  "read %s (%s) PriorLastTime : ",
                  ISiSProcedure::getCIStr(chipIndex).c_str(),
                  ISiSProcedure::getRSStr(rs).c_str());

        std::string dataStr;
        for(int i = 0; i < dataSize; i++)
        {
            char bData[10] = "";
            sprintf(bData, "%02x ", priorLastTime[i]);
            dataStr.append(bData);

            if( i % 16 == 15 || i == dataSize - 1)
            {
                SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "%s", dataStr.c_str());
                dataStr = "";
            }
        }
    }

    SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "");
    delete [] dataBuf;
    delete attributeReader;
    return new SerialData(priorLastTime, dataSize);
}

SerialData*
SiSProcedure_817_r64k::readProductID(ReferenceSource rs, int chipIndex)
{
    std::string msg = EXCEPTION_TITLE;
    char errorMsg[1024] = "";
    sprintf(errorMsg, "readProductID() : not support");
    msg.append(errorMsg);
    throw SPException( msg );

    return 0;
}

SerialData*
SiSProcedure_817_r64k::readTaskID(ReferenceSource rs, int chipIndex)
{
    SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "readTaskID() : rs=%d, chipIndex=%d", (int) rs, chipIndex);

    /* prepare */
    string keyFaTaskId;
    IAttributeReader* attributeReader = 0;

    switch (rs) {
    case RS_XRAM:
        keyFaTaskId = KEY_FA_TASK_ID_XRAM;
        attributeReader = generateAttributeReader();
        break;
    case RS_BIN:
        keyFaTaskId = KEY_FA_TASK_ID_ROM;
        attributeReader = generateAttributeReader( getBinWrap(chipIndex) );
        break;
    case RS_ROM:
    default:
        std::string msg = EXCEPTION_TITLE;
        char errorMsg[1024] = "";
        sprintf(errorMsg, "readTaskID() : not support ReferenceSource %d (%s)", (int) rs, ISiSProcedure::getRSStr(rs).c_str());
        msg.append(errorMsg);
        throw SPException( msg );
        break;
    }
    attributeReader->setCtlReportToOs( this->m_ctlReportToOsFlag );
    attributeReader->setChipIndex(chipIndex);

    /* read TaskID */
    unsigned char* dataBuf = 0;
    int dataSize = 0;
    attributeReader->readAttribute( keyFaTaskId, dataBuf, dataSize );

    /* check if address table error */
    IAttributeReader::checkDataSize(EXPECT_SIZE_FA_TASK_ID_XRAM, dataSize);

    /* judge */
    unsigned char* taskId = new unsigned char[dataSize];
    memset( taskId, 0x0, dataSize * sizeof(unsigned char) );

    for(int i = 0; i < dataSize; i++)
    {
        taskId[i] = dataBuf[i];
    }

    /* print data */
    if( SiSLog::getOwnerSiS()->isLOG_D() )
    {
        SIS_LOG_D(SiSLog::getOwnerSiS(), TAG,
                  "read %s (%s) TaskID : ",
                  ISiSProcedure::getCIStr(chipIndex).c_str(),
                  ISiSProcedure::getRSStr(rs).c_str());

        std::string dataStr;
        for(int i = 0; i < dataSize; i++)
        {
            char bData[10] = "";
            sprintf(bData, "%02x ", taskId[i]);
            dataStr.append(bData);

            if( i % 16 == 15 || i == dataSize - 1)
            {
                SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "%s", dataStr.c_str());
                dataStr = "";
            }
        }
    }

    SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "");
    delete [] dataBuf;
    delete attributeReader;
    return new SerialData(taskId, dataSize);
}

FwVersion
SiSProcedure_817_r64k::readFwVersion(ReferenceSource rs, int chipIndex)
{
    SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "readFwVersion() : rs=%d, chipIndex=%d", (int) rs, chipIndex);

    /* prepare */
    string keyFaFwVwesion;
    IAttributeReader* attributeReader = 0;

    switch (rs) {
    case RS_XRAM:
        keyFaFwVwesion = KEY_FA_FW_VERSION_XRAM;
        attributeReader = generateAttributeReader();
        break;
    case RS_BIN:
        keyFaFwVwesion = KEY_FA_FW_VERSION_ROM;
        attributeReader = generateAttributeReader( getBinWrap(chipIndex) );
        break;
    case RS_ROM:
    default:
        std::string msg = EXCEPTION_TITLE;
        char errorMsg[1024] = "";
        sprintf(errorMsg, "readFwVersion() : not support ReferenceSource %d (%s)", (int) rs, ISiSProcedure::getRSStr(rs).c_str());
        msg.append(errorMsg);
        throw SPException( msg );
        break;
    }
    attributeReader->setCtlReportToOs( this->m_ctlReportToOsFlag );
    attributeReader->setChipIndex(chipIndex);

    /* read FwVersion */
    unsigned char* dataBuf = 0;
    int dataSize = 0;
    attributeReader->readAttribute( keyFaFwVwesion, dataBuf, dataSize );

    /* check if address table error */
    IAttributeReader::checkDataSize(EXPECT_SIZE_FA_FW_VERSION_XRAM, dataSize);

    /* judge */
    FwVersion fwVersion = {0, 0};
    fwVersion.major = dataBuf[1] & 0xff;
    fwVersion.minor = dataBuf[0] & 0x0f;

    /* print data */
    if( SiSLog::getOwnerSiS()->isLOG_D() )
    {
        SIS_LOG_D(SiSLog::getOwnerSiS(), TAG,
                  "read %s (%s) FwVersion : %d.%d\n",
                  ISiSProcedure::getCIStr(chipIndex).c_str(),
                  ISiSProcedure::getRSStr(rs).c_str(),
                  fwVersion.major, fwVersion.minor);
    }

    delete [] dataBuf;
    delete attributeReader;
    return fwVersion;
}

FWSizeType
SiSProcedure_817_r64k::readRomFWSizeType(int chipIndex)
{
    SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "readRomFWSizeType() : chipIndex=%d", chipIndex);

    /* prepare */
    string keyFaMagic0x0200;
    string keyFaMagic0x10200;
    IAttributeReader* attributeReader = 0;

    keyFaMagic0x0200 = KEY_FA_MAGIC_0X0200_XRAM;
    keyFaMagic0x10200 = KEY_FA_MAGIC_0X10200_XRAM;
    attributeReader = generateAttributeReader();

    attributeReader->setCtlReportToOs( this->m_ctlReportToOsFlag );
    attributeReader->setChipIndex(chipIndex);

    /* read RomFWSizeType */
    /* Magic0X0200Xram */
    unsigned char* dataBufMagic0x0200Xram = 0;
    int dataSizeMagic0x0200Xram = 0;
    attributeReader->readAttribute( keyFaMagic0x0200, dataBufMagic0x0200Xram, dataSizeMagic0x0200Xram );

    /* check if address table error */
    IAttributeReader::checkDataSize(EXPECT_SIZE_FA_MAGIC_0X0200_XRAM, dataSizeMagic0x0200Xram);

    /* 0X10200Xram */
    unsigned char* dataBufMagic0x10200Xram = 0;
    int dataSizeMagic0x10200Xram = 0;
    attributeReader->readAttribute( keyFaMagic0x10200, dataBufMagic0x10200Xram, dataSizeMagic0x10200Xram );

    /* check if address table error */
    IAttributeReader::checkDataSize(EXPECT_SIZE_FA_MAGIC_0X10200_XRAM, dataSizeMagic0x10200Xram);

    /* judge */
    FWSizeType romFwSizeType = FST_INVALID;

    if( compareData(dataBufMagic0x0200Xram, dataSizeMagic0x0200Xram,
                    dataBufMagic0x10200Xram, dataSizeMagic0x10200Xram) )
    {
        romFwSizeType = FST_R64K;
    }
    else
    {
        romFwSizeType = FST_R128K;
    }

    /* print data */
    SIS_LOG_D(SiSLog::getOwnerSiS(), TAG,
              "read %s (%s) FwSizeType : %s\n",
              ISiSProcedure::getCIStr(chipIndex).c_str(),
              ISiSProcedure::getRSStr(RS_ROM).c_str(),
              ISiSProcedure::getFSTStr(romFwSizeType).c_str());

    delete [] dataBufMagic0x0200Xram;
    delete [] dataBufMagic0x10200Xram;
    delete attributeReader;
    return romFwSizeType;
}

SerialData*
SiSProcedure_817_r64k::readXramAttribute(std::string key, int chipIndex)
{
    SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "readXramAttribute() : key=%s, chipIndex=%d", key.c_str(), chipIndex);

    /* prepare */
    IAttributeReader* attributeReader = generateAttributeReader();
    attributeReader->setCtlReportToOs( this->m_ctlReportToOsFlag );
    attributeReader->setChipIndex(chipIndex);

    /* read */
    unsigned char* dataBuf = 0;
    int dataSize = 0;
    attributeReader->readAttribute( key, dataBuf, dataSize );

    /* return */
    SerialData* serialData = new SerialData(dataBuf, dataSize);
    serialData->setKeyword( key );
    serialData->setAddress( attributeReader->getAddressProvider()->getAddress(key) );
    delete attributeReader;
    return serialData;
}

SerialData*
SiSProcedure_817_r64k::readXramAddress(unsigned int address, int dataSize, int chipIndex)
{
    SIS_LOG_D(SiSLog::getOwnerSiS(), TAG,
              "readXramAddress() : address=0x%x, dataSize=%d, chipIndex=%d",
              address, dataSize, chipIndex);

    /* prepare */
    IAttributeReader* attributeReader = generateAttributeReader();
    attributeReader->setCtlReportToOs( this->m_ctlReportToOsFlag );
    attributeReader->setChipIndex(chipIndex);

    /* read */
    unsigned char* dataBuf = 0;
    attributeReader->readAddress(address, dataBuf, dataSize );

    /* return */
    SerialData* serialData = new SerialData(dataBuf, dataSize);

    std::string keyword = "";
    char msg[64] = "";
    sprintf(msg, "0x%x", address);
    keyword.append(msg);
    serialData->setKeyword( keyword );

    serialData->setAddress( address );
    delete attributeReader;
    return serialData;
}

SerialData*SiSProcedure_817_r64k::readNoneSiSCmdViaBridge(SerialData * readcommand, const size_t len, const int transitionID)
{
	std::string msg = EXCEPTION_TITLE;
	char errorMsg[1024] = "";
	sprintf(errorMsg, "readNoneSiSCmdViaBridge() : 817 does not support this function");
	msg.append(errorMsg);
	throw SPException( msg );
}

void
SiSProcedure_817_r64k::writeUpdateMark(ReferenceSource rs, int chipIndex, SerialData* serialData)
{
    SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "writeUpdateMark() : rs=%d, chipIndex=%d", (int) rs, chipIndex);

    /* prepare */
    string keyFaUpdateMark;
    IAttributeWriter* attributeWriter = 0;

    switch (rs) {
//    case RS_XRAM:
//        keyFaLastID = KEY_FA_LAST_ID_XRAM;
//        attributeWriter = generateAttributeWriter();
//        break;
    case RS_BIN:
        keyFaUpdateMark = KEY_FA_UPDATE_MARK_ROM;
        attributeWriter = generateAttributeWriter( getBinWrap(chipIndex) );
        break;
    case RS_XRAM:
    case RS_ROM:
    default:
        std::string msg = EXCEPTION_TITLE;
        char errorMsg[1024] = "";
        sprintf(errorMsg, "writeUpdateMark() : not support ReferenceSource %d (%s)", (int) rs, ISiSProcedure::getRSStr(rs).c_str());
        msg.append(errorMsg);
        throw SPException( msg );
        break;
    }
    attributeWriter->setCtlReportToOs( this->m_ctlReportToOsFlag );
    attributeWriter->setChipIndex(chipIndex);

    /* write LastID */
    attributeWriter->writeAttribute( keyFaUpdateMark, serialData->getData(), serialData->getSize() );

    SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "");
    delete attributeWriter;
}

void
SiSProcedure_817_r64k::writeLastID(ReferenceSource rs, int chipIndex, SerialData* serialData)
{
    SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "writeLastID() : rs=%d, chipIndex=%d", (int) rs, chipIndex);

    /* prepare */
    string keyFaLastID;
    IAttributeWriter* attributeWriter = 0;

    switch (rs) {
//    case RS_XRAM:
//        keyFaLastID = KEY_FA_LAST_ID_XRAM;
//        attributeWriter = generateAttributeWriter();
//        break;
    case RS_BIN:
        keyFaLastID = KEY_FA_LAST_ID_ROM;
        attributeWriter = generateAttributeWriter( getBinWrap(chipIndex) );
        break;
    case RS_XRAM:
    case RS_ROM:
    default:
        std::string msg = EXCEPTION_TITLE;
        char errorMsg[1024] = "";
        sprintf(errorMsg, "writeLastID() : not support ReferenceSource %d (%s)", (int) rs, ISiSProcedure::getRSStr(rs).c_str());
        msg.append(errorMsg);
        throw SPException( msg );
        break;
    }
    attributeWriter->setCtlReportToOs( this->m_ctlReportToOsFlag );
    attributeWriter->setChipIndex(chipIndex);

    /* write LastID */
    attributeWriter->writeAttribute( keyFaLastID, serialData->getData(), serialData->getSize() );

    SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "");
    delete attributeWriter;
}

void
SiSProcedure_817_r64k::writeLastTime(ReferenceSource rs, int chipIndex, SerialData* serialData)
{
    SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "writeLastTime() : rs=%d, chipIndex=%d", (int) rs, chipIndex);

    /* prepare */
    string keyFaLastTime;
    IAttributeWriter* attributeWriter = 0;

    switch (rs) {
//    case RS_XRAM:
//        keyFaLastTime = KEY_FA_LAST_TIME_XRAM;
//        attributeWriter = generateAttributeWriter();
//        break;
    case RS_BIN:
        keyFaLastTime = KEY_FA_LAST_TIME_ROM;
        attributeWriter = generateAttributeWriter( getBinWrap(chipIndex) );
        break;
    case RS_XRAM:
    case RS_ROM:
    default:
        std::string msg = EXCEPTION_TITLE;
        char errorMsg[1024] = "";
        sprintf(errorMsg, "writeLastTime() : not support ReferenceSource %d (%s)", (int) rs, ISiSProcedure::getRSStr(rs).c_str());
        msg.append(errorMsg);
        throw SPException( msg );
        break;
    }
    attributeWriter->setCtlReportToOs( this->m_ctlReportToOsFlag );
    attributeWriter->setChipIndex(chipIndex);

    /* write LastTime */
    attributeWriter->writeAttribute( keyFaLastTime, serialData->getData(), serialData->getSize() );

    SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "");
    delete attributeWriter;
}

void
SiSProcedure_817_r64k::writePriorLastID(ReferenceSource rs, int chipIndex, SerialData* serialData)
{
    SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "writePriorLastID() : rs=%d, chipIndex=%d", (int) rs, chipIndex);

    /* prepare */
    string keyFaPriorLastID;
    IAttributeWriter* attributeWriter = 0;

    switch (rs) {
//    case RS_XRAM:
//        keyFaPriorLastID = KEY_FA_PRIOR_LAST_ID_XRAM;
//        attributeWriter = generateAttributeWriter();
//        break;
    case RS_BIN:
        keyFaPriorLastID = KEY_FA_PRIOR_LAST_ID_ROM;
        attributeWriter = generateAttributeWriter( getBinWrap(chipIndex) );
        break;
    case RS_XRAM:
    case RS_ROM:
    default:
        std::string msg = EXCEPTION_TITLE;
        char errorMsg[1024] = "";
        sprintf(errorMsg, "writePriorLastID() : not support ReferenceSource %d (%s)", (int) rs, ISiSProcedure::getRSStr(rs).c_str());
        msg.append(errorMsg);
        throw SPException( msg );
        break;
    }
    attributeWriter->setCtlReportToOs( this->m_ctlReportToOsFlag );
    attributeWriter->setChipIndex(chipIndex);

    /* write PriorLastID */
    attributeWriter->writeAttribute( keyFaPriorLastID, serialData->getData(), serialData->getSize() );

    SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "");
    delete attributeWriter;
}

void
SiSProcedure_817_r64k::writePriorLastTime(ReferenceSource rs, int chipIndex, SerialData* serialData)
{
    SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "writePriorLastTime() : rs=%d, chipIndex=%d", (int) rs, chipIndex);

    /* prepare */
    string keyFaPriorLastTime;
    IAttributeWriter* attributeWriter = 0;

    switch (rs) {
//    case RS_XRAM:
//        keyFaPriorLastTime = KEY_FA_PRIOR_LAST_TIME_XRAM;
//        attributeWriter = generateAttributeWriter();
//        break;
    case RS_BIN:
        keyFaPriorLastTime = KEY_FA_PRIOR_LAST_TIME_ROM;
        attributeWriter = generateAttributeWriter( getBinWrap(chipIndex) );
        break;
    case RS_XRAM:
    case RS_ROM:
    default:
        std::string msg = EXCEPTION_TITLE;
        char errorMsg[1024] = "";
        sprintf(errorMsg, "writePriorLastTime() : not support ReferenceSource %d (%s)", (int) rs, ISiSProcedure::getRSStr(rs).c_str());
        msg.append(errorMsg);
        throw SPException( msg );
        break;
    }
    attributeWriter->setCtlReportToOs( this->m_ctlReportToOsFlag );
    attributeWriter->setChipIndex(chipIndex);

    /* write PriorLastTime */
    attributeWriter->writeAttribute( keyFaPriorLastTime, serialData->getData(), serialData->getSize() );

    SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "");
    delete attributeWriter;
}

void
SiSProcedure_817_r64k::writeIsUpdateBootloaderInfo(ReferenceSource rs, int chipIndex, SerialData* serialData)
{
    SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "writeIsUpdateBootloaderInfo() : rs=%d, chipIndex=%d", (int) rs, chipIndex);

    /* prepare */
    string keyFaIsUpdateBootloaderInfo;
    IAttributeWriter* attributeWriter = 0;

    switch (rs) {
//    case RS_XRAM:
//        keyFaPriorLastTime = KEY_FA_PRIOR_LAST_TIME_XRAM;
//        attributeWriter = generateAttributeWriter();
//        break;
    case RS_BIN:
        keyFaIsUpdateBootloaderInfo = KEY_FA_IS_UPDATE_BOOTLOADER_INFO_ROM;
        attributeWriter = generateAttributeWriter( getBinWrap(chipIndex) );
        break;
    case RS_XRAM:
    case RS_ROM:
    default:
        std::string msg = EXCEPTION_TITLE;
        char errorMsg[1024] = "";
        sprintf(errorMsg, "writeIsUpdateBootloaderInfo() : not support ReferenceSource %d (%s)", (int) rs, ISiSProcedure::getRSStr(rs).c_str());
        msg.append(errorMsg);
        throw SPException( msg );
        break;
    }
    attributeWriter->setCtlReportToOs( this->m_ctlReportToOsFlag );
    attributeWriter->setChipIndex(chipIndex);

    /* write IsUpdateBootloader Info */
    attributeWriter->writeAttribute( keyFaIsUpdateBootloaderInfo, serialData->getData(), serialData->getSize() );

    SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "");
    delete attributeWriter;
}

void SiSProcedure_817_r64k::writeNoneSiSCmdViaBridge( 
		SerialData * data, const int transitionID)
{
	std::string msg = EXCEPTION_TITLE;
	char errorMsg[1024] = "";
	sprintf(errorMsg, "writeNoneSiSCmdViaBridge() : 817 does not support this function");
	msg.append(errorMsg);
	throw SPException( msg );
}

void
SiSProcedure_817_r64k::burn(RomBurnFlag romBurnFlag, int chipIndex)
{
    IRomSegmentBD::BurnDump burnDump = IRomSegmentBD::BD_BURN_TO_ROM;
    IRomBDArranger::RomBDPolicy romBDPolicy = getPolicy(romBurnFlag);

    IRomBDArranger* romBDArranger = generateRomBDArranger(burnDump, romBDPolicy);
    romBDArranger->setChipIndex(chipIndex);

    if( romBurnFlag.getFlag(RomBurnFlag::UR_RESERVE, RomBurnFlag::RF_RODATA_CODE) )
    {
        romBDArranger->addMainReservePartial(IRomSegmentBD::RS_RODATA_CODE);
    }

    BinWrap* binWrap = getBinWrap(chipIndex);
    romBDArranger->exec(binWrap->getBuf(), binWrap->getSize());
}

void
SiSProcedure_817_r64k::clearCalibration(int chipIndex)
{
    IRomSegmentBD::BurnDump burnDump = IRomSegmentBD::BD_BURN_TO_ROM;
    IRomBDArranger::RomBDPolicy romBDPolicy = IRomBDArranger::RBDP_CLEAR_CALIBRATION_FLAG;

    IRomBDArranger* romBDArranger = generateRomBDArranger(burnDump, romBDPolicy);
    romBDArranger->setChipIndex(chipIndex);

    romBDArranger->exec(0, _128K); // note : RBDP_CLEAR_CALIBRATION_FLAG, not need binBuf, fake 128k
}

IRomBDArranger::RomBDPolicy
SiSProcedure_817_r64k::getPolicy(RomBurnFlag romBurnFlag)
{
    if( romBurnFlag.getFlag(RomBurnFlag::UR_UPDATE, RomBurnFlag::RF_CLEAR_BOOT_FLAG) )
    {
        return IRomBDArranger::RBDP_CLEAR_BOOT_FLAG;
    }
    else if( romBurnFlag.getFlag(RomBurnFlag::UR_UPDATE, RomBurnFlag::RF_BOOTLOADER) &&
            romBurnFlag.getFlag(RomBurnFlag::UR_UPDATE, RomBurnFlag::RF_MAIN_CODE) &&
            romBurnFlag.getFlag(RomBurnFlag::UR_UPDATE, RomBurnFlag::RF_RODATA_CODE) &&
            romBurnFlag.getFlag(RomBurnFlag::UR_UPDATE, RomBurnFlag::RF_FW_INFO) &&
            romBurnFlag.getFlag(RomBurnFlag::UR_UPDATE, RomBurnFlag::RF_REG_MEM) &&
            romBurnFlag.getFlag(RomBurnFlag::UR_UPDATE, RomBurnFlag::RF_DEF_MEM) &&
            romBurnFlag.getFlag(RomBurnFlag::UR_UPDATE, RomBurnFlag::RF_THQA) &&
            romBurnFlag.getFlag(RomBurnFlag::UR_UPDATE, RomBurnFlag::RF_USB_DEV_DESC) &&
            romBurnFlag.getFlag(RomBurnFlag::UR_UPDATE, RomBurnFlag::RF_USB_RPT_DESC) )
    {
        return IRomBDArranger::RBDP_COMPLETE;
    }
    else if( romBurnFlag.getFlag(RomBurnFlag::UR_UPDATE, RomBurnFlag::RF_BOOTLOADER) )
    {
        return IRomBDArranger::RBDP_BOOTLOADER;
    }
    else if( romBurnFlag.getFlag(RomBurnFlag::UR_UPDATE, RomBurnFlag::RF_MAIN_CODE) &&
             romBurnFlag.getFlag(RomBurnFlag::UR_UPDATE, RomBurnFlag::RF_RODATA_CODE) &&
             romBurnFlag.getFlag(RomBurnFlag::UR_UPDATE, RomBurnFlag::RF_FW_INFO) &&
             romBurnFlag.getFlag(RomBurnFlag::UR_UPDATE, RomBurnFlag::RF_REG_MEM) &&
             romBurnFlag.getFlag(RomBurnFlag::UR_UPDATE, RomBurnFlag::RF_DEF_MEM) &&
             romBurnFlag.getFlag(RomBurnFlag::UR_UPDATE, RomBurnFlag::RF_THQA) &&
             romBurnFlag.getFlag(RomBurnFlag::UR_UPDATE, RomBurnFlag::RF_USB_DEV_DESC) &&
             romBurnFlag.getFlag(RomBurnFlag::UR_UPDATE, RomBurnFlag::RF_USB_RPT_DESC) )
    {
        if( romBurnFlag.getFlag(RomBurnFlag::UR_RESERVE, RomBurnFlag::RF_MAIN_CODE) ||
                romBurnFlag.getFlag(RomBurnFlag::UR_RESERVE, RomBurnFlag::RF_RODATA_CODE) ||
                romBurnFlag.getFlag(RomBurnFlag::UR_RESERVE, RomBurnFlag::RF_FW_INFO) ||
                romBurnFlag.getFlag(RomBurnFlag::UR_RESERVE, RomBurnFlag::RF_REG_MEM) ||
                romBurnFlag.getFlag(RomBurnFlag::UR_RESERVE, RomBurnFlag::RF_DEF_MEM) ||
                romBurnFlag.getFlag(RomBurnFlag::UR_RESERVE, RomBurnFlag::RF_THQA) ||
                romBurnFlag.getFlag(RomBurnFlag::UR_RESERVE, RomBurnFlag::RF_USB_DEV_DESC) ||
                romBurnFlag.getFlag(RomBurnFlag::UR_RESERVE, RomBurnFlag::RF_USB_RPT_DESC) )
        {
            /* reserve at least one */
            return IRomBDArranger::RBDP_MAIN_WITH_RESERVE_PARTIAL;
        }
        else
        {
            return IRomBDArranger::RBDP_MAIN;
        }
    }
    else if( romBurnFlag.getFlag(RomBurnFlag::UR_UPDATE, RomBurnFlag::RF_MAIN_CODE) ||
             romBurnFlag.getFlag(RomBurnFlag::UR_UPDATE, RomBurnFlag::RF_RODATA_CODE) ||
             romBurnFlag.getFlag(RomBurnFlag::UR_UPDATE, RomBurnFlag::RF_FW_INFO) ||
             romBurnFlag.getFlag(RomBurnFlag::UR_UPDATE, RomBurnFlag::RF_REG_MEM) ||
             romBurnFlag.getFlag(RomBurnFlag::UR_UPDATE, RomBurnFlag::RF_DEF_MEM) ||
             romBurnFlag.getFlag(RomBurnFlag::UR_UPDATE, RomBurnFlag::RF_THQA) ||
             romBurnFlag.getFlag(RomBurnFlag::UR_UPDATE, RomBurnFlag::RF_USB_DEV_DESC) ||
             romBurnFlag.getFlag(RomBurnFlag::UR_UPDATE, RomBurnFlag::RF_USB_RPT_DESC) )
    {
        return IRomBDArranger::RBDP_MAIN_PARTIAL;
    }
    else
    {
        for( int ur = RomBurnFlag::UR_UPDATE; ur < RomBurnFlag::UR_COUNT; ur++ )
        {
            for( int rf = RomBurnFlag::RF_BOOTLOADER; rf < RomBurnFlag::RF_COUNT; rf++ )
            {
                SIS_LOG_D(SiSLog::getOwnerSiS(), TAG,
                          "romBurnFlag.getFlag(%d, %d) : %s",
                          ur,
                          rf,
                          romBurnFlag.getFlag( (RomBurnFlag::UpdateReserve) ur,
                                               (RomBurnFlag::RomFlag) rf) == true ? "true" : "false");
            }
        }

        std::string msg = EXCEPTION_TITLE;
        char errorMsg[1024] = "";
        sprintf(errorMsg, "getPolicy() : unknow");
        msg.append(errorMsg);
        throw SPException( msg );
    }
}
