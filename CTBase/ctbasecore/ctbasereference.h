#ifndef CTBASEREFERENCE_H
#define CTBASEREFERENCE_H

#include "ctbasecore/ctbasedefine.h"
#include "spbase/sisproceduredefine.h"
#include "sisprocedure/serialdata.h"
#include <string>
#include "ctbasedllexportdefine.h"

using namespace SiS::Procedure;

class CTBASE_EXPORT CTBaseReference
{
public:
    explicit CTBaseReference();
    virtual ~CTBaseReference();

    bool isDummy();

    DeviceType getDeviceType();
    bool isBroken();
    FWSizeType getFWSizeType();
    unsigned char getInterfaceID();
    unsigned char getMultiChipSelectiveID();
    int getSlaveNum();
    bool getCalibrationFlag();
    SerialData* getFirmwareID();
    SerialData* getBootloaderID();
    SerialData* getBootCodeCRC();
    SerialData* getMainCodeCRC();
    SerialData* getLastID();
    SerialData* getLastTime();
    SerialData* getPriorLastID();
    SerialData* getPriorLastTime();
    SerialData* getTaskID();
    FwVersion getFwVersion();
    SerialData* getProductID();

    SerialData* getUpdateMark();
    SerialData* getLastUpdateMark();
    SerialData* getPriorLastUpdateMark();

    void setDeviceType(DeviceType deviceType);
    void setBroken(bool brokenFlag);
    void setFWSizeType(FWSizeType fwSizeType);
    void setInterfaceID(unsigned char interfaceID);
    void setMultiChipSelectiveID(unsigned char multiChipSelectiveID);
    void setSlaveNum(int slaveNum);
    void setCalibrationFlag(bool calibrationFlag);
    void setFirmwareID(SerialData* firmwareID);
    void setBootloaderID(SerialData* bootloaderID);
    void setBootCodeCRC(SerialData* bootCodeCRC);
    void setMainCodeCRC(SerialData* mainCodeCRC);
    void setLastID(SerialData* lastID);
    void setLastTime(SerialData* lastTime);
    void setPriorLastID(SerialData* priorLastID);
    void setPriorLastTime(SerialData* priorLastTime);
    void setTaskID(SerialData* taskID);
    void setFwVersion(FwVersion fwVersion);
    void setProductID(SerialData* productID);

    void setUpdateMark(SerialData* updateMark);
    void setLastUpdateMark(SerialData* lastUpdateMark);
    void setPriorLastUpdateMark(SerialData* priorLastUpdateMark);


protected:
    DeviceType m_deviceType;
    bool m_broken;
    FWSizeType m_fwSizeType;
    unsigned char m_interfaceID;
    unsigned char m_multiChipSelectiveID;
    int m_slaveNum;
    bool m_calibrationFlag;
    SerialData* m_firmwareID;
    SerialData* m_bootloaderID;
    SerialData* m_bootCodeCRC;
    SerialData* m_mainCodeCRC;
    SerialData* m_lastID;
    SerialData* m_lastTime;
    SerialData* m_priorLastID;
    SerialData* m_priorLastTime;
    SerialData* m_taskID;
    FwVersion m_fwVersion;
    SerialData* m_productID; // Note : different to PID

    SerialData* m_updateMark;
    SerialData* m_lastUpdateMark;
    SerialData* m_priorLastUpdateMark;
};

#endif // CTBASEREFERENCE_H
