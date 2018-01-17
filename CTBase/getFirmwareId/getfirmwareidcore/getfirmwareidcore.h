#ifndef GETFIRMWAREIDCORE_H
#define GETFIRMWAREIDCORE_H

#include "getfirmwareidcore/igetfirmwareidcore.h"
#include "getfirmwareidcore/flowversion/getfirmwareidcore_general.h"
#include "getfirmwareidcore/flowversion/getfirmwareidcore_generalsingle.h"

namespace CT
{

class GetFirmwareIdCore : public IGetFirmwareIdCore
{
public:
    explicit GetFirmwareIdCore(GetFirmwareIdParameter* getFirmwareIdParameter);
    virtual ~GetFirmwareIdCore();

    void switchFlowVersion(DeviceType deviceType);

    virtual void setSiSProcedure(SiSProcedure* sisProcedure);

    virtual void init();
    virtual CTExitCode exec();

protected:
    virtual void prepareBinRef(int chipIndex) {}
    virtual void prepareAllXramRef() {}
    virtual void prepareXramRef(int chipIndex) {}

    virtual CTExitCode showFirmwareId() { return undoException("IGetFirmwareIdCore Exception : "); }
    virtual CTExitCode checkFirmwareIdUsingInput() { return undoException("IGetFirmwareIdCore Exception : "); }
    virtual CTExitCode checkFirmwareIdUsingFile() { return undoException("IGetFirmwareIdCore Exception : "); }

    virtual void checkInputConut(int inputCount) {}

private:
    GetFirmwareIdCore_general* getGetFirmwareIdCore_general();
    GetFirmwareIdCore_generalSingle* getGetFirmwareIdCore_generalSingle();

private:
    IGetFirmwareIdCore* m_iGetFirmwareIdCore;
    GetFirmwareIdCore_general* m_getFirmwareIdCore_general;
    GetFirmwareIdCore_generalSingle* m_getFirmwareIdCore_generalSingle;
};

} // CT

#endif // GETFIRMWAREIDCORE_H
