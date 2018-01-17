#include "updatefwcore_819single.h"

#include "sislog.h"
#include "ctexception/ctexception.h"
#include "spexception/spexception.h"
#include "binloader/binloader.h"

using namespace CT;
using namespace SiS;
using namespace SiS::Procedure;

#define EXCEPTION_TITLE "UpdateFWCore_819Single Exception : "

UpdateFWCore_819Single::UpdateFWCore_819Single(UpdateFWParameter* updateFWParameter) :
    UpdateFWCore_generalSingle(updateFWParameter)
{

}

UpdateFWCore_819Single::~UpdateFWCore_819Single()
{

}

void
UpdateFWCore_819Single::modifyUpdateStamp(int chipIndex)
{
    // todo
}
