#include "romsegmentbd_817_r64k.h"

#include "sislog.h"
#include "spexception/spexception.h"
#include "spexception/siscoreexception.h"
#include "spbase/sisproceduredefine.h"

#include "addressprovider/817/addressprovider_817_r64k.h"
#include "attributereader/817/attributereader_817_r64k.h"

//#include "CptFactoryFacade.h"

#include <string.h>

using namespace std;
using namespace SiS::Procedure;
using namespace SiS;

#define EXCEPTION_TITLE "RomSegmentBD_817_r64k Exception : "
#define TAG "RomSegmentBD_817_r64k"

RomSegmentBD_817_r64k::RomSegmentBD_817_r64k(SiSDeviceIO* sisDeviceIO, BurnDump burnDump, RomSegment romSegment) :
    IRomSegmentBD(sisDeviceIO, burnDump, romSegment)
{

}

RomSegmentBD_817_r64k::~RomSegmentBD_817_r64k()
{

}

IAddressProvider*
RomSegmentBD_817_r64k::getAddressProvider()
{
    if( m_addressProvider != 0 )
    {
        return m_addressProvider;
    }
    else
    {
        m_addressProvider = new AddressProvider_817_r64k();
        return m_addressProvider;
    }
}

std::string
RomSegmentBD_817_r64k::getAddressProviderTableName()
{
    if( m_addressProvider != 0 )
    {
        return m_addressProvider->getTableName();
    }
    else
    {
        return getAddressProvider()->getTableName();
    }
}

IAttributeReader*
RomSegmentBD_817_r64k::generateAttributeReader()
{
    return new AttributeReader_817_r64k(m_sisDeviceIO);
}

int
RomSegmentBD_817_r64k::mappingChipAddress(int chipIndex)
{
    if( chipIndex == CI_MASTER )
    {
        return 0;
    }
    else
    {
        int chipAddress = 5 + (chipIndex - (int) CI_SLAVE_0);
        SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "mappingChipAddress : chipIndex=%d -> chipAddress=0x%x", chipIndex, chipAddress );
        return chipAddress;
    }
}

int
RomSegmentBD_817_r64k::getMaxBlockSize()
{
    return _12K;
}

void
RomSegmentBD_817_r64k::exec(unsigned char* binBuffer, int binSize)
{
    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "--" );

    switch (m_romSegment) {
    case RS_CLEAR_BOOT_FLAG:
        execClearBootFlag(binBuffer, binSize);
        break;
    case RS_CLEAR_CALIBRATION_FLAG:
        execClearCalibrationFlag(binSize);
        break;
    case RS_BOOT_CODE:
    case RS_UPDATE_CODE:
    case RS_MAIN_CODE_1:
    case RS_MAIN_CODE_2:
    case RS_RODATA_CODE:
    case RS_REMAIN_SECTION:
        execIndependent(binBuffer, binSize);
        break;
    case RS_MAIN_COMBINE_INFO_REG_DEF_THQA_DEV_RPT:
    case RS_MAIN_COMBINE_THQA_DEV_RPT:
    case RS_MAIN_COMBINE_INFO_REG:
        execDependent(binBuffer, binSize);
        break;
    default:
        break;
    }

    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "" );
}

void
RomSegmentBD_817_r64k::execClearBootFlag(unsigned char* binBuffer, int binSize)
{
    std::string romKey = getRomSegmentKey(IRomSegmentBD::BD_BURN_TO_ROM, m_romSegment);
    std::string xramKey = getRomSegmentKey(IRomSegmentBD::BD_DUMP_FROM_XRAM, m_romSegment);

    /* clone 4k and clear boot flag */
    unsigned int address = getAddressProvider()->getAddress(xramKey.c_str());
    int length = getAddressProvider()->getLength(xramKey.c_str());
    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s DUMP FROM XRAM [%s] : ", SYMBOL_CLEAR_BOOT_FLAG, getAddressProviderTableName().c_str() );
    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG,
              "Key \"%s\" : address (0x%x), length (0x%x)",
              xramKey.c_str(),
              address,
              length );

    /* prepare */
    IAttributeReader* attributeReader = generateAttributeReader();
    attributeReader->setChipIndex(m_chipIndex);

    /* read */
    unsigned char* dataBuf = 0;
    int dataSize = 0;
    attributeReader->readAttribute( xramKey, dataBuf, dataSize );

    /* check if address table error */
    IAttributeReader::checkDataSize(_4K, dataSize); // 4k

    /* clone */
    unsigned char* fakeBinBuffer = new unsigned char[binSize];
    memset(fakeBinBuffer, 0x0, binSize * sizeof(unsigned char) );
    memcpy(fakeBinBuffer + getAddressProvider()->getAddress(romKey),
           dataBuf, dataSize * sizeof(unsigned char) );

    /* get updateTimeAddress / get updateFwInfoAddress */
    unsigned int updateTimeAddress = getAddressProvider()->getAddress("FA_LAST_TIME_ROM");
    unsigned int isUpdateBootloaderInfoAddress = getAddressProvider()->getAddress("FA_IS_UPDATE_BOOTLOADER_INFO_ROM");
    unsigned int updateFwInfoAddress = getAddressProvider()->getAddress("FA_UPDATE_FW_INFO_ROM");
    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "buried update fw information : updateFwInfoAddress=0x%x", updateFwInfoAddress );

    /* buried updateFwInfo */
    fakeBinBuffer[updateFwInfoAddress] = binBuffer[updateTimeAddress];
    fakeBinBuffer[updateFwInfoAddress + 1] = binBuffer[updateTimeAddress + 1];
    fakeBinBuffer[updateFwInfoAddress + 2] = binBuffer[updateTimeAddress + 2];
    fakeBinBuffer[updateFwInfoAddress + 3] = binBuffer[updateTimeAddress + 3];
    fakeBinBuffer[updateFwInfoAddress + 4] = binBuffer[isUpdateBootloaderInfoAddress];
    fakeBinBuffer[updateFwInfoAddress + 5] = binBuffer[isUpdateBootloaderInfoAddress + 1];

    /* get bootFlagAddress */
    unsigned int bootFlagAddress = getAddressProvider()->getAddress("FA_BOOT_FLAG_ROM");
    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "clear boot flag : bootFlagAddress=0x%x", bootFlagAddress );

    /* clear boot flag */
    fakeBinBuffer[bootFlagAddress] = 0x0;
    fakeBinBuffer[bootFlagAddress + 1] = 0x0;
    fakeBinBuffer[bootFlagAddress + 2] = 0x0;
    fakeBinBuffer[bootFlagAddress + 3] = 0x0;

    /* burn back to */
    address = getAddressProvider()->getAddress(romKey.c_str());
    length = getAddressProvider()->getLength(romKey.c_str());
    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s BURN TO ROM [%s] : ", SYMBOL_CLEAR_BOOT_FLAG, getAddressProviderTableName().c_str() );
    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG,
              "Key \"%s\" : address (0x%x), length (0x%x)",
              romKey.c_str(),
              address,
              length );

    /* burn */
    int ret = -1;
    try
    {
        //Component * binClone = new Component("SerialData", fakeBinBuffer, binSize);

        //if( m_chipIndex == CI_MASTER )
        //{
            //SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "BURST_WRITE, 0x%x (%0x%x)", address, length );
            //ret = m_sisCore->exe<int>("BURST_WRITE",
            //                          ADDRESS(m_sisCore, address),
            //                          WRITE_LEN(m_sisCore, length),
           //                           binClone);
           
        //}
        //else
        //{
            //SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "BURST_WRITE_S, 0x%x (0x%x), ChipAddress=0x%x", address, length, mappingChipAddress(m_chipIndex) );
            //ret = m_sisCore->exe<int>("BURST_WRITE_S",
            //                          ADDRESS(m_sisCore, address),
            //                          WRITE_LEN(m_sisCore, length),
            //                          binClone,
            //                          CHIP(m_sisCore, mappingChipAddress(m_chipIndex)));
        //}

        SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "burnToROM, 0x%x (0x%x), ChipAddress=0x%x", address, length, mappingChipAddress(m_chipIndex) );
        ret = m_sisDeviceIO->burnToROM( address, length, fakeBinBuffer, binSize, mappingChipAddress(m_chipIndex) );
    }
    SISCORE_EXCEPTION_SP_HANDLE

    if( ret != 1 )
    {
        std::string msg = EXCEPTION_TITLE;
        char errorMsg[1024] = "";
        sprintf(errorMsg, "execClearBootFlag : FAIL (ret=%d)", ret);
        msg.append(errorMsg);
        throw SPException( msg );
    }
}

void
RomSegmentBD_817_r64k::execClearCalibrationFlag(int binSize)
{
    std::string romKey = getRomSegmentKey(IRomSegmentBD::BD_BURN_TO_ROM, m_romSegment);

    /* fake binBuf */
    unsigned char* fakeBinBuffer = new unsigned char[binSize];
    memset(fakeBinBuffer, 0x0, binSize * sizeof(unsigned char) );

    /* get bootFlagAddress */
    unsigned int bootFlagAddress = getAddressProvider()->getAddress("FA_BOOT_FLAG_ROM");
    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "set boot flag : bootFlagAddress=0x%x", bootFlagAddress );

    /* set boot flag */
    fakeBinBuffer[bootFlagAddress] = 0x50;
    fakeBinBuffer[bootFlagAddress + 1] = 0x38;
    fakeBinBuffer[bootFlagAddress + 2] = 0x31;
    fakeBinBuffer[bootFlagAddress + 3] = 0x30;

    /* burn back to */
    unsigned int address = getAddressProvider()->getAddress(romKey.c_str());
    int length = getAddressProvider()->getLength(romKey.c_str());
    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s BURN TO ROM [%s] : ", SYMBOL_CLEAR_BOOT_FLAG, getAddressProviderTableName().c_str() );
    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG,
              "Key \"%s\" : address (0x%x), length (0x%x)",
              romKey.c_str(),
              address,
              length );

//    /* burn */
//    try
//    {
//        int ret = -1;
//        Component * binClone = new Component("SerialData", fakeBinBuffer, binSize);

//        if( m_chipIndex == CI_MASTER )
//        {
//            SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "BURST_WRITE, 0x%x (0x%x)", address, length );
//            ret = m_sisCore->exe<int>("BURST_WRITE",
//                                      ADDRESS(m_sisCore, address),
//                                      WRITE_LEN(m_sisCore, length),
//                                      binClone);
//        }
//        else
//        {
//            SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "BURST_WRITE_S, 0x%x (0x%x), ChipAddress=0x%x", address, length, mappingChipAddress(m_chipIndex) );
//            ret = m_sisCore->exe<int>("BURST_WRITE_S",
//                                      ADDRESS(m_sisCore, address),
//                                      WRITE_LEN(m_sisCore, length),
//                                      binClone,
//                                      CHIP(m_sisCore, mappingChipAddress(m_chipIndex)));
//        }

//        if( ret != 1 )
//        {
//            std::string msg = EXCEPTION_TITLE;
//            char errorMsg[1024] = "";
//            sprintf(errorMsg, "execClearBootFlag : FAIL (ret=%d)", ret);
//            msg.append(errorMsg);
//            throw SPException( msg );
//        }
//    }
//    SISCORE_EXCEPTION_SP_HANDLE
}

void
RomSegmentBD_817_r64k::execIndependent(unsigned char* binBuffer, int binSize)
{
    std::string key = getRomSegmentKey(m_burnDump, m_romSegment);
    unsigned int address = getAddressProvider()->getAddress(key.c_str());
    int length = getAddressProvider()->getLength(key.c_str());

    if( m_burnDump == IRomSegmentBD::BD_BURN_TO_ROM )
    {
        SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s BURN TO ROM [%s] : ", SYMBOL_INDEPENDENT, getAddressProviderTableName().c_str() );
    }
    else if( m_burnDump == IRomSegmentBD::BD_DUMP_FROM_XRAM )
    {
        SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s DUMP FROM XRAM [%s] : ", SYMBOL_INDEPENDENT, getAddressProviderTableName().c_str() );
    }
    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG,
              "key \"%s\" : address (0x%x), length (0x%x)",
              key.c_str(),
              address,
              length );

    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "getMaxBlockSize=0x%x", getMaxBlockSize() );

    /* remain section */
    if( m_romSegment == IRomSegmentBD::RS_REMAIN_SECTION &&
            m_burnDump == IRomSegmentBD::BD_BURN_TO_ROM )
    {
        SIS_LOG_W(SiSLog::getOwnerSiS(), TAG, "not burn : remain section !" );
        return;
    }

    /* clone */
    unsigned char* _binClone = new unsigned char[binSize];
    memcpy( _binClone, binBuffer, binSize * sizeof(unsigned char) );

    /* burn */
    int ret = -1;
    try
    {
//        Component * binClone = new Component("SerialData", _binClone, binSize);

//        if( m_chipIndex == CI_MASTER )
//        {
//            SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "BURST_WRITE, 0x%x (0x%x)", address, length );
//            ret = m_sisCore->exe<int>("BURST_WRITE",
//                                      ADDRESS(m_sisCore, address),
//                                      WRITE_LEN(m_sisCore, length),
//                                      binClone);
//        }
//        else
//        {
//            SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "BURST_WRITE_S, 0x%x (0x%x), ChipAddress=0x%x", address, length, mappingChipAddress(m_chipIndex) );
//            ret = m_sisCore->exe<int>("BURST_WRITE_S",
//                                      ADDRESS(m_sisCore, address),
//                                      WRITE_LEN(m_sisCore, length),
//                                      binClone,
//                                      CHIP(m_sisCore, mappingChipAddress(m_chipIndex)));
//        }

        SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "burnToROM, 0x%x (0x%x), ChipAddress=0x%x", address, length, mappingChipAddress(m_chipIndex) );
        ret = m_sisDeviceIO->burnToROM( address, length, _binClone, binSize, mappingChipAddress(m_chipIndex) );
    }
    SISCORE_EXCEPTION_SP_HANDLE

    if( ret != 1 )
    {
        std::string msg = EXCEPTION_TITLE;
        char errorMsg[1024] = "";
        sprintf(errorMsg, "execIndependent : FAIL (ret=%d)", ret);
        msg.append(errorMsg);
        throw SPException( msg );
    }
}

void
RomSegmentBD_817_r64k::execDependent(unsigned char* binBuffer, int binSize)
{
    execIndependent(binBuffer, binSize);
}
