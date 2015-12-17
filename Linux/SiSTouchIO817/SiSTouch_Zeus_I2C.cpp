/*
 * SiSTouchI2C.cpp
 *
 *  Created on: Mar 26, 2010
 *      Author: suzin
 */

#include "SiSTouchIO.h"
#include "SiSLogger.h"
#include <errno.h>
#include <string.h>

#ifdef VIA_IOCTL
#include <fcntl.h>
#include <sys/stat.h>
#define DEVICE_NAME "/dev/sis_touch_device"
#else
#define __NR_sis_I2C_stop       366
#define __NR_sis_I2C_start      367
#define __NR_sis_I2C_IO         368
#endif

//20120925
#define DELAY_FOR_GENEARL 2

int
SiSTouch_Zeus_I2C::stop_driver()
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
    else
        return 0;
}

int
SiSTouch_Zeus_I2C::start_driver()
{
    if( m_verbose)
    {
        LOGI("start_driver()");
    }

    int ret = io_start_driver();

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
    else
        return 0;
}



int
SiSTouch_Zeus_I2C::call_00( unsigned char *data, int size )
{
    if( m_verbose)
    {
        LOGI("call_00()");
    }
    int ret = simple_read( 0x00 );



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

int
SiSTouch_Zeus_I2C::call_04()
{

    if( m_verbose)
    {
        LOGI("call_04()");
    }

    int flag = 0;
    int ret;
    for( int i = 0; i < m_retry; i++ )
    {
        ret = simple_read( 0x04 );

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
SiSTouch_Zeus_I2C::call_81()
{

    if( m_verbose)
    {
        LOGI("call_81()");
    }

    int ret = simple_read( 0x81 );

    if ( ret < 0 )
    {
        ret = parse_syscall_return_value( ret, GENERAL_TYPE_FLAG );
        if ( m_verbose )
        {
            log_out_error_message( ret, 0x81 );
        }
        return ret;
    }
    return 0;

}

int
SiSTouch_Zeus_I2C::call_82()
{
    int ret = 0;

    if( m_verbose)
    {
        LOGI("call_82()");
    }

    for( int i = 0; i < m_retry; i++)
    {
        ret = simple_read( 0x82 );

        if ( ret < 0 )
        {
            ret = parse_syscall_return_value( ret, GENERAL_TYPE_FLAG );
            if( is_noneed_retry_error( ret ) )
            {
                break;
            }
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
            log_out_error_message( ret, 0x82 );
        }
    }
    return ret;
}

int
SiSTouch_Zeus_I2C::call_82_without_retry()
{

    if( m_verbose)
    {
        LOGI("call_82()");
    }

    int ret = simple_read( 0x82 );

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
SiSTouch_Zeus_I2C::call_83( int base, int pack_num )
{

    int idx = 0;
    int ret = 0;

    if( m_verbose)
    {
        LOGI("call_83()");
        LOGI(" base=%08x,pack_num=%d", base, pack_num);
    }

    for( int i = 0; i < m_retry; i++)
    {
        idx = make_83_buffer( base, pack_num );

        ret = io_command( m_buffer, idx );

        if ( ret < 0 )
        {
            ret = parse_syscall_return_value( ret, GENERAL_TYPE_FLAG );
            if( is_noneed_retry_error( ret ) )
            {
                break;
            }
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
SiSTouch_Zeus_I2C::call_84( const int* data, int size )
{

    int idx = 0;
    int ret = 0;

    if( m_verbose)
    {
        LOGI("call_84()");
        LOGI(" data=%08x,%08x,%08x,s=%d", data[0], data[1], data[2], size);
    }


    for( int i = 0; i < m_retry; i++)
    {
        idx = make_84_buffer( data, size );

        ret = io_command( m_buffer, idx );

        if ( ret < 0 )
        {
            ret = parse_syscall_return_value( ret, GENERAL_TYPE_FLAG );

            if( is_noneed_retry_error( ret ) )
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
SiSTouch_Zeus_I2C::call_85( int data )
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
        ret = io_command( m_buffer, idx );

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
SiSTouch_Zeus_I2C::call_86( int address, int *data, int size )
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
        ret = io_command( m_buffer, idx );

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
SiSTouch_Zeus_I2C::call_87()
{

    int ret = 0;

    if( m_verbose)
    {
        LOGI("call_87()");
    }

    for( int i = 0; i < m_retry; i++)
    {
        ret = simple_read( 0x87 );

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
SiSTouch_Zeus_I2C::call_88( int *address, int *data, int *mask, int number )
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
        ret = io_command( m_buffer, idx );

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
SiSTouch_Zeus_I2C::call_89( int *status )
{

    int ret = 0;

    if( m_verbose)
    {
        LOGI("call_89()");
    }

    for( int i = 0; i < m_retry; i++)
    {
        ret = simple_read( 0x89 );

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
SiSTouch_Zeus_I2C::call_90( int *powermode )
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
        ret = io_command( m_buffer, idx );

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
SiSTouch_Zeus_I2C::call_command( int cmd, int size, unsigned char *data, int addcrc )
{
    int ret = 0;
    int idx = 0;

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
        ret = io_command( m_buffer, idx );

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
        usleep( m_delay );
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
SiSTouch_Zeus_I2C::call_simple_command( int cmd, unsigned char *data )
{
    int ret = 0;

    if( m_verbose)
    {
        LOGI("call_%02x()", cmd);
    }

    for( int i = 0; i < m_retry; i++)
    {
        ret = simple_read( cmd );

        if ( ret < 0 )
        {
            ret = parse_syscall_return_value( ret, GENERAL_TYPE_FLAG );
            break;
        }
        else if( ret <= static_cast<int>(get_MAX_SIZE()))
        {
            for( int j = 0; j < ret; j++ )
                data[j] = m_buffer[j];
            return ret;
        }
        else
        {
            ret = ERROR_WRONG_FORMAT;
            if ( m_verbose )
            {
                log_out_error_message( ret, cmd );
            }
            return ret;
        }
        usleep( m_delay );
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
SiSTouch_Zeus_I2C::io_stop_driver()
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
    return syscall( __NR_sis_I2C_stop );
#endif
}

int
SiSTouch_Zeus_I2C::io_start_driver()
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
    return syscall( __NR_sis_I2C_start );
#endif

}

int
SiSTouch_Zeus_I2C::io_command(unsigned char * buf, int size)
{
    unsigned char cmd = buf[0];
    int ret = 0;
#ifdef VIA_IOCTL
    ret = write(m_fd, buf, size);
#else
    ret = syscall( __NR_sis_I2C_IO, buf, size );
#endif

    if ( m_verbose ) dbg_print_buffer_hex( cmd, buf, ret );

    if(m_IODelayms != 0)
    {
        usleep(m_IODelayms * 1000);
    }

    return ret;
}

int
SiSTouch_Zeus_I2C::simple_read( int cmd )
{
    m_buffer[0] = cmd;
    int ret = io_command( m_buffer, 1 );

    return ret;
}

int
SiSTouch_Zeus_I2C::is_noneed_retry_error( int ret )
{
    switch(ret)
    {
    case SiSTouchIO::ERROR_ACCESS_USER_MEMORY:
        return 1;
    case SiSTouchIO::ERROR_ALLOCATE_KERNEL_MEMORY:
        return 1;
    case SiSTouchIO::ERROR_COPY_FROM_USER:
        return 1;
    case SiSTouchIO::ERROR_CLIENT:
        return 1;
    case SiSTouchIO::ERROR_COPY_FROM_KERNEL:
        return 1;
    case SiSTouchIO::ERROR_TRANSMIT_I2C:
        return 0;
    default:
        return 1;
    }
}

void
SiSTouch_Zeus_I2C::log_out_error_message( int ret ,int cmd)
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
        LOGE( "CMD %02x error: %d", cmd, ret );
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
        case SiSTouchIO::ERROR_EACK:
            LOGE( "CMD %02x get error ack format", cmd );
            break;
        case SiSTouchIO::ERROR_TRANSMIT_I2C:
            LOGE( "CMD %02x data IO error", cmd );
            break;
            /*
            case SiSTouchIO::ERROR_TRANSMIT_READ:
               LOGE( "CMD %02x read data error", cmd );
               break;
            case SiSTouchIO::ERROR_TRANSMIT_WRITE:
               LOGE( "CMD %02x write data error", cmd );
               break;
            */
        case SiSTouchIO::ERROR_ACCESS_USER_MEMORY:
            LOGE( "Fail to access data from user memory" );
            break;
        case SiSTouchIO::ERROR_ALLOCATE_KERNEL_MEMORY:
            LOGE( "Fail to allocate kernel memory" );
            break;
        case SiSTouchIO::ERROR_COPY_FROM_USER:
            LOGE( "Fail to copy data from user memory to kernel memory" );
            break;
        case SiSTouchIO::ERROR_CLIENT:
            LOGE( "Devices are not bound to SiS driver" );
            break;
        case SiSTouchIO::ERROR_COPY_FROM_KERNEL:
            LOGE( "Fail to copy data from kernel memory to user memory" );
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
SiSTouch_Zeus_I2C::get_device_name()
{
	return DEVICE_NAME;
}

int
SiSTouch_Zeus_I2C::device_Exist()
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
SiSTouch_Zeus_I2C::test_write_data(unsigned char * buf, int size)
{
	return ERROR_NOT_SUPPORT;
}

int
SiSTouch_Zeus_I2C::test_read_data( unsigned char * buf, int size)
{
	return simple_read(0x0);
}

//20120903

//20120925
int
SiSTouch_Zeus_I2C::call_simple_command_with_wrong_crc( int cmd, unsigned char *data )
{
	return ERROR_NOT_SUPPORT;
}
//
