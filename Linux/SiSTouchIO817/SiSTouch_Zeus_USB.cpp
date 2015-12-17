/*
 * SiSTouchUSB.cpp
 *
 *  Created on: Mar 26, 2010
 *      Author: suzin
 */

#include "SiSTouchIO.h"
#include "SiSLogger.h"
#include <string.h>

#ifdef VIA_IOCTL
#include <fcntl.h>
#include <sys/stat.h>
#define DEVICE_NAME "/dev/sis_zeus_hid_bridge_touch_device"
#else
#define __NR_sis_HID_stop       366
#define __NR_sis_HID_start      367
#define __NR_sis_HID_query      368
#define __NR_sis_HID_IO         369
#endif

int
SiSTouch_Zeus_USB::stop_driver()
{
    int ret;

    if( m_verbose)
    {
        LOGI("stop_driver()");
    }

    ret = sis_usb_stop();

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


    int flag = 0;

    for( int i = 0; i < m_retry; i++ )
    {
        if ( sis_usb_query() >= 0 )
        {
            flag = 1;
            m_driver_exist = 1;
            break;
        }

        usleep( m_delay );
    }

    if ( !flag )
    {
        if ( m_verbose )
        {
            LOGE( "ERROR: query usb driver fail" );
        }
        ret = ERROR_QUERY;
        if ( m_verbose )
        {
            log_out_error_message( ret, -1 );
        }

        // fail of stop, but device has been open, so we need to close device
        sis_usb_start();
        return ret;
    }

    ret = usb_command_D7();

    if ( ret < 0 )
    {
        ret = parse_syscall_return_value( ret, GENERAL_TYPE_FLAG );
        if ( m_verbose )
        {
            log_out_error_message( ret, 0xD7 );
        }

        // fail of stop, but device has been open, so we need to close device
        sis_usb_start();

        return ret;
    }

    return 0;
}

int
SiSTouch_Zeus_USB::start_driver()
{
    int ret;

    if( m_verbose)
    {
        LOGI("start_driver()");
    }

    if ( m_driver_exist )
    {
        ret = usb_command_DA();
        if ( ret < 0)
        {
            ret = parse_syscall_return_value( ret, GENERAL_TYPE_FLAG );
            if ( m_verbose )
            {
                log_out_error_message( ret, 0xDA );
            }
            return ret;
        }
    }

    ret = sis_usb_start();
    if ( ret < 0)
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
        return ret;;
    }

    return 0;
}

int
SiSTouch_Zeus_USB::call_00( unsigned char *data, int size )
{
    int ret = 0;
    if( m_verbose)
    {
        LOGI("call_00()");
    }

    ret = command_d8_read( 0x0 );

    if ( ret < 0 )
    {
        ret = parse_syscall_return_value( ret, GENERAL_TYPE_FLAG );
    }
    else if( ret <= static_cast<int>(get_MAX_SIZE()))
    {
        int ret_size = 0;
        for( int i = 0; i < ret && i < size; i++ )
        {
            data[i] = m_buffer[i];
            ret_size++;
        }
        ret = ret_size;
    }
    else
    {
        ret = ERROR_WRONG_FORMAT;
    }

    if ( ret < 0 )
    {
        if ( m_verbose )
        {
            log_out_error_message( ret, 0x00 );
        }
    }

    return ret;
}


int SiSTouch_Zeus_USB::call_04()
{
    if( m_verbose)
    {
        LOGI("call_04()");
    }



    int flag = 0;
    int ret;
    for( int i = 0; i < m_retry; i++ )
    {
        ret = command_d8_read( 0x04 );

        if ( ret < 0 )
        {
            ret = parse_syscall_return_value( ret, GENERAL_TYPE_FLAG );
            break;
        }
        else if ( ret == 4 && m_buffer[0] == 0x00 && m_buffer[1] == 0x40 && m_buffer[2] == 0xff && m_buffer[3] == 0xff )
        {
        }
        else if ( ret == 6 && m_buffer[0] == 0x0 && m_buffer[1] == 0x80 )
        {
            flag = 1;
            break;
        }
        else
        {
            ret = ERROR_WRONG_FORMAT;
            break;
        }

        usleep( m_delay );
    }

    if ( !flag )
    {
        if ( ret == 4 && m_buffer[0] == 0x00 && m_buffer[1] == 0x40 && m_buffer[2] == 0xff && m_buffer[3] == 0xff )
        {
            if ( m_verbose )
            {
                LOGE( "ERROR: 04 command not ACK, give up" );
            }
            ret = ERROR_TIMEOUT;
        }
    }
    else if ( ret == 6 && m_buffer[2] == 0xff && m_buffer[3] == 0xff && m_buffer[4] == 0xde && m_buffer[5] == 0xad )
    {
        if ( m_verbose )
        {
            LOGE( "ERROR: cmd 04 return dead" );
        }
        ret = ERROR_DEAD;
    }
    else if(ret == 6 && m_buffer[2] == 0xff && m_buffer[3] == 0xff && m_buffer[4] == 0xff && m_buffer[5] == 0xff )
    {
        ret = 0;
    }
    else
    {
        ret = ERROR_WRONG_FORMAT;
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
SiSTouch_Zeus_USB::call_81()
{

    int ret = 0;

    if( m_verbose)
    {
        LOGI("call_81()");
    }

    ret = command_d8_read( 0x81 );

    if ( ret < 0 )
    {
        ret = parse_syscall_return_value( ret, GENERAL_TYPE_FLAG );
        if ( m_verbose )
        {
            log_out_error_message( ret, 0x81 );
        }
        return ret;
    }
    return ret;

}

int
SiSTouch_Zeus_USB::call_82()
{

    int ret = 0;

    if( m_verbose)
    {
        LOGI("call_82()");
    }


    for( int i = 0; i < m_retry; i++)
    {
        ret = command_d8_read( 0x82 );

        if ( ret < 0 )
        {
            ret = parse_syscall_return_value( ret, GENERAL_TYPE_FLAG );
            break;
        }
        else if( ret == 4 && m_buffer[0] == 0x0 && m_buffer[1] == 0x80 && m_buffer[2] == 0xff && m_buffer[3] == 0xff)
        {
            ret = 0;
            break;
        }
        else
        {
            ret = ERROR_WRONG_FORMAT;
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
    return ret;
}

int
SiSTouch_Zeus_USB::call_82_without_retry()
{

    if( m_verbose)
    {
        LOGI("call_82()");
    }

    int ret = command_d8_read( 0x82 );

    if ( ret < 0 )
    {
        ret = parse_syscall_return_value( ret, GENERAL_TYPE_FLAG );
    }
    else if( ret == 4 && m_buffer[0] == 0x0 && m_buffer[1] == 0x80 && m_buffer[2] == 0xff && m_buffer[3] == 0xff)
    {
        ret = 0;
    }
    else
    {
        ret = ERROR_WRONG_FORMAT;
    }
    if( ret < 0 )
    {
        if ( m_verbose )
        {
            log_out_error_message( ret, 0x82 );
        }
    }
    return ret;
}


int
SiSTouch_Zeus_USB::call_83( int base, int pack_num )
{
    int ret = 0;

    if( m_verbose)
    {
        LOGI("call_83()");
        LOGI(" base=%08x,pack_num=%d", base, pack_num);
    }

    for( int i = 0; i < m_retry; i++)
    {
        make_83_buffer( base, pack_num );
        ret = usb_command_D9( m_buffer, BUFFER_SIZE );

        if ( ret < 0 )
        {
            ret = parse_syscall_return_value( ret, GENERAL_TYPE_FLAG );
            break;
        }

        if ( limit_retry( &SiSTouch_Zeus_USB::usb_command_DC ) )
        {
            ret = ERROR_TIMEOUT;
            usleep( m_delay );
            break;
        }

        ret = command_d8_read( 0x0 );

        if ( ret < 0 )
        {
            ret = parse_syscall_return_value( ret, GENERAL_TYPE_FLAG );
            break;
        }
        else if ( ret == 4 && m_buffer[0] == 0x0 && m_buffer[1] == 0x40 && m_buffer[2] == 0x08 && m_buffer[3] == 0xff)
        {
            ret = ERROR_NACK;
            break;
        }
        else if ( ret == 4 && m_buffer[0] == 0x0 && m_buffer[1] == 0x40 && m_buffer[2] == 0xff && m_buffer[3] == 0x12)
        {
            ret = ERROR_NACK;
            break;
        }
        else if ( ret == 4 && m_buffer[0] == 0x0 && m_buffer[1] == 0x40 && m_buffer[2] == 0x08 && m_buffer[3] == 0x12)
        {
            ret = ERROR_NACK;
            break;
        }
        else if( ret == 4 && m_buffer[0] == 0x0 && m_buffer[1] == 0x80 && m_buffer[2] == 0xff && m_buffer[3] == 0xff)
        {
            ret = 0;
            break;
        }
        else
        {
            ret = ERROR_WRONG_FORMAT;
        }
        usleep( m_delay );
    }
    if( ret < 0 )
    {
        if ( m_verbose )
        {
            log_out_error_message( ret, 0x83 );
        }
    }
    return ret;
}

int
SiSTouch_Zeus_USB::call_84( const int* data, int size )
{
    int ret = 0;

    if( m_verbose)
    {
        LOGI("call_84()");
        LOGI(" data=%08x,%08x,%08x,s=%d", data[0], data[1], data[2], size);
    }


    for( int i = 0; i < m_retry; i++)
    {
        make_84_buffer( data, size );
        ret = usb_command_D9( m_buffer, BUFFER_SIZE );

        if ( ret < 0 )
        {
            ret = parse_syscall_return_value( ret, GENERAL_TYPE_FLAG );
            break;
        }

        if ( limit_retry( &SiSTouch_Zeus_USB::usb_command_DC ) )
        {
            ret = ERROR_TIMEOUT;
            break;
        }

        ret = command_d8_read( 0x0 );

        if ( ret < 0 )
        {
            ret = parse_syscall_return_value( ret, GENERAL_TYPE_FLAG );
            break;
        }
        else if ( ret == 4 && m_buffer[0] == 0x0 && m_buffer[1] == 0x40 )
        {
            ret = ERROR_NACK;
        }
        else if ( ret == 4 && m_buffer[0] == 0x0 && m_buffer[1] == 0x80 )
        {
            ret = 0;
            break;
        }
        else
        {
            ret = ERROR_WRONG_FORMAT;
            break;
        }
        usleep( m_delay );
    }
    if( ret < 0 )
    {
        if ( m_verbose )
        {
            log_out_error_message( ret, 0x84 );
        }
    }
    return ret;
}

int
SiSTouch_Zeus_USB::call_85( int data )
{

    int idx = 0;
    int ret = 0;

    unsigned char mode = data & 0xff;

    if( m_verbose)
    {
        LOGI("call_85()");
        LOGI(" data=%02x", mode);
    }


    for( int i = 0; i < m_retry; i++)
    {
        idx = make_85_buffer( mode );
        ret = usb_command_D9( m_buffer, idx );

        if ( ret < 0 )
        {
            ret = parse_syscall_return_value( ret, GENERAL_TYPE_FLAG );
            break;
        }

        if ( limit_retry( &SiSTouch_Zeus_USB::usb_command_DC ) )
        {
            ret = ERROR_TIMEOUT;
            break;
        }

        ret = command_d8_read( 0x0 );

        if ( ret < 0 )
        {
            ret = parse_syscall_return_value( ret, GENERAL_TYPE_FLAG );
            break;
        }
        else if ( ret == 4 && m_buffer[0] == 0x0 && m_buffer[1] == 0x80 && m_buffer[2] == 0x38 && m_buffer[3] == 0x31 )
        {
            ret = 0;
            break;
        }
        else
        {
            ret = ERROR_WRONG_FORMAT;
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
    return ret;
}

int
SiSTouch_Zeus_USB::call_86( int address, int *data, int size )
{

    int idx = 0;
    int ret = 0;

    if( m_verbose)
    {
    	LOGI("call_86()");
        LOGI(" address=%08x", address);
    }

    for( int i = 0; i < m_retry; i++)
    {
        idx = make_86_buffer( address );
        ret = usb_command_D9( m_buffer, idx );

        if ( ret < 0 )
        {
            ret = parse_syscall_return_value( ret, GENERAL_TYPE_FLAG );
            break;
        }

        if ( limit_retry( &SiSTouch_Zeus_USB::usb_command_DC ) )
        {
            ret = ERROR_TIMEOUT;
            break;
        }

        ret = command_d8_read( 0x0 );

        if ( ret < 0 )
        {
            ret = parse_syscall_return_value( ret, GENERAL_TYPE_FLAG );
            break;
        }
        else if ( ret == 4 && m_buffer[0] == 0x0 && m_buffer[1] == 0x40 && m_buffer[2] == 0x38 && m_buffer[3] == 0x31 )
        {
            ret = ERROR_NACK;
        }
        else if( ret == 14)
        {
            ret = 0;
            for ( int j = 0; j < 3 && j < size; j++ )
            {
                data[j] = 0;
                for ( int k = 0; k < 4; k++ )
                {
                    data[j] |= m_buffer[j*4+k] << 8 * k;
                }
                ret++;
            }
            break;
        }
        else
        {
            ret = ERROR_WRONG_FORMAT;
            break;
        }
        usleep( m_delay );
    }
    if( ret < 0 )
    {
        if ( m_verbose )
        {
            log_out_error_message( ret, 0x86 );
        }
    }
    return ret;
}

int
SiSTouch_Zeus_USB::call_87()
{

    int ret = 0;

    if( m_verbose)
    {
        LOGI("call_87()");
    }


    for( int i = 0; i < m_retry; i++)
    {
        ret = command_d8_read( 0x87 );

        if ( ret < 0 )
        {
            ret = parse_syscall_return_value( ret, GENERAL_TYPE_FLAG );
            break;
        }
        else if ( ret == 4 && m_buffer[0] == 0x0 && m_buffer[1] == 0x80 && m_buffer[2] == 0x38 && m_buffer[3] == 0x31 )
        {
            ret = 0;
            break;
        }
        else
        {
            ret = ERROR_WRONG_FORMAT;
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
    return ret;
}

int
SiSTouch_Zeus_USB::call_88( int* address, int *data, int* mask, int number )
{

    int idx = 0;
    int ret = 0;

    if( m_verbose)
    {
        LOGI("call_88()");
        LOGI(" address=%08x, data=%08x, mask=%08x", address[0], data[0], mask[0]);
    }


    for( int i = 0; i < m_retry; i++)
    {
        idx = make_88_buffer( address, data, mask, number );
        ret = usb_command_D9( m_buffer, idx );

        if ( ret < 0 )
        {
            ret = parse_syscall_return_value( ret, GENERAL_TYPE_FLAG );
            break;
        }

        if ( limit_retry( &SiSTouch_Zeus_USB::usb_command_DC ) )
        {
            ret = ERROR_TIMEOUT;
            break;
        }

        ret = command_d8_read( 0x0 );

        if ( ret < 0 )
        {
            ret = parse_syscall_return_value( ret, GENERAL_TYPE_FLAG );
            break;
        }
        else if ( ret == 4 && m_buffer[0] == 0x0 && m_buffer[1] == 0x40 )
        {
            ret = ERROR_NACK;
        }
        else if ( ret == 6)
        {
            ret = 0;
            for ( int j = 0; j < 1; j++ )
            {
                data[j] = 0;
                for ( int k = 0; k < 4; k++ )
                {
                    data[j] |= m_buffer[j*4+k] << 8 * k;
                }
                ret++;
            }
            break;
        }
        else
        {
            ret = ERROR_WRONG_FORMAT;
            break;
        }
        usleep( m_delay );
    }
    if( ret < 0 )
    {
        if ( m_verbose )
        {
            log_out_error_message( ret, 0x88 );
        }
    }
    return ret;
}

int
SiSTouch_Zeus_USB::call_89( int *status )
{

    int ret = 0;

    if( m_verbose)
    {
        LOGI("call_89()");
    }

    for( int i = 0; i < m_retry; i++)
    {
        ret = command_d8_read( 0x89 );

        if ( ret < 0 )
        {
            ret = parse_syscall_return_value( ret, GENERAL_TYPE_FLAG );
            break;
        }
        else if(ret == 6)
        {
            for ( int j = 0; j < 1; j++ )
            {
                status[j] = 0;
                for ( int k = 0; k < 4; k++ )
                {
                    status[j] |= m_buffer[j*4+k] << 8 * k;
                }
            }
            ret = 1;
            break;
        }
        else
        {
            ret = ERROR_WRONG_FORMAT;
            break;
        }
        usleep( m_delay );
    }
    if( ret < 0 )
    {
        if ( m_verbose )
        {
            log_out_error_message( ret, 0x89 );
        }
    }

    return ret;
}

int
SiSTouch_Zeus_USB::call_90( int *powermode )
{

    int idx = 0;
    int ret = 0;

    unsigned char mode = powermode[0] & 0xff;

    if( m_verbose)
    {
        LOGI("call_90()");
        LOGI(" powermode=%02x", mode);
    }

    for( int i = 0; i < m_retry; i++)
    {

        idx = make_90_buffer( mode );
        ret = usb_command_D9( m_buffer, idx );

        if ( ret < 0 )
        {
            ret = parse_syscall_return_value( ret, GENERAL_TYPE_FLAG );
            break;
        }

        if ( limit_retry( &SiSTouch_Zeus_USB::usb_command_DC ) )
        {
            ret = ERROR_TIMEOUT;
            break;
        }

        ret = command_d8_read( 0x0 );

        if ( ret < 0 )
        {
            ret = parse_syscall_return_value( ret, GENERAL_TYPE_FLAG );
            break;
        }
        else if( ret == 3 )
        {
            powermode[0] = m_buffer[0];
            ret = 1;
            break;
        }
        else
        {
            ret = ERROR_WRONG_FORMAT;
            break;
        }
        usleep( m_delay );
    }
    if( ret < 0 )
    {
        if ( m_verbose )
        {
            log_out_error_message( ret, 0x90 );
        }
    }
    return ret;

}

int
SiSTouch_Zeus_USB::call_command( int cmd, int size, unsigned char *data, int addcrc )
{

    int idx = 0;
    int ret = 0;

    if( m_verbose)
    {
        LOGI("call_%02x()", cmd);

        //char * str = new char[256];
        char str [256] = {0};
        sprintf(str, "%02x", data[0]);
        for( int i = 1; i < size; i++ )
        {
            sprintf(str, "%s %02x", str, data[i]);
        }
        LOGI(" data=%s", str);
        //delete[] str;
    }

    for( int i = 0; i < m_retry; i++)
    {

        idx = make_command_buffer( cmd , size, data, addcrc);
        ret = usb_command_D9( m_buffer, idx );

        if ( ret < 0 )
        {
            ret = parse_syscall_return_value( ret, GENERAL_TYPE_FLAG );
            break;
        }

        if ( limit_retry( &SiSTouch_Zeus_USB::usb_command_DC ) )
        {
            ret = ERROR_TIMEOUT;
            break;
        }

        ret = command_d8_read( 0x0 );

        if ( ret < 0 )
        {
            ret = parse_syscall_return_value( ret, GENERAL_TYPE_FLAG );
            break;
        }
        else if( ret <= static_cast<int>(get_MAX_SIZE()))
        {
            for( int j = 0; j < ret; j++ )
                data[j] = m_buffer[j];
            break;
        }
        else
        {
            ret = ERROR_WRONG_FORMAT;
            break;
        }
    }

    if( ret < 0 )
    {
        if ( m_verbose )
        {
            log_out_error_message( ret, cmd );
        }
    }
    return ret;
}

int
SiSTouch_Zeus_USB::call_simple_command( int cmd, unsigned char *data )
{

    int ret = 0;

    if( m_verbose)
    {
        LOGI("call_%02x()", cmd);
    }

    for( int i = 0; i < m_retry; i++)
    {
        ret = command_d8_read( cmd );

        if( ret < 0 )
        {
            ret = parse_syscall_return_value( ret, GENERAL_TYPE_FLAG );
            break;
        }
        else if( ret <= static_cast<int>(get_MAX_SIZE()))
        {
            for( int j = 0; j < ret; j++ )
                data[j] = m_buffer[j];
            break;
        }
        else
        {
            ret = ERROR_WRONG_FORMAT;
            break;
        }
    }
    if( ret < 0 )
    {
        if ( m_verbose )
        {
            log_out_error_message( ret, cmd );
        }
    }
    return ret;

}


int SiSTouch_Zeus_USB::limit_retry( int (SiSTouch_Zeus_USB::*func)() )
{
    int flag = 0;
    for( int i = 0; i < m_retry; i++ )
    {
        if ( NACK != (this->*func)() )
        {
            flag = 1;
            break;
        }

        usleep( m_delay );
    }

    if ( !flag )
    {
        if ( m_verbose )
        {
            LOGE( "ERROR: command not ACK, give up" );
        }
        return 1;
    }
    else
        return 0;
}

int
SiSTouch_Zeus_USB::command_d8_read( int cmd )
{
    int ret;

    ret = sis_usb_io( 0xD8, 1, cmd, 0, m_buffer, 2, DEFAULT_TIMEOUT );

    if ( ret < 0 )
    {
        return parse_syscall_return_value( ret, GENERAL_TYPE_FLAG );
    }

    if ( limit_retry( &SiSTouch_Zeus_USB::usb_command_DB ) )
        return ERROR_TIMEOUT;

    ret = sis_usb_io( 0xD8, 1, 0x0, 0, m_buffer, BUFFER_SIZE, DEFAULT_TIMEOUT );

    if ( m_verbose ) dbg_print_buffer_hex( cmd, m_buffer, ret );

    if ( ret < 0 )
    {
        if ( m_verbose )
        {
            LOGE( "syscall error: %d", ret );
        }
        return ret;
    }
    else
        return ret;
}

int
SiSTouch_Zeus_USB::is_noneed_retry_error( int ret )
{
    return 0;
}



int
SiSTouch_Zeus_USB::sis_usb_start(void)
{
#ifdef VIA_IOCTL
    int ret = 0;
    if(m_fd >= 0)
    {
        ret = close(m_fd);
        m_fd = -1;
    }
    return ret;
#else
    int ret = syscall( __NR_sis_HID_start );

    return ret;
#endif
}

int
SiSTouch_Zeus_USB::sis_usb_stop(void)
{
#ifdef VIA_IOCTL
    const char* devName = SiSTouchIO::getActiveDeviceName();
    if (devName==NULL)
    	devName = DEVICE_NAME;

    LOGI("%s: devName = %s\n", __FUNCTION__, devName);

    if(m_fd >= 0)
    {
        close(m_fd);
        m_fd = -1;
    }
    m_fd = open(devName,O_RDWR);
    return m_fd;
#else
    int ret = syscall( __NR_sis_HID_stop );

    return ret;
#endif
}

int
SiSTouch_Zeus_USB::sis_usb_query(void)
{
#ifdef VIA_IOCTL
    return 0;
#else
    int ret = syscall( __NR_sis_HID_query );

    return ret;
#endif
}

int
SiSTouch_Zeus_USB::sis_usb_io( unsigned char request, unsigned char dir, unsigned int value, unsigned int index, void* data, unsigned int size, int timeout)
{
    int ret = 0;
    unsigned char * buf = (unsigned char *)data;
#ifdef VIA_IOCTL


    unsigned char databuf [37] = {0};
    databuf[0] = request;

    databuf[1] = dir;

    databuf[2] = value >> 24 & 0xff;
    databuf[3] = value >> 16 & 0xff;
    databuf[4] = value >> 8 & 0xff;
    databuf[5] = value & 0xff;

    databuf[6] = index >> 24 & 0xff;
    databuf[7] = index >> 16 & 0xff;
    databuf[8] = index >> 8 & 0xff;
    databuf[9] = index & 0xff;

    for(unsigned int i=0; i<get_MAX_SIZE(); i++)
    {
        databuf[10 + i] = buf[i];
    }

    databuf[29] = size >> 24 & 0xff;
    databuf[30] = size >> 16 & 0xff;
    databuf[31] = size >> 8 & 0xff;
    databuf[32] = size & 0xff;

    databuf[33] = timeout >> 24 & 0xff;
    databuf[34] = timeout >> 16 & 0xff;
    databuf[35] = timeout >> 8 & 0xff;
    databuf[36] = timeout & 0xff;

    ret = write(m_fd, databuf, 37);

    for(unsigned int i=0; i<get_MAX_SIZE(); i++)
    {
        buf[i] = databuf[10 + i];
    }

#else
    ret = syscall( __NR_sis_HID_IO, request, dir, value, index, data, size, timeout );
#endif

    if(m_IODelayms != 0)
    {
        usleep(m_IODelayms * 1000);
    }

    return ret;
}


int
SiSTouch_Zeus_USB::check_ack( const unsigned char* data )
{
    if ( data[0] == 0xAB && data[1] == 0xCD )
    {
        return ACK;
    }
    else if ( data[0] == 0xDC && data[1] == 0xBA )
    {
        return NACK;
    }
    else
    {
        return ERROR_ACK;
    }
}

int
SiSTouch_Zeus_USB::usb_simple_command( int request )
{
    unsigned char buffer[BUFFER_SIZE];
    int ret = sis_usb_io( request, 1, 0, 0, buffer, 2, DEFAULT_TIMEOUT );

    if( ret < 0 )
    {
        return ret;
    }
    return check_ack( buffer );
}

int
SiSTouch_Zeus_USB::usb_command_D7()
{
    return usb_simple_command( 0xD7 );
}

int
SiSTouch_Zeus_USB::usb_command_D8_P810_reset()
{
    unsigned char buffer[BUFFER_SIZE];
    LOGI( "Reset (0x82)" );

    int ret = sis_usb_io( 0xD8, 1, 0x82, 0, buffer, 2, DEFAULT_TIMEOUT );
    if( ret < 0 )
    {
        return ret;
    }
    return check_ack( buffer );
}


int
SiSTouch_Zeus_USB::usb_command_D8_P810_04_query()
{
    unsigned char buffer[BUFFER_SIZE];
    LOGI( "Query (0x04)" );
    int ret = sis_usb_io( 0xD8, 1, 0x4, 0, buffer, 2, DEFAULT_TIMEOUT );
    if( ret < 0 )
    {
        return ret;
    }
    return check_ack( buffer );
}


void
SiSTouch_Zeus_USB::usb_command_D8_get_data( unsigned char* data, int* length )
{
    unsigned char buffer[BUFFER_SIZE];
    int ret = sis_usb_io( 0xD8, 1, 0x0, 0, buffer, get_MAX_SIZE(), DEFAULT_TIMEOUT );

    *length = ret;
    data = buffer;
}


int
SiSTouch_Zeus_USB::usb_command_DA()
{
    return usb_simple_command( 0xDA );
}

int
SiSTouch_Zeus_USB::usb_command_DB()
{
    return usb_simple_command( 0xDB );
}

int
SiSTouch_Zeus_USB::usb_command_DC()
{
    return usb_simple_command( 0xDC );
}

int
SiSTouch_Zeus_USB::usb_command_D9( unsigned char* buffer, int size )
{
    return sis_usb_io( 0xD9, 0, 0, 0, buffer, size, DEFAULT_TIMEOUT );
}


void
SiSTouch_Zeus_USB::log_out_error_message( int ret, int cmd)
{
#ifdef VIA_IOCTL
    const char* devName = SiSTouchIO::getActiveDeviceName();
    if (devName==NULL)
      	devName = DEVICE_NAME;
#endif

    if(cmd == -1)
    {
        LOGE( "CMD error: %d", ret );
        switch(ret)
        {
        case SiSTouchIO::ERROR_SYSCALL:
            LOGE( "called error system call" );
            break;
#ifdef VIA_IOCTL
        case SiSTouchIO::ERROR_ACCESS:
            LOGE( "cannot access device %s", devName );
            break;
        case SiSTouchIO::ERROR_ENTRY_NOT_EXIST:
            LOGE( "entry %s not exist", devName );
            break;
        case SiSTouchIO::ERROR_DEVICE_NOT_EXIST:
            LOGE( "device %s not exist", devName );
            break;
        default:
            LOGE( "Got errno %d", ret );
            break;
#else
        default:
            LOGE( "ASSERT: Got impossible error code %d", ret );
            break;
#endif
        }
    }
    else
    {
        LOGE( "CMD %02x syscall error: %d", cmd, ret );
        switch(ret)
        {
        case SiSTouchIO::ERROR:
            LOGE( "CMD %02x error", cmd );
            break;
        case SiSTouchIO::ERROR_NACK:
            LOGE( "CMD %02x not receive ack", cmd );
            break;
        case SiSTouchIO::ERROR_DEAD:
            LOGE( "CMD %02x dead", cmd );
            break;
        case SiSTouchIO::ERROR_SYSCALL:
            LOGE( "CMD %02x invoke non exist system call", cmd );
            break;
        case SiSTouchIO::ERROR_TIMEOUT:
            LOGE( "CMD %02x transmit timeout", cmd );
            break;
        case SiSTouchIO::ERROR_QUERY:
            LOGE( "cannot query driver" );
            break;
        case SiSTouchIO::ERROR_EACK:
            LOGE( "CMD %02x get error ack format", cmd );
            break;
            /*
               case SiSTouchIO::ERROR_USBNACK:
                   LOGE( "CMD %02x get USB nack", cmd );
                   break;
            */
        case SiSTouchIO::ERROR_USBEACK:
            LOGE(" CMD %02x get error USB ack format", cmd );
            break;
        case SiSTouchIO::ERROR_ACCESS_USER_MEMORY:
            LOGE( "Fail to access data from user memory" );
            break;
        case SiSTouchIO::ERROR_ALLOCATE_KERNEL_MEMORY:
            LOGE( "Fail to allocate kernel memory" );
            break;
        case SiSTouchIO::ERROR_COPY_FROM_USER:
            LOGE( "Fail to copy data from user memory to kernel memory" );
            break;
        case SiSTouchIO::ERROR_COPY_FROM_KERNEL:
            LOGE( "Fail to copy data from kernel memory to user memory" );
            break;
        case SiSTouchIO::ERROR_USB_DEV_NULL:
            LOGE( "SiS Touch device not exist" );
            break;
        case ERROR_BROKEN_PIPE:
            LOGE( "USB I/O broken pipe" );
            break;
        case SiSTouchIO::ERROR_WRONG_FORMAT:
            LOGE( "CMD %02x return not recognized format", cmd);
            break;
        case SiSTouchIO::ERROR_NOT_SUPPORT:
            LOGE( "CMD %02x not supported by this interface", cmd);
            break;
        case SiSTouchIO::ERROR_CRC_MISMATCH:
            LOGE( "CMD %02x crc error", cmd);
            break;
        default:
            LOGE( "ASSERT: CMD %02x got impossible error code", cmd );
            break;
        }
    }
}

const char*
SiSTouch_Zeus_USB::get_device_name()
{
	return DEVICE_NAME;
}

int
SiSTouch_Zeus_USB::device_Exist()
{

#ifdef VIA_IOCTL
    const char* devName = SiSTouchIO::getActiveDeviceName();
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
    return 0;
#else
    return 0;
#endif

}
//20120903
int
SiSTouch_Zeus_USB::test_write_data(unsigned char * buf, int size)
{
	return command_d8_read(0x0);
}

int
SiSTouch_Zeus_USB::test_read_data( unsigned char * buf, int size)
{
	return usb_command_D9(buf, size);
}

//20120903

//20120925
int
SiSTouch_Zeus_USB::call_simple_command_with_wrong_crc( int cmd, unsigned char *data )
{
	return ERROR_NOT_SUPPORT;
}
//
