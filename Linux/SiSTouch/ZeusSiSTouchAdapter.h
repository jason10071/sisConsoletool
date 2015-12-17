#ifndef ZEUSSISTOUCHADAPTER_H
#define ZEUSSISTOUCHADAPTER_H
#include "SiSTouchAdapter.h"




class ZeusSiSTouchAdapter : public SiSTouchAdapter
{


protected:

    ZeusSiSTouchAdapter( SiSTouchIO* io, int max_retry, int delay, int verbose, int ioDelay )
        : SiSTouchAdapter(io, max_retry, delay, verbose, ioDelay)
    {
    }

    enum base
    {
        PARAM_BASE = 0x4000,
        PHASE1_BASE = 0x7000,
        ZEUS_BASE = 0x8000,
        LIB_BASE = 0x3000,
        BOOT_BASE = 0x0,
        FLORA_BASE = 0x8000,
        INFO_BASE = 0x7000,
        FIRMWARE_INFO_BASE = 0x8000,
        FIRMWARE_INFO_ADDRESS = 0xa0008000,
        FIRMWARE_ID_BASE = 0x8004,
        FIRMWARE_ID_ADDRESS = 0xa0008004,
        BOOTLOADER_ID_BASE = 0x8030,
        BOOTLOADER_ID_ADDRESS = 0xa0008030,
        CALIBRATION_STATE_ADDRESS = 0x70000001,
        CALIBRATION_STATE_DONE_FLAG = 0x00000001,
        CALIBRATION_STATE_MASK = 0xffffffff,
        CALIBRATION_FLAG_BASE = 0x7ffc,
        CALIBRATION_FLAG_ADDRESS = 0xa0007ffc,
        CALIBRATION_FLAG_VALUE = 0x0038003c,
        LAYOUT_OFFSET_ADDRESS = 0xffffffff,
        BOOT_FLAG_BASE = 0x7ff0,
        BOOT_FLAG_ADDRESS = 0Xa0007ff0,
        BOOT_FLAG_VALUE = 0x50383130,
        TARGET_VOTLAGE_ADDRESS = 0xa000440c,
        WIDTH_HEIGHT_ADDRESS = 0xa0004000,
        VIRTUAL_ADDRESS_BLOCK_MASK = 0x50000000,
        ADDRESS_BLOCK_MASK = 0xf0000000,
        WIDTH_MASK = 0xff000000,
        WIDTH_SHIFT = 24,
        HEIGHT_MASK = 0x00ff0000,
        HEIGHT_SHIFT = 16,
        TARGET_VOLTAGE_MASK = 0x00000ff0,
        TARGET_VOLTAGE_SHIFT = 4,
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
    }

    virtual inline unsigned int getTargetVolatgeAddress()
    {
        return TARGET_VOTLAGE_ADDRESS;
    }

    virtual inline unsigned int getWidthHeightAddress()
    {
        return WIDTH_HEIGHT_ADDRESS;
    }

    virtual unsigned int getBootFlagBase()
    {
        return BOOT_FLAG_BASE;
    }

    virtual unsigned int getBootFlagAddr()
    {
        return BOOT_FLAG_ADDRESS;
    }

    virtual inline unsigned int getBootFlagValue()
    {
        return BOOT_FLAG_VALUE;
    }

    virtual inline unsigned int getVirtualAddressMask()
    {
        return VIRTUAL_ADDRESS_BLOCK_MASK;
    }

    virtual inline unsigned int getAddressBlockMask()
    {
        return ADDRESS_BLOCK_MASK;
    }

	virtual unsigned int getLayoutOffsetAddress()
    {
        return LAYOUT_OFFSET_ADDRESS;
    }

    virtual inline bool loadTargetVoltage(float *targetVoltage)
    {
        return false;
    }

    virtual inline bool loadWidthHeight(int* width, int* height)
    {
        return false;
    }

    virtual inline bool getDeviceInfo(OpenShortData *data)
    {
        return false;
    }

    virtual inline bool getDeviceVoltageData(OpenShortData *data)
    {
        return false;
    }

    // MutualShortExtraCheck, begin
    virtual inline int calculateCycleTxDiff(OpenShortConfig* config, OpenShortData *deviceData, OpenShortResult *result)
    {
        return ERROR_NOT_IMPLEMENTED;
    }

    virtual inline int calculateCycleRxDiff(OpenShortConfig* config, OpenShortData *deviceData, OpenShortResult *result)
    {
        return ERROR_NOT_IMPLEMENTED;
    }

    virtual inline int JudgeTxShortResult(OpenShortConfig* config, OpenShortData *deviceData, OpenShortResult *result)
    {
        return ERROR_NOT_IMPLEMENTED;
    }

    virtual inline int JudgeRxShortResult(OpenShortConfig* config, OpenShortData *deviceData, OpenShortResult *result)
    {
        return ERROR_NOT_IMPLEMENTED;
    }

    virtual inline int checkMutualShortExtra(OpenShortConfig* config, OpenShortData* deviceData, OpenShortResult *result)
    {
        return ERROR_NOT_IMPLEMENTED;
    }
    // MutualShortExtraCheck, end

    // MutualNearFarCheck, begin
    virtual inline int checkMutualNearFar(OpenShortConfig* config, OpenShortData* deviceData, OpenShortResult *result)
    {
        return ERROR_NOT_IMPLEMENTED;
    }

    virtual inline void checkMutualNearFarTx(OpenShortConfig* config, OpenShortData* deviceData, OpenShortResult *result)
    {
        //return ERROR_NOT_IMPLEMENTED;
    }

    virtual inline void checkMutualNearFarRx(OpenShortConfig* config, OpenShortData* deviceData, OpenShortResult *result)
    {
        //return ERROR_NOT_IMPLEMENTED;
    }

    // MutualNearFarCheck, end

    virtual inline int testPointOpenShort(OpenShortConfig* config, OpenShortData *deviceData, OpenShortResult *result)
    {
        return ERROR_NOT_IMPLEMENTED;
    }
    virtual inline int testPointOpenShort(OpenShortConfig* config, OpenShortData *deviceData, OpenShortData *sampleData, OpenShortResult *result)
    {
        return ERROR_NOT_IMPLEMENTED;
    }

    virtual inline int testLineOpenShort(OpenShortConfig* config, OpenShortData *deviceData, OpenShortResult *result)
    {
        return ERROR_NOT_IMPLEMENTED;
    }

    virtual inline int testLineOpenShort(OpenShortConfig* config, OpenShortData *deviceData, OpenShortData *sampleData, OpenShortResult *result)
    {
        return ERROR_NOT_IMPLEMENTED;
    }

    virtual int updateFlow( int* buf, int buf_length, int file_length, bool update_bootloader, bool isNewBootloader, bool reserve_RODATA, bool update_parameter, bool restart = true );
	virtual int updateCalibrationFlow(OpenShortData *data);
	virtual int eraseCalibrationFlag(bool checkBootLoader = false);
    virtual void prepaerBuffer(int *buf, int buf_length);
    virtual int isSiSFWFile(int* buf, int buf_length);

    virtual int parameterCheck(bool& update_bootloader, bool& update_bootloader_auto, bool &isNewBootloader, bool& reserve_RODATA, bool &update_parameter, bool force_update, int *buf, int file_Length);

    virtual inline int get_READ_LENGTH()
    {
        return READ_LENGTH;
    };

    virtual inline int get_BUFFER_SIZE()
    {
        return BUFFER_SIZE;
    };

    virtual inline int get_actual_pending_byte_length(int pending_length)
    {
        return READ_LENGTH * sizeof(int);
    }


private:
    enum
    {
        BUFFER_SIZE = 16,
        READ_LENGTH = 12 / sizeof(int),
    };

};



class ZeusI2CSiSTouchAdapter : public ZeusSiSTouchAdapter
{
public:
    ZeusI2CSiSTouchAdapter( int max_retry, int delay, int verbose, int ioDelay )
        : ZeusSiSTouchAdapter(new SiSTouch_Zeus_I2C(max_retry, delay, verbose, ioDelay), max_retry, delay, verbose, ioDelay )
    {
        
    }




};


class ZeusUSBSiSTouchAdapter : public ZeusSiSTouchAdapter
{
public:
    ZeusUSBSiSTouchAdapter( int max_retry, int delay, int verbose, int ioDelay )
        : ZeusSiSTouchAdapter(new SiSTouch_Zeus_USB(max_retry, delay, verbose, ioDelay), max_retry, delay, verbose, ioDelay)
    {

    }





};


class ZeusInterUSBSiSTouchAdapter : public ZeusSiSTouchAdapter
{
public:
    ZeusInterUSBSiSTouchAdapter( int max_retry, int delay, int verbose, int ioDelay )
        : ZeusSiSTouchAdapter(new SiSTouch_Zeus_InterUSB(max_retry, delay, verbose, ioDelay), max_retry, delay, verbose, ioDelay)
    {

    }


protected:


    virtual inline int get_READ_LENGTH()
    {
        return READ_LENGTH;
    };

    virtual inline int get_BUFFER_SIZE()
    {
        return BUFFER_SIZE;
    };


private:
    enum
    {
        BUFFER_SIZE = 19,
        READ_LENGTH = 16 / sizeof(int),
    };

};


#endif
