#ifndef GETFIRMWAREINFOCORE_GENERAL_H
#define GETFIRMWAREINFOCORE_GENERAL_H

#include "getfirmwareinfocore/igetfirmwareinfocore.h"

namespace CT
{

class GetFirmwareInfoCore_general : public IGetFirmwareInfoCore
{
public:
    explicit GetFirmwareInfoCore_general(GetFirmwareInfoParameter* getFirmwareInfoParameter);
    virtual ~GetFirmwareInfoCore_general();

    virtual CTExitCode exec();

protected:
    virtual CTExitCode showList();
    virtual CTExitCode getInfoUsingAddress();
    virtual CTExitCode getInfoUsingKeyword();
    virtual CTExitCode getInfoDefault();
};

} // CT

#endif // GETFIRMWAREINFOCORE_GENERAL_H
