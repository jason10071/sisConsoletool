#include "updatefwcore_819.h"

#include "sislog.h"
#include "ctexception/ctexception.h"
#include "spexception/spexception.h"
#include "binloader/binloader.h"

using namespace CT;
using namespace SiS;
using namespace SiS::Procedure;

#define EXCEPTION_TITLE "UpdateFWCore_819 Exception : "

UpdateFWCore_819::UpdateFWCore_819(UpdateFWParameter* updateFWParameter) :
    UpdateFWCore_general(updateFWParameter)
{

}

UpdateFWCore_819::~UpdateFWCore_819()
{

}

void
UpdateFWCore_819::modifyUpdateStamp(int chipIndex)
{
    // todo
}

bool
UpdateFWCore_819::ifNeedRestructureBootloader()
{
    // not implement
    return false;
}

bool
UpdateFWCore_819::ifNeedRestructureBootloader(int chipIndex)
{
    // not implement
    return false;
}

void
UpdateFWCore_819::restructureBootloader()
{
    // not implement
}

void
UpdateFWCore_819::restructureBootloader(int chipIndex)
{
    // not implement
}
