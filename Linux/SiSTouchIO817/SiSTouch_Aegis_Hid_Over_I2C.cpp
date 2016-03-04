#include "SiSTouchIO.h"
#include "SiSLogger.h"
#include <errno.h>
#include <string.h>

#define DEVICE_CHANGEMODE_RETRY_TIMES 3 //40
#define DEVICE_CHANGEMODE_RETRY_INTERVAL 100


#ifdef VIA_IOCTL
#include <fcntl.h>
#include <sys/stat.h>
#define DEVICE_NAME "/dev/hidraw*"	// same as master

//////////////
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>

#else
//#error "SiS817 not support system call"
#endif

#define CLOSE_AFTER_RESET

#define WAIT_DEVICE_REMOVE 3000

#define DUMMYBYTE 0
#define OUTPUT_IGNORE 4

//HEADER_LENGTH is the byte count before Length field
#define HEADER_LENGTH 2

//0x81 delay time (ms)
//write 8K need delay 900ms, 12K need delay 1500ms
#define DELAY_FOR_81 1500
#define DELAY_FOR_87 3000
#define DELAY_FOR_GENEARL 2

//AP_MODE & TOUCH_MODE is not usable in I2C
//#define AP_MODE 0x00000f00
//#define TOUCH_MODE 0x00000f01

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



enum
{
    OUTPUT_REGISTER = 0x0004,

    OUTPUT_REPORT_ID = 0x9,
    INPUT_REPORT_ID = 0xA,

    DATA_UNIT_9257 = 52,
};

const char*
SiSTouch_Aegis_Hid_Over_I2C::get_device_name()
{
	return getActiveDeviceName();
}

int
SiSTouch_Aegis_Hid_Over_I2C::io_stop_driver()
{
#ifdef VIA_IOCTL
    const char* devName = SiSTouchIO::getActiveDeviceName();
    if (devName==0)
    	devName = DEVICE_NAME;
    LOGI("%s: devName = %s\n", __FUNCTION__, devName);

    if(m_fd >= 0)
    {
        close(m_fd);
        m_fd = -1;
    }
    m_fd = open(devName,O_RDWR | O_NONBLOCK);
    return m_fd;
#else
    return ERROR_NOT_SUPPORT;
#endif
}

void 
SiSTouch_Aegis_Hid_Over_I2C::waitIOReady(int &fd, int timeoutSec)
{
	fd_set rfds;
	struct timeval tv;
	int retval;

	FD_ZERO(&rfds); /* clear the set */
	FD_SET(fd, &rfds); /* add our file descriptor to the set */

	if ( m_verbose )
	{
		LOGI("Set timeoutSec: %d(sec)", timeoutSec);
	}

	tv.tv_sec = timeoutSec;
	tv.tv_usec = 0;

	retval = select(fd + 1, &rfds, NULL, NULL, &tv);

	if(retval == -1)
	{
		perror("select"); /* an error accured */
	}
	else if(retval == 0)
	{
		if ( m_verbose )
		{
			LOGI("IO timeout"); /* a timeout occured */
		}
	}
	else
	{
		if ( m_verbose )
		{
			LOGI("IO data ready"); /* there was data to read */
		}
	}
}

int
SiSTouch_Aegis_Hid_Over_I2C::write_data(unsigned char * buf, int size, int timeout)
{
    int ret = 0;
#ifdef VIA_IOCTL
	/* IGNORE byte0 ~ byte3 (Output Register LSB, Output Register MSB, Length field LSB, Length field MSB) */
    ret = write(m_fd, buf + OUTPUT_IGNORE, size - OUTPUT_IGNORE);

	waitIOReady(m_fd, timeout/1000);
#else
    ret = ERROR_NOT_SUPPORT;
#endif

    return ret;
}

int
SiSTouch_Aegis_Hid_Over_I2C::read_data( unsigned char * buf, int size, int timeout)
{
    int ret = 0;
#ifdef VIA_IOCTL
    ret = read(m_fd, buf, size);

    //real packect size is less then return size - 1
    if(ret > 0)
    {
        ret -= DUMMYBYTE;

		/* insert byte0(Length field LSB) and byte1(Length field MSB) */
		ret += 2;
		unsigned char lengthLSB = ret & 0xff;
		unsigned char lengthMSB = (ret >> 8) & 0xff;
		
		for(int i = ret - 1; i > 1; i--)
		{
			buf[i] = buf[i - 2];
		}
		buf[0] = lengthLSB;
		buf[1] = lengthMSB;
		/* --- */
    }
#else
    ret = ERROR_NOT_SUPPORT;
#endif

    return ret;
}

int
SiSTouch_Aegis_Hid_Over_I2C::call_04()
{

    if( m_verbose)
    {
        LOGI("call_04() ");
    }

    int flag = 0;
    int ret;
    for( int i = 0; i < m_retry; i++ )
    {
        //0x81 need delay between read and write

        ret = simple_io( 0x81, DELAY_FOR_81 ,CMD81_TIMEOUT); // 20160303 add CMD81_TIMEOUT

        if ( ret < 0 )
        {
            ret = parse_syscall_return_value( ret, GENERAL_TYPE_FLAG );
            if(is_noneed_retry_error(ret))
            {
                break;
            }
        }
        else
        {

            ret = read_simple_ack(ret);

            if ( ret == 0 )
                flag = 1;

            if ( ret != ERROR_NACK )
                break;
        }

        usleep( m_delay );
    }

    if ( !flag )
    {
        if ( ret == ERROR_NACK )
            ret = ERROR_TIMEOUT;

        if ( m_verbose )
            LOGE( "ERROR: 81 command not ACK, give up" );
    }

    if( ret < 0 )
    {
        if ( m_verbose )
        {
            log_out_error_message( ret, 0x04 );
        }
    }
    return ret;

}

int
SiSTouch_Aegis_Hid_Over_I2C::call_82()
{
    int ret = 0;

    if( m_verbose)
    {
        LOGI("call_82()");
    }

    for( int i = 0; i < m_retry; i++)
    {
        ret = simple_io( 0x82, DELAY_FOR_GENEARL );

        if ( ret < 0 )
        {
            ret = parse_syscall_return_value( ret, GENERAL_TYPE_FLAG );
            if( is_noneed_retry_error( ret ) )
                break;
        }
        else
        {

            ret = read_simple_ack(ret);
            if ( ret == 0 )//success
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

#ifdef CLOSE_AFTER_RESET
        usleep(WAIT_DEVICE_REMOVE * 1000); // 20160303 add
#endif

#ifdef PWR_MODE
        if(m_ChangePWRMode)
        {
            for(int i = 0; i < DEVICE_CHANGEMODE_RETRY_TIMES; i++ )
            {
                ret = call_85(PW_CMD_ACTIVE);

                if(ret >= 0)
                {
                    break;
                }

                usleep( DEVICE_CHANGEMODE_RETRY_INTERVAL * 1000 );
            }

            if(ret >= 0)
            {
#ifdef PWR_MODE_QUERY
                for(int i = 0; i < DEVICE_CHANGEPWR_RETRY_TIMES; i++ )
                {
                    int addr[1] = {QUERY_STATUS_ADDRESS};
                    int data[1] = {0};
                    int mask[1] = {0};
                    ret = call_88(addr, data, mask, 1);

                    int pwCMDState = (data[0] >> 16) & 0xff;

                    int active = PW_CMD_ACTIVE & 0xff;

                    if(ret >= 0 && pwCMDState == active)
                    {
                        break;
                    }

                    usleep( DEVICE_CHANGEPWR_RETRY_INTERVAL * 1000 );
                }
#else
                usleep( DEVICE_CHANGEPWR_RETRY_INTERVAL *5 * 1000 );
#endif
            }
        }
#endif

#ifdef DIAGNOSIS_MODE
        if(m_ChangeDiagMode)
        {
            if(ret >= 0)
            {
                for(int i = 0; i < DEVICE_CHANGEMODE_RETRY_TIMES; i++ )
                {
                    ret =call_85(ENABLE_DIAGNOSIS_MODE);

                    if(ret >= 0)
                    {
                        break;
                    }

                    usleep( DEVICE_CHANGEMODE_RETRY_INTERVAL * 1000 );
                }
            }
        }
#endif
    }


    return ret;
}

int
SiSTouch_Aegis_Hid_Over_I2C::call_82_without_retry()
{

    if( m_verbose)
    {
        LOGI("call_82()");
    }

    int ret = simple_io( 0x82, DELAY_FOR_GENEARL );

    if ( ret < 0 )
    {
        ret = parse_syscall_return_value( ret, GENERAL_TYPE_FLAG );
    }
    else
    {

        ret = read_simple_ack(ret);
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

#ifdef CLOSE_AFTER_RESET
        usleep(WAIT_DEVICE_REMOVE * 1000); // 20160303 add
#endif

#ifdef PWR_MODE
        if(m_ChangePWRMode)
        {
            for(int i = 0; i < DEVICE_CHANGEMODE_RETRY_TIMES; i++ )
            {
                ret = call_85(PW_CMD_ACTIVE);

                if(ret >= 0)
                {
                    break;
                }

                usleep( DEVICE_CHANGEMODE_RETRY_INTERVAL * 1000 );
            }

            if(ret >= 0)
            {
#ifdef PWR_MODE_QUERY
                for(int i = 0; i < DEVICE_CHANGEPWR_RETRY_TIMES; i++ )
                {
                    int addr[1] = {QUERY_STATUS_ADDRESS};
                    int data[1] = {0};
                    int mask[1] = {0};
                    ret = call_88(addr, data, mask, 1);

                    int pwCMDState = (data[0] >> 16) & 0xff;

                    int active = PW_CMD_ACTIVE & 0xff;

                    if(ret >= 0 && pwCMDState == active)
                    {
                        break;
                    }

                    usleep( DEVICE_CHANGEPWR_RETRY_INTERVAL * 1000 );
                }
#else
                usleep( DEVICE_CHANGEPWR_RETRY_INTERVAL *5 * 1000 );
#endif
            }
        }
#endif


#ifdef DIAGNOSIS_MODE
        if(m_ChangeDiagMode)
        {
            if(ret >= 0)
            {
                for(int i = 0; i < DEVICE_CHANGEMODE_RETRY_TIMES; i++ )
                {
                    ret =call_85(ENABLE_DIAGNOSIS_MODE);

                    if(ret >= 0)
                    {
                        break;
                    }

                    usleep( DEVICE_CHANGEMODE_RETRY_INTERVAL * 1000 );
                }
            }
        }
#endif
    }

    return ret;
}

int
SiSTouch_Aegis_Hid_Over_I2C::call_85( int data )
{

    int ret = 0;

    if( m_verbose)
    {
        LOGI("call_85()");
        LOGI(" data=%08x", data);
    }

    for( int i = 0; i < m_retry; i++)
    {

        int len = make_85_buffer( data );


        ret = io_command( m_buffer, len, DELAY_FOR_GENEARL );

        if ( ret < 0 )
        {
            ret = parse_syscall_return_value( ret, GENERAL_TYPE_FLAG );
            break;
        }
        else
        {
            ret = read_simple_ack(ret);
            break;
        }

        usleep( m_delay );
    }

    if( ret < 0 )
    {
        if ( m_verbose )
        {
            log_out_error_message( ret, 0x85 );
        }
    }
#ifdef DIAGNOSIS_MODE
    else
    {
    	// 20160303 add
		// just waitIOReady 
    	// single chip: change done if ack-beef -> dont need wait
    	// slave chip need polling -> wait
		if(this->m_chipNum > 1)
		{
			if(data == DISABLE_DIAGNOSIS_MODE || data == ENABLE_DIAGNOSIS_MODE)
        	{
        		if ( m_verbose ) LOGI("DELAY_FOR_DIAGNOSIS_MODE_CHANGE=%d\n", DELAY_FOR_DIAGNOSIS_MODE_CHANGE);
            	usleep(DELAY_FOR_DIAGNOSIS_MODE_CHANGE * 1000);
        	}
		}
    }
#endif


    return ret;

}


int
SiSTouch_Aegis_Hid_Over_I2C::call_87()
{

    int ret = 0;

    if( m_verbose)
    {
        LOGI("call_87()");
    }

    for( int i = 0; i < m_retry; i++)
    {

        ret = simple_io( 0x87, DELAY_FOR_87 , CMD87_TIMEOUT); // 20160303 add CMD87_TIMEOUT


        if ( ret < 0 )
        {
            ret = parse_syscall_return_value( ret, GENERAL_TYPE_FLAG );
            break;
        }
        else
        {

            ret = read_simple_ack(ret);

            if ( ret != ERROR_WRONG_FORMAT )
                break;
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

#ifdef PWR_MODE
        if(m_ChangePWRMode)
        {
            for(int i = 0; i < DEVICE_CHANGEMODE_RETRY_TIMES; i++ )
            {
                ret = call_85(PW_CMD_ACTIVE);

                if(ret >= 0)
                {
                    break;
                }

                usleep( DEVICE_CHANGEMODE_RETRY_INTERVAL * 1000 );
            }


            if(ret >= 0)
            {
#ifdef PWR_MODE_QUERY
                for(int i = 0; i < DEVICE_CHANGEPWR_RETRY_TIMES; i++ )
                {
                    int addr[1] = {QUERY_STATUS_ADDRESS};
                    int data[1] = {0};
                    int mask[1] = {0};
                    ret = call_88(addr, data, mask, 1);

                    int pwCMDState = (data[0] >> 16) & 0xff;

                    int active = PW_CMD_ACTIVE & 0xff;

                    if(ret >= 0 && pwCMDState == active)
                    {
                        break;
                    }

                    usleep( DEVICE_CHANGEPWR_RETRY_INTERVAL * 1000 );
                }
#else
                usleep( DEVICE_CHANGEPWR_RETRY_INTERVAL *5 * 1000 );
#endif
            }
        }

#endif


#ifdef DIAGNOSIS_MODE
        if(m_ChangeDiagMode)
        {
            if(ret >= 0)
            {
                for(int i = 0; i < DEVICE_CHANGEMODE_RETRY_TIMES; i++ )
                {
                    ret =call_85(ENABLE_DIAGNOSIS_MODE);

                    if(ret >= 0)
                    {
                        break;
                    }

                    usleep( DEVICE_CHANGEMODE_RETRY_INTERVAL * 1000 );
                }
            }
        }
#endif
    }

    return ret;
}




