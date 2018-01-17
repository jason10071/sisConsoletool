#ifndef GETFIRMWAREIDCORE_GENERAL_H
#define GETFIRMWAREIDCORE_GENERAL_H

#include "getfirmwareidcore/igetfirmwareidcore.h"

namespace CT
{

class GetFirmwareIdCore_general : public IGetFirmwareIdCore
{
public:
    explicit GetFirmwareIdCore_general(GetFirmwareIdParameter* getFirmwareIdParameter);
    virtual ~GetFirmwareIdCore_general();

    virtual CTExitCode exec();

protected:
    virtual void prepareAllXramRef();
    virtual void prepareXramRef(int chipIndex);
    virtual void prepareBinRef(int chipIndex);

    virtual CTExitCode showFirmwareId();
    virtual CTExitCode checkFirmwareIdUsingInput();
    virtual CTExitCode checkFirmwareIdUsingFile();

    virtual void checkInputConut(int inputCount);
};

} // CT

#endif // GETFIRMWAREIDCORE_GENERAL_H
