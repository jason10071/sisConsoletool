#ifndef AEGISSISTOUCHADAPTER_H
#define AEGISSISTOUCHADAPTER_H
#include "SiSTouchAdapter.h"
#include "MultiOpenShortData.h"
#include "MultiOpenShortResult.h"
#include "UpdateFWParameter.h"

class AegisSiSTouchAdapter : public SiSTouchAdapter
{
protected:
    AegisSiSTouchAdapter( SiSTouchIO* io, int max_retry, int delay, int verbose, int ioDelay, int changeDiagMode, int changePWRMode )
        : SiSTouchAdapter(io, max_retry, delay, verbose, ioDelay, changeDiagMode, changePWRMode)
    {
    }

protected:

    enum base
    {
        INITIAL_BASE = 0x0,
        BUILDER_CHIPINFO_ADDRESS = 0x200,
        BUILDER_SISMARK_ADDRESS = 0x20c,
        CALIBRATION_BASE =0xd000,
        CALIBRATION_BASE2 =0xe000,
        UPDATE_BASE = 0xf000,
        MAINCODE_BASE =0x1000,
        MAINCODE_ADDRESS =0xa0001000,
        AEGIS_BASE = 0xc000,
        FIRMWARE_INFO_BASE = 0xc000,
        FIRMWARE_INFO_ADDRESS = 0xa000c000,
        FIRMWARE_ID_BASE = 0xc004,
        FIRMWARE_ID_ADDRESS = 0xa000c004,
        BOOTLOADER_ID_BASE = 0xc030,
        BOOTLOADER_ID_ADDRESS = 0xa000c030,
        CALIBRATION_STATE_ADDRESS = 0x50000000,
        CALIBRATION_STATE_DONE_FLAG = 0x00010000,
        CALIBRATION_STATE_MASK = 0x00ff0000,
        CALIBRATION_FLAG_BASE = 0xeffc,
        CALIBRATION_FLAG_ADDRESS = 0xa000effc,
        CALIBRATION_FLAG_VALUE = 0x50383130,
        LAYOUT_OFFSET_ADDRESS = 0xa000c028,
        BOOT_FLAG_BASE = 0Xeff0,
        BOOT_FLAG_ADDRESS = 0Xa000eff0,
        BOOT_FLAG_VALUE = 0x50383130,
        DEVICE_CALIBRATION_DATA_ADDRESS = 0Xa000d000,
        TARGET_VOTLAGE_ADDRESS = 0x5a000002,
        WIDTH_HEIGHT_ADDRESS = 0x5a000000,
        BASE_VOTLAGE_ADDRESS = 0x51000000,
        RAW_VOTLAGE_ADDRESS = 0x52000000,
        DIFF_VOTLAGE_ADDRESS = 0x53000000,
        CYCLE_DATA_ADDRESS = 0x54000000,
        LOOP_DATA_ADDRESS = 0x55000000,
        FW_MODE_ADDRESS = 0x58000000,
        VIRTUAL_ADDRESS_BLOCK_MASK = 0x50000000,
        ADDRESS_BLOCK_MASK = 0xf0000000,
        WIDTH_MASK = 0x00ff0000,
        WIDTH_SHIFT = 16,
        HEIGHT_MASK = 0x0000ff00,
        HEIGHT_SHIFT = 8,
        TARGET_VOLTAGE_MASK = 0x0ff00000,
        TARGET_VOLTAGE_SHIFT = 20,
        MEMORY_ALIGNMENT = 4,
        MAIN_CODE_LENGTH = 0xB000 / sizeof(int),
        MAINCDOE_CRC_BASE = 0xc044,
        BOOTLOADER_CRC_BASE = 0xc048,

        INTERFACE_FLAG_ADDR = 0xa000c024,
        INTERFACE_FLAG_BASE = 0xc024,
        INTERFACE_SHIFT = 8,
        INTERFACE_MASK = 0xff,
        MULTI_SHIFT = 0,
        MULTI_MASK = 0xff,

        NON_MULTI_DEVICE_FLAG = 0x0,
        SLAVE_DEVICE0_FLAG = 0x5,
        SLAVE_DEVICE1_FLAG = 0x6,
        SLAVE_DEVICE2_FLAG = 0x7,
        MASTER_DEVICE_FLAG = 0x1,
        BRIDGE_DEVICE_FLAG = 0xb,

        SLAVE_NUM_ADDR = 0xa000c028,
        SLAVE_NUM_SHIFT = 24,
        SLAVE_NUM_MASK = 0xff,

    };

    virtual inline unsigned int getFirmwareInfoBase()
    {
        return FIRMWARE_INFO_BASE;
    };

    virtual inline unsigned int getFirmwareInfoAddr()
    {
        return FIRMWARE_INFO_ADDRESS;
    };

    virtual inline unsigned int getFirmwareIdBase()
    {
        return FIRMWARE_ID_BASE;
    };

    virtual inline unsigned int getFirmwareIdAddr()
    {
        return FIRMWARE_ID_ADDRESS;
    };

    virtual inline unsigned int getBootloaderIdBase()
    {
        return BOOTLOADER_ID_BASE;
    };

    virtual inline unsigned int getBootloaderIdAddr()
    {
        return BOOTLOADER_ID_ADDRESS;
    };

    virtual inline unsigned int getCalibrationStateAddr()
    {
        return CALIBRATION_STATE_ADDRESS;
    };

    virtual inline unsigned int getCalibrationStateMask()
    {
        return  CALIBRATION_STATE_MASK;
    };

    virtual inline unsigned int getCalibrationStateDoneFlag()
    {
        return CALIBRATION_STATE_DONE_FLAG;
    };

    virtual inline unsigned int getCalibrationFlagBase()
    {
        return CALIBRATION_FLAG_BASE;
    };

    virtual inline unsigned int getCalibrationFlagAddr()
    {
        return CALIBRATION_FLAG_ADDRESS;
    };

    virtual inline unsigned int getCalibrationFlagValue()
    {
        return CALIBRATION_FLAG_VALUE;
    };

    virtual inline unsigned int getTargetVolatgeAddress()
    {
        return TARGET_VOTLAGE_ADDRESS;
    };

    virtual inline unsigned int getWidthHeightAddress()
    {
        return WIDTH_HEIGHT_ADDRESS;
    };

    virtual inline unsigned int getBaseVoltageAddress()
    {
        return BASE_VOTLAGE_ADDRESS;
    };

    virtual inline unsigned int getDiffVoltageAddress()
    {
        return DIFF_VOTLAGE_ADDRESS;
    };

    virtual inline unsigned int getRawVoltageAddress()
    {
        return RAW_VOTLAGE_ADDRESS;
    };

    virtual inline unsigned int getCycleAddress()
    {
        return CYCLE_DATA_ADDRESS;
    };

    virtual inline unsigned int getLoopAddress()
    {
        return LOOP_DATA_ADDRESS;
    };

    virtual unsigned int getBootFlagBase()
    {
        return BOOT_FLAG_BASE;
    };

    virtual unsigned int getBootFlagAddr()
    {
        return BOOT_FLAG_ADDRESS;
    };

    virtual inline unsigned int getBootFlagValue()
    {
        return BOOT_FLAG_VALUE;
    };

    virtual inline unsigned int getVirtualAddressMask()
    {
        return VIRTUAL_ADDRESS_BLOCK_MASK;
    };

    virtual inline unsigned int getAddressBlockMask()
    {
        return ADDRESS_BLOCK_MASK;
    };

    virtual inline int get_READ_LENGTH()
    {
        return READ_LENGTH;
    };

    virtual inline int get_BUFFER_SIZE()
    {
        return BUFFER_SIZE;
    };

    virtual inline int getMainCodeCrcBase()
    {
        return MAINCDOE_CRC_BASE;
    };

    virtual inline int getBootloaderCrcBase()
    {
        return BOOTLOADER_CRC_BASE;
    };


	virtual unsigned int getLayoutOffsetAddress()
    {
        return LAYOUT_OFFSET_ADDRESS;
    };

	virtual unsigned int getFwModeAddr()
	{
		return FW_MODE_ADDRESS;
	};

    virtual bool loadTargetVoltage(float *targetVoltage);
    virtual bool loadWidthHeight(int* width, int* height);

    virtual bool getDeviceInfo(OpenShortData *data);
    virtual bool getDeviceVoltageData(OpenShortData *data);

    virtual bool loadBaseVoltage(float *dataBuf, int buf_width, int buf_height);
    virtual bool loadRawVoltage(float *dataBuf, int buf_width, int buf_height);
    virtual bool loadCycle(float *dataBuf, int buf_width, int buf_height);
    virtual bool loadLoop(float *dataBuf, int buf_width, int buf_height);
    virtual bool loadDiff(float *dataBuf, int buf_width, int buf_height);

    // MutualShortExtraCheck, begin
    virtual int calculateCycleTxDiff(OpenShortConfig* config, OpenShortData *deviceData, OpenShortResult *result);
    virtual int calculateCycleRxDiff(OpenShortConfig* config, OpenShortData *deviceData, OpenShortResult *result);
    virtual int JudgeTxShortResult(OpenShortConfig* config, OpenShortData *deviceData, OpenShortResult *result);
    virtual int JudgeRxShortResult(OpenShortConfig* config, OpenShortData *deviceData, OpenShortResult *result);
    virtual int checkMutualShortExtra(OpenShortConfig* config, OpenShortData* deviceData, OpenShortResult *result);
    // MutualShortExtraCheck, end

    // MutualNearFarCheck, begin
    virtual int checkMutualNearFar(OpenShortConfig* config, OpenShortData* deviceData, OpenShortResult *result);
    virtual void checkMutualNearFarTx(OpenShortConfig* config, OpenShortData* deviceData, OpenShortResult *result);
    virtual void checkMutualNearFarRx(OpenShortConfig* config, OpenShortData* deviceData, OpenShortResult *result);
    // MutualNearFarCheck, end

    virtual int testPointOpenShort(OpenShortConfig* config, OpenShortData *deviceData, OpenShortResult *result);
    virtual int testPointOpenShort(OpenShortConfig* config, OpenShortData *deviceData, OpenShortData *sampleData, OpenShortResult *result);
    virtual int testLineOpenShort(OpenShortConfig* config, OpenShortData *deviceData, OpenShortResult *result);
    virtual int testLineOpenShort(OpenShortConfig* config, OpenShortData *deviceData, OpenShortData *sampleData, OpenShortResult *result);


    virtual int updateFlow( int* buf, int buf_length, int file_length, bool update_bootloader, bool isNewBootloader, bool reserve_RODATA, bool update_parameter, bool restart = true );
	virtual int updateCalibrationFlow(OpenShortData* data);
	virtual int eraseCalibrationFlag(bool checkBootLoader = false);
    virtual void prepaerBuffer(int *buf, int buf_length);
    virtual int isSiSFWFile(int* buf, int buf_length);

    virtual int parameterCheck(bool& update_bootloader, bool& update_bootloader_auto, bool &isNewBootloader, bool& reserve_RODATA, bool &update_parameter, bool force_update, int *buf, int file_Length);

    virtual inline int get_actual_pending_byte_length(int pending_length)
    {
//        return READ_LENGTH * sizeof(int);
        int max_pending_length = READ_LENGTH * sizeof(int);
        return ( pending_length < max_pending_length ) ? pending_length:max_pending_length;
    }

private:
    enum
    {
        BUFFER_SIZE = 64,
        READ_LENGTH = 52 / sizeof(int),
    };

    float getDiffPercentage(float numerator, float denominator);
    float getAbsDiffPercentage(float numerator, float denominator);


};





class AegisI2CSiSTouchAdapter : public AegisSiSTouchAdapter
{
public:
    AegisI2CSiSTouchAdapter( int max_retry, int delay, int verbose, int ioDelay, int changeDiagMode, int changePWRMode )
        : AegisSiSTouchAdapter(new SiSTouch_Aegis_I2C(max_retry, delay, verbose, ioDelay, changeDiagMode, changePWRMode), max_retry, delay, verbose, ioDelay, changeDiagMode, changePWRMode)
    {

    }

	AegisI2CSiSTouchAdapter( int max_retry, int delay, int verbose, int ioDelay, int changeDiagMode, int changePWRMode, int detectHidrawFlag )
        : AegisSiSTouchAdapter(new SiSTouch_Aegis_I2C_FOR_NEW_DRIVER(max_retry, delay, verbose, ioDelay, changeDiagMode, changePWRMode), max_retry, delay, verbose, ioDelay, changeDiagMode, changePWRMode)
    {

    }

    virtual int isSiSFWFile(int* buf, int buf_length);

};

class AegisUSBSiSTouchAdapter : public AegisSiSTouchAdapter
{

public:
    AegisUSBSiSTouchAdapter( int max_retry, int delay, int verbose, int ioDelay, int changeDiagMode, int changePWRMode )
        : AegisSiSTouchAdapter( new SiSTouch_Aegis_USB(max_retry, delay, verbose, ioDelay, changeDiagMode, changePWRMode), max_retry, delay, verbose, ioDelay, changeDiagMode, changePWRMode)
    {

    }

    virtual int isSiSFWFile(int* buf, int buf_length);


};


class AegisMultiSiSTouchAdapter : public AegisSiSTouchAdapter
{

protected:
    AegisMultiSiSTouchAdapter( SiSTouchIO* io, int max_retry, int delay, int verbose, int ioDelay, int changeDiagMode, int changePWRMode )
        : AegisSiSTouchAdapter(io, max_retry, delay, verbose, ioDelay, changeDiagMode, changePWRMode)
    {

    }

public:


    enum chipIdx
    {
        INDEX_MASTER = 0,
        INDEX_SLAVE0 = 1,
        INDEX_SLAVE1 = 2,
        INDEX_SLAVE2 = 3,
        INDEX_SLAVE3 = 4,
        INDEX_SLAVE4 = 5,
        INDEX_SLAVE5 = 6,
    };

    enum multiAddr
    {
        MASTER_DEVICE_ADDR = 0,
        SLAVE_DEVICE0_ADDR = 5,
        SLAVE_DEVICE1_ADDR = 6,
        SLAVE_DEVICE2_ADDR = 7,
        SLAVE_DEVICE3_ADDR = 8,
        SLAVE_DEVICE4_ADDR = 9,
        SLAVE_DEVICE5_ADDR = 10,
        NON_MULTI_DEVICE_ADDR = -1,
    };

    AegisMultiSiSTouchAdapter( int max_retry, int delay, int verbose, int ioDelay, int changeDiagMode, int changePWRMode )
        : AegisSiSTouchAdapter(new SiSTouch_Aegis_Multi(max_retry, delay, verbose, ioDelay, changeDiagMode, changePWRMode), max_retry, delay, verbose, ioDelay, changeDiagMode, changePWRMode)
    {

    }

	AegisMultiSiSTouchAdapter( int max_retry, int delay, int verbose, int ioDelay, int changeDiagMode, int changePWRMode, int detectHidrawFlag )
        : AegisSiSTouchAdapter(new SiSTouch_Aegis_Multi_FOR_NEW_DRIVER(max_retry, delay, verbose, ioDelay, changeDiagMode, changePWRMode), max_retry, delay, verbose, ioDelay, changeDiagMode, changePWRMode)
    {
    }

    virtual int isSiSFWFile(int* buf, int buf_length);

    int doUpdate(FILE** multi_files, int multi_Num, const UpdateFWParameter & param);
	int doEraseCalibrationFlag( int multi_Num );

    int getMultiDeviceOpenShortData(MultiOpenShortData *data);


    bool checkContinueNG(OpenShortConfig* config, MultiOpenShortResult *result, int x, int y);
	void checkSecondThreshold(OpenShortConfig* config, MultiOpenShortResult *result);
    int doMultiOpenShort(MultiOpenShortData* deviceData, OpenShortConfig* config, MultiOpenShortResult *result);
    int doMultiOpenShort(MultiOpenShortData* deviceData, MultiOpenShortData* sampleData, OpenShortConfig* config, MultiOpenShortResult *result);

    // MutualShortExtraCheckByTP, begin
    int testLineOpenShortByTP(OpenShortConfig* config, MultiOpenShortData *deviceData, MultiOpenShortResult *result);
    float getAbsDiffPercentage(float numerator, float denominator);
    int JudgeTxShortResultByTP(OpenShortConfig* config, MultiOpenShortData *deviceData, MultiOpenShortResult *result);
    int JudgeRxShortResultByTP(OpenShortConfig* config, MultiOpenShortData *deviceData, MultiOpenShortResult *result);
    int checkMutualShortExtraByTP(OpenShortConfig* config, MultiOpenShortData *deviceData, MultiOpenShortResult *result);
    // MutualShortExtraCheckByTP, end

    // MutualNearFarCheckByTP, begin
    int checkMutualNearFarByTP(OpenShortConfig* config, MultiOpenShortData *deviceData, MultiOpenShortResult *result);
    void checkMutualNearFarTxByTP(OpenShortConfig* config, MultiOpenShortData *deviceData, MultiOpenShortResult *result);
    void checkMutualNearFarRxByTP(OpenShortConfig* config, MultiOpenShortData *deviceData, MultiOpenShortResult *result);
    // MutualNearFarCheckByTP, end

    virtual void setSlaveAddr( int slaveAddr );
    virtual int getSlaveAddr();

    static int getStaticSlaveAddr(int idx);

    virtual bool doDetectIsMulti();
    virtual int doDetectSlaveNum();

    static bool ioStaticDetectIsMulti(int max_retry, int retryDelay, int verbose, int ioDelay, int changeDiagMode, int changePWRMode);


};

class AegisInterUSBSiSTouchAdapter : public AegisMultiSiSTouchAdapter
{
public:
    AegisInterUSBSiSTouchAdapter( int max_retry, int delay, int verbose, int ioDelay, int changeDiagMode, int changePWRMode )
        : AegisMultiSiSTouchAdapter(new SiSTouch_Aegis_InterUSB(max_retry, delay, verbose, ioDelay, changeDiagMode, changePWRMode), max_retry, delay, verbose, ioDelay, changeDiagMode, changePWRMode)
    {

    }

    virtual int isSiSFWFile(int* buf, int buf_length);



};


class AegisSlaveSiSTouchAdapter : public AegisMultiSiSTouchAdapter
{
public:
    AegisSlaveSiSTouchAdapter( int max_retry, int delay, int verbose, int ioDelay, int changeDiagMode, int changePWRMode )
        : AegisMultiSiSTouchAdapter( new SiSTouch_Aegis_Slave(max_retry, delay, verbose, ioDelay, changeDiagMode, changePWRMode), max_retry, delay, verbose, ioDelay, changeDiagMode, changePWRMode)
    {

    }

    virtual int isSiSFWFile(int* buf, int buf_length);



};






#endif
