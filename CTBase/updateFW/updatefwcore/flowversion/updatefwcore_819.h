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
    virtual void modifyUpdateStamp(int chipIndex);

    virtual bool ifNeedRestructureBootloader();
    virtual bool ifNeedRestructureBootloader(int chipIndex);

    virtual void restructureBootloader();
    virtual void restructureBootloader(int chipIndex);
};

} // CT

#endif // UPDATEFWCORE_819_H
