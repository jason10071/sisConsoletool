#ifndef UPDATEFWCORE_H
#define UPDATEFWCORE_H

#include "updatefwcore/iupdatefwcore.h"
#include "updatefwcore/flowversion/updatefwcore_general.h"
#include "updatefwcore/flowversion/updatefwcore_819.h"
#include "updatefwcore/flowversion/updatefwcore_generalsingle.h"
#include "updatefwcore/flowversion/updatefwcore_819single.h"

namespace CT
{

class UpdateFWCore : public IUpdateFWCore
{
public:
    explicit UpdateFWCore(UpdateFWParameter* updateFWParameter);
    virtual ~UpdateFWCore();

    void switchFlowVersion(DeviceType deviceType);

    virtual void setSiSProcedure(SiSProcedure* sisProcedure);

    virtual void init();
    virtual CTExitCode exec();
    virtual void destroy(bool isEnableCtlReportToOs = true);

protected:
    virtual void prepareBinMasterRef() {}
    virtual void prepareXramMasterRef() {}
    virtual void prepareRomMasterRef() {}
    virtual void showMasterInfo() {}
    virtual void prepareBinSlaveRef(int chipIndex) {}
    virtual void prepareXramSlaveRef(int chipIndex) {}
    virtual void prepareRomSlaveRef(int chipIndex) {}
    virtual void showSlaveInfo(int chipIndex) {}
    virtual void modifyUpdateStamp(int chipIndex) {}

    virtual void checkFullBinCount() {}
    virtual void checkAllBinSize() {}
    virtual void checkAllBinDeviceType() {}
    virtual void checkAllBinInterfaceID() {}
    virtual void checkAllBinSelectiveID() {}
    virtual void checkMasterBinProductID() {}
    virtual void checkAllBinProductID() {}
    virtual bool ifNeedMasterRecovery() { return false; }
    virtual bool ifNeedRestructureBootloader() { return false; }
    virtual bool ifNeedRestructureBootloader(int chipIndex) { return false; }
    virtual bool ifNeedUpdateBootloader(int chipIndex) { return false; }

    virtual void confirmUpdate() {}

    virtual void clearBootFlag(int chipIndex) {}
    virtual void masterRecovery() {}
    virtual void restructureBootloader() {}
    virtual void restructureBootloader(int chipIndex) {}
    virtual void doUpdateFW() {}
    virtual void doUpdateFW(int chipIndex) {}

private:
    UpdateFWCore_general* getUpdateFWCore_general();
    UpdateFWCore_819* getUpdateFWCore_819();
    UpdateFWCore_generalSingle* getUpdateFWCore_generalSingle();
    UpdateFWCore_819Single* getUpdateFWCore_819Single();

private:
    IUpdateFWCore* m_iUpdateFWCore;
    UpdateFWCore_general* m_updateFWCore_general;
    UpdateFWCore_819* m_updateFWCore_819;
    UpdateFWCore_generalSingle* m_updateFWCore_generalSingle;
    UpdateFWCore_819Single* m_updateFWCore_819Single;

};

} // CT

#endif // UPDATEFWCORE_H
