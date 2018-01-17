#ifndef IGETFIRMWAREIDCORE_H
#define IGETFIRMWAREIDCORE_H

#include "ctbasecore/ctbasecore.h"
#include "getfirmwareidcore/getfirmwareidparameter.h"
#include "getfirmwareidcore/getfirmwareidreference.h"

namespace CT
{

class IGetFirmwareIdCore : public CTBaseCore
{
public:
    explicit IGetFirmwareIdCore(GetFirmwareIdParameter* getFirmwareIdParameter);
    virtual ~IGetFirmwareIdCore();

protected:
    virtual CTBaseReference* generateReference();
    virtual GetFirmwareIdReference* getGetFirmwareIdReference(ReferenceSource rs, int chipIndex);

    virtual void prepareAllBin();

    virtual void prepareAllXramRef() = 0;
    virtual void prepareXramRef(int chipIndex) = 0;
    virtual void prepareBinRef(int chipIndex) = 0;

    virtual CTExitCode showFirmwareId() = 0;
    virtual CTExitCode checkFirmwareIdUsingInput() = 0;
    virtual CTExitCode checkFirmwareIdUsingFile() = 0;

    virtual void checkInputConut(int inputCount) = 0;

protected:
    GetFirmwareIdParameter* m_getFirmwareIdParameter;
};

} // CT

#endif // IGETFIRMWAREIDCORE_H
