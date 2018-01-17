#ifndef UPDATEFWCORE_GENERALSINGLE
#define UPDATEFWCORE_GENERALSINGLE

#include "updatefwcore/flowversion/updatefwcore_general.h"

namespace CT
{
class UpdateFWCore_generalSingle : public UpdateFWCore_general
{

public:
    explicit UpdateFWCore_generalSingle(UpdateFWParameter* updateFWParameter);
    virtual ~UpdateFWCore_generalSingle();

    virtual CTExitCode exec();
};

} // CT

#endif // UPDATEFWCORE_GENERALSINGLE

