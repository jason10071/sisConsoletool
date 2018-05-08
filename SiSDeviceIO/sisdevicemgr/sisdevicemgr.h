#ifndef SISDEVICEMGR_H
#define SISDEVICEMGR_H

#include "stdio.h"
#include "string"
#include <string.h>
#include <list>

#include "sisdeviceio/sisdeviceexception.h"
#include "sisattribute/sisdeviceattribute.h"

typedef std::list<SiSDeviceAttribute*> SiSDeviceAttributeList;

class SiSDeviceMgr
{
public:
    SiSDeviceMgr();
    ~SiSDeviceMgr();

    void clearDeviceAttributeList();

    SiSDeviceAttribute* getOpened();
	bool detectBySiSDeviceNode(std::string deviceName);
    bool detectByHidrawName(std::string deviceName);

private:
    SiSDeviceAttribute* getElement(SiSDeviceAttributeList _list, int _i);

    /*=====================================================*/
    /* hid-example.c */
    /*
    * Hidraw Userspace Example
    *
    * Copyright (c) 2010 Alan Ott <alan@signal11.us>
    * Copyright (c) 2010 Signal 11 Software
    *
    * The code may be used by anyone for any purpose,
    * and can serve as a starting point for developing
    * applications using hidraw.
    */
    bool getHidInfo(const char* deviceName, SiSDeviceAttribute* sisDeviceAttribute);
    const char *bus_str(int bus);
    /*=====================================================*/

private:
    SiSDeviceAttributeList m_sisDeviceAttributeList;
    int m_openedIndex;

};

#endif // SISDEVICEMGR_H
