#ifndef ISISDEVICEIO_H
#define ISISDEVICEIO_H

#include <unistd.h>
#include <sys/syscall.h>
#include "stdio.h"
#include "string"

#include "sisdevicemgr/sisdevicemgr.h"
#include "sisattribute/sisioattribute.h"

/* chip info */
#define MASTER_ADDRESS 0

/* return code */
#define ACK_BEEF 1

/* CMD_85 Mode */
#define PWR_CMD_FWCTRL 			0x00005009
#define PWR_CMD_ACTIVE 			0x00005109
#define ENABLE_DIAGNOSIS_MODE 	0x00002101
#define DISABLE_DIAGNOSIS_MODE 	0x00002001
#define JUMP_BOOTLOADER			0x00000108

namespace SiS
{
namespace DeviceIO
{

class ISiSDeviceIO
{
public:
    explicit ISiSDeviceIO();
    virtual ~ISiSDeviceIO();

    std::string deviceNameOpened();
    SiSDeviceMgr* getSiSDeviceMgr();
    SiSIOAttribute* getSiSIOAttribute();

    void setUserConnectType(SiSDeviceAttribute::ConnectType userConnectType);
    virtual void boot(std::string deviceName);
    virtual void shutdown();

    virtual int ctlReportToOs(bool isReport);
    virtual int ctlSoftReset();
    virtual int jumpBootloader(int chipAddress) = 0;
    virtual bool readFromAddress(unsigned int address, int size, unsigned char* buffer, int chipAddress);
    virtual int burnToROM(unsigned int address, int size, unsigned char* binBuffer, int binSize, int chipAddress) = 0;

//protected:
    virtual int openDevice() = 0;
    virtual int closeDevice() = 0;

    void printBuffer( int cmd, const unsigned char* data, int size );
    void printBuffer(const unsigned char* data, int size);
    virtual int getMaxBufferSize() = 0;

    virtual int writeData(unsigned char * buf, int size, int timeoutMsec) = 0;
    virtual int readData( unsigned char * buf, int size, int timeoutMsec) = 0;
    void waitIOReady(int &fd, int timeoutSec);
    int ioCommand(int cmd, unsigned char * buf, int writeSize, int readSize, int timeoutMsec = 5);
    virtual bool isUsingIOCTL() = 0;

    virtual int readSimpleAck(int size, int chipAddress) = 0;
    virtual int readSimpleAckMaster(int size, int chipAddress) = 0;
    virtual int readSimpleAckSlave(int size, int chipAddress) = 0;

    virtual int getReportIdIndex(bool isOut = true) = 0;
    virtual int getReportId(int size, bool isOut = true) = 0;
    virtual int getBufferSizeOfReportId(int reportId) = 0;
	
    virtual int makeCommonHeader(int cmd, int length, int chipAddress) = 0;
    virtual int makeCommonHeaderMaster(int cmd, int length, int chipAddress) = 0;
    virtual int makeCommonHeaderSlave(int cmd, int length, int chipAddress) = 0;
	
    virtual void generateOutputCRC(int chipAddress) = 0;
    virtual void generateOutputCRCMaster(int chipAddress) = 0;
    virtual void generateOutputCRCSlave(int chipAddress) = 0;

    virtual int makeBuffer85(int data, int chipAddress) = 0;
    virtual int call85(int data, int chipAddress) = 0;
    virtual int call85WithRetry(int data, int chipAddress);

    virtual int formatData86Unit(int size) = 0;
    virtual int getData86StartIndex(int chipAddress) = 0;
    virtual int makeBuffer86(unsigned int address, int size, int chipAddress) = 0;
    virtual int call86(unsigned int address, unsigned char* data, int size, int chipAddress) = 0;
    virtual int call86WithRetry(unsigned int address, unsigned char* data, int size, int chipAddress);

    virtual int makeBuffer82() = 0;
    virtual int call82() = 0;
    virtual int call82WithRetry();

    virtual int makeBuffer84(unsigned int pageAddress, unsigned char* data, int size, int chipAddress) = 0;
    virtual int call84(unsigned int pageAddress, unsigned char* data, int size, int chipAddress) = 0;
    virtual int comparePageAddrAck(unsigned int pageAddress, int chipAddress) = 0;
    virtual int comparePageAddrAckMaster(unsigned int pageAddress, int chipAddress) = 0;
    virtual int comparePageAddrAckSlave(unsigned int pageAddress, int chipAddress) = 0;

    virtual int makeBuffer81(unsigned int pageStartAddress, unsigned int pageLength, int chipAddress) = 0;
    virtual int call81(unsigned int pageStartAddress, unsigned int pageLength, int chipAddress) = 0;

    virtual int makeBuffer83(unsigned int pageStartAddress, unsigned int pageLength, int chipAddress) = 0;
    virtual int call83(unsigned int pageStartAddress, unsigned int pageLength, int chipAddress) = 0;
    virtual int call83WithRetry(unsigned int pageStartAddress, unsigned int pageLength, int chipAddress);

protected:
    void appendCRC(char data);

protected:
    SiSDeviceMgr* m_sisDeviceMgr;
    SiSIOAttribute* m_sisIOAttribute;
    int m_fd;
    unsigned short m_crc; // save crc current value
    unsigned char* m_buffer;
    SiSDeviceAttribute::ConnectType m_userConnectType;
    
};

}
}

#endif // ISISDEVICEIO_H
