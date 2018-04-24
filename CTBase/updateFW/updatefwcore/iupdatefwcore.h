#ifndef IUPDATEFWCORE_H
#define IUPDATEFWCORE_H

#include "ctbasecore/ctbasecore.h"
#include "updatefwcore/updatefwparameter.h"
#include "updatefwcore/updatefwreference.h"

#define SYMBOL_DO_UPDATE "[->]"
#define SYMBOL_CTL_FLOW "[?]"

namespace CT
{

class IUpdateFWCore : public CTBaseCore
{
public:
    explicit IUpdateFWCore(UpdateFWParameter* updateFWParameter);
    virtual ~IUpdateFWCore();

protected:
    virtual CTBaseReference* generateReference();
    virtual UpdateFWReference* getUpdateFWReference(ReferenceSource rs, int chipIndex);

    virtual void prepareAllBin();

    void resetDevice();

    virtual void prepareBinMasterRef() = 0;
    virtual void prepareXramMasterRef() = 0;
    virtual void prepareRomMasterRef() = 0;
    virtual void showMasterInfo() = 0;
    virtual void prepareBinSlaveRef(int chipIndex) = 0;
    virtual void prepareXramSlaveRef(int chipIndex) = 0;
    virtual void prepareRomSlaveRef(int chipIndex) = 0;
    virtual void showSlaveInfo(int chipIndex) = 0;
    virtual void modifyUpdateStamp(int chipIndex) = 0;

    virtual void checkFullBinCount() = 0;
    virtual void checkAllBinSize() = 0;
    virtual void checkAllBinDeviceType() = 0;
    virtual void checkAllBinInterfaceID() = 0;
    virtual void checkAllBinSelectiveID() = 0;
    virtual void checkMasterBinProductID() = 0;
    virtual void checkAllBinProductID() = 0;
    virtual bool ifNeedMasterRecovery() = 0;
    virtual bool ifNeedRestructureBootloader() = 0;
    virtual bool ifNeedRestructureBootloader(int chipIndex) = 0;
    virtual bool ifNeedUpdateBootloader(int chipIndex) = 0;

    virtual void confirmUpdate() = 0;

    virtual void clearBootFlag(int chipIndex) = 0;
    virtual void masterRecovery() = 0;
    virtual void restructureBootloader() = 0;
    virtual void restructureBootloader(int chipIndex) = 0;
    virtual void doUpdateFW() = 0;
    virtual void doUpdateFW(int chipIndex) = 0;

protected:
    UpdateFWParameter* m_updateFWParameter;
};

} // CT

#endif // IUPDATEFWCORE_H
