#ifndef UPDATEFWCORE_819SINGLE
#define UPDATEFWCORE_819SINGLE

#include "updatefwcore/flowversion/updatefwcore_819.h"

namespace CT
{
class UpdateFWCore_819Single : public UpdateFWCore_819
{

public:
    explicit UpdateFWCore_819Single(UpdateFWParameter* updateFWParameter);
    virtual ~UpdateFWCore_819Single();

    virtual CTExitCode exec();
};

} // CT

#endif // UPDATEFWCORE_819SINGLE

