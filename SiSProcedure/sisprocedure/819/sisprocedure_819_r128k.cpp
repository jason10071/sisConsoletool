#include "sisprocedure_819_r128k.h"

#include "sislog.h"
#include "spexception/spexception.h"
#include "spexception/siscoreexception.h"
#include "addressprovider/addresstable/attributekeydefine.h"

#include "attributereader/819/attributereader_819_r128k.h"
#include "attributewriter/819/attributewriter_819_r128k.h"
#include "rombd/819/rombdarranger_819_r128k.h"

//#include "HydraDefine.h"
//#include "CptFactoryFacade.h"

using namespace std;
using namespace SiS::Procedure;
using namespace SiS;

#define EXCEPTION_TITLE "SiSProcedure_819_r128k Exception : "
#define TAG "SiSProcedure_819_r128k"

SiSProcedure_819_r128k::SiSProcedure_819_r128k() :
    SiSProcedure_817_r64k()
{

}

SiSProcedure_819_r128k::~SiSProcedure_819_r128k()
{

}

IAttributeReader*
SiSProcedure_819_r128k::generateAttributeReader(BinWrap* binWrap)
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

        return new AttributeReader_819_r128k(m_sisDeviceIO);
    }
    else
    {
        return new AttributeReader_819_r128k(binWrap->getBuf(), binWrap->getSize());
    }
}

IAttributeWriter*
SiSProcedure_819_r128k::generateAttributeWriter(BinWrap* binWrap)
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

        return new AttributeWriter_819_r128k(m_sisDeviceIO);
    }
    else
    {
        return new AttributeWriter_819_r128k(binWrap->getBuf(), binWrap->getSize());
    }
}

IRomBDArranger*
SiSProcedure_819_r128k::generateRomBDArranger(IRomSegmentBD::BurnDump burnDump, IRomBDArranger::RomBDPolicy romBDPolicy)
{
    return new RomBDArranger_819_r128k(m_sisDeviceIO, burnDump, romBDPolicy);
}

/*
void
SiSProcedure_819_r128k::disableCtlReportToOs()
{
    if( m_sisCore == 0 )
    {
        std::string msg = EXCEPTION_TITLE;
        char errorMsg[1024] = "";
        sprintf(errorMsg, "disableCtlReportToOs() : no sisCore");
        msg.append(errorMsg);
        throw SPException( msg );
    }

    int ret = -1;
    try
    {
        SiSDebug::LOG_I("CTL_CHANGE_MODE, MODE(m_sisCore, 0x%x)\n", Hydra::ENABLE_DIAGNOSIS);
        ret = m_sisCore->exe<int>("CTL_CHANGE_MODE", MODE(m_sisCore, Hydra::ENABLE_DIAGNOSIS));
        SiSDebug::LOG_D("CTL_CHANGE_MODE : (ret=%d)\n", ret);
    }
    SISCORE_EXCEPTION_SP_HANDLE

    if( ret != 1 )
    {
        std::string msg = EXCEPTION_TITLE;
        char errorMsg[1024] = "";
        sprintf(errorMsg, "CTL_CHANGE_MODE, MODE(m_sisCore, 0x%x) : FAIL (ret=%d)", Hydra::ENABLE_DIAGNOSIS, ret);
        msg.append(errorMsg);
        throw SPException( msg );
    }
}

void
SiSProcedure_819_r128k::enableCtlReportToOs()
{
    if( m_sisCore == 0 )
    {
        std::string msg = EXCEPTION_TITLE;
        char errorMsg[1024] = "";
        sprintf(errorMsg, "enableCtlReportToOs() : no sisCore");
        msg.append(errorMsg);
        throw SPException( msg );
    }

    int ret = -1;
    try
    {
        SiSDebug::LOG_I("CTL_CHANGE_MODE, MODE(m_sisCore, 0x%x)\n", Hydra::DISABLE_DIAGNOSIS);
        ret = m_sisCore->exe<int>("CTL_CHANGE_MODE", MODE(m_sisCore, Hydra::DISABLE_DIAGNOSIS));
        SiSDebug::LOG_D("CTL_CHANGE_MODE : (ret=%d)\n", ret);
    }
    SISCORE_EXCEPTION_SP_HANDLE

    if( ret != 1 )
    {
        std::string msg = EXCEPTION_TITLE;
        char errorMsg[1024] = "";
        sprintf(errorMsg, "CTL_CHANGE_MODE, MODE(m_sisCore, 0x%x) : FAIL (ret=%d)", Hydra::DISABLE_DIAGNOSIS, ret);
        msg.append(errorMsg);
        throw SPException( msg );
    }
}
*/

void
SiSProcedure_819_r128k::jumpBootloader(int chipIndex)
{
    if( m_sisDeviceIO == 0 )
    {
        std::string msg = EXCEPTION_TITLE;
        char errorMsg[1024] = "";
        sprintf(errorMsg, "jumpBootloader() : no sisDeviceIO");
        msg.append(errorMsg);
        throw SPException( msg );
    }

    int ret = -1;

    if( chipIndex == CI_MASTER )
    {
        try
        {
            SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "jumpBootloader, chipAddress : 0x%x", CI_MASTER );
            //ret = m_sisCore->exe<int>("CTL_CHANGE_MODE", MODE(m_sisCore, Hydra::BOOTLOADER));
            ret = m_sisDeviceIO->jumpBootloader(CI_MASTER);
            SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "jumpBootloader : (ret=%d)", ret );
        }
        SISCORE_EXCEPTION_SP_HANDLE
    }
    else
    {
        IAttributeReader* attributeReader = generateAttributeReader();
        int chipAddress = attributeReader->mappingChipAddress(chipIndex);
        delete attributeReader;

        try
        {
            SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "jumpBootloader, chipAddress : 0x%x", chipAddress );
            //ret = m_sisCore->exe<int>("CTL_CHANGE_MODE_S", MODE(m_sisCore, Hydra::BOOTLOADER), CHIP(m_sisCore, chipAddress));
            ret = m_sisDeviceIO->jumpBootloader(chipAddress);
            SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "jumpBootloader : (ret=%d)", ret );
        }
        SISCORE_EXCEPTION_SP_HANDLE
    }

    if ( ret >= 0 )
    {
        SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "jumpBootloader : OK" );
    }
    else
    {
        std::string msg = EXCEPTION_TITLE;
        char errorMsg[1024] = "";
        sprintf(errorMsg, "jumpBootloader : FAIL (ret=%d)", ret);
        msg.append(errorMsg);
        throw SPException( msg );
    }
}

void
SiSProcedure_819_r128k::softReset()
{
    if( m_sisDeviceIO == 0 )
    {
        std::string msg = EXCEPTION_TITLE;
        char errorMsg[1024] = "";
        sprintf(errorMsg, "softReset() : no sisDeviceIO");
        msg.append(errorMsg);
        throw SPException( msg );
    }

    int ret = -1;
    try
    {
        SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "CTL_SOFT_RESET" );
        //ret = m_sisCore->exe<int>("CTL_SOFT_RESET");
        ret = m_sisDeviceIO->ctlSoftReset();
        SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "CTL_SOFT_RESET : (ret=%d)", ret );
    }
    SISCORE_EXCEPTION_SP_HANDLE

    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "CTL_SOFT_RESET : 819 didn't need to check (ret=%d)", ret );

//    if( ret != 1 )
//    {
//        std::string msg = EXCEPTION_TITLE;
//        char errorMsg[1024] = "";
//        sprintf(errorMsg, "CTL_SOFT_RESET : FAIL (ret=%d)", ret);
//        msg.append(errorMsg);
//        throw SPException( msg );
//    }
}

//void
//SiSProcedure_819_r128k::reCalibration()
//{
//    SiSDebug::LOG_I("\nTemp: SiSProcedure_819_r128k::reCalibration()\n\n");

//    if( m_sisCore == 0 )
//    {
//        std::string msg = EXCEPTION_TITLE;
//        char errorMsg[1024] = "";
//        sprintf(errorMsg, "reCalibration() : no sisCore");
//        msg.append(errorMsg);
//        throw SPException( msg );
//    }

//    int ret = -1;
//    try
//    {
//        SiSDebug::LOG_I("CTL_RECALIBRATION\n");
//        ret = m_sisCore->exe<int>("CTL_RECALIBRATION");
//        SiSDebug::LOG_D("CTL_RECALIBRATION : (ret=%d)\n", ret);
//    }
//    SISCORE_EXCEPTION_SP_HANDLE

//    if( ret != 1 )
//    {
//        std::string msg = EXCEPTION_TITLE;
//        char errorMsg[1024] = "";
//        sprintf(errorMsg, "CTL_RECALIBRATION : FAIL (ret=%d)", ret);
//        msg.append(errorMsg);
//        throw SPException( msg );
//    }
//}

FWSizeType
SiSProcedure_819_r128k::readFWSizeType(ReferenceSource rs, int chipIndex, bool isBroken)
{
    SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "readFWSizeType() : rs=%d, chipIndex=%d, isBroken=%s", (int) rs, chipIndex, isBroken == true ? "true" : "false" );

    /* judge */
    FWSizeType fwSizeType = FST_819_R128K;

    /* print data */
    SIS_LOG_D(SiSLog::getOwnerSiS(), TAG,
              "read %s (%s) FwSizeType : %s\n",
              ISiSProcedure::getCIStr(chipIndex).c_str(),
              ISiSProcedure::getRSStr(rs).c_str(),
              ISiSProcedure::getFSTStr(fwSizeType).c_str() );

    return fwSizeType;
}

FwVersion
SiSProcedure_819_r128k::readFwVersion(ReferenceSource rs, int chipIndex)
{
    SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "readFwVersion() : rs=%d, chipIndex=%d", (int) rs, chipIndex );

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
    FwVersion fwVersion;
    fwVersion.major = dataBuf[0] & 0xff; // different to 817
    fwVersion.minor = dataBuf[1] & 0xff; // different to 817

    /* print data */
    if( SiSLog::getOwnerSiS()->isLOG_D() )
    {
        SIS_LOG_D(SiSLog::getOwnerSiS(), TAG,
                  "read %s (%s) FwVersion : %d.%d\n",
                  ISiSProcedure::getCIStr(chipIndex).c_str(),
                  ISiSProcedure::getRSStr(rs).c_str(),
                  fwVersion.major, fwVersion.minor );
    }

    delete [] dataBuf;
    delete attributeReader;
    return fwVersion;
}

//int
//SiSProcedure_819_r128k::readSlaveNum(ReferenceSource rs)
//{
//    SiSDebug::LOG_D("readSlaveNum() : rs=%d\n", (int) rs);

//    /* judge */
//    int slaveNum = 0;

//    /* print data */
//    SiSDebug::LOG_D("read %s (%s) SlaveNum : %d\n",
//                    ISiSProcedure::getCIStr(CI_MASTER).c_str(),
//                    ISiSProcedure::getRSStr(rs).c_str(),
//                    slaveNum);

//    SiSDebug::LOG_D("\n");
//    return slaveNum;
//}

FWSizeType
SiSProcedure_819_r128k::readRomFWSizeType(int chipIndex)
{
    SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "readRomFWSizeType() : chipIndex=%d", chipIndex );

    /* judge */
    FWSizeType romFwSizeType = FST_R128K;

    /* print data */
    SIS_LOG_D(SiSLog::getOwnerSiS(), TAG,
              "read %s (%s) FwSizeType : %s\n",
              ISiSProcedure::getCIStr(chipIndex).c_str(),
              ISiSProcedure::getRSStr(RS_ROM).c_str(),
              ISiSProcedure::getFSTStr(romFwSizeType).c_str() );

    return romFwSizeType;
}


SerialData* SiSProcedure_819_r128k::readNoneSiSCmdViaBridge(SerialData * readcommand, const size_t len, const int transitionID)
{
    if( m_sisDeviceIO == 0 )
    {
        throw SPException("readNoneSiSCmdViaBridge() : no sisCore");
    }

//    try
//    {
//		Component * readcommandCpt = new Component("ReadcommandSerialData", readcommand->getData(), readcommand->getSize());
//		Component * receiveBuffer = SERIAL_DATA(m_sisCore, len);

//		int receiveResult = m_sisCore->exe<int>(
//                "RECEIVE_NONE_SIS_CMD",
//				TRANSITION_ID(m_sisCore, transitionID),
//				READ_LEN(m_sisCore, len),
//				readcommandCpt,
//				receiveBuffer);
//		if ( Hydra::OperationSuccess != receiveResult )
//		{
//			throw SPException("readNoneSiSCmdViaBridge() : receive command fail\n");
//		}

//		BYTE * receiveDataInArray = Component::valueInByte(receiveBuffer);
//		SerialData * receiveData = new SerialData(receiveDataInArray, len);

//		delete receiveBuffer;

//		return receiveData;
//    }
//    SISCORE_EXCEPTION_SP_HANDLE

    return 0;
}

void SiSProcedure_819_r128k::writeNoneSiSCmdViaBridge( 
		SerialData * data, const int transitionID)
{
    if( m_sisDeviceIO == 0 )
    {
        throw SPException("writeNoneSiSCmdViaBridge() : no sisCore");
    }

//    try
//    {
//		Component * dataForSiSCore = new Component("SerialData", data->getData(), data->getSize());

//		int sendResult = m_sisCore->exe<int>(
//				"SEND_NONE_SIS_CMD",
//				TRANSITION_ID(m_sisCore, transitionID),
//				dataForSiSCore);
//		if ( Hydra::OperationSuccess != sendResult )
//		{
//			throw SPException("writeNoneSiSCmdViaBridge() : send command fail\n");
//		}
//    }
//    SISCORE_EXCEPTION_SP_HANDLE
}
