/*
 * SiSTouchInterUSB.cpp
 *
 *  Created on: May 2, 2011
 *      Author: enya
 */

#include "SiSTouchIO.h"
#include "SiSLogger.h"
#include <errno.h>
#include <string.h>

#define DEVICE_CLOSE_RETRY_TIMES 20
#define DEVICE_CLOSE_RETRY_INTERVAL 1000

#ifdef VIA_IOCTL
#include <fcntl.h>
#include <sys/stat.h>
#define DEVICE_NAME "/dev/sis_zeus_hid_touch_device"
#else
#define __NR_sis_HID_stop       366
#define __NR_sis_HID_start      367
#define __NR_sis_HID_query      368
#define __NR_sis_HID_IO         369
#endif

#define CLOSE_AFTER_RESET

#define WAIT_DEVICE_REMOVE 1000

int
SiSTouch_Zeus_InterUSB::make_stop_buffer()
{
    int buf_idx = 0;

    m_buffer[ buf_idx++ ] = 0x5A;
    m_buffer[ buf_idx++ ] = 0x30;
    m_buffer[ buf_idx++ ] = 0x0;

    if ( m_verbose ) dbg_print_buffer_hex( m_buffer, buf_idx );

    return buf_idx;
}

int
SiSTouch_Zeus_InterUSB::stop_driver()
{
    int ret;

    if( m_verbose)
    {
        LOGI("stop_driver()");
    }

    for(int i = 0; i < DEVICE_CLOSE_RETRY_TIMES; i++ )
    {
        ret = sis_usb_stop();
        if(ret >= 0)
        {
            break;
        }
        else
        {
#ifdef FOR_ANDROID_X86
            if(ret == -ENOSYS)
            {
                break;
            }
#ifdef VIA_IOCTL
            if(ret == -EACCES || ret == -ENOENT || ret == -ENODEV)
            {
                break;
            }
#endif
#else
            if(errno == ENOSYS)
            {
                break;
            }
#ifdef VIA_IOCTL
            if(errno == EACCES || errno == ENOENT || errno == ENODEV)
            {
                break;
            }
#endif
#endif
        }
        usleep( DEVICE_CLOSE_RETRY_INTERVAL * 1000 );
    }


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

    //stop report to os
    int buf_idx = make_stop_buffer();

    ret = sis_usb_io( 0, 0, 0, 0, m_buffer, buf_idx, DEFAULT_TIMEOUT );

    if( m_verbose)
    {
        LOGI("stop report to os-io_out, ret=%d", ret);
    }
    if ( ret < 0 )
    {
        ret = parse_syscall_return_value( ret, GENERAL_TYPE_FLAG );
        if ( m_verbose )
        {
            log_out_error_message( ret, 0x5A );
        }

        // fail of stop, but device has been open, so we need to close device
        sis_usb_start();
        return ret;
    }

    return 0;
}

int
SiSTouch_Zeus_InterUSB::make_start_buffer()
{
    int buf_idx = 0;

    m_buffer[ buf_idx++ ] = 0x5A;
    m_buffer[ buf_idx++ ] = 0x31;
    m_buffer[ buf_idx++ ] = 0x0;

    if ( m_verbose ) dbg_print_buffer_hex( m_buffer, buf_idx );

    return buf_idx;
}

int
SiSTouch_Zeus_InterUSB::start_driver()
{
    int ret;

    if( m_verbose)
    {
        LOGI("start_driver()");
    }


    //start report to os
    int buf_idx = make_start_buffer();

    ret = sis_usb_io( 0, 0, 0, 0, m_buffer, buf_idx, DEFAULT_TIMEOUT );
    if( m_verbose)
    {
        LOGI("start report to os-io_out, ret=%d", ret);
    }
    if ( ret < 0 )
    {
        ret = parse_syscall_return_value( ret, GENERAL_TYPE_FLAG );
        if ( m_verbose )
        {
            log_out_error_message( ret, 0x5A );
        }
        return ret;
    }


    for(int i = 0; i < DEVICE_CLOSE_RETRY_TIMES; i++ )
    {
        ret = sis_usb_start();
        if(ret >= 0)
        {
            break;
        }
        else
        {
#ifdef FOR_ANDROID_X86
            if(ret == -ENOSYS)
            {
                break;
            }
            if(ret == -EACCES || ret == -ENOENT || ret == -ENODEV)
            {
                break;
            }
#ifdef VIA_IOCTL
#endif
#else
            if(errno == ENOSYS)
            {
                break;
            }
#ifdef VIA_IOCTL
            if(errno == EACCES || errno == ENOENT || errno == ENODEV)
            {
                break;
            }
#endif
#endif
        }
        usleep( DEVICE_CLOSE_RETRY_INTERVAL * 1000 );
    }

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
        return ret;
    }

    return 0;
}

int
SiSTouch_Zeus_InterUSB::make_00_buffer()
{
    int buf_idx = 0;

    m_buffer[ buf_idx++ ] = 0x0;
    m_buffer[ buf_idx++ ] = 0x0;
    m_buffer[ buf_idx++ ] = 0x0;

    if ( m_verbose ) dbg_print_buffer_hex( m_buffer, buf_idx );

    return buf_idx;
}

int
SiSTouch_Zeus_InterUSB::call_00( unsigned char *data, int size )
{
    if( m_verbose)
    {
        LOGI("call_00()");
    }

    int ret;
    int buf_idx = make_00_buffer();

    ret = sis_usb_io( 0, 0, 0, 0, m_buffer, buf_idx, DEFAULT_TIMEOUT );
    if( m_verbose)
    {
        LOGI("call_00()-io_out, ret=%d", ret);
    }
    if ( ret < 0 )
    {
        ret = parse_syscall_return_value( ret, GENERAL_TYPE_FLAG );
        if ( m_verbose )
        {
            log_out_error_message( ret, 0x00 );
        }
        return ret;
    }

    ret = sis_usb_io( 0, 1, 0, 0, m_buffer, BUFFER_SIZE, DEFAULT_TIMEOUT );
    if( m_verbose)
    {
        LOGI("call_00()-io_in, ret=%d", ret);
    }


    if ( ret < 0 )
    {
        ret = parse_syscall_return_value( ret, GENERAL_TYPE_FLAG);
    }
    else if( ret <= static_cast<int>(get_MAX_SIZE()) )
    {
        int len = 0;
        unsigned char buf[256];
        len = (m_buffer[1] > 16) ? 16 : m_buffer[1];

        for(int i=0; i<len && i<15; i++)
        {
            buf[i] = m_buffer[i+3];
        }
        if(len == 16)
            buf[15] = m_buffer[2];

        ret = 0;
        for (int i = 0; i < len && i < size; i++ )
        {
            data[i] = buf[i];
            ret++;
        }
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

int
SiSTouch_Zeus_InterUSB::make_04_start_buffer()
{
    int buf_idx = 0;

    m_buffer[ buf_idx++ ] = 0x4B;
    m_buffer[ buf_idx++ ] = 0x0;
    m_buffer[ buf_idx++ ] = 0x0;

    if ( m_verbose ) dbg_print_buffer_hex( m_buffer, buf_idx );

    return buf_idx;
}

int
SiSTouch_Zeus_InterUSB::make_04_query_buffer()
{
    int buf_idx = 0;

    m_buffer[ buf_idx++ ] = 0x4B;
    m_buffer[ buf_idx++ ] = 0x5A;
    m_buffer[ buf_idx++ ] = 0x0;

    if ( m_verbose ) dbg_print_buffer_hex( m_buffer, buf_idx );

    return buf_idx;
}

int SiSTouch_Zeus_InterUSB::call_04()
{
    if( m_verbose)
    {
        LOGI("call_04()");
    }

    int ret;
    int buf_idx;

    //program finish
    buf_idx = make_04_start_buffer();

    ret = sis_usb_io( 0, 0, 0, 0, m_buffer, buf_idx, DEFAULT_TIMEOUT );
    if( m_verbose)
    {
        LOGI("call_04_prog()-io_out, ret=%d", ret);
    }
    if ( ret < 0 )
    {
        ret = parse_syscall_return_value( ret, GENERAL_TYPE_FLAG );
        if ( m_verbose )
        {
            log_out_error_message( ret, 0x04 );
        }
        return ret;
    }

    //wait ready
    for( int i = 0; i < m_retry; i++ )
    {
        buf_idx = make_04_query_buffer();

        ret = sis_usb_io( 0, 0, 0, 0, m_buffer, buf_idx, DEFAULT_TIMEOUT );

        if( m_verbose)
        {
            LOGI("call_04()-io_out, ret=%d", ret);
        }
        if ( ret < 0 )
        {
            ret = parse_syscall_return_value( ret, GENERAL_TYPE_FLAG );

            if ( m_verbose )
            {
                log_out_error_message( ret, 0x04 );
            }
            return ret;
        }

        ret = sis_usb_io( 0, 1, 0, 0, m_buffer, BUFFER_SIZE, DEFAULT_TIMEOUT );

        if( m_verbose)
        {
            LOGI("call_04()-io_in, ret=%d", ret);
            LOGI("buf=%02x %02x %02x", m_buffer[0], m_buffer[1], m_buffer[2]);
        }


        if ( ret < 0 )
        {
            ret = parse_syscall_return_value( ret, GENERAL_TYPE_FLAG );

            if ( m_verbose )
            {
                log_out_error_message( ret, 0x04 );
            }
            return ret;
        }
        else
        {
            if(m_buffer[1]==0xBE && m_buffer[2]==0xEF)
            {
                return 0;
            }
            else if(m_buffer[1]==0xDE && m_buffer[2]==0xAD)
            {
                return ERROR;
            }
            else
            {
                return ERROR;
            }
        }
        usleep( m_delay );
    }

    return ret;

}

int
SiSTouch_Zeus_InterUSB::call_81()
{


    if( m_verbose)
    {
        LOGI("call_81()-not implement for internal usb");
    }

    return ERROR;
}

int
SiSTouch_Zeus_InterUSB::make_82_buffer()
{
    int buf_idx = 0;

    m_buffer[ buf_idx++ ] = 0x69;
    m_buffer[ buf_idx++ ] = 0x0;
    m_buffer[ buf_idx++ ] = 0x0;

    if ( m_verbose ) dbg_print_buffer_hex( m_buffer, buf_idx );

    return buf_idx;
}

int
SiSTouch_Zeus_InterUSB::call_82()
{

    if( m_verbose)
    {
        LOGI("call_82()");
    }

    int ret;
    int buf_idx = make_82_buffer();

    ret = sis_usb_io( 0, 0, 0, 0, m_buffer, buf_idx, DEFAULT_TIMEOUT );
    if( m_verbose)
    {
        LOGI("call_82()-io_out, ret=%d", ret);
    }
    if ( ret < 0 )
    {
        ret = parse_syscall_return_value( ret, GENERAL_TYPE_FLAG );
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
            usleep( DEVICE_CLOSE_RETRY_INTERVAL * 1000 );
        }
#else
        ret = stop_driver();
#endif
    }

    return ret;
}

int
SiSTouch_Zeus_InterUSB::call_82_without_retry()
{

    if( m_verbose)
    {
        LOGI("call_82_without_retry()");
    }

    int ret;
    int buf_idx = make_82_buffer();

    ret = sis_usb_io( 0, 0, 0, 0, m_buffer, buf_idx, DEFAULT_TIMEOUT );
    if( m_verbose)
    {
        LOGI("call_82()-io_out, ret=%d", ret);
    }
    if ( ret < 0 )
    {
        ret = parse_syscall_return_value( ret, GENERAL_TYPE_FLAG );
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
            usleep( DEVICE_CLOSE_RETRY_INTERVAL * 1000 );
        }
#else
        ret = stop_driver();
#endif
    }
    return ret;

}


int
SiSTouch_Zeus_InterUSB::make_83_buffer( int base, int pack_num )
{
    int buf_idx = 0;
    unsigned char temp;

    m_buffer[ buf_idx++ ] = 0x2D;

    for ( int i = 0; i <= 1; i++ )
    {
        temp = ( pack_num >> (8*i) ) & 0xff;
        m_buffer[ buf_idx++ ] = temp;
    }

    for ( int i = 0; i < 4; i++ )
    {
        temp = ( base >> (8*i) ) & 0xff;
        m_buffer[ buf_idx++ ] = temp;
    }

    if ( m_verbose ) dbg_print_buffer_hex( m_buffer, buf_idx );

    return buf_idx;
}

int
SiSTouch_Zeus_InterUSB::call_83( int base, int pack_num )
{

    if( m_verbose)
    {
        LOGI("call_83()");
        LOGI(" base=%08x,pack_num=%d", base, pack_num);
    }


    int ret;
    int buf_idx = make_83_buffer(base, pack_num);

    ret = sis_usb_io( 0, 0, 0, 0, m_buffer, buf_idx, DEFAULT_TIMEOUT );
    if( m_verbose)
    {
        LOGI("call_83()-io_out, ret=%d", ret);
    }
    if ( ret < 0 )
    {
        ret = parse_syscall_return_value( ret, GENERAL_TYPE_FLAG );
        if ( m_verbose )
        {
            log_out_error_message( ret, 0x83 );
        }
    }
    return ret;
}

int
SiSTouch_Zeus_InterUSB::make_84_buffer( const int* data, int size )
{
    int buf_idx = 0;

    m_buffer[ buf_idx++ ] = 0x3C;
    m_buffer[ buf_idx++ ] = 16;
    m_buffer[ buf_idx++ ] = data[3] & 0xff;
    m_buffer[ buf_idx++ ] = data[0] >> 24 & 0xff;
    m_buffer[ buf_idx++ ] = data[0] >> 16 & 0xff;
    m_buffer[ buf_idx++ ] = data[0] >> 8 & 0xff;
    m_buffer[ buf_idx++ ] = data[0] & 0xff;
    m_buffer[ buf_idx++ ] = data[1] >> 24 & 0xff;
    m_buffer[ buf_idx++ ] = data[1] >> 16 & 0xff;
    m_buffer[ buf_idx++ ] = data[1] >> 8 & 0xff;
    m_buffer[ buf_idx++ ] = data[1] & 0xff;
    m_buffer[ buf_idx++ ] = data[2] >> 24 & 0xff;
    m_buffer[ buf_idx++ ] = data[2] >> 16 & 0xff;
    m_buffer[ buf_idx++ ] = data[2] >> 8 & 0xff;
    m_buffer[ buf_idx++ ] = data[2] & 0xff;
    m_buffer[ buf_idx++ ] = data[3] >> 24 & 0xff;
    m_buffer[ buf_idx++ ] = data[3] >> 16 & 0xff;
    m_buffer[ buf_idx++ ] = data[3] >> 8 & 0xff;

    if ( m_verbose ) dbg_print_buffer_hex( m_buffer, buf_idx );

    return buf_idx;
}

int
SiSTouch_Zeus_InterUSB::call_84( const int* data, int size )
{

    if( m_verbose)
    {
        LOGI("call_84()");
        LOGI(" data=%08x,%08x,%08x,%08x,s=%d", data[0], data[1], data[2], data[3], size);
    }


    int ret;
    int buf_idx = make_84_buffer(data, size);

    ret = sis_usb_io( 0, 0, 0, 0, m_buffer, buf_idx, DEFAULT_TIMEOUT );
    if( m_verbose)
    {
        LOGI("call_84()-io_out, ret=%d", ret);
    }
    if ( ret < 0 )
    {
        ret = parse_syscall_return_value( ret, GENERAL_TYPE_FLAG );
        if ( m_verbose )
        {
            log_out_error_message( ret, 0x84 );
        }
    }

    return ret;

}

int
SiSTouch_Zeus_InterUSB::make_85_buffer( unsigned char data )
{
    int buf_idx = 0;

    m_buffer[ buf_idx++ ] = 0x78;
    m_buffer[ buf_idx++ ] = data;
    m_buffer[ buf_idx++ ] = 0x0;

    if ( m_verbose ) dbg_print_buffer_hex( m_buffer, buf_idx );

    return buf_idx;
}

int
SiSTouch_Zeus_InterUSB::call_85(int data )
{



    int ret;
    int idx = 0;

    unsigned char mode = data & 0xff;

    if( m_verbose)
    {
        LOGI("call_85()");
        LOGI(" data=%02x", mode);
    }

    idx = make_85_buffer(mode);


    ret = sis_usb_io( 0, 0, 0, 0, m_buffer, idx, DEFAULT_TIMEOUT );
    if( m_verbose)
    {
        LOGI("call_85()-io_out, ret=%d", ret);
    }
    if ( ret < 0 )
    {
        ret = parse_syscall_return_value( ret, GENERAL_TYPE_FLAG );
        if ( m_verbose )
        {
            log_out_error_message( ret, 0x85 );
        }
    }

    return ret;

}

int
SiSTouch_Zeus_InterUSB::make_86_buffer( int addr )
{
    int buf_idx = 0;
    unsigned char temp;

    m_buffer[ buf_idx++ ] = 0x1E;
    m_buffer[ buf_idx++ ] = 0x0;
    m_buffer[ buf_idx++ ] = 0x0;
    for ( int i = 0; i < 4; i++ )
    {
        temp = ( addr >> (8*i) ) & 0xff;
        m_buffer[ buf_idx++ ] = temp;
    }

    if ( m_verbose ) dbg_print_buffer_hex( m_buffer, buf_idx );

    return buf_idx;
}

int
SiSTouch_Zeus_InterUSB::call_86( int address, int *data, int size )
{

    if( m_verbose)
    {
    	LOGI("call_86()");
        LOGI(" address=%08x", address);
    }

    int ret;
    int buf_idx = make_86_buffer(address);

    ret = sis_usb_io( 0, 0, 0, 0, m_buffer, buf_idx, DEFAULT_TIMEOUT );
    if( m_verbose)
    {
        LOGI("call_86()-io_out, ret=%d", ret);
    }
    if ( ret < 0 )
    {
        ret = parse_syscall_return_value( ret, GENERAL_TYPE_FLAG );
        if ( m_verbose )
        {
            log_out_error_message( ret, 0x86 );
        }
        return ret;
    }

    ret = sis_usb_io( 0, 1, 0, 0, m_buffer, BUFFER_SIZE, DEFAULT_TIMEOUT );
    if( m_verbose)
    {
        LOGI("call_86()-io_in, ret=%d", ret);
    }

    if ( ret < 0 )
    {
        ret = parse_syscall_return_value( ret, GENERAL_TYPE_FLAG );
    }
    else if( ret <= static_cast<int>(get_MAX_SIZE()) )
    {
        int len = 0;
        unsigned char buf[256];
        len = (m_buffer[1] > 16) ? 16 : m_buffer[1];

        for(int i=0; i<len && i<15; i++)
        {
            buf[i] = m_buffer[i+3];
        }
        if(len == 16)
            buf[15] = m_buffer[2];

        ret = 0;
        for (int i = 0; i < 4 && i < size; i++ )
        {
            ret++;
            data[i] = 0;
            for ( int j = 0; j < 4; j++ )
            {
                data[i] |= (buf[i*4+j] << (8 * j));
            }
        }
    }
    else
    {
        ret = ERROR_WRONG_FORMAT;
    }

    if ( ret < 0 )
    {
        if ( m_verbose )
        {
            log_out_error_message( ret, 0x86 );
        }
    }

    return ret;
}


int
SiSTouch_Zeus_InterUSB::make_87_buffer()
{
    int buf_idx = 0;

    m_buffer[ buf_idx++ ] = 0x69;
    m_buffer[ buf_idx++ ] = 0xCA;
    m_buffer[ buf_idx++ ] = 0x0;

    return buf_idx;
}

int
SiSTouch_Zeus_InterUSB::call_87()
{

    if( m_verbose)
    {
        LOGI("call_87()");
    }
    int ret;
    int buf_idx = make_87_buffer();

    ret = sis_usb_io( 0, 0, 0, 0, m_buffer, buf_idx, DEFAULT_TIMEOUT );
    if( m_verbose)
    {
        LOGI("call_87()-io_out, ret=%d", ret);
    }
    if ( ret < 0 )
    {
        ret = parse_syscall_return_value( ret, GENERAL_TYPE_FLAG );
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
            usleep( DEVICE_CLOSE_RETRY_INTERVAL * 1000 );
        }
#else
        ret = stop_driver();
#endif
    }

    return ret;

}

int
SiSTouch_Zeus_InterUSB::make_88_buffer( int *addr, int *data, int *mask, int number )
{
    int buf_idx = 0;
    unsigned char temp;

    if(addr != 0)
    {
        m_buffer[ buf_idx++ ] = 0x2D;
        m_buffer[ buf_idx++ ] = 0x0;
        m_buffer[ buf_idx++ ] = 0x0;
        for ( int i = 0; i < 4; i++ )
        {
            temp = ( addr[0] >> (8*i) ) & 0xff;
            m_buffer[ buf_idx++ ] = temp;
        }
        for ( int i = 0; i < 4; i++ )
        {
            temp = ( data[0] >> (8*i) ) & 0xff;
            m_buffer[ buf_idx++ ] = temp;
        }
        for ( int i = 0; i < 4; i++ )
        {
            temp = ( mask[0] >> (8*i) ) & 0xff;
            m_buffer[ buf_idx++ ] = temp;
        }
    }
    else
    {
        m_buffer[ buf_idx++ ] = 0x0;
        m_buffer[ buf_idx++ ] = 0x0;
        m_buffer[ buf_idx++ ] = 0x0;
    }

    if ( m_verbose ) dbg_print_buffer_hex( m_buffer, buf_idx );

    return buf_idx;
}

int
SiSTouch_Zeus_InterUSB::call_88( int *address, int *data, int *mask, int number )
{

    if( m_verbose)
    {
        LOGI("call_88()");
        LOGI(" address=%08x, data=%08x, mask=%08x", address[0], data[0], mask[0]);
    }

    int ret;
    int buf_idx = make_88_buffer(address, data, mask, number);

    ret = sis_usb_io( 0, 0, 0, 0, m_buffer, buf_idx, DEFAULT_TIMEOUT );
    if( m_verbose)
    {
        LOGI("call_88()-io_out, ret=%d", ret);
    }

    if ( ret < 0 )
    {
        ret = parse_syscall_return_value( ret, GENERAL_TYPE_FLAG );
        if ( m_verbose )
        {
            log_out_error_message( ret, 0x88 );
        }
        return ret;
    }


    ret = sis_usb_io( 0, 1, 0, 0, m_buffer, BUFFER_SIZE, DEFAULT_TIMEOUT );
    if( m_verbose)
    {
        LOGI("call_88()-io_in, ret=%d", ret);
    }

    if ( ret < 0 )
    {
        ret = parse_syscall_return_value( ret, GENERAL_TYPE_FLAG);
    }
    else if( ret <= static_cast<int>(get_MAX_SIZE()) )
    {
        ret= 0;
        for ( int i = 0; i < 1; i++ )
        {
            data[i] = 0;
            for ( int j = 0; j < 4; j++ )
                data[i] |= m_buffer[i*4+j+3] << 8 * j;
            ret++;
        }
    }
    else
    {
        ret = ERROR_WRONG_FORMAT;
    }


    if ( ret < 0 )
    {
        if ( m_verbose )
        {
            log_out_error_message( ret, 0x88 );
        }
    }

    return ret;

}

int
SiSTouch_Zeus_InterUSB::call_89( int *status )
{
    int ret = ERROR_NOT_SUPPORT;
    if ( m_verbose )
    {
        log_out_error_message( ret, 0x89 );
    }
    return ret;
}

int
SiSTouch_Zeus_InterUSB::call_90( int *powermode )
{

    unsigned char mode = powermode[0] & 0xff;

    if( m_verbose)
    {
        LOGI("call_90()");
        LOGI(" powermode=%02x", mode);
    }

    int ret = ERROR_NOT_SUPPORT;
    if ( m_verbose )
    {
        log_out_error_message( ret, 0x90 );
    }
    return ret;
}

int
SiSTouch_Zeus_InterUSB::make_command_buffer( int cmd, int size, unsigned char *data, int addcrc )
{
    int buf_idx = 0;

    m_buffer[ buf_idx++ ] = cmd;
    m_buffer[ buf_idx++ ] = 0x0;
    m_buffer[ buf_idx++ ] = 0x0;

    for ( int i = 0; i < size; i++ )
    {
        //hid id 0x09 will be added by driver
        if(buf_idx >= (static_cast<int>(get_MAX_SIZE()) - 1) )
        {
            if ( m_verbose ) LOGE("Data exceed buffer size truncate");
            break;
        }
        m_buffer[ buf_idx++ ] = data[i];
    }


    if ( m_verbose ) dbg_print_buffer_hex( m_buffer, buf_idx );



    return buf_idx;
}

int
SiSTouch_Zeus_InterUSB::limit_retry( int (SiSTouch_Zeus_InterUSB::*func)() )
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
SiSTouch_Zeus_InterUSB::call_command( int cmd, int size, unsigned char *data, int addcrc )
{
    int ret = 0;
    int buf_idx;

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

    buf_idx = make_command_buffer(cmd, size, data, addcrc);

    ret = sis_usb_io( 0, 0, 0, 0, m_buffer, buf_idx, DEFAULT_TIMEOUT );
    if( m_verbose)
    {
        LOGI("call_%02x()-io_out, ret=%d", cmd, ret);
    }

    if ( ret < 0 )
    {
        ret = parse_syscall_return_value( ret, GENERAL_TYPE_FLAG);
        if ( m_verbose )
        {
            log_out_error_message( ret, cmd );
        }
        return ret;
    }

    ret = sis_usb_io( 0, 1, 0, 0, m_buffer, BUFFER_SIZE, DEFAULT_TIMEOUT );
    if( m_verbose)
    {
        LOGI("call_%02x()-io_in, ret=%d", cmd, ret);
    }
    if ( ret < 0 )
    {
        ret = parse_syscall_return_value( ret, GENERAL_TYPE_FLAG);
        if ( m_verbose )
        {
            log_out_error_message( ret, cmd );
        }
        return ret;
    }
    else
    {
        for( int i = 0; i < (ret - 3) ; i++ )
        {
            data[i] = m_buffer[i + 2];
        }
        return (ret - 3);
    }

}

int
SiSTouch_Zeus_InterUSB::call_simple_command( int cmd, unsigned char *data )
{
    int ret = 0;
    int buf_idx;

    if( m_verbose)
    {
        LOGI("call_%02x()", cmd);
    }

    buf_idx = make_command_buffer(cmd, 0, 0, 0);

    ret = sis_usb_io( 0, 0, 0, 0, m_buffer, buf_idx, DEFAULT_TIMEOUT );
    if( m_verbose)
    {
        LOGI("call_%02x()-io_out, ret=%d", cmd, ret);
    }
    if ( ret < 0 )
    {
        ret = parse_syscall_return_value( ret, GENERAL_TYPE_FLAG);
        if ( m_verbose )
        {
            log_out_error_message( ret, cmd );
        }
        return ret;
    }

    return ret;

}

int
SiSTouch_Zeus_InterUSB::is_noneed_retry_error( int ret )
{
    return 0;
}

int
SiSTouch_Zeus_InterUSB::sis_usb_start(void)
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
SiSTouch_Zeus_InterUSB::sis_usb_stop(void)
{
#ifdef VIA_IOCTL
    const char* devName = SiSTouchIO::getActiveDeviceName();
    if (devName==NULL)
    	devName = DEVICE_NAME;
    LOGI("%s: devName = %s\n", __FUNCTION__, devName);

    if(m_fd >= 0)
    {
        //driver has bug at close after 0x82 and 0x87
        //close(m_fd);
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
SiSTouch_Zeus_InterUSB::sis_usb_query(void)
{
#ifdef VIA_IOCTL
    return 0;
#else
    int ret = syscall( __NR_sis_HID_query );

    return ret;
#endif
}

int
SiSTouch_Zeus_InterUSB::sis_usb_io( unsigned char request, unsigned char dir, unsigned int value, unsigned int index, void* data, unsigned int size, int timeout)
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

    //ret is the total size + 1

    if(m_IODelayms != 0)
    {
        usleep(m_IODelayms * 1000);
    }


    if(dir == 1)
    {
        if ( m_verbose )
        {
            if(ret >= 1)
            {
                //the size contain removed hid id 0x0a
                dbg_print_buffer_hex( buf, ret-1 );
            }
            else
            {
                dbg_print_buffer_hex( buf, ret );
            }
        }
    }


    return ret;
}



void
SiSTouch_Zeus_InterUSB::log_out_error_message( int ret, int cmd)
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
SiSTouch_Zeus_InterUSB::get_device_name()
{
	return DEVICE_NAME;
}

int
SiSTouch_Zeus_InterUSB::device_Exist()
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
SiSTouch_Zeus_InterUSB::test_write_data(unsigned char * buf, int size)
{
	return sis_usb_io( 0, 0, 0, 0, buf, size, DEFAULT_TIMEOUT );
}

int
SiSTouch_Zeus_InterUSB::test_read_data( unsigned char * buf, int size)
{
	return sis_usb_io( 0, 1, 0, 0, buf, size, DEFAULT_TIMEOUT );
}

//20120903

//20120925
int
SiSTouch_Zeus_InterUSB::call_simple_command_with_wrong_crc( int cmd, unsigned char *data )
{
	return ERROR_NOT_SUPPORT;
}
//
