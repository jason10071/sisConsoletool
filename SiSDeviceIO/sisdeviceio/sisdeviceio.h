#ifndef SISDEVICEIO_H
#define SISDEVICEIO_H

#include "stdio.h"
#include "string"

#include "sisdeviceio/isisdeviceio.h"

namespace SiS
{
namespace DeviceIO
{

class SiSDeviceIO : public ISiSDeviceIO
{
public:
    explicit SiSDeviceIO();
    virtual ~SiSDeviceIO();

    virtual void boot(std::string deviceName);
    virtual void shutdown();

    virtual int ctlReportToOs(bool isReport);
    virtual int ctlSoftReset();
    virtual int jumpBootloader(int chipAddress);
    virtual bool readFromAddress(unsigned int address, int size, unsigned char* buffer, int chipAddress);
    virtual int burnToROM(unsigned int address, int size, unsigned char* binBuffer, int binSize, int chipAddress);

private:
    virtual int openDevice();
    virtual int closeDevice();

    virtual int getMaxBufferSize();

    virtual int writeData(unsigned char * buf, int size, int timeoutMsec);
    virtual int readData( unsigned char * buf, int size, int timeoutMsec);

    virtual bool isUsingIOCTL();

    virtual int readSimpleAck(int size, int chipAddress);
    virtual int readSimpleAckMaster(int size, int chipAddress);
    virtual int readSimpleAckSlave(int size, int chipAddress);

    virtual int getReportIdIndex(bool isOut = true);
    virtual int getReportId(int size, bool isOut = true);
    virtual int getBufferSizeOfReportId(int reportId);

    virtual int makeCommonHeader(int cmd, int length, int chipAddress);
    virtual int makeCommonHeaderMaster(int cmd, int length, int chipAddress);
    virtual int makeCommonHeaderSlave(int cmd, int length, int chipAddress);
	
    virtual void generateOutputCRC(int chipAddress);
    virtual void generateOutputCRCMaster(int chipAddress);
    virtual void generateOutputCRCSlave(int chipAddress);

    virtual int makeBuffer85(int data, int chipAddress);
    virtual int call85(int data, int chipAddress);
    virtual int call85WithRetry(int data, int chipAddress);

    virtual int formatData86Unit(int size);
    virtual int getData86StartIndex(int chipAddress);
    virtual int makeBuffer86(unsigned int address, int size, int chipAddress);
    virtual int call86(unsigned int address, unsigned char* data, int size, int chipAddress);
    virtual int call86WithRetry(unsigned int address, unsigned char* data, int size, int chipAddress);

    virtual int makeBuffer82();
    virtual int call82();
    virtual int call82WithRetry();

    virtual int makeBuffer84(unsigned int pageAddress, unsigned char* data, int size, int chipAddress);
    virtual int call84(unsigned int pageAddress, unsigned char* data, int size, int chipAddress);
    virtual int comparePageAddrAck(unsigned int pageAddress, int chipAddress);
    virtual int comparePageAddrAckMaster(unsigned int pageAddress, int chipAddress);
    virtual int comparePageAddrAckSlave(unsigned int pageAddress, int chipAddress);

    virtual int makeBuffer81(unsigned int pageStartAddress, unsigned int pageLength, int chipAddress);
    virtual int call81(unsigned int pageStartAddress, unsigned int pageLength, int chipAddress);

    virtual int makeBuffer83(unsigned int pageStartAddress, unsigned int pageLength, int chipAddress);
    virtual int call83(unsigned int pageStartAddress, unsigned int pageLength, int chipAddress);

private:
    ISiSDeviceIO* m_iSiSDeviceIO;

};

}
}

#endif // SISDEVICEIO_H
