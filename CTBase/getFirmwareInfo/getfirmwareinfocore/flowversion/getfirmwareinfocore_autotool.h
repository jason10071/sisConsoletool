#ifndef GETFIRMWAREINFOCORE_AUTOTOOL_H
#define GETFIRMWAREINFOCORE_AUTOTOOL_H

#include "getfirmwareinfocore/flowversion/getfirmwareinfocore_general.h"

namespace CT
{

class GetFirmwareInfoCore_autotool : public GetFirmwareInfoCore_general
{
public:
    explicit GetFirmwareInfoCore_autotool(GetFirmwareInfoParameter* getFirmwareInfoParameter);
    virtual ~GetFirmwareInfoCore_autotool();

    virtual CTExitCode exec();

protected:
    virtual void prepareXramMasterRef();
};

} // CT

#endif // GETFIRMWAREINFOCORE_AUTOTOOL_H
