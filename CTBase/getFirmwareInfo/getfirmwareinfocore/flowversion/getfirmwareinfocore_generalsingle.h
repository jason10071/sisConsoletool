#ifndef GETFIRMWAREINFOCORE_GENERALSINGLE_H
#define GETFIRMWAREINFOCORE_GENERALSINGLE_H

#include "getfirmwareinfocore/flowversion/getfirmwareinfocore_general.h"

namespace CT
{

class GetFirmwareInfoCore_generalSingle : public GetFirmwareInfoCore_general
{
public:
    explicit GetFirmwareInfoCore_generalSingle(GetFirmwareInfoParameter* getFirmwareInfoParameter);
    virtual ~GetFirmwareInfoCore_generalSingle();

protected:
    virtual CTExitCode getInfoUsingAddress();
    virtual CTExitCode getInfoUsingKeyword();
    virtual CTExitCode getInfoDefault();
};

} // CT

#endif // GETFIRMWAREINFOCORE_GENERALSINGLE_H
