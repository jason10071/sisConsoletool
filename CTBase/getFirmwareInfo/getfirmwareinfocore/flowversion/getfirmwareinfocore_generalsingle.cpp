#include "getfirmwareinfocore_generalsingle.h"

#include "sislog.h"
#include "ctexception/ctexception.h"
#include "spexception/spexception.h"

using namespace CT;
using namespace SiS;
using namespace SiS::Procedure;

#define EXCEPTION_TITLE "GetFirmwareInfoCore_generalSingle Exception : "
#define TAG "GetFirmwareInfoCore_generalSingle"

GetFirmwareInfoCore_generalSingle::GetFirmwareInfoCore_generalSingle(GetFirmwareInfoParameter* getFirmwareInfoParameter) :
    GetFirmwareInfoCore_general(getFirmwareInfoParameter)
{

}

GetFirmwareInfoCore_generalSingle::~GetFirmwareInfoCore_generalSingle()
{

}

CTExitCode
GetFirmwareInfoCore_generalSingle::getInfoUsingAddress()
{
    unsigned int address = this->m_getFirmwareInfoParameter->getAddress();
    int length = this->m_getFirmwareInfoParameter->getLength();
    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "0x%08x (0x%x) :", address, length);

    int chipIndex = m_getFirmwareInfoParameter->getSpecificChip();

    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s :", ISiSProcedure::getCIStr(chipIndex).c_str());

    SerialData* data = m_sisProcedure->readXramAddress(address, length, chipIndex);
    showSerialData(data, this->m_getFirmwareInfoParameter->getShowEngineerMode());
    delete data;

    return CT_EXIT_PASS;
}

CTExitCode
GetFirmwareInfoCore_generalSingle::getInfoUsingKeyword()
{
    std::string keyword = this->m_getFirmwareInfoParameter->getKeyword();
    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s :", keyword.c_str());

    int chipIndex = m_getFirmwareInfoParameter->getSpecificChip();

    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s :", ISiSProcedure::getCIStr(chipIndex).c_str());

    SerialData* data = m_sisProcedure->readXramAttribute(keyword, chipIndex);
    showSerialData(data, this->m_getFirmwareInfoParameter->getShowEngineerMode());
    delete data;

    return CT_EXIT_PASS;
}

CTExitCode
GetFirmwareInfoCore_generalSingle::getInfoDefault()
{
    int chipIndex = m_getFirmwareInfoParameter->getSpecificChip();

    /* 1. Device build info */
    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "== Device build info ==");
    std::string keyword = "FA_DEVICE_BUILD_INFO_XRAM";
    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s :", keyword.c_str());

    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s :", ISiSProcedure::getCIStr(chipIndex).c_str());

    SerialData* data = m_sisProcedure->readXramAttribute(keyword, chipIndex);
    showSerialData(data, this->m_getFirmwareInfoParameter->getShowEngineerMode());
    delete data;

    /* 2. Device build info */
    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "== Device fw info ==");
    keyword = "FA_DEVICE_FW_INFO_XRAM";
    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s :", keyword.c_str());

    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s :", ISiSProcedure::getCIStr(chipIndex).c_str());

    data = m_sisProcedure->readXramAttribute(keyword, chipIndex);
    showSerialData(data, this->m_getFirmwareInfoParameter->getShowEngineerMode());
    delete data;

    return CT_EXIT_PASS;
}
