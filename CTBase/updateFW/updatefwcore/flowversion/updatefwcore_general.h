#ifndef UPDATEFWCORE_GENERAL_H
#define UPDATEFWCORE_GENERAL_H

#include "updatefwcore/iupdatefwcore.h"

namespace CT
{

class UpdateFWCore_general : public IUpdateFWCore
{
public:
    explicit UpdateFWCore_general(UpdateFWParameter* updateFWParameter);
    virtual ~UpdateFWCore_general();

    virtual CTExitCode exec();

protected:
    virtual void prepareBinRef(int chipIndex);
    virtual void prepareXramRef(int chipIndex);
    virtual void prepareRomRef(int chipIndex);
    virtual void showInfo(int chipIndex);
    virtual void prepareBinMasterRef();
    virtual void prepareXramMasterRef();
    virtual void prepareRomMasterRef();
    virtual void showMasterInfo();
    virtual void prepareBinSlaveRef(int chipIndex);
    virtual void prepareXramSlaveRef(int chipIndex);
    virtual void prepareRomSlaveRef(int chipIndex);
    virtual void showSlaveInfo(int chipIndex);
    virtual void modifyUpdateStamp(int chipIndex);

    virtual void checkFullBinCount();
    virtual void checkAllBinSize();
    virtual void checkAllBinDeviceType();
    virtual void checkAllBinInterfaceID();
    virtual void checkAllBinSelectiveID();
    virtual void checkMasterBinProductID();
    virtual void checkAllBinProductID();

    virtual bool ifNeedMasterRecovery();
    virtual bool ifNeedRestructureBootloader();
    virtual bool ifNeedRestructureBootloader(int chipIndex);
    virtual bool ifNeedUpdateBootloader(int chipIndex);

    virtual void confirmUpdate();

    virtual void clearBootFlag(int chipIndex);
    virtual void masterRecovery();
    virtual void restructureBootloader();
    virtual void restructureBootloader(int chipIndex);
    virtual void doUpdateFW();
    virtual void doUpdateFW(int chipIndex);
};

} // CT

#endif // UPDATEFWCORE_GENERAL_H
