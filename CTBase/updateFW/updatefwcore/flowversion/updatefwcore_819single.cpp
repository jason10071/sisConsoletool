#include "updatefwcore_819single.h"

#include "sislog.h"
#include "ctexception/ctexception.h"
#include "spexception/spexception.h"
#include "binloader/binloader.h"

using namespace CT;
using namespace SiS;
using namespace SiS::Procedure;

#define EXCEPTION_TITLE "UpdateFWCore_819Single Exception : "
#define TAG "UpdateFWCore_819Single"

UpdateFWCore_819Single::UpdateFWCore_819Single(UpdateFWParameter* updateFWParameter) :
    UpdateFWCore_819(updateFWParameter)
{

}

UpdateFWCore_819Single::~UpdateFWCore_819Single()
{

}

CTExitCode
UpdateFWCore_819Single::exec()
{
    int chipIndex = m_updateFWParameter->getSpecificChip();

    /* prepareBaseXramMasterRef */
    if( chipIndex != CI_MASTER )
    {
        prepareBaseXramMasterRef(true); // stop if broken
    }

    /* prepare All bin */
    prepareAllBin();

    /* prepare Bin Master Ref */
    prepareBinRef(chipIndex);

    /* prepare Xram Master Ref */
    prepareXramRef(chipIndex);

    /* prepare Rom Master Ref */
    prepareRomRef(chipIndex);

    /* show Master Info */
    showInfo(chipIndex);
    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "");

    /* modify Update Stamp */
    if( !m_updateFWParameter->getDisableStamp() )
    {
        modifyUpdateStamp(chipIndex);
    }

    /* check */
    checkAllBinSize();
    checkAllBinDeviceType();
    checkAllBinInterfaceID();
    checkAllBinSelectiveID();
    checkAllBinProductID();
    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "");

    /* confirm update */
    confirmUpdate();

    if( ifNeedRestructureBootloader(chipIndex) )
    {
        std::string msg = EXCEPTION_TITLE;
        char errorMsg[1024] = "";
        sprintf(errorMsg, "ifNeedRestructureBootloader : Yes ! single not support",
                ISiSProcedure::getCIStr(chipIndex).c_str() );
        msg.append(errorMsg);
        throw CTException( msg );
    }

    /* do UpdateFW */
    doUpdateFW(chipIndex);

    resetDevice();

    return CT_EXIT_PASS;
}
