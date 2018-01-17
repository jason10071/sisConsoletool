#ifndef UPDATEFWCORE_819SINGLE
#define UPDATEFWCORE_819SINGLE

#include "updatefwcore/flowversion/updatefwcore_generalsingle.h"

namespace CT
{
class UpdateFWCore_819Single : public UpdateFWCore_generalSingle
{

public:
    explicit UpdateFWCore_819Single(UpdateFWParameter* updateFWParameter);
    virtual ~UpdateFWCore_819Single();

    //virtual CTExitCode exec();

protected:
    virtual void modifyUpdateStamp(int chipIndex);
};

} // CT

#endif // UPDATEFWCORE_819SINGLE

