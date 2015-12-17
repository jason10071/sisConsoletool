#include "SiSTouchIO.h"
#include <errno.h>
#include <string.h>

#ifdef VIA_IOCTL
#include <fcntl.h>
#include <sys/stat.h>
#define DEVICE_NAME "/dev/sis_aegis_hid_touch_device"	// same as master
#else
//#error "SiS817 not support system call"
#endif

const char*
SiSTouch_Aegis_InterUSB::get_device_name()
{
	return DEVICE_NAME;
}

int
SiSTouch_Aegis_InterUSB::device_Exist()
{

#ifdef VIA_IOCTL
    char const* devName = SiSTouchIO::getActiveDeviceName();
    if (devName==NULL)
    	devName = DEVICE_NAME;


    // check format
    if (strncmp(devName, DEVICE_NAME, strlen(DEVICE_NAME))!=0)
    		return 0;

    struct stat s;
    if(stat( devName , &s ) == 0)
	{
		if( S_ISBLK( s.st_mode ) || S_ISCHR( s.st_mode ))
		{
			return 1;
		}
	}
    printf("Can't find devName:%s\n", devName);

    return 0;
#else
    return 0;
#endif

}

