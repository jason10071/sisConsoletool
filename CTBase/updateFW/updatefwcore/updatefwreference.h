#ifndef UPDATEFWREFERENCE_H
#define UPDATEFWREFERENCE_H

#include "ctbasecore/ctbasereference.h"

namespace CT
{

class UpdateFWReference : public CTBaseReference
{
public:
    explicit UpdateFWReference();
    virtual ~UpdateFWReference();

    bool getIsRecoveryDone();
    bool getIsRestructureBootloaderDone();

    void setIsRecoveryDone(bool isRecoveryDone);
    void setIsRestructureBootloaderDone(bool isRestructureBootloaderDone);

private:
    bool m_isRecoveryDone;
    bool m_isRestructureBootloaderDone;

};

} // CT

#endif // UPDATEFWREFERENCE_H
