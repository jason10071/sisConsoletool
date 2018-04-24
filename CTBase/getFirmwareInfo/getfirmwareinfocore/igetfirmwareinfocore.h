#ifndef IGETFIRMWAREINFOCORE_H
#define IGETFIRMWAREINFOCORE_H

#include "ctbasecore/ctbasecore.h"
#include "getfirmwareinfocore/getfirmwareinfoparameter.h"
#include "getfirmwareinfocore/getfirmwareinforeference.h"

namespace CT
{

class IGetFirmwareInfoCore : public CTBaseCore
{
public:
    explicit IGetFirmwareInfoCore(GetFirmwareInfoParameter* getFirmwareInfoParameter);
    virtual ~IGetFirmwareInfoCore();

protected:
    virtual CTBaseReference* generateReference();
    virtual GetFirmwareInfoReference* getGetFirmwareInfoReference(ReferenceSource rs, int chipIndex);

    virtual CTExitCode showList() = 0;
    virtual CTExitCode getInfoUsingAddress() = 0;
    virtual CTExitCode getInfoUsingKeyword() = 0;
    virtual CTExitCode getInfoDefault() = 0;

    void showSerialData(SerialData* serialData, bool showEngineerMode);

protected:
    GetFirmwareInfoParameter* m_getFirmwareInfoParameter;
};

} // CT

#endif // IGETFIRMWAREINFOCORE_H
