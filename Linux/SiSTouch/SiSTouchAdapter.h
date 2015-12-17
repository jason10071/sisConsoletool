#ifndef SISTOUCHADAPTER_H
#define SISTOUCHADAPTER_H
#include "SiSTouchIO.h"
#include "SiSLogger.h"
#include "OpenShortData.h"
#include "OpenShortResult.h"
#include "OpenShortConfig.h"
#include "Parameter.h"

#define SLEEP_RETRY 100
#define SLEEP_RETRY_TIME (50 * 1000)
//1s
#define QUERY_CALIBRATION_SLEEP_TIME (1000 * 1000)

#define MAX_RAW_DATA 255.0f
#define MAX_VOLTAGE 3.3f
#define CLK_DIV 25.0f
#define CLK_SCALE 0.001f

//#define RESERVER_RO_DATA_ONLY_SUCCESS


class SiSTouchAdapter
{
public:

    enum
    {
        SUCCESS = 1,
        RESULT_SAME = 1,
        RESULT_DIFFERENT = 0,
        ERROR_FAIL_GET_INPUT_ID = -256,
        ERROR_FAIL_GET_DEVICE_ID = -257,
        ERROR_FAIL_GET_FILE_ID = -258,
        ERROR_START_DRIVER = -259,
        ERROR_STOP_DRIVER = -260,
        ERROR_IN_READ_DATA = -261,
        ERROR_IN_UPDATEFLOW = -262,
        ERROR_INVALID_FILE = -263,
        ERROR_NULL_FILE = -264,
        ERROR_NULL_INPUT_ARRAY = -265,
        ERROR_CALIBRAION_TIMEOUT = -266,
        ERROR_IN_CALIBRAION = -267,
        ERROR_FAIL_CALIBRATION_FLAG = -268,
        ERROR_NOT_IMPLEMENTED = 269,
        ERROR_NONCOOPERATE_DATA = -270,
        ERROR_NODATA = -271,
        ERROR_FAIL_BOOT_FLAG = -272,
        ERROR_IN_COMPARE_BOOTLOADER = -273,
        ERROR_IN_COMPARE_MAINCODE = -274,
    };

    enum
    {
        DEFALUT_MULTI_NUM = 2,
        DEFALUT_SLAVE_NUM = 0,
        DEFALUT_SINGLE_NUM = 1,
    };

    //GetFwMode command
    enum Phase2StatusIndex {
    	INDEX_PALM = 0,
    	INDEX_WATER,
    	INDEX_MOIST,
    	INDEX_PRESS_DETECT,
    	INDEX_INIT_PALM,
    	INDEX_DYNAMIC_CALI,
    	INDEX_HOPPING,
    	STATUS_INDEX_MAX,
    };

    static const char phase2StatusStr[STATUS_INDEX_MAX][64];
    static bool phase2Status[STATUS_INDEX_MAX];

    virtual ~SiSTouchAdapter()
    {

        if(m_io != 0)
        {
            delete m_io;
        }

        releaseLogger();

    }

	int doUpdateCalibrationData(OpenShortData* data);
    int doUpdate( FILE* file, bool update_bootloader, bool update_bootloader_auto, bool reserve_RODATA, bool update_parameter, bool force_update, bool jump_check);
    int doCompareId(FILE* file);
    int doCompareId(char* input);
    int showDeviceId();
	int doReset(int time, bool noquery);
    int doCalibraion(int time, bool noquery);
    int doCheckCalibrationFlag();
	int doEraseCalibrationFlag();
    int doCheckITO();

    bool checkContinueNG(OpenShortConfig* config, OpenShortResult *result, int x, int y);
	void checkSecondThreshold(OpenShortConfig* config, OpenShortResult *result);
    int doOpenShort(OpenShortData* deviceData, OpenShortConfig* config, OpenShortResult *result, bool bCheckSecondThreshold = false);
    int doOpenShort(OpenShortData* deviceData, OpenShortData* sampleData, OpenShortConfig* config, OpenShortResult *result, bool bCheckSecondThreshold = false);
    int doCheckFWState();
    virtual bool doDetectIsMulti();
    virtual int doDetectSlaveNum();
    int doDumpROM(int *buf);
    int doSoftReset();

    bool getInputId(const char* input, int* input_firmware_id, int id_Length);
	bool getLayoutOffset(int &offset);
    int getDeviceId(int* device_firmware_id, int id_Length);
    int getCalibrationFlag(int *calibration_flag, int flag_Length);
    bool getFileId(FILE* file, int* file_firmware_id, int id_Length);
    int getBootFlag(int *boot_flag, int flag_Length);
    int getFwMode();

    int getDeviceOpenShortData(OpenShortData *data);

    int setIODelay(int delayms);


    static SiSTouchAdapter* prepareAdapter(const Parameter & param);

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
        CON_CUSTOM = 99,
    };

    //2013/6/27 support multi-device
    static int setActiveDeviceName(char* devName, int devNameLen);

protected:

    SiSTouchAdapter( int max_retry, int delay, int verbose, int ioDelay, int changeDiagMode, int changePWRMode )
        :m_io( 0 )
    {
        if(verbose)
        {
            initialLogger();
        }

    }

    SiSTouchAdapter( SiSTouchIO* io, int max_retry, int delay, int verbose, int ioDelay, int changeDiagMode, int changePWRMode )
        :m_io( io )
    {
        if(verbose)
        {
            initialLogger();
        }

    }

    SiSTouchAdapter( SiSTouchIO* io, int max_retry, int delay, int verbose, int ioDelay )
        :m_io( io )
    {
        if(verbose)
        {
            initialLogger();
        }

    }

    enum
    {
        CALIBRATION_FLAG_LENGTH = 4 /sizeof(int),
        BOOT_FLAG_LENGTH = 4 /sizeof(int),
        FIRMWARE_ID_LENGTH = 12 / sizeof(int),
        FIRMWARE_INFO_LENGTH = 0x50 / sizeof(int),
        BOOTLOADER_ID_LENGTH = 4 / sizeof(int),
        BOOT_INFO_LENGTH = 0x10 / sizeof(int),
        MAX_BUF_LENGTH = 0x10000 / sizeof(int),
        _64K = 0x10000,
        _56K = 0xe000,
        _48K = 0xc000,
        _12K = 0x3000,
        _4K = 0x1000,
        _8K = 0x2000,
        _4K_ALIGN_12 = 0x1000 / 12 * 12,
        _4K_ALIGN_16 = 0x1000 / 16 * 16,

        I2C_INTERFACE = 0x1,
        USB_INTERFACE = 0x2,
    };


    bool compareId( int* idA, int* idB, int length );
    bool checkFileBufIsValid( int file_length, int* buf, int buf_length );

    virtual int get_READ_LENGTH() = 0;
    virtual int get_BUFFER_SIZE() = 0;
    virtual int get_actual_pending_byte_length(int pending_length)= 0;
    virtual unsigned int getFirmwareInfoBase() = 0;
    virtual unsigned int getFirmwareInfoAddr() = 0;
    virtual unsigned int getFirmwareIdBase() = 0;
    virtual unsigned int getFirmwareIdAddr() = 0;
    virtual unsigned int getBootloaderIdBase() = 0;
    virtual unsigned int getBootloaderIdAddr() = 0;
    virtual unsigned int getCalibrationStateAddr() = 0;
    virtual unsigned int getCalibrationStateMask() = 0;
    virtual unsigned int getCalibrationStateDoneFlag() = 0;
    virtual unsigned int getBootFlagBase() = 0;
    virtual unsigned int getBootFlagAddr() = 0;
    virtual unsigned int getBootFlagValue() = 0;
    virtual unsigned int getCalibrationFlagBase() = 0;
    virtual unsigned int getCalibrationFlagAddr() = 0;
    virtual unsigned int getCalibrationFlagValue() = 0;
    virtual unsigned int getVirtualAddressMask() = 0;
    virtual unsigned int getAddressBlockMask() = 0;
	virtual unsigned int getLayoutOffsetAddress() = 0;
	virtual unsigned int getFwModeAddr() { return 0; }

    virtual void prepaerBuffer(int *buf, int buf_length) = 0;
    virtual int updateFlow( int* buf, int buf_length, int file_length, bool update_bootloader, bool isNewBootloader, bool reserve_RODATA, bool update_parameter, bool restart = true) = 0;
	virtual int updateCalibrationFlow(OpenShortData* data) = 0;
	virtual int eraseCalibrationFlag(bool checkBootLoader = false) = 0;
    virtual int isSiSFWFile(int* buf, int buf_length) = 0;

    virtual bool loadTargetVoltage(float *targetVoltage) = 0;
    virtual bool loadWidthHeight(int* width, int* height) = 0;

    virtual bool getDeviceInfo(OpenShortData *data) = 0;
    virtual bool getDeviceVoltageData(OpenShortData *data) = 0;

    // MutualShortExtraCheck, begin
    virtual int calculateCycleTxDiff(OpenShortConfig* config, OpenShortData *deviceData, OpenShortResult *result) = 0;
    virtual int calculateCycleRxDiff(OpenShortConfig* config, OpenShortData *deviceData, OpenShortResult *result) = 0;
    virtual int JudgeTxShortResult(OpenShortConfig* config, OpenShortData *deviceData, OpenShortResult *result) = 0;
    virtual int JudgeRxShortResult(OpenShortConfig* config, OpenShortData *deviceData, OpenShortResult *result) = 0;
    virtual int checkMutualShortExtra(OpenShortConfig* config, OpenShortData* deviceData, OpenShortResult *result) = 0;
    // MutualShortExtraCheck, end

    // MutualNearFarCheck, begin
    virtual int checkMutualNearFar(OpenShortConfig* config, OpenShortData* deviceData, OpenShortResult *result) = 0;
    virtual void checkMutualNearFarTx(OpenShortConfig* config, OpenShortData* deviceData, OpenShortResult *result) = 0;
    virtual void checkMutualNearFarRx(OpenShortConfig* config, OpenShortData* deviceData, OpenShortResult *result) = 0;
    // MutualNearFarCheck, end

    virtual int testPointOpenShort(OpenShortConfig* config, OpenShortData *deviceData, OpenShortResult *result) = 0;
    virtual int testPointOpenShort(OpenShortConfig* config, OpenShortData *deviceData, OpenShortData *sampleData, OpenShortResult *result) = 0;
    virtual int testLineOpenShort(OpenShortConfig* config, OpenShortData *deviceData, OpenShortResult *result) = 0;
    virtual int testLineOpenShort(OpenShortConfig* config, OpenShortData *deviceData, OpenShortData *sampleData, OpenShortResult *result) = 0;

    virtual int parameterCheck(bool& update_bootloader, bool &update_bootloader_auto, bool &isNewBootloader, bool& reserve_RODATA, bool &update_parameter, bool force_update, int *buf, int file_Length) = 0;

    SiSTouchIO* m_io;

    bool isVirtualAddr(unsigned int addr);

    int invert_endian( int data );
    bool read_from_buffer( int* buf, int buf_length, unsigned int offset, int* id, int size );
    bool read_from_address( unsigned int addr, int* buffer, int size );
    int read_file_to_buffer( FILE *file, long offset, int size, int* buf );
    bool write_buffer_to_address( int* buf, int offset, int buf_length, int length, unsigned int addr );
    long getFileLength(FILE* file);
	bool callReset();
    bool callCalibration();
    bool isCalibrationDone();
    float transferRawDataToVoltage(int rawData);
    float transferRawDataToCycle(int rawData);
private:
    // copy constructor
    SiSTouchAdapter(const SiSTouchAdapter & rhs): m_io(0) {}
    // assignment operator
    SiSTouchAdapter & operator = (const SiSTouchAdapter & rhs) { return *this; }

};

#endif
