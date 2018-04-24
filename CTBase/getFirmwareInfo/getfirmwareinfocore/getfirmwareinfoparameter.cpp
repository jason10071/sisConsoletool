#include "getfirmwareinfoparameter.h"

#include "sislog.h"
#include "ctexception/ctexception.h"
#include "sisprocedure/isisprocedure.h"

#include <iostream>
#include <string>
#include <sstream>

using namespace CT;
using namespace CT::GFINFOP;
using namespace SiS;
using namespace SiS::Procedure;

#define EXCEPTION_TITLE "GetFirmwareInfoParameter Exception : "
#define TAG "GetFirmwareInfoParameter"

GetFirmwareInfoParameter::GetFirmwareInfoParameter() :
    CTBaseParameter(),
    m_address(INVALID_ADDRESS),
    m_length(1),
    m_keyword(""),
    m_showList(false),
    m_showEngineerMode(false),
    m_autotool(false)
{

}

GetFirmwareInfoParameter::~GetFirmwareInfoParameter()
{

}

void
GetFirmwareInfoParameter::showHelp()
{
    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s=<VALUE>", ARG_ADDRESS.c_str() );
    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "\t#set adderss to get info from. ex: -addr=0xc004\n" );

    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s=<VALUE>", ARG_LENGTH.c_str() );
    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "\t#set length to get info from\n" );

    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s=<VALUE>", ARG_KEYWORD.c_str() );
    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "\t#set keyword to get info from\n" );

    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s", ARG_SHOW_LIST.c_str() );
    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "\t#show keyword list\n" );

    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s", ARG_SHOW_ENGINEER_MODE.c_str() );
    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "\t#show Engineer Mode\n" );

    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s", ARG_AUTOTOOL.c_str() );
    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "\t#show info used by AutoTool\n" );

    CTBaseParameter::showHelp();
}

bool
GetFirmwareInfoParameter::isLegalArgument(ArgumentExpression* argumentExpression)
{
    /* super */
    if( CTBaseParameter::isLegalArgument(argumentExpression) )
    {
        return true;
    }

    /* this */
    if( argumentExpression->getName().compare( ARG_ADDRESS ) == 0 &&
            argumentExpression->getAttribute() == ArgumentExpression::ARGUMENT_VALUE )
    {
        return true;
    }
    else if( argumentExpression->getName().compare( ARG_LENGTH ) == 0 &&
            argumentExpression->getAttribute() == ArgumentExpression::ARGUMENT_VALUE )
    {
        return true;
    }
    else if( argumentExpression->getName().compare( ARG_KEYWORD ) == 0 &&
            argumentExpression->getAttribute() == ArgumentExpression::ARGUMENT_VALUE )
    {
        return true;
    }
    else if( argumentExpression->getName().compare( ARG_SHOW_LIST ) == 0 &&
             argumentExpression->getAttribute() == ArgumentExpression::ARGUMENT_BOOL )
    {
        return true;
    }
    else if( argumentExpression->getName().compare( ARG_SHOW_ENGINEER_MODE ) == 0 &&
             argumentExpression->getAttribute() == ArgumentExpression::ARGUMENT_BOOL )
    {
        return true;
    }
    else if( argumentExpression->getName().compare( ARG_AUTOTOOL ) == 0 &&
             argumentExpression->getAttribute() == ArgumentExpression::ARGUMENT_BOOL )
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool
GetFirmwareInfoParameter::setArgument(ArgumentExpression* argumentExpression)
{
    /* super */
    if( CTBaseParameter::setArgument(argumentExpression) )
    {
        return true;
    }

    /* this */
    if( contains(argumentExpression->getName()) )
    {
        if( argumentExpression->getName().find( ARG_ADDRESS ) == 0 )
        {
            std::string addressStr = argumentExpression->getValue();
            unsigned int address;

            if( addressStr.find("0x") == 0 )
            {
                addressStr = addressStr.substr( addressStr.find("0x") + 2, addressStr.length() - 2 );
                stringstream ss(addressStr);
                ss >> std::hex >> address;
            }
            else
            {
                stringstream ss(addressStr);
                ss >> address;
            }

            this->m_address = address;
            SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "set address : 0x%x", m_address);
            return true;
        }
        else if( argumentExpression->getName().find( ARG_LENGTH ) == 0 )
        {
            std::string lengthStr = argumentExpression->getValue();
            int length;

            if( lengthStr.find("0x") == 0 )
            {
                lengthStr = lengthStr.substr( lengthStr.find("0x") + 2, lengthStr.length() - 2 );
                stringstream ss(lengthStr);
                ss >> std::hex >> length;
            }
            else
            {
                stringstream ss(lengthStr);
                ss >> length;
            }

            this->m_length = length;
            if( m_length > 0 )
            {
                SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "set length : 0x%x", m_length);
                return true;
            }
            else
            {
                std::string msg = EXCEPTION_TITLE;
                char errorMsg[1024] = "";
                sprintf(errorMsg, "set length FAIL : can't be %d\n", m_length);
                msg.append(errorMsg);
                throw CTException( msg );
            }
            return true;
        }
        else if( argumentExpression->getName().find( ARG_KEYWORD ) == 0 )
        {
            this->m_keyword = argumentExpression->getValue();
            SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "set keyword : %s", m_keyword.c_str());
            return true;
        }
        else if( argumentExpression->getName().find( ARG_SHOW_LIST ) == 0 )
        {
            if( argumentExpression->getValueToBool() )
            {
                this->m_showList = true;
                SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "set showList : true");
            }
            else
            {
                this->m_showList = false;
                SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "set showList : false");
            }
            return true;
        }
        else if( argumentExpression->getName().find( ARG_SHOW_ENGINEER_MODE ) == 0 )
        {
            if( argumentExpression->getValueToBool() )
            {
                this->m_showEngineerMode = true;
                SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "set showEngineerMode : true");
            }
            else
            {
                this->m_showEngineerMode = false;
                SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "set showEngineerMode : false");
            }
            return true;
        }
        else if( argumentExpression->getName().find( ARG_AUTOTOOL ) == 0 )
        {
            if( argumentExpression->getValueToBool() )
            {
                this->m_autotool = true;
                SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "set autotool : true");
            }
            else
            {
                this->m_autotool = false;
                SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "set autotool : false");
            }
            return true;
        }
    }

    return false;
}

bool
GetFirmwareInfoParameter::hasArgumentConflict()
{
    /* super */
    if( CTBaseParameter::hasArgumentConflict() )
    {
        return true;
    }

    /* this */
    if( this->m_address != INVALID_ADDRESS && !m_keyword.empty() )
    {
        SIS_LOG_E(SiSLog::getOwnerSiS(), TAG, "-addr and -kw is conflict");
        return true;
    }

    return false;
}


unsigned int
GetFirmwareInfoParameter::getAddress()
{
    return this->m_address;
}

int
GetFirmwareInfoParameter::getLength()
{
    return this->m_length;
}

std::string
GetFirmwareInfoParameter::getKeyword()
{
    return this->m_keyword;
}

bool
GetFirmwareInfoParameter::getShowList()
{
    return this->m_showList;
}

bool
GetFirmwareInfoParameter::getShowEngineerMode()
{
    return this->m_showEngineerMode;
}

bool
GetFirmwareInfoParameter::getAutotool()
{
    return this->m_autotool;
}

void
GetFirmwareInfoParameter::setAddress(unsigned int address)
{
    this->m_address = address;
}

void
GetFirmwareInfoParameter::setLength(int length)
{
    this->m_length = length;
}

void
GetFirmwareInfoParameter::setKeyword(std::string keyword)
{
    this->m_keyword = keyword;
}

void
GetFirmwareInfoParameter::setShowList(bool showList)
{
    this->m_showList = showList;
}

void
GetFirmwareInfoParameter::setShowEngineerMode(bool showEngineerMode)
{
    this->m_showEngineerMode = showEngineerMode;
}

void
GetFirmwareInfoParameter::setAutotool(bool autotool)
{
    this->m_autotool = autotool;
}
