#ifndef SISDEVICEATTRIBUTE_H
#define SISDEVICEATTRIBUTE_H

#include "stdio.h"
#include "string"
#include "string.h"

class SiSDeviceAttribute
{
public:
    enum ConnectType
    {
        CON_UNKNOW = 0,
        /* USB */
        CON_819_USB_HID,
        /* I2C */
        CON_819_HID_OVER_I2C,
    };
		
    SiSDeviceAttribute();
    ~SiSDeviceAttribute();

    std::string getDeviceName();
    void setDeviceName(std::string deviceName);

    std::string getNodeName();
    void setNodeName(std::string nodeName);

    SiSDeviceAttribute::ConnectType getConnectType();
    void setConnectType(SiSDeviceAttribute::ConnectType connectType);

    int getVID();
    void setVID(int vid);

    int getPID();
    void setPID(int pid);

    std::string getRawName();
    void setRawName(std::string rawName);

private:
    std::string m_deviceName;
    std::string m_nodeName;
    SiSDeviceAttribute::ConnectType m_connectType;
    int m_vid;
    int m_pid;
    std::string m_rawName;

};

#endif // SISDEVICEATTRIBUTE_H
