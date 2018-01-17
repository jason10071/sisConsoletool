#ifndef GETFIRMWAREIDCORE_GENERALSINGLE_H
#define GETFIRMWAREIDCORE_GENERALSINGLE_H

#include "getfirmwareidcore/flowversion/getfirmwareidcore_general.h"

namespace CT
{

class GetFirmwareIdCore_generalSingle : public GetFirmwareIdCore_general
{
public:
    GetFirmwareIdCore_generalSingle(GetFirmwareIdParameter* getFirmwareIdParameter);
    ~GetFirmwareIdCore_generalSingle();

protected:
    virtual void prepareAllXramRef();

    virtual CTExitCode showFirmwareId();
    virtual CTExitCode checkFirmwareIdUsingInput();
    virtual CTExitCode checkFirmwareIdUsingFile();

    virtual void checkInputConut(int inputCount);
};

} // CT

#endif // GETFIRMWAREIDCORE_GENERALSINGLE_H
