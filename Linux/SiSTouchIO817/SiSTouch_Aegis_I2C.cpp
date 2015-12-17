#include "SiSTouchIO.h"
#include "SiSLogger.h"
#include <errno.h>
#include <string.h>

#define DEVICE_CHANGEMODE_RETRY_TIMES 40
#define DEVICE_CHANGEMODE_RETRY_INTERVAL 100


#ifdef VIA_IOCTL
#include <fcntl.h>
#include <sys/stat.h>
#define DEVICE_NAME "/dev/sis_aegis_touch_device"
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


int
SiSTouch_Aegis_I2C::stop_driver()
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
SiSTouch_Aegis_I2C::start_driver()
{

    int ret = 0;

    if( m_verbose)
    {
        LOGI("start_driver()");
    }


#ifdef PWR_MODE
    if(m_ChangePWRMode)
    {
        for(int i = 0; i < DEVICE_CHANGEMODE_RETRY_TIMES; i++ )
        {
            ret = call_85(PW_CMD_FWCTRL);

            if(ret >= 0)
            {
                break;
            }

            usleep( DEVICE_CHANGEMODE_RETRY_INTERVAL * 1000 );
        }

    //don't care
    //    if(ret < 0)
    //    {
    //        return ret;
    //    }
    }
#endif


#ifdef DIAGNOSIS_MODE    
    if(m_ChangeDiagMode)
    {
        for(int i = 0; i < DEVICE_CHANGEMODE_RETRY_TIMES; i++ )
        {
            ret = call_85(DISABLE_DIAGNOSIS_MODE);

            if(ret >= 0)
            {
                break;
            }

            usleep( DEVICE_CHANGEMODE_RETRY_INTERVAL * 1000 );
        }

    //don't care
    //    if(ret < 0)
    //    {
    //        return ret;
    //    }
    }
#endif


    ret = io_start_driver();

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
SiSTouch_Aegis_I2C::call_00( unsigned char *data, int size )
{
    if( m_verbose)
    {
        LOGI("call_00()");
    }

    int ret = read_data(data, BUFFER_SIZE);

    if ( m_verbose ) dbg_print_buffer_hex( 0x00, data, ret );

    if(m_IODelayms >= 0)
    {
        usleep(m_IODelayms * 1000);
    }

    if ( ret < 0 )
    {
        ret = parse_syscall_return_value( ret, GENERAL_TYPE_FLAG );
    }
    else
    {
        unsigned int read_size = ret - ( 8 - DUMMYBYTE );

        ret = read_simple_ack(ret);

        if ( ret == 0 )
        {
            read_size = ( read_size > get_MAX_SIZE() ) ? get_MAX_SIZE() : read_size;

            ret = 0;
            for(unsigned int i = 0; i < read_size && i < static_cast<unsigned int>(size); i++)
            {
                data[i] = m_buffer[i + ( 8 - DUMMYBYTE )];
                ret++;
            }
        }
    }

    if( ret < 0 )
    {
        if ( m_verbose )
        {
            log_out_error_message( ret, 0x00 );
        }
    }

    return ret;
}

int
SiSTouch_Aegis_I2C::call_04()
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

        ret = simple_io( 0x81, DELAY_FOR_81 );

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
SiSTouch_Aegis_I2C::call_81()
{

    if( m_verbose)
    {
        LOGI("call_81()");
    }

    int ret = ERROR_NOT_SUPPORT;
    if ( m_verbose )
    {
        log_out_error_message( ret, 0x81 );
    }
    return ret;

}

int
SiSTouch_Aegis_I2C::call_82()
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
SiSTouch_Aegis_I2C::call_82_without_retry()
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
SiSTouch_Aegis_I2C::make_83_buffer( int base, int pack_num )
{

    int len = 14;

    make_common_header( 0x83, len );

    m_buffer[8] = base & 0xff;
    m_buffer[9] = ( base >> 8 ) & 0xff;
    m_buffer[10] = ( base >> 16 ) & 0xff;
    m_buffer[11] = ( base >> 24 ) & 0xff;
    m_buffer[12] = pack_num & 0xff;
    m_buffer[13] = ( pack_num >> 8 ) & 0xff;

    generate_output_crc();

    if ( m_verbose ) dbg_print_buffer_hex( m_buffer, len );

    return len;
}

int
SiSTouch_Aegis_I2C::call_83( int base, int pack_num )
{

    int ret = 0;

    if( m_verbose)
    {
        LOGI("call_83()");
        LOGI(" base=%08x,pack_num=%d", base, pack_num);
    }

    for( int i = 0; i < m_retry; i++)
    {

        int len = make_83_buffer( base, pack_num );

        ret = io_command( m_buffer, len, DELAY_FOR_GENEARL );

        if ( ret < 0 )
        {
            ret = parse_syscall_return_value( ret, GENERAL_TYPE_FLAG );
            if( is_noneed_retry_error( ret ) )
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
            log_out_error_message( ret, 0x83 );
        }
    }
    return ret;
}

int
SiSTouch_Aegis_I2C::make_84_buffer( const int *data, int size )
{
    int len = size * 4 + 8;

    make_common_header( 0x84, len );


    for ( int j = 0; j < size; j++ )
    {
        for ( int k = 0; k < 4; k++ )
        {
////////////////////////////// the endian is reverse with zeus///////////////////////
            char temp = ( data[j] >> (8*(3-k)) ) & 0xff;
//                char temp = ( data[j] >> (8*k) ) & 0xff;
            m_buffer[ 8 + 4 * j + k ] = temp;
        }
    }

    generate_output_crc();


    if ( m_verbose ) dbg_print_buffer_hex( m_buffer, len );

    return len;

}


int
SiSTouch_Aegis_I2C::call_84( const int* data, int size )
{

    int ret = 0;

    if( m_verbose)
    {
        LOGI("call_84()");
        LOGI(" data=%08x,%08x,%08x,...,size=%d", data[0], data[1], data[2], size);
    }


    for( int i = 0; i < m_retry; i++)
    {

        int len = make_84_buffer(data, size);

        ret = io_command( m_buffer, len, DELAY_FOR_GENEARL );

        if ( ret < 0 )
        {
            ret = parse_syscall_return_value( ret, GENERAL_TYPE_FLAG );
            if( is_noneed_retry_error( ret ) )
                break;
        }
        else
        {

            ret = read_simple_ack(ret);

            if ( ret != ERROR_NACK )
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
SiSTouch_Aegis_I2C::make_85_buffer( int data )
{

    int len = 10;

    make_common_header( 0x85, len );

    m_buffer[8] = data & 0xff;
    m_buffer[9] = ( data >> 8 ) & 0xff;

    generate_output_crc();

    if ( m_verbose ) dbg_print_buffer_hex( m_buffer, len );

    return len;

}


int
SiSTouch_Aegis_I2C::call_85( int data )
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
        if(data == DISABLE_DIAGNOSIS_MODE || data == ENABLE_DIAGNOSIS_MODE)
        {
            usleep(DELAY_FOR_DIAGNOSIS_MODE_CHANGE * 1000);
        }
    }
#endif


    return ret;

}

int
SiSTouch_Aegis_I2C::make_86_buffer( int addr )
{

    int len = 14;

    make_common_header( 0x86, len );

    m_buffer[8] = addr & 0xff;
    m_buffer[9] = ( addr >> 8 ) & 0xff;
    m_buffer[10] = ( addr >> 16 ) & 0xff;
    m_buffer[11] = ( addr >> 24 ) & 0xff;
    m_buffer[12] = DATA_UNIT_9257 & 0xff;
    m_buffer[13] = ( DATA_UNIT_9257 >> 8 ) & 0xff;

    generate_output_crc();

    if ( m_verbose ) dbg_print_buffer_hex( m_buffer, len );

    return len;

}

int
SiSTouch_Aegis_I2C::call_86( int address, int *data, int size )
{

    int ret = 0;

    if( m_verbose)
    {
        LOGI("call_86()");
        LOGI(" address=%08x", address);
    }

    for( int i = 0; i < m_retry; i++)
    {
        int len = make_86_buffer( address );

        ret = io_command( m_buffer, len, DELAY_FOR_GENEARL );

        if ( ret < 0 )
        {
            ret = parse_syscall_return_value( ret, GENERAL_TYPE_FLAG );
            break;
        }
        else
        {

            unsigned int read_size = ret - ( 8 - DUMMYBYTE );


            ret = read_simple_ack(ret);

            if ( ret == 0 )
            {
                read_size = ( read_size > DATA_UNIT_9257 ) ? DATA_UNIT_9257 : read_size;

                ret = 0;
                for (unsigned int j = 0; j < read_size / 4 && j < static_cast<unsigned int>(size); j++ )
                {
                    data[j] = 0;
                    for ( int k = 0; k < 4; k++ )
                    {
                        data[j] |= m_buffer[j * 4 + k + ( 8 - DUMMYBYTE )] << 8 * k;
                    }
                    ret++;
                }
                break;
            }

            if ( ret != ERROR_NACK )
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
SiSTouch_Aegis_I2C::call_87()
{

    int ret = 0;

    if( m_verbose)
    {
        LOGI("call_87()");
    }

    for( int i = 0; i < m_retry; i++)
    {

        ret = simple_io( 0x87, DELAY_FOR_87 );


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

int
SiSTouch_Aegis_I2C::make_88_buffer( int* addr, int* data, int* mask, int number )
{
    int len = number * 12 + 8;

    make_common_header( 0x88, len );

    for(int j = 0; j < number && j < 4; j++)
    {
        m_buffer[j*12+8] = addr[j] & 0xff;
        m_buffer[j*12+9] = ( addr[j] >> 8 ) & 0xff;
        m_buffer[j*12+10] = ( addr[j] >> 16 ) & 0xff;
        m_buffer[j*12+11] = ( addr[j] >> 24 ) & 0xff;
        m_buffer[j*12+12] = data[j] & 0xff;
        m_buffer[j*12+13] = ( data[j] >> 8 ) & 0xff;
        m_buffer[j*12+14] = ( data[j] >> 16 ) & 0xff;
        m_buffer[j*12+15] = ( data[j] >> 24 ) & 0xff;
        m_buffer[j*12+16] = mask[j] & 0xff;
        m_buffer[j*12+17] = ( mask[j] >> 8 ) & 0xff;
        m_buffer[j*12+18] = ( mask[j] >> 16 ) & 0xff;
        m_buffer[j*12+19] = ( mask[j] >> 24 ) & 0xff;
    }

    for(int j = number; j < 4; j++)
    {
        m_buffer[j*12+8] = 0;
        m_buffer[j*12+9] = 0;
        m_buffer[j*12+10] = 0;
        m_buffer[j*12+11] = 0;
        m_buffer[j*12+12] = 0;
        m_buffer[j*12+13] = 0;
        m_buffer[j*12+14] = 0;
        m_buffer[j*12+15] = 0;
        m_buffer[j*12+16] = 0;
        m_buffer[j*12+17] = 0;
        m_buffer[j*12+18] = 0;
        m_buffer[j*12+19] = 0;
    }



    generate_output_crc();

    if ( m_verbose ) dbg_print_buffer_hex( m_buffer, len);

    return len;
}

int
SiSTouch_Aegis_I2C::call_88( int* address, int *data, int* mask, int number )
{

    int ret = 0;

    if( m_verbose)
    {
        LOGI("call_88()");
        for(int i = 0; i < number; i++)
        {
            LOGI(" address=%08x, data=%08x, mask=%08x", address[i], data[i], mask[i]);
        }
    }

    for( int i = 0; i < m_retry; i++)
    {

        int len = make_88_buffer(address, data, mask, number);

        ret = io_command( m_buffer, len, DELAY_FOR_GENEARL );

        if ( ret < 0 )
        {
            ret = parse_syscall_return_value( ret, GENERAL_TYPE_FLAG );
            break;
        }
        else
        {


            ret = read_simple_ack(ret);


            if ( ret == 0 )
            {
                ret = 0;
                for ( int j = 0; j < number && j < 4; j++ )
                {
                    data[j] = 0;
                    for ( int k = 0; k < 4; k++ )
                    {
                        data[j] |= m_buffer[j * 4 + k + ( 8 - DUMMYBYTE )] << 8 * k;
                    }
                    ret++;
                }

                break;
            }

            if ( ret != ERROR_NACK )
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
SiSTouch_Aegis_I2C::call_89( int *status )
{
    if( m_verbose)
    {
        LOGI("call_89()");
    }

    int ret = ERROR_NOT_SUPPORT;
    if ( m_verbose )
    {
        log_out_error_message( ret, 0x89 );
    }
    return ret;
}

int
SiSTouch_Aegis_I2C::make_90_buffer( int powermode )
{

    int len = 10;

    make_common_header( 0x90, len );

    m_buffer[8] = powermode & 0xff;
    m_buffer[9] = ( powermode >> 8 ) & 0xff;

    generate_output_crc();

    if ( m_verbose ) dbg_print_buffer_hex( m_buffer, len );

    return len;
}

int
SiSTouch_Aegis_I2C::call_90( int *powermode )
{

    int ret = 0;

    if( m_verbose)
    {
        LOGI("call_90()");
        LOGI(" powermode=%08x", powermode[0]);
    }

    for( int i = 0; i < m_retry; i++)
    {

        int len = make_90_buffer( powermode[0] );

        ret = io_command( m_buffer, len, DELAY_FOR_GENEARL );

        if ( ret < 0 )
        {
            ret = parse_syscall_return_value( ret, GENERAL_TYPE_FLAG );
            break;
        }
        else
        {
            ret = read_simple_ack(ret);

            if ( ret == 0 )
            {
                powermode[0] = ( m_buffer[7 - DUMMYBYTE ] << 8 ) | m_buffer[6 - DUMMYBYTE];
                ret = 1;
                break;
            }

            if ( ret != ERROR_NACK )
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
SiSTouch_Aegis_I2C::make_command_buffer( int cmd, int size, unsigned char *data, int addcrc )
{

    int len = size + 8;

    make_common_header( cmd, len );

    for(int j = 0; j < size; j++)
    {
    	if( 8 + j >= BUFFER_SIZE || j >= AEGIS_I2C_MAX_SIZE)
    	{
    		break;
    	}
        m_buffer[8+j] = data[j];
    }

    if( addcrc )
    {
        generate_output_crc();
    }
    //20120926
    else
    {
    	generate_wrong_output_crc();
    }
    //20120926

    if ( m_verbose ) dbg_print_buffer_hex( m_buffer, len );


    return len;

}


int
SiSTouch_Aegis_I2C::call_command( int cmd, int size, unsigned char *data, int addcrc )
{
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

        int len = make_command_buffer(cmd, size, data, addcrc);


        ret = io_command( m_buffer, len, DELAY_FOR_GENEARL );


        if ( ret < 0 )
        {
            ret = parse_syscall_return_value( ret, GENERAL_TYPE_FLAG );
            break;
        }
        else
        {
            unsigned int read_size = ret - ( 8 - DUMMYBYTE );

            ret = read_simple_ack(ret);

            if ( ret == 0 )
            {
                read_size = ( read_size > DATA_UNIT_9257 ) ? DATA_UNIT_9257 : read_size;

                for (unsigned int j = 0; j < read_size; j++ )
                {
                    data[j] = m_buffer[j + ( 8 - DUMMYBYTE )];
                }

                ret = read_size;

                break;
            }

            if ( ret != ERROR_NACK )
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
SiSTouch_Aegis_I2C::call_simple_command( int cmd, unsigned char *data )
{

    int ret = 0;

    if( m_verbose)
    {
        LOGI("call_%02x()", cmd);
    }

    for( int i = 0; i < m_retry; i++)
    {
        ret = simple_io( cmd, DELAY_FOR_GENEARL );

        if ( ret < 0 )
        {
            ret = parse_syscall_return_value( ret, GENERAL_TYPE_FLAG );
            if( is_noneed_retry_error( ret ) )
                break;
        }
        else
        {

            unsigned int read_size = ret - ( 8 - DUMMYBYTE );

            ret = read_simple_ack(ret);

            if ( ret == 0 )
            {
                read_size = ( read_size > DATA_UNIT_9257 ) ? DATA_UNIT_9257 : read_size;

                for (unsigned int j = 0; j < read_size; j++ )
                {
                    data[j] = m_buffer[j + ( 8 - DUMMYBYTE )];
                }

                ret = read_size;

                break;
            }

            if ( ret != ERROR_NACK )
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
SiSTouch_Aegis_I2C::io_stop_driver()
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
    m_fd = open(devName,O_RDWR);
    return m_fd;
#else
    return ERROR_NOT_SUPPORT;
#endif
}

int
SiSTouch_Aegis_I2C::io_start_driver()
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
    return ERROR_NOT_SUPPORT;
#endif

}


int
SiSTouch_Aegis_I2C::io_command(unsigned char * buf, int size, int sleepms)
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

    if ( m_verbose ) dbg_print_buffer_hex( cmd, buf, ret );

    if(m_IODelayms >= 0)
    {
        usleep(m_IODelayms * 1000);
    }

    return ret;
}


int
SiSTouch_Aegis_I2C::write_data(unsigned char * buf, int size)
{
    int ret = 0;
#ifdef VIA_IOCTL
    ret = write(m_fd, buf, size);
#else
    ret = ERROR_NOT_SUPPORT;
#endif

    return ret;
}

int
SiSTouch_Aegis_I2C::read_data( unsigned char * buf, int size)
{
    int ret = 0;
#ifdef VIA_IOCTL
    ret = read(m_fd, buf, size);

    //real packect size is less then return size - 1
    if(ret > 0)
    {
        ret -= DUMMYBYTE;
    }
#else
    ret = ERROR_NOT_SUPPORT;
#endif

    return ret;
}



void
SiSTouch_Aegis_I2C::make_common_header( int cmd, int length )
{
    int byte_cnt = length - HEADER_LENGTH;

    m_buffer[0] = OUTPUT_REGISTER & 0xff;
    m_buffer[1] = ( OUTPUT_REGISTER >> 8 ) & 0xff;
    m_buffer[2] = byte_cnt & 0xff;
    m_buffer[3] = ( byte_cnt >> 8 ) & 0xff;
    m_buffer[4] = OUTPUT_REPORT_ID;
    m_buffer[5] = 0;
    m_buffer[6] = cmd;
}

void
SiSTouch_Aegis_I2C::generate_output_crc()
{
    int byte_cnt = ( m_buffer[3] << 8 ) | m_buffer[2];

    m_crc = 0;

    AppendCRC( m_buffer[6] ); //command field
    //append payload to CRC, payload begin from byte8
    for (int i = 8; i < ( byte_cnt + HEADER_LENGTH ) && i < static_cast<int>(get_MAX_SIZE()); i++ )
    {
        AppendCRC( m_buffer[i] );
    }

    m_buffer[7] = m_crc & 0xff;
}
//20120926
void
SiSTouch_Aegis_I2C::generate_wrong_output_crc()
{
    int byte_cnt = ( m_buffer[3] << 8 ) | m_buffer[2];

    m_crc = 0;

    AppendWrongCRC( m_buffer[6] );
    for (int i = 8; i < ( byte_cnt + HEADER_LENGTH ) && i < static_cast<int>(get_MAX_SIZE()); i++ )
    {
    	AppendWrongCRC( m_buffer[i] );
    }

    m_buffer[7] = m_crc & 0xff;
}
//20120926

int
SiSTouch_Aegis_I2C::read_simple_ack(int size)
{
    int length = size;

    m_crc = 0;

    for (int i = (4 - DUMMYBYTE); i < length && i < static_cast<int>(get_MAX_SIZE()); i++ )
    {
        AppendCRC( m_buffer[i] );
    }

    if ( m_buffer[3 - DUMMYBYTE] != ( m_crc & 0xff ) )
    {
        return ERROR_CRC_MISMATCH;
    }

    if( m_buffer[4 - DUMMYBYTE] == 0xEF && m_buffer[5 - DUMMYBYTE] == 0xBE )
    {
        return 0;//success
    }
    else if( m_buffer[4 - DUMMYBYTE] == 0xAD && m_buffer[5 - DUMMYBYTE] == 0xDE )
    {
        return ERROR_NACK;
    }

    return ERROR_WRONG_FORMAT;

}

int
SiSTouch_Aegis_I2C::make_simple_buffer( int cmd )
{
    int len = 8;
    make_common_header( cmd, len );
    generate_output_crc();

    if ( m_verbose ) dbg_print_buffer_hex( m_buffer, len );

    return len;
}

//simple_io : transfer no payload command
int
SiSTouch_Aegis_I2C::simple_io( int cmd, int sleepms )
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

    if ( m_verbose ) dbg_print_buffer_hex( cmd, m_buffer, ret );

    if(m_IODelayms >= 0)
    {
        usleep(m_IODelayms * 1000);
    }


    return ret;
}

int
SiSTouch_Aegis_I2C::is_noneed_retry_error( int ret )
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
SiSTouch_Aegis_I2C::log_out_error_message( int ret ,int cmd)
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
SiSTouch_Aegis_I2C::get_device_name()
{
	return DEVICE_NAME;
}

int
SiSTouch_Aegis_I2C::device_Exist()
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
            LOGI("devName = %s\n", devName);
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
SiSTouch_Aegis_I2C::test_write_data(unsigned char * buf, int size)
{
	int ret = write_data(buf, size);
	if(ret < 0)
	{
		ret = parse_syscall_return_value( ret, GENERAL_TYPE_FLAG );
	}

	return ret;
}

int
SiSTouch_Aegis_I2C::test_read_data( unsigned char * buf, int size)
{
	int ret = read_data(buf, size);
	if (ret < 0)
	{
		ret = parse_syscall_return_value(ret, GENERAL_TYPE_FLAG);
	}
	return ret;
}

//

//20120925
int
SiSTouch_Aegis_I2C::call_simple_command_with_wrong_crc( int cmd, unsigned char *data )
{
    int ret = 0;

    if( m_verbose)
    {
        LOGI("call_%02x()", cmd);
    }

    for( int i = 0; i < m_retry; i++)
    {
        ret = simple_io( cmd, DELAY_FOR_GENEARL );

        if ( ret < 0 )
        {
            ret = parse_syscall_return_value( ret, GENERAL_TYPE_FLAG );
            if( is_noneed_retry_error( ret ) )
                break;
        }
        else
        {

            unsigned int read_size = ret - ( 8 - DUMMYBYTE );

            ret = read_simple_ack_with_wrong_crc(ret);

            if ( ret == 0 )
            {
                read_size = ( read_size > DATA_UNIT_9257 ) ? DATA_UNIT_9257 : read_size;

                for (unsigned int j = 0; j < read_size; j++ )
                {
                    data[j] = m_buffer[j + ( 8 - DUMMYBYTE )];
                }

                ret = read_size;

                break;
            }

            if ( ret != ERROR_NACK )
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
SiSTouch_Aegis_I2C::read_simple_ack_with_wrong_crc(int size)
{
    int length = size;

    m_crc = 0;

    for ( int i = (4 - DUMMYBYTE); i < length && i < static_cast<int>(get_MAX_SIZE()); i++ )
    {
    	AppendWrongCRC( m_buffer[i] );
    }
    if(m_verbose)
    {
    	LOGI("crc:%02x\n", m_crc);
    }


    if ( m_buffer[3 - DUMMYBYTE] != ( m_crc & 0xff ) )
    {
        return ERROR_CRC_MISMATCH;
    }

    if( m_buffer[4 - DUMMYBYTE] == 0xEF && m_buffer[5 - DUMMYBYTE] == 0xBE )
    {
        return 0;
    }
    else if( m_buffer[4 - DUMMYBYTE] == 0xAD && m_buffer[5 - DUMMYBYTE] == 0xDE )
    {
        return ERROR_NACK;
    }

    return ERROR_WRONG_FORMAT;

}
//
