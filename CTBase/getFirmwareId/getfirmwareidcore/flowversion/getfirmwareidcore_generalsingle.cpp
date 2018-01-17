#include "getfirmwareidcore_generalsingle.h"

#include "sislog.h"
#include "ctexception/ctexception.h"
#include "spexception/spexception.h"

using namespace CT;
using namespace SiS;
using namespace SiS::Procedure;

#define EXCEPTION_TITLE "GetFirmwareIdCore_generalSingle Exception : "
#define TAG "GetFirmwareIdCore_generalSingle"

GetFirmwareIdCore_generalSingle::GetFirmwareIdCore_generalSingle(GetFirmwareIdParameter* getFirmwareIdParameter) :
    GetFirmwareIdCore_general(getFirmwareIdParameter)
{

}

GetFirmwareIdCore_generalSingle::~GetFirmwareIdCore_generalSingle()
{

}

void
GetFirmwareIdCore_generalSingle::prepareAllXramRef()
{
    int chipIndex = m_getFirmwareIdParameter->getSpecificChip();

    /* prepareXramRef for specific chip */
    prepareXramRef(chipIndex);
}

void
GetFirmwareIdCore_generalSingle::checkInputConut(int inputCount)
{
    int chipCount = 1; // single

    if( chipCount != inputCount )
    {
        /* stop if input conut not match chip count */
        std::string msg = EXCEPTION_TITLE;
        char errorMsg[1024] = "";
        sprintf(errorMsg, "checkInputConut : not match single ! chipCount(%d), inputCount(%d)\n"
                ,chipCount, inputCount);
        msg.append(errorMsg);
        throw CTException( msg );
    }
}

CTExitCode
GetFirmwareIdCore_generalSingle::showFirmwareId()
{
    int chipIndex = m_getFirmwareIdParameter->getSpecificChip();

    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s Firmware Id : %s\n",
                    ISiSProcedure::getCIStr(chipIndex).c_str(),
                    getGetFirmwareIdReference(RS_XRAM, chipIndex)->getFirmwareID()->getSerial().c_str() );

    return CT_EXIT_PASS;
}

CTExitCode
GetFirmwareIdCore_generalSingle::checkFirmwareIdUsingInput()
{
    int chipIndex = m_getFirmwareIdParameter->getSpecificChip();

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

    return CT_EXIT_PASS;
}

CTExitCode
GetFirmwareIdCore_generalSingle::checkFirmwareIdUsingFile()
{
    prepareAllBin();

    int chipIndex = m_getFirmwareIdParameter->getSpecificChip();

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

    return CT_EXIT_PASS;
}
