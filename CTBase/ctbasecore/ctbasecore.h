#ifndef CTBASECORE_H
#define CTBASECORE_H

#include "ctbasedllexportdefine.h"
#include "ctexitcode/ctexitcode.h"
#include "ctbasecore/ctbaseparameter.h"
#include "ctbasecore/ctbasereference.h"
#include "sisprocedure/sisprocedure.h"
#include <map>

namespace CT
{

class CTBASE_EXPORT CTBaseCore
{
public:
    explicit CTBaseCore(CTBaseParameter* ctBaseParameter);
    virtual ~CTBaseCore();

    virtual void setSiSProcedure(SiSProcedure* sisProcedure);

    virtual void init();
    virtual CTExitCode exec() = 0;
    virtual void destroy();

protected:
    CTExitCode undoException(std::string exceptionTitle);

    virtual CTBaseReference* generateReference();
    void insertReference(ReferenceSource rs, int chipIndex, CTBaseReference* ctBaseReference);
    CTBaseReference* getCTBaseReference(ReferenceSource rs, int chipIndex);

    void openDevice();
    void disableCtlReportToOs();
    void enableCtlReportToOs();
    void jumpBootloader(int chipIndex);
    void resetDevice();

    virtual void prepareBaseXramMasterRef(bool stopIfBroken);

    void fetchOSDeviceType();
    void fetchOSDeviceInterface();
    void fetchDeviceType(ReferenceSource rs, int chipIndex);
    void fetchBroken(ReferenceSource rs, int chipIndex);
    void fetchFWSizeType(ReferenceSource rs, int chipIndex, bool isBroken);
    void fetchInterfaceID(ReferenceSource rs, int chipIndex);
    void fetchSlaveNum(ReferenceSource rs);
    void fetchMultiChipSelectiveID(ReferenceSource rs, int chipIndex);
    void fetchCalibrationFlag(ReferenceSource rs, int chipIndex);
    void fetchFirmwareID(ReferenceSource rs, int chipIndex);
    void fetchBootloaderID(ReferenceSource rs, int chipIndex);
    void fetchBootCodeCRC(ReferenceSource rs, int chipIndex);
    void fetchMainCodeCRC(ReferenceSource rs, int chipIndex);
    void fetchLastID(ReferenceSource rs, int chipIndex);
    void fetchLastTime(ReferenceSource rs, int chipIndex);
    void fetchPriorLastID(ReferenceSource rs, int chipIndex);
    void fetchPriorLastTime(ReferenceSource rs, int chipIndex);
    void fetchProductID(ReferenceSource rs, int chipIndex);
    void fetchTaskID(ReferenceSource rs, int chipIndex);
    void fetchFwVersion(ReferenceSource rs, int chipIndex);
    void fetchUpdateMark(ReferenceSource rs, int chipIndex);
    void fetchLastUpdateMark(ReferenceSource rs, int chipIndex);
    void fetchPriorLastUpdateMark(ReferenceSource rs, int chipIndex);

    void showDeviceType(int chipIndex, ReferenceSource rs = RS_COUNT);
    void showBroken(int chipIndex, ReferenceSource rs = RS_COUNT);
    void showFWSizeType(int chipIndex, ReferenceSource rs = RS_COUNT);
    void showInterfaceID(int chipIndex, ReferenceSource rs = RS_COUNT);
    void showSlaveNum(ReferenceSource rs = RS_COUNT);
    void showMultiChipSelectiveID(int chipIndex, ReferenceSource rs = RS_COUNT);
    void showCalibrationFlag(int chipIndex, ReferenceSource rs = RS_COUNT);
    void showFirmwareID(int chipIndex, ReferenceSource rs = RS_COUNT);
    void showBootloaderID(int chipIndex, ReferenceSource rs = RS_COUNT);
    void showBootCodeCRC(int chipIndex, ReferenceSource rs = RS_COUNT);
    void showMainCodeCRC(int chipIndex, ReferenceSource rs = RS_COUNT);
    void showLastID(int chipIndex, ReferenceSource rs = RS_COUNT);
    void showLastTime(int chipIndex, ReferenceSource rs = RS_COUNT);
    void showPriorLastID(int chipIndex, ReferenceSource rs = RS_COUNT);
    void showPriorLastTime(int chipIndex, ReferenceSource rs = RS_COUNT);
    void showProductID(int chipIndex, ReferenceSource rs = RS_COUNT);
    void showUpdateMark(int chipIndex, ReferenceSource rs = RS_COUNT);
    void showLastUpdateMark(int chipIndex, ReferenceSource rs = RS_COUNT);
    void showPriorLastUpdateMark(int chipIndex, ReferenceSource rs = RS_COUNT);

protected:
    CTBaseParameter* m_ctBaseParameter;
    std::map<ChipIndexKey, CTBaseReference*> m_referenceMap[RS_COUNT];
    SiSProcedure* m_sisProcedure;
};

} // CT

#endif // CTBASECORE_H
