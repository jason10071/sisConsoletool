#include "SisTouchFinder.h"    

#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>

/*=============================================================================*/
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
/* Linux */
#include <linux/types.h>
#include <linux/input.h>
#include <linux/hidraw.h>

/*
 * Ugly hack to work around failing compilation on systems that don't
 * yet populate new version of hidraw.h to userspace.
 *
 * If you need this, please have your distro update the kernel headers.
 */
#ifndef HIDIOCSFEATURE
#define HIDIOCSFEATURE(len)    _IOC(_IOC_WRITE|_IOC_READ, 'H', 0x06, len)
#define HIDIOCGFEATURE(len)    _IOC(_IOC_WRITE|_IOC_READ, 'H', 0x07, len)
#endif

/* Unix */
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

/* C */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
/*=============================================================================*/

#include <linux/i2c-dev.h>
#include <linux/i2c.h>


SisTouchFinder::SisTouchFinder() :
	m_vId(0x0),
	m_deviceType(UNKNOW)
{
}

SisTouchFinder::~SisTouchFinder()
{
}

const char*
SisTouchFinder::autoDetectDevicePath()
{
	// only detect hidraw
    /* Detect hidraw* */
    const char* deviceName = autoDetectHidDevicePath();
	if(deviceName != 0)
	{
	    return deviceName;
	}

	return 0;
}

SisTouchFinder::DeviceType 
SisTouchFinder::getDeviceType()
{
    return m_deviceType;
}

/* Detect hidraw* ==================================================================*/
const char*
SisTouchFinder::autoDetectHidDevicePath()
{
	FILE* pipe = popen("ls /dev/hidraw*", "r");
	if (pipe)
	{
	    char buffer[128];
		char* deviceName = new char[128];
		
	    while(!feof(pipe))
	    {
	        if(fgets(buffer, 128, pipe) != NULL)
			{
			    strcpy(deviceName, buffer);

				if(deviceName[strlen(deviceName) - 1] == '\n')
				{
				    deviceName[strlen(deviceName) - 1] = '\0';
				}

				/* check if deviceName is hid_Sis_touch */
				printf("[check HID: %s]\n", deviceName);
				if(isSisTouchHid(deviceName))
				{
				    pclose(pipe);
					return deviceName; // search first
				} 
			}
	    }
		
	    pclose(pipe);
	}

    return 0;
}

bool 
SisTouchFinder::isSisTouchHid(const char* deviceName)
{
	STF_DEBUG printf("[isSisTouchHid] deviceName=%s\n", deviceName);

    bool ret = getHidInfo(deviceName);

	if(!ret)
	{
	    return false;
	}

	if(m_vId != 0x0457)
	{
	    return false;
	}

	if(strstr(m_rawName, "HID") != NULL)
	{
	    m_deviceType = USB_817;
	}
	else
	{
	}

	return true;
	
}

bool
SisTouchFinder::getHidInfo(const char* deviceName)
{
	int fd;
	int i, res, desc_size = 0;
	char buf[256];
	struct hidraw_report_descriptor rpt_desc;
	struct hidraw_devinfo info;

	STF_DEBUG printf("getHidInfo: %s\n", deviceName);

	/* Open the Device with non-blocking reads. In real life,
	   don't use a hard coded path; use libudev instead. */
	fd = open(deviceName, O_RDWR|O_NONBLOCK);

	if (fd < 0) {
		STF_DEBUG perror("Unable to open device");
		return false;
	}

	memset(&rpt_desc, 0x0, sizeof(rpt_desc));
	memset(&info, 0x0, sizeof(info));
	memset(buf, 0x0, sizeof(buf));

	/* Get Report Descriptor Size */
	/*
	res = ioctl(fd, HIDIOCGRDESCSIZE, &desc_size);
	if (res < 0) {
		STF_DEBUG perror("HIDIOCGRDESCSIZE");
	}
	else {
		STF_DEBUG printf("Report Descriptor Size: %d\n", desc_size);
	}
	*/

	/* Get Report Descriptor */
	/*
	rpt_desc.size = desc_size;
	res = ioctl(fd, HIDIOCGRDESC, &rpt_desc);
	if (res < 0) {
		STF_DEBUG perror("HIDIOCGRDESC");
	} else {
		STF_DEBUG printf("Report Descriptor:\n");
		for (i = 0; i < rpt_desc.size; i++)
			STF_DEBUG printf("%hhx ", rpt_desc.value[i]);
		STF_DEBUG puts("\n");
	}
	*/

	/* Get Raw Name */
	res = ioctl(fd, HIDIOCGRAWNAME(256), buf);
	if (res < 0) {
		STF_DEBUG perror("HIDIOCGRAWNAME");
	}
	else {
		STF_DEBUG printf("Raw Name: %s\n", buf);

		// save raw name
		strcpy(m_rawName, buf);
		printf("[rawName=%s]\n", m_rawName);
	}

	/* Get Physical Location */
	/*
	res = ioctl(fd, HIDIOCGRAWPHYS(256), buf);
	if (res < 0) {
		STF_DEBUG perror("HIDIOCGRAWPHYS");
	}
	else {
		STF_DEBUG printf("Raw Phys: %s\n", buf);
	}
	*/

	/* Get Raw Info */
	res = ioctl(fd, HIDIOCGRAWINFO, &info);
	if (res < 0) {
		STF_DEBUG perror("HIDIOCGRAWINFO");
	} else {
		STF_DEBUG printf("Raw Info:\n");
		STF_DEBUG printf("\tbustype: %d (%s)\n",
			info.bustype, bus_str(info.bustype));
		
		STF_DEBUG printf("\tvendor: 0x%04hx\n", info.vendor);

		// save vid
		m_vId = info.vendor;
		printf("[vid=0x%04hx]\n", m_vId);
			
		STF_DEBUG printf("\tproduct: 0x%04hx\n", info.product);
	}

	/* Set Feature */
	/*
	buf[0] = 0x9; // Report Number
	buf[1] = 0xff;
	buf[2] = 0xff;
	buf[3] = 0xff;
	res = ioctl(fd, HIDIOCSFEATURE(4), buf);
	if (res < 0) {
		STF_DEBUG perror("HIDIOCSFEATURE");
	}
	else {
		STF_DEBUG printf("ioctl HIDIOCGFEATURE returned: %d\n", res);
	}
	*/

	/* Get Feature */
	/*
	buf[0] = 0x9; // Report Number
	res = ioctl(fd, HIDIOCGFEATURE(256), buf);
	if (res < 0) {
		STF_DEBUG perror("HIDIOCGFEATURE");
	} else {
		STF_DEBUG printf("ioctl HIDIOCGFEATURE returned: %d\n", res);
		STF_DEBUG printf("Report data (not containing the report number):\n\t");
		for (i = 0; i < res; i++)
			printf("%hhx ", buf[i]);
		STF_DEBUG puts("\n");
	}
	*/

	close(fd);
	return true;

}

const char *
SisTouchFinder::bus_str(int bus)
{
    switch (bus) 
    {
    case BUS_USB:		
        return "USB";	
        break;
    case BUS_HIL:		
        return "HIL";	
        break;	
    case BUS_BLUETOOTH:		
        return "Bluetooth";
        break;
    /*
    case BUS_VIRTUAL:		
        return "Virtual";
        break;
    */
    default:	
        return "Other";
        break;	
	}
}

/* Detect i2c-* ==================================================================*/
const char*
SisTouchFinder::autoDetectI2cDevicePath()
{
	// only detect hidraw
    return 0;
}

bool 
SisTouchFinder::isSisTouchI2c(const char* deviceName)
{
	// only detect hidraw
	return false;
}





