#include "iupdatefwcore.h"

#include "sislog.h"
#include "ctexception/ctexception.h"
#include "binloader/binloader.h"

#include <typeinfo>

//#include "UnderlyingDevDefine.h"
//#include "UnderlyingDevKey.h"
//#include "CptFactoryFacade.h"

using namespace CT;
using namespace SiS;
using namespace SiS::DeviceIO;
using namespace SiS::Procedure;

#define EXCEPTION_TITLE "IUpdateFWCore Exception : "
#define TAG "IUpdateFWCore"

IUpdateFWCore::IUpdateFWCore(UpdateFWParameter* updateFWParameter) :
    CTBaseCore(updateFWParameter),
    m_updateFWParameter(updateFWParameter)
{

}

IUpdateFWCore::~IUpdateFWCore()
{

}

CTBaseReference*
IUpdateFWCore::generateReference()
{
    return new UpdateFWReference();
}

UpdateFWReference*
IUpdateFWCore::getUpdateFWReference(ReferenceSource rs, int chipIndex)
{
    CTBaseReference* ctBaseRef = getCTBaseReference(rs, chipIndex);
    try
    {
        UpdateFWReference* uRef = dynamic_cast<UpdateFWReference*>(ctBaseRef);
        return uRef;
    }
    catch(bad_cast)
    {
        std::string msg = EXCEPTION_TITLE;
        char errorMsg[1024] = "";
        sprintf(errorMsg, "bad_cast UpdateFWReference* : rs=%d, chipIndex=%d\n", rs, chipIndex);
        msg.append(errorMsg);
        throw CTException( msg );
    }
}

void
IUpdateFWCore::prepareAllBin()
{
    for(std::map<ChipIndexKey, BinWrap*>::iterator it = m_updateFWParameter->getBinWrapMap().begin();
        it != m_updateFWParameter->getBinWrapMap().end();
        ++it)
    {
        BinWrap* binWrap = it->second;
        binWrap->loadBin();
    }

    m_sisProcedure->setBinWrapMap( m_updateFWParameter->getBinWrapMap() );
}

void
IUpdateFWCore::resetDevice(bool isCheckDeviceBack)
{
    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s resetDevice", SYMBOL_DO_UPDATE);
    CTBaseCore::resetDevice(isCheckDeviceBack);
}
