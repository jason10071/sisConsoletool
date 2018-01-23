#include "sisdeviceattribute.h"

SiSDeviceAttribute::SiSDeviceAttribute() :
    m_deviceName(""),
    m_nodeName(""),
    m_connectType(SiSDeviceAttribute::CON_UNKNOW),
    m_vid(0x0),
    m_pid(0x0),
    m_rawName("")
{

}

SiSDeviceAttribute::~SiSDeviceAttribute()
{

}

std::string
SiSDeviceAttribute::getDeviceName()
{
   return this->m_deviceName;
}
   
void
SiSDeviceAttribute::setDeviceName(std::string deviceName)
{
    this->m_deviceName = deviceName;
}

std::string
SiSDeviceAttribute::getNodeName()
{
   return this->m_nodeName;
}
   
void
SiSDeviceAttribute::setNodeName(std::string nodeName)
{
    this->m_nodeName = nodeName;
}
	
SiSDeviceAttribute::ConnectType 
SiSDeviceAttribute::getConnectType()
{
    return this->m_connectType;
}

void
SiSDeviceAttribute::setConnectType(SiSDeviceAttribute::ConnectType connectType)
{
    this->m_connectType = connectType;
}

int 
SiSDeviceAttribute::getVID()
{
    return this->m_vid;
}

void
SiSDeviceAttribute::setVID(int vid)
{
    this->m_vid = vid;
}

int
SiSDeviceAttribute::getPID()
{
    return this->m_pid;
}

void
SiSDeviceAttribute::setPID(int pid)
{
    this->m_pid = pid;
}

std::string
SiSDeviceAttribute::getRawName()
{
    return this->m_rawName;
}

void
SiSDeviceAttribute::setRawName(std::string rawName)
{
    this->m_rawName = rawName;
}

