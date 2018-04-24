#ifndef GETFIRMWAREINFOCORE_H
#define GETFIRMWAREINFOCORE_H

#include "getfirmwareinfocore/igetfirmwareinfocore.h"
#include "getfirmwareinfocore/flowversion/getfirmwareinfocore_general.h"
#include "getfirmwareinfocore/flowversion/getfirmwareinfocore_generalsingle.h"
#include "getfirmwareinfocore/flowversion/getfirmwareinfocore_autotool.h"

namespace CT
{

class GetFirmwareInfoCore : public IGetFirmwareInfoCore
{
public:
    explicit GetFirmwareInfoCore(GetFirmwareInfoParameter* getFirmwareInfoParameter);
    virtual ~GetFirmwareInfoCore();

    void switchFlowVersion(DeviceType deviceType);

    virtual void setSiSProcedure(SiSProcedure* sisProcedure);

    virtual void init();
    virtual CTExitCode exec();

protected:
    virtual CTExitCode showList() { return undoException("IGetFirmwareInfoCore Exception : "); }
    virtual CTExitCode getInfoUsingAddress() { return undoException("IGetFirmwareInfoCore Exception : "); }
    virtual CTExitCode getInfoUsingKeyword() { return undoException("IGetFirmwareInfoCore Exception : "); }
    virtual CTExitCode getInfoDefault() { return undoException("IGetFirmwareInfoCore Exception : "); }

private:
    GetFirmwareInfoCore_general* getGetFirmwareInfoCore_general();
    GetFirmwareInfoCore_generalSingle* getGetFirmwareInfoCore_generalSingle();
    GetFirmwareInfoCore_autotool* getGetFirmwareInfoCore_autotool();

private:
    IGetFirmwareInfoCore* m_iGetFirmwareInfoCore;
    GetFirmwareInfoCore_general* m_getFirmwareInfoCore_general;
    GetFirmwareInfoCore_generalSingle* m_getFirmwareInfoCore_generalSingle;
    GetFirmwareInfoCore_autotool* m_getFirmwareInfoCore_autotool;
};

} // CT

#endif // GETFIRMWAREINFOCORE_H
