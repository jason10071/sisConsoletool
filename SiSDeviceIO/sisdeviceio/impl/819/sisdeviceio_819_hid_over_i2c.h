#ifndef SISDEVICEIO_819_HID_OVER_I2C_H
#define SISDEVICEIO_819_HID_OVER_I2C_H

#include "stdio.h"
#include "string"

#include "sisdeviceio/impl/819/isisdeviceio_819_i2c.h"

namespace SiS
{
namespace DeviceIO
{

class SiSDeviceIO_819_hid_over_i2c : public ISiSDeviceIO_819_i2c
{
public:
    explicit SiSDeviceIO_819_hid_over_i2c(SiSDeviceMgr* sisDeviceMgr, SiSIOAttribute* sisIOAttribute);
    virtual ~SiSDeviceIO_819_hid_over_i2c();

    virtual int jumpBootloader(int chipAddress);

//protected:
    virtual int openDevice();
    virtual int closeDevice();

    virtual int writeData(unsigned char * buf, int size, int timeoutMsec);
    virtual int readData( unsigned char * buf, int size, int timeoutMsec);

    virtual bool isUsingIOCTL();

    virtual int readSimpleAck(int size, int chipAddress);
    virtual int readSimpleAckMaster(int size, int chipAddress);
    virtual int readSimpleAckSlave(int size, int chipAddress);

    virtual int makeCommonHeader(int cmd, int length, int chipAddress);
    virtual int makeCommonHeaderMaster(int cmd, int length, int chipAddress);
    virtual int makeCommonHeaderSlave(int cmd, int length, int chipAddress);

    virtual void generateOutputCRC(int chipAddress);
    virtual void generateOutputCRCMaster(int chipAddress);
    virtual void generateOutputCRCSlave(int chipAddress);

    virtual int makeBuffer85(int data, int chipAddress);

    virtual int formatData86Unit(int size);
    virtual int getData86StartIndex(int chipAddress);
    virtual int makeBuffer86(unsigned int address, int size, int chipAddress);

    virtual int makeBuffer82();

    virtual int makeBuffer84(unsigned int pageAddress, unsigned char* data, int size, int chipAddress);
    virtual int comparePageAddrAck(unsigned int pageAddress, int chipAddress);
    virtual int comparePageAddrAckMaster(unsigned int pageAddress, int chipAddress);
    virtual int comparePageAddrAckSlave(unsigned int pageAddress, int chipAddress);

    virtual int makeBuffer81(unsigned int pageStartAddress, unsigned int pageLength, int chipAddress);

    virtual int makeBuffer83(unsigned int pageStartAddress, unsigned int pageLength, int chipAddress);

protected:
    void initBuffer();

private:
	void throwNotSupportException(std::string functionName);

private:
    //explicit ~SiSDeviceIO_819_hid_over_i2c();
};

}
}

#endif // SISDEVICEIO_819_HID_OVER_I2C_H
