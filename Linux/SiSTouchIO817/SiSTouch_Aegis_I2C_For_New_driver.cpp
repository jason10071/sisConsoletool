#include "SiSTouchIO.h"
#include "SiSLogger.h"
#include <errno.h>
#include <string.h>

#define DEVICE_CHANGEMODE_RETRY_TIMES 40
#define DEVICE_CHANGEMODE_RETRY_INTERVAL 100


#ifdef VIA_IOCTL
#include <fcntl.h>
#include <sys/stat.h>
#define DEVICE_NAME "/dev/i2c-*"

//////////////
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>

#else
//#error "SiS817 not support system call"
#endif

#define DUMMYBYTE 0
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
SiSTouch_Aegis_I2C_FOR_NEW_DRIVER::get_device_name()
{
	return getActiveDeviceName();
}

int
SiSTouch_Aegis_I2C_FOR_NEW_DRIVER::stop_driver()
{
    if( m_verbose)
    {
        LOGI("stop_driver()");
    }

    int ret = io_stop_driver();

    if ( ret < 0 )
    {
#ifdef VIA_IOCTL
        ret = parse_syscall_return_value( ret, NOT_SIS_IO_FLAG );
#else
        ret = parse_syscall_return_value( ret );
#endif

        if ( m_verbose )
        {
            log_out_error_message( ret, -1 );
        }
        return ret;
    }
	/* set I2C_SLAVE_FORCE after open m_fd for new driver*/
	else
	{
		int addr = 0x5c;

		if ( m_verbose )
        {
            printf("set I2C_SLAVE_FORCE, addr=0x%02x\n", addr); 
        }
				
        ret = ioctl(m_fd, I2C_SLAVE_FORCE, addr);
        if(ret < 0)
		{
		    perror("set I2C_SLAVE_FORCE errer:"); 
			printf("set I2C_SLAVE_FORCE, addr=0x%02x\n", addr); 
			
			io_start_driver();
			return ret;
		}
	}
	/* ===================== */


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

        if(ret < 0)
        {
            // fail of stop, but device has been open, so we need to close device
            io_start_driver();
            return ret;
        }    

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
#endif



#ifdef DIAGNOSIS_MODE
    if(m_ChangeDiagMode)
    {
        for(int i = 0; i < DEVICE_CHANGEMODE_RETRY_TIMES; i++ )
        {
            ret = call_85(ENABLE_DIAGNOSIS_MODE);

            if(ret >= 0)
            {
                break;
            }

            usleep( DEVICE_CHANGEMODE_RETRY_INTERVAL * 1000 );
        }

        if(ret < 0)
        {
            // fail of stop, but device has been open, so we need to close device
            io_start_driver();
            return ret;
        }
    }
#endif
    
    return 0;


}

int
SiSTouch_Aegis_I2C_FOR_NEW_DRIVER::io_command(unsigned char * buf, int size, int sleepms)
{
    int ret = 0;
    unsigned char cmd = buf[6];

    ret = write_data(buf, size);

    if(ret < 0)
    {
        return ret;
    }

    if(sleepms >= 0)
    {
        usleep(sleepms * 1000);
    }

    ret = read_data(buf, BUFFER_SIZE);

	/* read Length_field */
	if(ret > 0)
	{
	    ret = ( m_buffer[1] << 8 ) | m_buffer[0];
	}
	/* =========== */

    if ( m_verbose ) dbg_print_buffer_hex( cmd, buf, ret );

    if(m_IODelayms >= 0)
    {
        usleep(m_IODelayms * 1000);
    }

    return ret;
}

//simple_io : transfer no payload command
int
SiSTouch_Aegis_I2C_FOR_NEW_DRIVER::simple_io( int cmd, int sleepms )
{

	if ( m_verbose ) LOGI("simple_io : %02x DELAY=%d", cmd, sleepms);
    make_simple_buffer(cmd);

    int ret = write_data( m_buffer, 8 );

    if(ret < 0)
    {
        return ret;
    }

    if(sleepms >= 0)
    {
        usleep(sleepms * 1000);
    }

    ret = read_data(m_buffer, BUFFER_SIZE );

    /* read Length_field */
	if(ret > 0)
	{
	    ret = ( m_buffer[1] << 8 ) | m_buffer[0];
	}
	/* =========== */

    if ( m_verbose ) dbg_print_buffer_hex( cmd, m_buffer, ret );

    if(m_IODelayms >= 0)
    {
        usleep(m_IODelayms * 1000);
    }


    return ret;
}






