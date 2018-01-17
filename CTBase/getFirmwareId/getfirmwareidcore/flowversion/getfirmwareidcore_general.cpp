#include "getfirmwareidcore_general.h"

#include "sislog.h"
#include "ctexception/ctexception.h"
#include "spexception/spexception.h"

using namespace CT;
using namespace SiS;
using namespace SiS::Procedure;

#define EXCEPTION_TITLE "GetFirmwareIdCore_general Exception : "
#define TAG "GetFirmwareIdCore_general"

GetFirmwareIdCore_general::GetFirmwareIdCore_general(GetFirmwareIdParameter* getFirmwareIdParameter) :
    IGetFirmwareIdCore(getFirmwareIdParameter)
{

}

GetFirmwareIdCore_general::~GetFirmwareIdCore_general()
{

}

CTExitCode
GetFirmwareIdCore_general::exec()
{
    /* prepareBaseXramMasterRef */
    prepareBaseXramMasterRef(false); // stop if broken

    /* prepareAllXramRef */
    prepareAllXramRef();

    /* do getFirmwareId function */
    if( this->m_getFirmwareIdParameter->getFirmwareCheckIdMap().size() == 0 &&
            this->m_getFirmwareIdParameter->getBinWrapMap().size() == 0)
    {
        return showFirmwareId();
    }
    else if( this->m_getFirmwareIdParameter->getFirmwareCheckIdMap().size() > 0 )
    {
        checkInputConut( this->m_getFirmwareIdParameter->getFirmwareCheckIdMap().size() );
        return checkFirmwareIdUsingInput();
    }
    else if( this->m_getFirmwareIdParameter->getBinWrapMap().size() > 0 )
    {
        checkInputConut( this->m_getFirmwareIdParameter->getBinWrapMap().size() );
        return checkFirmwareIdUsingFile();
    }

    return undoException(EXCEPTION_TITLE);
}

void
GetFirmwareIdCore_general::prepareAllXramRef()
{
    GetFirmwareIdReference* xramMasterRef = getGetFirmwareIdReference(RS_XRAM, CI_MASTER);

    /* prepareXramRef for each chip */
    for(int chipIndex = CI_MASTER; chipIndex < xramMasterRef->getSlaveNum() + 1; chipIndex++)
    {
        if( xramMasterRef->isBroken() && chipIndex != CI_MASTER )
        {
            /* init reference */
            insertReference(RS_XRAM, chipIndex, generateReference());
            continue; // can't get slave if broken
        }

        prepareXramRef(chipIndex);
    }
}

void
GetFirmwareIdCore_general::prepareXramRef(int chipIndex)
{
    /* switch SiSProcedure by OS DeviceType */
    m_sisProcedure->switchSiSProcedure( m_ctBaseParameter->getOSDeviceType() );

    /* init reference */
    insertReference(RS_XRAM, chipIndex, generateReference());

    fetchFirmwareID(RS_XRAM, chipIndex); /* 8. FirmwareID */
}

void
GetFirmwareIdCore_general::prepareBinRef(int chipIndex)
{
    /* init reference */
    insertReference(RS_BIN, chipIndex, generateReference());

    fetchFirmwareID(RS_BIN, chipIndex); /* 8. FirmwareID */
}

void
GetFirmwareIdCore_general::checkInputConut(int inputCount)
{
    GetFirmwareIdReference* xramMasterRef = getGetFirmwareIdReference(RS_XRAM, CI_MASTER);
    int chipCount = xramMasterRef->getSlaveNum() + 1;

    if( chipCount != inputCount )
    {
        /* stop if input conut not match chip count */
        std::string msg = EXCEPTION_TITLE;
        char errorMsg[1024] = "";
        sprintf(errorMsg, "checkInputConut : not match ! chipCount(%d), inputCount(%d)\n"
                ,chipCount, inputCount);
        msg.append(errorMsg);
        throw CTException( msg );
    }
}

CTExitCode
GetFirmwareIdCore_general::showFirmwareId()
{
    GetFirmwareIdReference* xramMasterRef = getGetFirmwareIdReference(RS_XRAM, CI_MASTER);

    /* version */
    unsigned char major = 0;
    unsigned char minor = 0;
    unsigned char* idData = xramMasterRef->getFirmwareID()->getData();
	if( xramMasterRef->getFirmwareID()->getSize() == 14 && !xramMasterRef->isBroken() )
	{
        major = idData[13];
        minor = idData[12];
	}
    SIS_LOG_I( SiSLog::getOwnerSiS(), TAG, "Active Firmware Version : %d.%d\n", major, minor );

    for(int chipIndex = CI_MASTER; chipIndex < xramMasterRef->getSlaveNum() + 1; chipIndex++)
    {
        SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s Firmware Id : %s\n",
                        ISiSProcedure::getCIStr(chipIndex).c_str(),
                        getGetFirmwareIdReference(RS_XRAM, chipIndex)->getFirmwareID()->getSerial().c_str() );
    }

    return CT_EXIT_PASS;
}

CTExitCode
GetFirmwareIdCore_general::checkFirmwareIdUsingInput()
{
    GetFirmwareIdReference* xramMasterRef = getGetFirmwareIdReference(RS_XRAM, CI_MASTER);

    for(int chipIndex = CI_MASTER; chipIndex < xramMasterRef->getSlaveNum() + 1; chipIndex++)
    {
        /* start to compare */

        SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s :", ISiSProcedure::getCIStr(chipIndex).c_str());

        SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "XRAM Firmware Id (%s)",
                        getGetFirmwareIdReference(RS_XRAM, chipIndex)->getFirmwareID()->getSerial().c_str() );

        SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "compare to input (%s)",
                        this->m_getFirmwareIdParameter->getFirmwareCheckIdMap(chipIndex).c_str() );

        if( getGetFirmwareIdReference(RS_XRAM, chipIndex)->getFirmwareID()->getSerial().compare(this->m_getFirmwareIdParameter->getFirmwareCheckIdMap(chipIndex)) == 0 ||
                getGetFirmwareIdReference(RS_XRAM, chipIndex)->getFirmwareID()->getString().compare(this->m_getFirmwareIdParameter->getFirmwareCheckIdMap(chipIndex)) == 0 )
        {
            SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "the same\n");
        }
        else
        {
            SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "different !!!\n");
            return CT_EXIT_FAIL;
        }
    }

    return CT_EXIT_PASS;
}

CTExitCode
GetFirmwareIdCore_general::checkFirmwareIdUsingFile()
{
    prepareAllBin();

    GetFirmwareIdReference* xramMasterRef = getGetFirmwareIdReference(RS_XRAM, CI_MASTER);

    for(int chipIndex = CI_MASTER; chipIndex < xramMasterRef->getSlaveNum() + 1; chipIndex++)
    {
        /* prepareBinRef for each chip */
        prepareBinRef(chipIndex);

        /* start to compare */

        SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s :", ISiSProcedure::getCIStr(chipIndex).c_str());

        SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "XRAM Firmware Id (%s)",
                        getGetFirmwareIdReference(RS_XRAM, chipIndex)->getFirmwareID()->getSerial().c_str() );

        SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "compare to file (%s)",
                        getGetFirmwareIdReference(RS_BIN, chipIndex)->getFirmwareID()->getSerial().c_str() );

        if( getGetFirmwareIdReference(RS_XRAM, chipIndex)->getFirmwareID()->getSerial().compare(
                    getGetFirmwareIdReference(RS_BIN, chipIndex)->getFirmwareID()->getSerial()) == 0 )
        {
            SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "the same\n");
        }
        else
        {
            SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "different !!!\n");
            return CT_EXIT_FAIL;
        }
    }

    return CT_EXIT_PASS;
}
