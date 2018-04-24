#include "getfirmwareinfocore_general.h"

#include "sislog.h"
#include "ctexception/ctexception.h"
#include "spexception/spexception.h"

using namespace CT;
using namespace SiS;
using namespace SiS::Procedure;

#define EXCEPTION_TITLE "GetFirmwareInfoCore_general Exception : "
#define TAG "GetFirmwareInfoCore_general"

GetFirmwareInfoCore_general::GetFirmwareInfoCore_general(GetFirmwareInfoParameter* getFirmwareInfoParameter) :
    IGetFirmwareInfoCore(getFirmwareInfoParameter)
{

}

GetFirmwareInfoCore_general::~GetFirmwareInfoCore_general()
{

}

CTExitCode
GetFirmwareInfoCore_general::exec()
{
    /* showList */
    if( this->m_getFirmwareInfoParameter->getShowList() )
    {
        return showList();
    }

    /* prepareBaseXramMasterRef */
    prepareBaseXramMasterRef(true); // stop if broken

    /* do getFirmwareInfo function */
    if( this->m_getFirmwareInfoParameter->getAddress() != INVALID_ADDRESS )
    {
        return getInfoUsingAddress();
    }
    else if( !this->m_getFirmwareInfoParameter->getKeyword().empty() )
    {
        return getInfoUsingKeyword();
    }
    else
    {
        return getInfoDefault();
    }

    return undoException(EXCEPTION_TITLE);
}

CTExitCode
GetFirmwareInfoCore_general::showList()
{
    IAttributeReader* attributeReader = m_sisProcedure->generateAttributeReader();

    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s", attributeReader->getAddressProviderTableName().c_str());
    attributeReader->getAddressProvider()->showAddressTable(AddressAttribute::AAC_ALL);

    delete attributeReader;
    return CT_EXIT_PASS;
}

CTExitCode
GetFirmwareInfoCore_general::getInfoUsingAddress()
{
    unsigned int address = this->m_getFirmwareInfoParameter->getAddress();
    int length = this->m_getFirmwareInfoParameter->getLength();
    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "0x%08x (0x%x) :", address, length);

    GetFirmwareInfoReference* xramMasterRef = getGetFirmwareInfoReference(RS_XRAM, CI_MASTER);
    for(int chipIndex = CI_MASTER; chipIndex < xramMasterRef->getSlaveNum() + 1; chipIndex++)
    {
        SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s :", ISiSProcedure::getCIStr(chipIndex).c_str());

        SerialData* data = m_sisProcedure->readXramAddress(address, length, chipIndex);
        showSerialData(data, this->m_getFirmwareInfoParameter->getShowEngineerMode());
        delete data;
    }

    return CT_EXIT_PASS;
}

CTExitCode
GetFirmwareInfoCore_general::getInfoUsingKeyword()
{
    std::string keyword = this->m_getFirmwareInfoParameter->getKeyword();
    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s :", keyword.c_str());

    GetFirmwareInfoReference* xramMasterRef = getGetFirmwareInfoReference(RS_XRAM, CI_MASTER);
    for(int chipIndex = CI_MASTER; chipIndex < xramMasterRef->getSlaveNum() + 1; chipIndex++)
    {
        SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s :", ISiSProcedure::getCIStr(chipIndex).c_str());

        SerialData* data = m_sisProcedure->readXramAttribute(keyword, chipIndex);
        showSerialData(data, this->m_getFirmwareInfoParameter->getShowEngineerMode());
        delete data;
    }

    return CT_EXIT_PASS;
}

CTExitCode
GetFirmwareInfoCore_general::getInfoDefault()
{
    /* 1. Device build info */
    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "== Device build info ==");
    std::string keyword = "FA_DEVICE_BUILD_INFO_XRAM";
    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s :", keyword.c_str());

    GetFirmwareInfoReference* xramMasterRef = getGetFirmwareInfoReference(RS_XRAM, CI_MASTER);
    for(int chipIndex = CI_MASTER; chipIndex < xramMasterRef->getSlaveNum() + 1; chipIndex++)
    {
        SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s :", ISiSProcedure::getCIStr(chipIndex).c_str());

        SerialData* data = m_sisProcedure->readXramAttribute(keyword, chipIndex);
        showSerialData(data, this->m_getFirmwareInfoParameter->getShowEngineerMode());
        delete data;
    }

    /* 2. Device build info */
    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "== Device fw info ==");
    keyword = "FA_DEVICE_FW_INFO_XRAM";
    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s :", keyword.c_str());

    for(int chipIndex = CI_MASTER; chipIndex < xramMasterRef->getSlaveNum() + 1; chipIndex++)
    {
        SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s :", ISiSProcedure::getCIStr(chipIndex).c_str());

        SerialData* data = m_sisProcedure->readXramAttribute(keyword, chipIndex);
        showSerialData(data, this->m_getFirmwareInfoParameter->getShowEngineerMode());
        delete data;
    }

    return CT_EXIT_PASS;
}
