#include "SiSTouchIO.h"
#include "SiSLogger.h"
#include <errno.h>
#include <string.h>

#include "SisTouchFinder.h"


#define DEVICE_CHANGEMODE_RETRY_TIMES 40
#define DEVICE_CHANGEMODE_RETRY_INTERVAL 100

#define DEVICE_CLOSE_RETRY_TIMES 20
#define DEVICE_CLOSE_RETRY_INTERVAL 1000


#ifdef VIA_IOCTL
#include <fcntl.h>
#include <sys/stat.h>
#define DEVICE_NAME "/dev/hidraw*"	// same as master
#else
//#error "SiS817 not support system call"
#endif

#define DUMMYBYTE 0
#define HEADER_LENGTH 1


//0x81 delay time (ms)
#define DELAY_FOR_GENEARL 2
#define DELAY_FOR_81 500
#define TIMEOUT_FOR_87 12000

//AP_MODE & TOUCH_MODE is replaced by DIAGNOSIS MODE
//#define AP_MODE 0x00000f00
//#define TOUCH_MODE 0x00000f0

#define DIAGNOSIS_MODE

#ifdef DIAGNOSIS_MODE
#define ENABLE_DIAGNOSIS_MODE 0x00000121
#define DISABLE_DIAGNOSIS_MODE 0x00000120
#define DELAY_FOR_DIAGNOSIS_MODE_CHANGE 1000
#endif

#define PWR_MODE

#ifdef PWR_MODE
#define PW_CMD_FWCTRL 0x00000950
#define PW_CMD_ACTIVE 0x00000951
#define PW_CMD_SLEEP 0x00000952

//#define PWR_MODE_QUERY

#define DEVICE_CHANGEPWR_RETRY_TIMES 10
#define DEVICE_CHANGEPWR_RETRY_INTERVAL 20

#endif

#define QUERY_STATUS_ADDRESS 0x50000000

#define CLOSE_AFTER_RESET

#define WAIT_DEVICE_REMOVE 3000

enum
{
    OUTPUT_REPORT_ID = 0x9,
    INPUT_REPORT_ID = 0xA,

    DATA_UNIT_9257 = 52,

};

const char*
SiSTouch_Aegis_Multi_FOR_NEW_DRIVER::get_device_name()
{
    return getActiveDeviceName();
}

int
SiSTouch_Aegis_Multi_FOR_NEW_DRIVER::sis_usb_write( void* data, unsigned int size, int timeout)
{
    int ret = 0;
    unsigned char * buf = (unsigned char *)data;
	
#ifdef VIA_IOCTL

	ret = write(m_fd, buf, size);

#else

    ret = ERROR_NOT_SUPPORT;

#endif
    return ret;
}

int
SiSTouch_Aegis_Multi_FOR_NEW_DRIVER::sis_usb_read( void* data, unsigned int size, int timeout)
{
    int ret = 0;
    unsigned char * buf = (unsigned char *)data;
	
#ifdef VIA_IOCTL

    ret = read(m_fd, buf, size);

#else

    ret = ERROR_NOT_SUPPORT;

#endif

    return ret;
}

int
SiSTouch_Aegis_Multi_FOR_NEW_DRIVER::simple_io_master( int cmd, int sleepms, int wTimeout, int rTimeout )
{

    int len = make_simple_buffer_master(cmd);

    int ret = sis_usb_write( m_buffer, len, wTimeout );

    if(ret < 0)
    {
        return ret;
    }

    if(sleepms >= 0)
    {
        usleep(sleepms * 1000);
    }

    ret = sis_usb_read( m_buffer, BUFFER_SIZE, rTimeout );


	/* close m_fd for remove /dev/hidraw =======================================*/
	if(cmd == 0x82 || cmd == 0x87)
	{
	    int ret = sis_usb_start();
		if(ret != 0)
		{
		    printf("close m_fd (%s): fail , ret=%d\n", get_device_name(), ret);
		}
	}
	/* =============================================================== */
	

    if ( m_verbose ) dbg_print_buffer_hex( cmd, m_buffer, ret );

    //usb return fixed 64 byte
    if( ret >= 0)
    {
        ret = m_buffer[1] + 1;
    }


    if(m_IODelayms >= 0)
    {
        usleep(m_IODelayms * 1000);
    }


    return ret;
}

bool SiSTouch_Aegis_Multi_FOR_NEW_DRIVER::reFindSisTouchName()
{
	SisTouchFinder sisTouchFinder;
	const char* deviceName = sisTouchFinder.autoDetectDevicePath();
	
	if(deviceName != 0)
	{
		strcpy(m_activeDeviceName, deviceName);
		delete deviceName;
	
		printf("re-find activeDeviceName=%s\n\n", m_activeDeviceName);
	}
	else
	{
		return false;
	}

	return true;
}

int
SiSTouch_Aegis_Multi_FOR_NEW_DRIVER::call_82()
{

    int ret = 0;

    if( m_verbose)
    {
        LOGI("call_82()");
    }

    for( int i = 0; i < m_retry; i++)
    {
        ret = simple_io_master( 0x82, DELAY_FOR_GENEARL );

        if ( ret < 0 )
        {
            ret = parse_syscall_return_value( ret, GENERAL_TYPE_FLAG );
            if( is_noneed_retry_error( ret ) )
                break;
        }
        else
        {

            ret = read_simple_ack_master(ret);

            if ( ret == 0 )
                break;
        }

        usleep( m_delay );

    }

    if( ret < 0 )
    {
        if ( m_verbose )
        {
            log_out_error_message( ret, 0x82 );
        }
    }
    else
    {

#ifdef VIA_IOCTL

#ifdef CLOSE_AFTER_RESET
        sis_usb_start();
        usleep(WAIT_DEVICE_REMOVE * 1000);
#endif
        for(int i = 0; i < DEVICE_CLOSE_RETRY_TIMES; i++ )
        {		
            ret = stop_driver();
            // these three error not repeat retry in stop_driver()
            if(ret != ERROR_ACCESS && ret != ERROR_ENTRY_NOT_EXIST && ret != ERROR_DEVICE_NOT_EXIST)
            {
                break;
            }

			/* re-find /dev/hidraw* if hidrawCnt changed ====================*/
			if(reFindSisTouchName() == false)
            {
            }
			/* ===========================================*/
						
            usleep( DEVICE_CLOSE_RETRY_INTERVAL * 1000 );
        }

#else
        ret = stop_driver();
#endif
    }

    return ret;
}

int
SiSTouch_Aegis_Multi_FOR_NEW_DRIVER::call_82_without_retry()
{

    if( m_verbose)
    {
        LOGI("call_82()");
    }

    int ret = simple_io_master( 0x82, DELAY_FOR_GENEARL );

    if ( ret < 0 )
    {
        ret = parse_syscall_return_value( ret, GENERAL_TYPE_FLAG );
    }
    else
    {

        ret = read_simple_ack_master(ret);
    }

    if( ret < 0 )
    {
        if ( m_verbose )
        {
            log_out_error_message( ret, 0x82 );
        }
    }
    else
    {

#ifdef VIA_IOCTL

#ifdef CLOSE_AFTER_RESET
        sis_usb_start();
        usleep(WAIT_DEVICE_REMOVE * 1000);
#endif
        for(int i = 0; i < DEVICE_CLOSE_RETRY_TIMES; i++ )
        {
            ret = stop_driver();
            // these three error not repeat retry in stop_driver()
            if(ret != ERROR_ACCESS && ret != ERROR_ENTRY_NOT_EXIST && ret != ERROR_DEVICE_NOT_EXIST)
            {
                break;
            }

			/* re-find /dev/hidraw* if hidrawCnt changed ====================*/
			if(reFindSisTouchName() == false)
            {
            }
			/* ===========================================*/
			
            usleep( DEVICE_CLOSE_RETRY_INTERVAL * 1000 );
        }

#else
        ret = stop_driver();
#endif
    }

    return ret;

}

int
SiSTouch_Aegis_Multi_FOR_NEW_DRIVER::call_87()
{

    int ret = 0;

    if( m_verbose)
    {
        LOGI("call_87()");
    }

    for( int i = 0; i < m_retry; i++)
    {
        ret = simple_io_master( 0x87, DELAY_FOR_GENEARL , DEFAULT_TIMEOUT, TIMEOUT_FOR_87 );
		
        // [20150304]
        if(this->m_chipNum >= 7)
        {
		    printf("chipNum >= 7, delay 4s\n");
            int sleepTime = 4 * 1000 * 1000;
            usleep( sleepTime );
        }
				
        if ( ret < 0 )
        {
            ret = parse_syscall_return_value( ret, GENERAL_TYPE_FLAG );
            break;
        }
        else
        {

            ret = read_simple_ack_master(ret);

            if ( ret != ERROR_WRONG_FORMAT )
			{
                break;
			}
        }

        usleep( m_delay );

    }

    if( ret < 0 )
    {
        if ( m_verbose )
        {
            log_out_error_message( ret, 0x87 );
        }
    }
    else
    {

#ifdef VIA_IOCTL

#ifdef CLOSE_AFTER_RESET
        sis_usb_start();
        usleep(WAIT_DEVICE_REMOVE * 1000);
#endif
        for(int i = 0; i < DEVICE_CLOSE_RETRY_TIMES; i++ )
        {
            ret = stop_driver();
            // these three error not repeat retry in stop_driver()
            if(ret != ERROR_ACCESS && ret != ERROR_ENTRY_NOT_EXIST && ret != ERROR_DEVICE_NOT_EXIST)
            {
                break;
            }
			
			/* re-find /dev/hidraw* if hidrawCnt changed ====================*/
			if(reFindSisTouchName() == false)
            {
            }
			/* ===========================================*/
			
            usleep( DEVICE_CLOSE_RETRY_INTERVAL * 1000 );
        }
#else
        ret = stop_driver();
#endif
    }
    return ret;

}


