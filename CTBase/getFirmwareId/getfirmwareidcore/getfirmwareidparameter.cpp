#include "getfirmwareidparameter.h"

#include "sislog.h"
#include "ctexception/ctexception.h"
#include "sisprocedure/isisprocedure.h"

#include <iostream>
#include <string>
#include <sstream>

using namespace CT;
using namespace CT::BFP;
using namespace CT::GFIDP;
using namespace SiS;
using namespace SiS::Procedure;

#define EXCEPTION_TITLE "GetFirmwareIdParameter Exception : "
#define TAG "GetFirmwareIdParameter"

GetFirmwareIdParameter::GetFirmwareIdParameter() :
    BinFileParameter()
{

}

GetFirmwareIdParameter::~GetFirmwareIdParameter()
{

}

void
GetFirmwareIdParameter::showHelp()
{
    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s=<VALUE>", ARG_INPUT_MASTER.c_str() );
    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "\t#set master input for checking firmwareId\n" );

    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s=<VALUE>", ARG_INPUT_SLAVE.c_str() );
    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "\t#set slave%s input for checking firmwareId\n", EXPRESSION_KEYWORD_SLAVE_INDEX );

    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s=<FILE>", ARG_BIN_FILE.c_str() );
    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "\t@<FILE>=[*.bin]" );
    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "\t#set master/slave%s bin file for checking firmwareId. (EX: M.bin S0.bin S1.bin ...)\n", EXPRESSION_KEYWORD_SLAVE_INDEX );

    BinFileParameter::showHelp();
}

bool
GetFirmwareIdParameter::isLegalArgument(ArgumentExpression* argumentExpression)
{
    /* super */
    if( BinFileParameter::isLegalArgument(argumentExpression) )
    {
        return true;
    }

    /* this */
    if( argumentExpression->getName().find( ARG_INPUT_MASTER ) == 0 &&
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
GetFirmwareIdParameter::setArgument(ArgumentExpression* argumentExpression)
{
    /* this */
    if( contains(argumentExpression->getName()) )
    {
        if( argumentExpression->getName().find( ARG_INPUT_MASTER ) == 0 )
        {
            int chipIndex = 0;
            if( argumentExpression->getName().compare( ARG_INPUT_MASTER ) == 0 )
            {
                chipIndex = CI_MASTER;
            }
            else
            {
                std::string SLAVE_KEYWORD = "-input-s";
                std::string slaveIndex = argumentExpression->getName().
                        substr( argumentExpression->getName().find(SLAVE_KEYWORD) + SLAVE_KEYWORD.length() );

                int slaveIndexNum;
				stringstream ss(slaveIndex);
                ss >> slaveIndexNum;
                chipIndex = CI_SLAVE_0 + slaveIndexNum;
            }

            SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "set %s-input : %s",
                            ISiSProcedure::getCIStr(chipIndex).c_str(),
                            argumentExpression->getValue().c_str() );
            setFirmwareCheckIdMap( chipIndex, argumentExpression->getValue() );
            return true;

        }
    }

    /* super */
    if( BinFileParameter::setArgument(argumentExpression) )
    {
        return true;
    }

    return false;
}

bool
GetFirmwareIdParameter::hasArgumentConflict()
{
    /* super */
    if( BinFileParameter::hasArgumentConflict() )
    {
        return true;
    }

    /* this */
    if( m_firmwareCheckIdMap.size() > 0 && m_binWrapMap.size() > 0 )
    {
        SIS_LOG_E(SiSLog::getOwnerSiS(), TAG, "-input and bin-file is conflict");
        return true;
    }

    return false;
}

std::map<ChipIndexKey, std::string>&
GetFirmwareIdParameter::getFirmwareCheckIdMap()
{
    return this->m_firmwareCheckIdMap;
}

std::string
GetFirmwareIdParameter::getFirmwareCheckIdMap(int chipIndex)
{
    std::map<ChipIndexKey, std::string>::iterator it = m_firmwareCheckIdMap.find( chipIndex );
    if( it == m_firmwareCheckIdMap.end() )
    {
        std::string msg = EXCEPTION_TITLE;
        char errorMsg[1024] = "";
        sprintf(errorMsg, "no %s-input setting", ISiSProcedure::getCIStr(chipIndex).c_str());
        msg.append(errorMsg);
        throw CTException( msg );
    }

    return it->second;
}

void
GetFirmwareIdParameter::setFirmwareCheckIdMap(int chipIndex, std::string checkId)
{
    std::map<ChipIndexKey, std::string>::iterator it = m_firmwareCheckIdMap.find( chipIndex );
    if( it != m_firmwareCheckIdMap.end() )
    {
        m_firmwareCheckIdMap.erase( it->first );
    }

    m_firmwareCheckIdMap[chipIndex] = checkId;
}

void
GetFirmwareIdParameter::clearFirmwareCheckIdMap()
{
    /* delete m_firmwareCheckIdMap */
    m_firmwareCheckIdMap.clear();
}
