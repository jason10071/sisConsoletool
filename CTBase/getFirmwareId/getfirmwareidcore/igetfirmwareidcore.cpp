#include "igetfirmwareidcore.h"

#include "sislog.h"
#include "ctexception/ctexception.h"

#include <typeinfo>

using namespace CT;
using namespace SiS;
using namespace SiS::Procedure;

#define EXCEPTION_TITLE "IGetFirmwareIdCore Exception : "

IGetFirmwareIdCore::IGetFirmwareIdCore(GetFirmwareIdParameter* getFirmwareIdParameter) :
    CTBaseCore(getFirmwareIdParameter),
    m_getFirmwareIdParameter(getFirmwareIdParameter)
{

}

IGetFirmwareIdCore::~IGetFirmwareIdCore()
{

}

CTBaseReference*
IGetFirmwareIdCore::generateReference()
{
    return new GetFirmwareIdReference();
}

GetFirmwareIdReference*
IGetFirmwareIdCore::getGetFirmwareIdReference(ReferenceSource rs, int chipIndex)
{
    CTBaseReference* ctBaseRef = getCTBaseReference(rs, chipIndex);

    try
    {
        GetFirmwareIdReference* gRef = dynamic_cast<GetFirmwareIdReference*>(ctBaseRef);
        return gRef;
    }
    catch(bad_cast)
    {
        std::string msg = EXCEPTION_TITLE;
        char errorMsg[1024] = "";
        sprintf(errorMsg, "bad_cast GetFirmwareIdReference* : rs=%d, chipIndex=%d\n", rs, chipIndex);
        msg.append(errorMsg);
        throw CTException( msg );
    }
}

void
IGetFirmwareIdCore::prepareAllBin()
{
    for(std::map<ChipIndexKey, BinWrap*>::iterator it = m_getFirmwareIdParameter->getBinWrapMap().begin();
        it != m_getFirmwareIdParameter->getBinWrapMap().end();
        ++it)
    {
        BinWrap* binWrap = it->second;
        binWrap->loadBin();
    }

    m_sisProcedure->setBinWrapMap( m_getFirmwareIdParameter->getBinWrapMap() );
}

