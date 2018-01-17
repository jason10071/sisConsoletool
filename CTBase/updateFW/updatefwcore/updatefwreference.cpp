#include "updatefwreference.h"
#include "addressprovider/addresstable/attributekeydefine.h"
#include "sislog.h"

using namespace CT;
using namespace SiS::Procedure;

UpdateFWReference::UpdateFWReference() :
    CTBaseReference(),
    m_isRecoveryDone(false),
    m_isRestructureBootloaderDone(false)
{

}

UpdateFWReference::~UpdateFWReference()
{

}

bool
UpdateFWReference::getIsRecoveryDone()
{
    return this->m_isRecoveryDone;
}

bool
UpdateFWReference::getIsRestructureBootloaderDone()
{
    return this->m_isRestructureBootloaderDone;
}

void
UpdateFWReference::setIsRecoveryDone(bool isRecoveryDone)
{
    this->m_isRecoveryDone = isRecoveryDone;
}

void
UpdateFWReference::setIsRestructureBootloaderDone(bool isRestructureBootloaderDone)
{
    this->m_isRestructureBootloaderDone = isRestructureBootloaderDone;
}
