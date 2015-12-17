/*
 * SiSTouchDeviceName.h
 *
 *  Created on: 2013/6/28
 *      Author: swing
 */

#ifndef SISTOUCHDEVICENAME_H_
#define SISTOUCHDEVICENAME_H_

class SiSTouchDeviceName {
public:
	SiSTouchDeviceName();
	virtual ~SiSTouchDeviceName();

	bool compareDeviceName(char* devName1, const char* devName2);
	int getDeviceTypeByTable(char* devName);
	int getDeviceType(char* devName, int max_retry, int retry_delay, int verbose, int ioDelay, int changeDiagMode, int changePWRMode);
};

#endif /* SISTOUCHDEVICENAME_H_ */
