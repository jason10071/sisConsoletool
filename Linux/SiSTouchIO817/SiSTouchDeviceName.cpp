/*
 * SiSTouchDeviceName.cpp
 *
 *  Created on: 2013/6/28
 *      Author: swing
 */

#include "SiSTouchDeviceName.h"
#include "SiSTouchIO.h"
#include <string.h>

SiSTouchDeviceName::SiSTouchDeviceName() {
	// TODO Auto-generated constructor stub

}

SiSTouchDeviceName::~SiSTouchDeviceName() {
	// TODO Auto-generated destructor stub
}

struct DevNameToType {
	char devName[DEVNAME_MAX_LEN];
	int conType;
};

DevNameToType tblDevNameToType[1] = {
		{"/dev/hp_hid_touch_device" , SiSTouchIO::CON_AEGIS_INTERUSB}
};

bool
SiSTouchDeviceName::compareDeviceName(char* devName1, const char* devName2)
{
	//ex. sis_aegis_hid_bridge_touch_device
	bool ret = false;
	int  splitNum1 = 0, splitNum2=0;
	char cmpDevName1[DEVNAME_MAX_LEN]="", cmpDevName2[DEVNAME_MAX_LEN]="";
	char splitDevName1[10][10], splitDevName2[10][10];
	char *pch=NULL;

	if (strlen(devName1)>DEVNAME_MAX_LEN) {
		printf("%s: Error! device name '%s' TOO long!\n", __FUNCTION__, devName1);
		return ret;
	}

	if (strlen(devName2)>DEVNAME_MAX_LEN) {
		printf("%s: Error! device name '%s' TOO long!\n", __FUNCTION__, devName2);
		return ret;
	}

	int str_len = (strlen(devName1) <= strlen(devName2) ? strlen(devName1) : strlen(devName2));
	if (strncmp(devName1, devName2, str_len)==0) {
		return true;
	}

	strncpy(cmpDevName1, devName1, DEVNAME_MAX_LEN);
	strncpy(cmpDevName2, devName2, DEVNAME_MAX_LEN);

	pch = strtok(cmpDevName1, "_");

	while ( pch!=NULL && splitNum1<10 ) {
		strcpy(splitDevName1[splitNum1], pch);
		if ( strlen(splitDevName1[splitNum1]) > 10 ) {
			printf("%s : split fail, devicename1=%s\n", __FUNCTION__, devName1);
			return false;
		}
		pch = strtok(NULL, "_");
		splitNum1++;
	}

	if (splitNum1==0 || splitNum1>=10) {
		printf("%s : illegal format, devicename1=%s\n", __FUNCTION__, devName1);
		return false;
	}

	pch = NULL;
	pch = strtok(cmpDevName2, "_");

	while ( pch!=NULL && splitNum2<10 ) {
		strcpy(splitDevName2[splitNum2], pch);
		if ( strlen(splitDevName2[splitNum2]) > 10 ) {
			printf("%s : split fail, deviceName2=%s\n", __FUNCTION__, devName2);
			return false;
		}
		pch = strtok(NULL, "_");
		splitNum2++;
	}

	if (splitNum2==0 || splitNum2>=10) {
		printf("%s : illegal format, devicename2=%s\n", __FUNCTION__, devName2);
		return false;
	}

	//Same device type will have the same split Number of device name
	//ex. hp_aegis_hid_bridge_touch_device123 & sis_aegis_hid_bridge_touch_device has the same device type
	if (splitNum1 == splitNum2) {
		// first splitDevName is company name, can be different for same device type
		for (int i=1; i<splitNum1-1; i++) {
			if (strcmp(splitDevName1[i], splitDevName2[i])!=0 ) {
				return false;
			}
		}

		//last splitDevName may have sequence number
		int len1 = strlen(splitDevName1[splitNum1-1]);
		int len2 = strlen(splitDevName1[splitNum2-1]);
		if ( len1 == len2 ) {
			ret = true;
		}
		else if ( len1 > len2 ) {
			if ( strncmp(splitDevName1[splitNum1-1], splitDevName1[splitNum2-1], len2)==0
					&& splitDevName1[splitNum1-1][len2]>='0' &&  splitDevName1[splitNum1-1][len2]<='9' ) {
				ret = true;
			}
		}
		else {
			if ( strncmp(splitDevName1[splitNum1-1], splitDevName1[splitNum2-1], len1)==0
					&& splitDevName2[splitNum2-1][len1]>='0' &&  splitDevName2[splitNum2-1][len1]<='9' ) {
				ret = true;
			}
		}
	}

	return ret;
}

int
SiSTouchDeviceName::getDeviceTypeByTable(char* devName)
{
	for (unsigned int i=0; i<sizeof(tblDevNameToType)/sizeof(DevNameToType) ;i++)
	{
		if ( strcmp(devName, tblDevNameToType[i].devName)==0 ||
			 (strcmp(devName, tblDevNameToType[i].devName)>0 && devName[strlen(tblDevNameToType[i].devName)]>='0' && devName[strlen(tblDevNameToType[i].devName)]<='9' )
		   )
		{

			return tblDevNameToType[i].conType;
		}
	}

	return SiSTouchIO::CON_NONE;
}


int
SiSTouchDeviceName::getDeviceType(char* devName, int max_retry, int retry_delay, int verbose, int ioDelay, int changeDiagMode, int changePWRMode)
{
	int con = getDeviceTypeByTable(devName);

	if ( con != SiSTouchIO::CON_NONE )
		return con;

	if ( compareDeviceName(devName, SiSTouch_Zeus_I2C::get_device_name()) )
	{
		con = SiSTouchIO::CON_ZEUS_I2C;
	}
	else if( compareDeviceName(devName, SiSTouch_Zeus_USB::get_device_name()) )
	{
		con = SiSTouchIO::CON_ZEUS_USB;
	}
	else if( compareDeviceName(devName, SiSTouch_Zeus_InterUSB::get_device_name()) )
	{
		con = SiSTouchIO::CON_ZEUS_INTERUSB;
	}
	else if( compareDeviceName(devName, SiSTouch_Aegis_I2C::get_device_name()) )
	{
		con = SiSTouchIO::CON_AEGIS_I2C;
	}
	else if( compareDeviceName(devName, SiSTouch_Aegis_USB::get_device_name()) )
	{
		con = SiSTouchIO::CON_AEGIS_USB;
	}
	else if( compareDeviceName(devName, SiSTouch_Aegis_InterUSB::get_device_name()) )
	{
		// caller need to check is SiSTouchIO::CON_AEGIS_MULTI or SiSTouchIO::CON_AEGIS_INTERUSB
		con = SiSTouchIO::CON_AEGIS_INTERUSB;
	}

	return con;
}
