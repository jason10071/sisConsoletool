#include "binfileparameter.h"

#include "sislog.h"
#include "ctexception/ctexception.h"
#include "sisprocedure/isisprocedure.h"

#include "sisbin/sisbinmanager.h"

#include <iostream>
#include <string>
#include <sstream>

using namespace CT;
using namespace CT::BFP;
using namespace SiS;
using namespace SiS::Procedure;

#define EXCEPTION_TITLE "BinFileParameter Exception : "
#define TAG "BinFileParameter"

BinFileParameter::BinFileParameter() :
    CTBaseParameter(),
    m_binDir("")
{

}

BinFileParameter::~BinFileParameter()
{
    clearBinWrapMap();
}

void
BinFileParameter::showHelp()
{
    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s=<DIRECTORY>", ARG_BIN_DIR.c_str() );
    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "\t#set bin directory for selecting bin files" );
    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "\t#Name of bin file must be named end with M.bin (S0.bin) if multi\n" );

    CTBaseParameter::showHelp();
}

bool
BinFileParameter::isLegalArgument(ArgumentExpression* argumentExpression)
{
    /* super */
    if( CTBaseParameter::isLegalArgument(argumentExpression) )
    {
        return true;
    }

    /* this */
    if( argumentExpression->getName().compare( ARG_BIN_DIR ) == 0  &&
             (argumentExpression->getAttribute() == ArgumentExpression::ARGUMENT_VALUE ||
              argumentExpression->getAttribute() == ArgumentExpression::ARGUMENT_DIRECTORY))
    {
        return true;
    }
    else if( argumentExpression->getAttribute() == ArgumentExpression::ARGUMENT_UNKNOW )
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool
BinFileParameter::setArgument(ArgumentExpression* argumentExpression)
{
    /* super */
    if( CTBaseParameter::setArgument(argumentExpression) )
    {
        return true;
    }

    /* this */
    if( argumentExpression->getName().compare( ARG_BIN_DIR ) == 0 )
    {
        this->m_binDir = argumentExpression->getValue();
        SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "set bin directory : %s", m_binDir.c_str());
        return true;
    }
    else if( contains(argumentExpression->getName()) )
    {
        if( argumentExpression->getAttribute() == ArgumentExpression::ARGUMENT_UNKNOW )
        {
            int chipIndex = 0;
            if( m_binWrapMap.size() == 0 )
            {
                chipIndex = CI_MASTER;
            }
            else
            {
                chipIndex = CI_SLAVE_0 + (m_binWrapMap.size() - 1);
            }

            SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "set %s-bin : %s",
                            ISiSProcedure::getCIStr(chipIndex).c_str(),
                            argumentExpression->getName().c_str() );
            setBinWrapMap(chipIndex, new BinWrap(argumentExpression->getName(), chipIndex));
            return true;
        }
    }

    return false;
}

bool
BinFileParameter::hasArgumentConflict()
{
    /* super */
    if( CTBaseParameter::hasArgumentConflict() )
    {
        return true;
    }

    /* this */
    if( m_binWrapMap.size() > 0 && !m_binDir.empty() )
    {
        SIS_LOG_E(SiSLog::getOwnerSiS(), TAG, "{BIN_FILE} and -bindir is conflict");
        return true;
    }
    else if( !m_binDir.empty() )
    {
        /* assignBinWrapMapFromBinDir */
        assignBinWrapMapFromBinDir(m_binDir);
    }

    return false;
}

void
BinFileParameter::assignBinWrapMapFromBinDir(std::string binDir)
{
    SIS_LOG_D(SiSLog::getOwnerSiS(), TAG, "getBinMap from %s", binDir.c_str() );
    std::map<BinIndex,BinName> binMap = SiSBinManager::getBinMap(binDir);

    if( binMap.size() == 0 )
    {
        std::string msg = EXCEPTION_TITLE;
        char errorMsg[1024] = "";
        sprintf(errorMsg, "can't find bin file from %s\n", binDir.c_str());
        msg.append(errorMsg);
        throw CTException( msg );
    }

    for(std::map<BinIndex,BinName>::iterator iter = binMap.begin(); iter != binMap.end(); ++iter)
    {
        BinIndex binIndex = iter->first;
        BinName binName = iter->second;

        SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "set %s-bin : %s",
                        ISiSProcedure::getCIStr(binIndex).c_str(),
                        binName.c_str() );
        setBinWrapMap(binIndex, new BinWrap(binName, binIndex));
    }
}

std::map<ChipIndexKey, BinWrap*>&
BinFileParameter::getBinWrapMap()
{
    return this->m_binWrapMap;
}

BinWrap*
BinFileParameter::getBinWrapMap(int chipIndex)
{
    std::map<ChipIndexKey, BinWrap*>::iterator it = m_binWrapMap.find( chipIndex );
    if( it == m_binWrapMap.end() )
    {
        std::string msg = EXCEPTION_TITLE;
        char errorMsg[1024] = "";
        sprintf(errorMsg, "no %s-bin setting", ISiSProcedure::getCIStr(chipIndex).c_str());
        msg.append(errorMsg);
        throw CTException( msg );
    }

    return it->second;
}

std::string
BinFileParameter::getBinDir()
{
    return this->m_binDir;
}

void
BinFileParameter::setBinWrapMap(std::map<ChipIndexKey, BinWrap*> binWrapMap)
{
    clearBinWrapMap();
    m_binWrapMap = binWrapMap;
}

void
BinFileParameter::setBinWrapMap(int chipIndex, BinWrap* binWrap)
{
    std::map<ChipIndexKey, BinWrap*>::iterator it = m_binWrapMap.find( chipIndex );
    if( it != m_binWrapMap.end() )
    {
        BinWrap* binWrap = it->second;
        m_binWrapMap.erase( it->first );
        delete binWrap;
        binWrap = 0;
    }

    m_binWrapMap[chipIndex] = binWrap;
}

void
BinFileParameter::clearBinWrapMap()
{
    /* delete m_binWrapMap */
    for(std::map<ChipIndexKey, BinWrap*>::iterator it = m_binWrapMap.begin(); it != m_binWrapMap.end(); ++it)
    {
        BinWrap* binWrap = it->second;
        delete binWrap;
        binWrap = 0;
    }
    m_binWrapMap.clear();
}

void
BinFileParameter::setBinDir(std::string binDir)
{
    this->m_binDir = binDir;
}
