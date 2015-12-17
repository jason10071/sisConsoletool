/*
 * SiSTouchIO.h
 *
 *  Created on: Mar 25, 2010
 *      Author: suzin
 */

#ifndef SISTOUCHIO_H_
#define SISTOUCHIO_H_

#include <unistd.h>
#include <sys/syscall.h>
#include <stdio.h>


//FOR_ANDROID_X86 will assume system call can return negative value directly
//#define FOR_ANDROID_X86

#define VIA_IOCTL

#define ZEUS_I2C_MAX_SIZE 16
#define ZEUS_USB_INTERNAL_MAX_SIZE 19
#define AEGIS_I2C_MAX_SIZE 64
#define DEVNAME_MAX_LEN		128

const int GENERAL_TYPE_FLAG = 0x0;
const int ONLY_RETURN_ZERO_FLAG = 0x1;
const int NOT_SIS_IO_FLAG = 0x2;

const int INTERFACE_FLAG_ADDR = 0xa000c024;

class SiSTouchIO
{
public:

    enum
    {
        BUFFER_SIZE = 64,
        DEFAULT_DELAY = 1000,
        DEFAULT_RETRY = 10000,
    };

    enum ERR_NO
    {
        ERROR = -1,
        ERROR_NACK = -2, // receive 0x0040
        ERROR_DEAD = -3, // 04 command receive 0x0080dead
        ERROR_SYSCALL = -4, // syscall itself return error
        ERROR_TIMEOUT = -5, // receive nack exceed max_retry
        ERROR_QUERY = -6, // cannnot query driver
        ERROR_EACK = -7, //error ack format
        //ERROR_USBNACK = -8, //receive 0xDCBA (NACK)
        ERROR_USBEACK = -9, //receive others format than 0xABCD and 0xDCB
        ERROR_CRC_MISMATCH = -10,   // crc check fail
        ERROR_TRANSMIT_I2C = -21,// transmit error in i2c
        //ERROR_TRANSMIT_READ = -16, // transmit read error in i2c
        //ERROR_TRANSMIT_WRITE = -15, // transmit write error in i2c
        ERROR_ACCESS_USER_MEMORY = -11, // access user memory fail
        ERROR_ALLOCATE_KERNEL_MEMORY = -12, // fail to allocate kernel memory
        ERROR_COPY_FROM_USER = -14, // copy data from user memory to kernel memory fail
        ERROR_CLIENT = -13, //client not created
        ERROR_COPY_FROM_KERNEL = -19, //copy data from kernel memory to user memory fail
        ERROR_WRONG_FORMAT = -22, //not recognized format
        ERROR_BROKEN_PIPE = -32, //broken pipe of usb_io
#ifdef VIA_IOCTL
        ERROR_ACCESS = -33, //access device descriptor fail
        ERROR_ENTRY_NOT_EXIST = -34, //entry not exist
        ERROR_DEVICE_NOT_EXIST = -35, //device not exist
#endif
        ERROR_NOT_SUPPORT = -36, //cmd not support
        ERROR_USB_DEV_NULL = -777, //usb device not exist
        ERROR_TRANSMIT_TIMOUT = -110, //error due to io timeout
    };

    enum
    {
    	CON_NONE = -2,
        CON_AUTO = -1,
        CON_ZEUS_I2C = 0,
        CON_ZEUS_USB = 1,
        CON_ZEUS_INTERUSB = 2,
        CON_AEGIS_I2C = 3,
        CON_AEGIS_USB = 4,
        CON_AEGIS_INTERUSB = 5,
        CON_AEGIS_SLAVE = 6,
        CON_AEGIS_MULTI = 7,
        CON_AEGIS_MULTI_FOR_NEW_DRIVER = 8,
        CON_AEGIS_I2C_FOR_NEW_DRIVER = 9,
        CON_CUSTOM = 99,
    };

    SiSTouchIO( int max_retry, int delay, int verbose, int byte_count, int ioDelay, int changeDiagMode, int changePWRMode )
        : m_retry( max_retry)
        , m_delay( delay )
        , m_verbose( verbose )
        , m_add_bytecount( byte_count )
        , m_IODelayms(ioDelay)
        , m_ChangeDiagMode(changeDiagMode)
        , m_ChangePWRMode(changePWRMode)
        , m_chipNum(1) // [20150304]
#ifdef VIA_IOCTL
        , m_fd(-1)
#endif
        ,m_crc(0)
    {
    }

    SiSTouchIO( int max_retry, int delay, int verbose, int byte_count, int ioDelay )
        : m_retry( max_retry)
        , m_delay( delay )
        , m_verbose( verbose )
        , m_add_bytecount( byte_count )
        , m_IODelayms(ioDelay)
        , m_ChangeDiagMode(1)
        , m_ChangePWRMode(1)
        , m_chipNum(1) // [20150304]
#ifdef VIA_IOCTL
        , m_fd(-1)
#endif
        ,m_crc(0)
    {
    }

    virtual ~SiSTouchIO();

    // return value: 0 for success, negative for error

    virtual int stop_driver() = 0;
    virtual int start_driver() = 0;

    virtual int call_00( unsigned char *data, int size ) = 0;
    virtual int call_04() = 0;
    virtual int call_81() = 0;
    virtual int call_82() = 0;
    virtual int call_82_without_retry() = 0;
    virtual int call_83( int base, int pack_num ) = 0;
    virtual int call_84( const int *data, int size ) = 0;
    virtual int call_85( int mode ) = 0;
    virtual int call_86( int address, int *data, int size ) = 0;
    virtual int call_87() = 0;
    virtual int call_88( int *address, int *data, int *mask, int number ) = 0;
    virtual int call_89( int *status ) = 0;
    virtual int call_90( int* powermode ) = 0;

    virtual int call_command( int cmd, int size, unsigned char *data, int addcrc ) = 0;
    virtual int call_simple_command( int cmd, unsigned char *data ) = 0;

    virtual int is_noneed_retry_error( int ret ) = 0;
    virtual void log_out_error_message( int ret, int cmd ) = 0;
	//20120903
	virtual int test_write_data(unsigned char * buf, int size) = 0;
	virtual int test_read_data(unsigned char * buf, int size) = 0;


	//20120925
	virtual int call_simple_command_with_wrong_crc( int cmd, unsigned char *data ) = 0;

    void set_retry( int max_retry, int delay );

    int setVerbose(int verbose);
    int getVerbose();
    int setIODelay(int delayms);
    int getIODelay();

    //20130410 for custom device name/type setting
    static void setCustomDevice(bool isCustom, int devType, char devName[], int devNameLen);
    static bool isCustomDevice();
    static char *getCustomDeviceName();

    static bool m_isCustomDevice;
    static int m_customDeviceType;
    static char m_customDeviceName[DEVNAME_MAX_LEN];

    //[begin] 20130627 support multi-device
    static int device_Exist();
    static int setActiveDeviceName(char* devName, int devNameLen);
    static char *getActiveDeviceName();
	static int getDeviceType(int max_retry, int retry_delay, int verbose, int ioDelay, int changeDiagMode, int changePWRMode);
    static int getDeviceType(int max_retry, int retry_delay, int verbose, int ioDelay, int changeDiagMode, int changePWRMode, int detectHidrawFlag);

    static char m_activeDeviceName[DEVNAME_MAX_LEN];
    //[end] 20130627 support multi-device

    virtual int getChipNum(); // [20150304]
    virtual void setChipNum(int chipNum); // [20150304]

protected:

    int m_retry;
    int m_delay;
    int m_verbose;
    int m_add_bytecount;
    int m_IODelayms;
    int m_ChangeDiagMode;
    int m_ChangePWRMode;

    int m_chipNum; // [20150304]


#ifdef VIA_IOCTL
    int m_fd;
#endif

    virtual int make_83_buffer( int base, int pack_num );
    virtual int make_84_buffer( const int *data, int size );
    virtual int make_85_buffer( unsigned char data );
    virtual int make_86_buffer( int addr );
    virtual int make_88_buffer( int *addr, int *data, int *mask, int number );
    virtual int make_90_buffer( unsigned char powermode );
    virtual int make_command_buffer( int cmd, int size, unsigned char *data, int addcrc );

    virtual void dbg_print_buffer_hex( const unsigned char* data, int size );
    virtual void dbg_print_buffer_hex( int cmd, const unsigned char* data, int size );

    virtual int parse_syscall_return_value( int ret, int type = ONLY_RETURN_ZERO_FLAG );

    unsigned char m_buffer[BUFFER_SIZE];


    unsigned short m_crc; ///< save crc current value
    virtual void AppendCRC( char data );
    //20120925
    virtual void AppendWrongCRC( char data );

    virtual inline unsigned int get_MAX_SIZE()
    {
        return ZEUS_I2C_MAX_SIZE;
    }
};

class SiSTouch_Zeus_USB : public SiSTouchIO
{
public:

    enum
    {
        DEFAULT_TIMEOUT = 1000,
    };

    enum ACK_TYPE
    {
        ACK = 0,
        NACK = -8,
        ERROR_ACK = -9,
    };

    SiSTouch_Zeus_USB( int max_retry, int delay, int verbose, int ioDelay)
        : SiSTouchIO( max_retry, delay, verbose, 1, ioDelay), m_driver_exist(0)
    {
    }

    // return value: 0 for success, negative for error

    virtual int stop_driver();
    virtual int start_driver();

    virtual int call_00( unsigned char *data, int size );
    virtual int call_04();
    virtual int call_81();
    virtual int call_82();
    virtual int call_82_without_retry();
    virtual int call_83( int base, int pack_num );
    virtual int call_84( const int *data, int size );
    virtual int call_85( int data );
    virtual int call_86( int address, int *data, int size );
    virtual int call_87();
    virtual int call_88( int *address, int *data, int *mask, int number );
    virtual int call_89( int *status );
    virtual int call_90( int *powermode );
    virtual int call_command( int cmd, int size, unsigned char *data, int addcrc );
    virtual int call_simple_command( int cmd, unsigned char *data );
    virtual int is_noneed_retry_error( int ret );
    virtual void log_out_error_message( int ret, int cmd );
	//20120903
	virtual int test_write_data(unsigned char * buf, int size);
	virtual int test_read_data(unsigned char * buf, int size);
	//
	//20120925
	virtual int call_simple_command_with_wrong_crc( int cmd, unsigned char *data );

    static int device_Exist();
    static const char* get_device_name();


protected:
    virtual int sis_usb_start();
    virtual int sis_usb_stop();
    virtual int sis_usb_io( unsigned char request, unsigned char dir, unsigned int value, unsigned int index, void* data, unsigned int size, int timeout);
    virtual int sis_usb_query();

private:

    int m_driver_exist;

    int command_d8_read( int cmd );
    int limit_retry( int ( SiSTouch_Zeus_USB::*func)() );

    int check_ack( const unsigned char* data );
    int usb_simple_command( int request );

    int usb_command_D7();
    int usb_command_D8_P810_reset();
    int usb_command_D8_P810_04_query();
    void usb_command_D8_get_data( unsigned char* data, int* length );
    int usb_command_DA();
    int usb_command_DB();
    int usb_command_DC();
    int usb_command_D9( unsigned char* buffer, int size );
};

class SiSTouch_Zeus_I2C : public SiSTouchIO
{
public:

    SiSTouch_Zeus_I2C( int max_retry, int delay, int verbose, int ioDelay)
        : SiSTouchIO( max_retry, delay, verbose, 1, ioDelay)
    {
    }

    // return value: 0 for success, negative for error

    virtual int stop_driver();
    virtual int start_driver();

    virtual int call_00( unsigned char *data, int size );
    virtual int call_04();
    virtual int call_81();
    virtual int call_82();
    virtual int call_82_without_retry();
    virtual int call_83( int base, int pack_num );
    virtual int call_84( const int *data, int size );
    virtual int call_85( int data );
    virtual int call_86( int address, int *data, int size );
    virtual int call_87();
    virtual int call_88( int *address, int *data, int *mask, int number );
    virtual int call_89( int *status );
    virtual int call_90( int *powermode );
    virtual int call_command( int cmd, int size, unsigned char *data, int addcrc );
    virtual int call_simple_command( int cmd, unsigned char *data );
    virtual int is_noneed_retry_error( int ret );
    virtual void log_out_error_message( int ret , int cmd );
	//20120903
	virtual int test_write_data(unsigned char * buf, int size);
	virtual int test_read_data(unsigned char * buf, int size);
	//20120925
	virtual int call_simple_command_with_wrong_crc( int cmd, unsigned char *data );
    static int device_Exist();
    static const char* get_device_name();



private:
    int simple_read( int cmd );
    int io_stop_driver();
    int io_start_driver();
    int io_command(unsigned char * buf, int size);

};

class SiSTouch_Zeus_InterUSB : public SiSTouchIO
{
public:

    enum
    {
        DEFAULT_TIMEOUT = 1000,
    };

    enum ACK_TYPE
    {
        ACK = 0,
        NACK = -8,
        ERROR_ACK = -9,
    };

    SiSTouch_Zeus_InterUSB( int max_retry, int delay, int verbose, int ioDelay)
        : SiSTouchIO( max_retry, delay, verbose, 1, ioDelay), m_driver_exist(0)
    {
    }

    // return value: 0 for success, negative for error

    virtual int stop_driver();
    virtual int start_driver();

    virtual int call_00( unsigned char *data, int size );
    virtual int call_04();
    virtual int call_81();
    virtual int call_82();
    virtual int call_82_without_retry();
    virtual int call_83( int base, int pack_num );
    virtual int call_84( const int *data, int size );
    virtual int call_85( int data );
    virtual int call_86( int address, int *data, int size );
    virtual int call_87();
    virtual int call_88( int *address, int *data, int *mask, int number );
    virtual int call_89( int *status );
    virtual int call_90( int *powermode );
    virtual int call_command( int cmd, int size, unsigned char *data, int addcrc );
    virtual int call_simple_command( int cmd, unsigned char *data );
    virtual int is_noneed_retry_error( int ret );
    virtual void log_out_error_message( int ret, int cmd );
	//20120903
	virtual int test_write_data(unsigned char * buf, int size);
	virtual int test_read_data(unsigned char * buf, int size);
	//
	//20120925
	virtual int call_simple_command_with_wrong_crc( int cmd, unsigned char *data );

    static int device_Exist();
    static const char* get_device_name();



protected:
    virtual int make_stop_buffer();
    virtual int make_start_buffer();
    virtual int make_00_buffer();
    virtual int make_04_start_buffer();
    virtual int make_04_query_buffer();
    virtual int make_82_buffer();
    virtual int make_83_buffer( int base, int pack_num );
    virtual int make_84_buffer( const int *data, int size );
    virtual int make_85_buffer( unsigned char data );
    virtual int make_86_buffer( int addr );
    virtual int make_87_buffer();
    virtual int make_88_buffer( int* addr, int *data, int* mask, int number );
    virtual int make_command_buffer( int cmd, int size, unsigned char *data, int addcrc );
    virtual int sis_usb_start();
    virtual int sis_usb_stop();
    virtual int sis_usb_io( unsigned char request, unsigned char dir, unsigned int value, unsigned int index, void* data, unsigned int size, int timeout);
    virtual int sis_usb_query();

private:


    int m_driver_exist;

    int limit_retry( int (SiSTouch_Zeus_InterUSB::*func)() );

    virtual inline unsigned int get_MAX_SIZE()
    {
        return ZEUS_USB_INTERNAL_MAX_SIZE;
    }
};

class SiSTouch_Aegis_I2C : public SiSTouchIO
{
public:

    SiSTouch_Aegis_I2C( int max_retry, int delay, int verbose, int ioDelay, int changeDiagMode, int changePWRMode)
        : SiSTouchIO( max_retry, delay, verbose, 1, ioDelay, changeDiagMode, changePWRMode)
    {
    }


    // return value: 0 for success, negative for error

    virtual int stop_driver();
    virtual int start_driver();

    virtual int call_00( unsigned char *data, int size );
    virtual int call_04();
    virtual int call_81();
    virtual int call_82();
    virtual int call_82_without_retry();
    virtual int call_83( int base, int pack_num );
    virtual int call_84( const int *data, int size );
    virtual int call_85( int data );
    virtual int call_86( int address, int *data, int size );
    virtual int call_87();
    virtual int call_88( int *address, int *data, int *mask, int number );
    virtual int call_89( int *status );
    virtual int call_90( int *powermode );
    virtual int call_command( int cmd, int size, unsigned char *data, int addcrc );
    virtual int call_simple_command( int cmd, unsigned char *data );
    virtual int is_noneed_retry_error( int ret );
    virtual void log_out_error_message( int ret , int cmd );
	//20120903
	virtual int test_write_data(unsigned char * buf, int size);
	virtual int test_read_data(unsigned char * buf, int size);
	//
	//20120925
	virtual int call_simple_command_with_wrong_crc( int cmd, unsigned char *data );

    static int device_Exist();
    static const char* get_device_name();


protected:
    virtual int make_simple_buffer( int cmd );
    virtual int make_83_buffer( int base, int pack_num );
    virtual int make_84_buffer( const int *data, int size );
    virtual int make_85_buffer( int data );
    virtual int make_86_buffer( int addr );
    virtual int make_88_buffer( int *addr, int *data, int *mask, int number );
    virtual int make_90_buffer( int powermode );
    virtual int make_command_buffer( int cmd, int size, unsigned char *data, int addcrc );

private:
	
	
    void make_common_header( int cmd, int length );
    void generate_output_crc();
    int read_simple_ack(int size);
    //20120925
    int read_simple_ack_with_wrong_crc(int size);

    virtual inline unsigned int get_MAX_SIZE()
    {
        return AEGIS_I2C_MAX_SIZE;
    }
    //20120926
    void generate_wrong_output_crc();
    //20120926

protected:
	int io_stop_driver();
	int io_start_driver();
	
	virtual int simple_io( int cmd, int sleepms = 0 );
    virtual int io_command(unsigned char * buf, int size, int sleepms = 0);

	int write_data(unsigned char * buf, int size);
    int read_data(unsigned char * buf, int size);

};

/* for /dev/i2c-* device */
class SiSTouch_Aegis_I2C_FOR_NEW_DRIVER : public SiSTouch_Aegis_I2C
{
public:

    SiSTouch_Aegis_I2C_FOR_NEW_DRIVER( int max_retry, int delay, int verbose, int ioDelay, int changeDiagMode, int changePWRMode)
        : SiSTouch_Aegis_I2C( max_retry, delay, verbose, ioDelay, changeDiagMode, changePWRMode)
    {
    }

	virtual int stop_driver();

	static const char* get_device_name();

private:
	virtual int io_command(unsigned char * buf, int size, int sleepms = 0);
	virtual int simple_io( int cmd, int sleepms = 0 );

};


class SiSTouch_Aegis_USB : public SiSTouchIO
{
public:

    enum
    {
        DEFAULT_TIMEOUT = 1000,
    };

    SiSTouch_Aegis_USB( int max_retry, int delay, int verbose, int ioDelay, int changeDiagMode, int changePWRMode)
        : SiSTouchIO( max_retry, delay, verbose, 1, ioDelay, changeDiagMode, changePWRMode)
    {
    }


    // return value: 0 for success, negative for error

    virtual int stop_driver();
    virtual int start_driver();

    virtual int call_00( unsigned char *data, int size );
    virtual int call_04();
    virtual int call_81();
    virtual int call_82();
    virtual int call_82_without_retry();
    virtual int call_83( int base, int pack_num );
    virtual int call_84( const int *data, int size );
    virtual int call_85( int data );
    virtual int call_86( int address, int *data, int size );
    virtual int call_87();
    virtual int call_88( int *address, int *data, int *mask, int number );
    virtual int call_89( int *status );
    virtual int call_90( int *powermode );
    virtual int call_command( int cmd, int size, unsigned char *data, int addcrc );
    virtual int call_simple_command( int cmd, unsigned char *data );
    virtual int is_noneed_retry_error( int ret );
    virtual void log_out_error_message( int ret , int cmd );
	//20120903
	virtual int test_write_data(unsigned char * buf, int size);
	virtual int test_read_data(unsigned char * buf, int size);
	//
	//20120925
	virtual int call_simple_command_with_wrong_crc( int cmd, unsigned char *data );

    static int device_Exist();
    static const char* get_device_name();


protected:

    virtual int make_simple_buffer( int cmd );
    virtual int make_83_buffer( int base, int pack_num );
    virtual int make_84_buffer( const int *data, int size );
    virtual int make_85_buffer( int data );
    virtual int make_86_buffer( int addr );
    virtual int make_88_buffer( int *addr, int *data, int *mask, int number );
    virtual int make_90_buffer( int powermode );
    virtual int make_command_buffer( int cmd, int size, unsigned char *data, int addcrc );

private:

    virtual inline unsigned int get_MAX_SIZE()
    {
        return AEGIS_I2C_MAX_SIZE;
    }

    virtual int sis_usb_start();
    virtual int sis_usb_stop();

    int simple_io( int cmd, int sleepms = 0 );
    int io_command( unsigned char * buf, int size, int sleepms = 0 );
    int sis_usb_write( void* data, unsigned int size, int timeout );
    int sis_usb_read( void* data, unsigned int size, int timeout );

    void make_common_header( int cmd, int length );
    void generate_output_crc();
    int read_simple_ack(int size);
    int read_simple_ack_with_wrong_crc(int size);
    //20120926
    void generate_wrong_output_crc();
    //20120926

};

class SiSTouch_Aegis_Multi : public SiSTouchIO
{
public:


    enum
    {
        DEFAULT_TIMEOUT = 1000,
    };

    SiSTouch_Aegis_Multi ( int max_retry, int delay, int verbose, int ioDelay, int changeDiagMode, int changePWRMode)
        : SiSTouchIO( max_retry, delay, verbose, 1, ioDelay, changeDiagMode, changePWRMode), m_SlaveAddr(0)
    {
    }

    SiSTouch_Aegis_Multi ( int max_retry, int delay, int verbose, int ioDelay, int changeDiagMode, int changePWRMode, int slaveAddr)
        : SiSTouchIO( max_retry, delay, verbose, 1, ioDelay, changeDiagMode, changePWRMode), m_SlaveAddr(slaveAddr)
    {
    }


    // return value: 0 for success, negative for error

    virtual int stop_driver();
    virtual int start_driver();

    virtual int call_00( unsigned char *data, int size );
    virtual int call_04();
    virtual int call_81();
    virtual int call_82();
    virtual int call_82_without_retry();
    virtual int call_83( int base, int pack_num );
    virtual int call_84( const int *data, int size );
    virtual int call_85( int data );
    virtual int call_86( int address, int *data, int size );
    virtual int call_87();
    virtual int call_88( int *address, int *data, int *mask, int number );
    virtual int call_88_master( int *address, int *data, int *mask, int number );
    virtual int call_89( int *status );
    virtual int call_90( int *powermode );
    virtual int call_command( int cmd, int size, unsigned char *data, int addcrc );
    virtual int call_simple_command( int cmd, unsigned char *data );
    virtual int is_noneed_retry_error( int ret );
    virtual void log_out_error_message( int ret , int cmd );
    void setSlaveAddr( int slaveAddr );
    int getSlaveAddr();
	//20120903
	virtual int test_write_data(unsigned char * buf, int size);
	virtual int test_read_data(unsigned char * buf, int size);
	//
	//20120925
	virtual int call_simple_command_with_wrong_crc( int cmd, unsigned char *data );

    static int device_Exist();
    static const char* get_device_name();

protected:

    enum
    {
        MASTER_ADDR = 0,
        SLAVE0_ADDR = 5,
        SLAVE1_ADDR = 6,
        SLAVE2_ADDR = 7,
    };

    virtual int make_simple_buffer( int cmd );
    virtual int make_simple_buffer_master( int cmd );
    virtual int make_simple_buffer_slave( int cmd );
    virtual int get_data_start_idx();
    virtual int get_data_start_idx_master();
    virtual int get_data_start_idx_slave();
    virtual int make_83_buffer( int base, int pack_num );
    virtual int make_83_buffer_master( int base, int pack_num );
    virtual int make_83_buffer_slave( int base, int pack_num );
    virtual int make_84_buffer( const int *data, int size );
    virtual int make_84_buffer_master( const int *data, int size );
    virtual int make_84_buffer_slave( const int *data, int size );
    virtual int make_85_buffer( int data );
    virtual int make_86_buffer( int addr );
    virtual int make_86_buffer_master( int addr );
    virtual int make_86_buffer_slave( int addr );
    virtual int make_88_buffer( int *addr, int *data, int *mask, int number );
    virtual int make_88_buffer_master( int *addr, int *data, int *mask, int number );
    virtual int make_88_buffer_slave( int *addr, int *data, int *mask, int number );
    virtual int make_90_buffer( int powermode );
    virtual int make_90_buffer_master( int powermode );
    virtual int make_90_buffer_slave( int powermode );
    virtual int make_command_buffer( int cmd, int size, unsigned char *data, int addcrc );
    virtual int make_command_buffer_master( int cmd, int size, unsigned char *data, int addcrc );
    virtual int make_command_buffer_slave( int cmd, int size, unsigned char *data, int addcrc );

private:

    int m_SlaveAddr;

    virtual inline unsigned int get_MAX_SIZE()
    {
        return AEGIS_I2C_MAX_SIZE;
    }

    
    virtual int sis_usb_stop();

    int simple_io( int cmd, int sleepms = 0, int wTimeout = DEFAULT_TIMEOUT, int rTimeout = DEFAULT_TIMEOUT );
    
    int simple_io_slave( int cmd, int sleepms = 0, int wTimeout = DEFAULT_TIMEOUT, int rTimeout = DEFAULT_TIMEOUT );
    int io_command( unsigned char * buf, int size, int sleepms = 0, int wTimeout = DEFAULT_TIMEOUT, int rTimeout = DEFAULT_TIMEOUT );

    void make_common_header( int cmd, int length );
    void generate_output_crc();
    int read_simple_ack(int size);

    void make_common_header_master( int cmd, int length );
    void generate_output_crc_master();
    

    void make_common_header_slave( int cmd, int length );
    void generate_output_crc_slave();
    int read_simple_ack_slave(int size);
    int read_simple_ack_with_wrong_crc(int size);
    //20120926
    void generate_wrong_output_crc();
    void generate_wrong_output_crc_master();
    void generate_wrong_output_crc_slave();
    //20120926


protected:

    virtual int sis_usb_start();
	virtual int sis_usb_write( void* data, unsigned int size, int timeout );
    virtual int sis_usb_read( void* data, unsigned int size, int timeout );
	virtual int simple_io_master( int cmd, int sleepms = 0, int wTimeout = DEFAULT_TIMEOUT, int rTimeout = DEFAULT_TIMEOUT );
	int read_simple_ack_master(int size);

};

class SiSTouch_Aegis_Multi_FOR_NEW_DRIVER  : public SiSTouch_Aegis_Multi
{
public:


    SiSTouch_Aegis_Multi_FOR_NEW_DRIVER ( int max_retry, int delay, int verbose, int ioDelay, int changeDiagMode, int changePWRMode)
        : SiSTouch_Aegis_Multi( max_retry, delay, verbose, ioDelay, changeDiagMode, changePWRMode )
    {
    }

    virtual int call_82();
    virtual int call_82_without_retry();
    virtual int call_87();

	static const char* get_device_name();

private:
	virtual int sis_usb_write( void* data, unsigned int size, int timeout );
    virtual int sis_usb_read( void* data, unsigned int size, int timeout );
	virtual int simple_io_master( int cmd, int sleepms = 0, int wTimeout = DEFAULT_TIMEOUT, int rTimeout = DEFAULT_TIMEOUT );

	bool reFindSisTouchName();

};

class SiSTouch_Aegis_InterUSB  : public SiSTouch_Aegis_Multi
{
public:


    SiSTouch_Aegis_InterUSB ( int max_retry, int delay, int verbose, int ioDelay, int changeDiagMode, int changePWRMode)
        : SiSTouch_Aegis_Multi( max_retry, delay, verbose, ioDelay, changeDiagMode, changePWRMode, MASTER_ADDR)
    {
    }

    static int device_Exist();
    static const char* get_device_name();

};

class SiSTouch_Aegis_Slave  : public SiSTouch_Aegis_Multi
{

public:

    SiSTouch_Aegis_Slave ( int max_retry, int delay, int verbose, int ioDelay, int changeDiagMode, int changePWRMode)
        : SiSTouch_Aegis_Multi( max_retry, delay, verbose, ioDelay, changeDiagMode, changePWRMode, SLAVE0_ADDR)
    {
    }

    static int device_Exist();
    static const char* get_device_name();

};

#endif /* SISTOUCHIO_H_ */
