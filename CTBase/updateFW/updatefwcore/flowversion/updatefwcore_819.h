#ifndef UPDATEFWCORE_819_H
#define UPDATEFWCORE_819_H

#include "updatefwcore/flowversion/updatefwcore_general.h"

namespace CT
{
class UpdateFWCore_819 : public UpdateFWCore_general
{

public:
    explicit UpdateFWCore_819(UpdateFWParameter* updateFWParameter);
    virtual ~UpdateFWCore_819();

    //virtual CTExitCode exec();

protected:
    virtual void showMasterInfo();
    virtual void prepareBinMasterRef();
    virtual void prepareXramMasterRef();
    virtual void showSlaveInfo(int chipIndex);
    virtual void prepareBinSlaveRef(int chipIndex);
    virtual void prepareXramSlaveRef(int chipIndex);

    virtual void modifyUpdateStamp(int chipIndex);

    virtual void checkMasterBinProductID();
    virtual void checkAllBinProductID();

    virtual bool ifNeedRestructureBootloader();
    virtual bool ifNeedRestructureBootloader(int chipIndex);

    virtual void restructureBootloader();
    virtual void restructureBootloader(int chipIndex);
};

} // CT

#endif // UPDATEFWCORE_819_H
