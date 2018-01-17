#ifndef ISISDEVICEIO_819_I2C_H
#define ISISDEVICEIO_819_I2C_H

#include "stdio.h"
#include "string"

#include "sisdeviceio/isisdeviceio.h"

#define MAX_BUFFER_SIZE_819_I2C 64

/* report id : out-09, in-0a*/
#define REPORT_ID_OUT 0x9
#define REPORT_ID_IN 0xa
#define REPORT_ID_OUT_SIZE 64
#define REPORT_ID_IN_SIZE 64

/* burn info */
#define PAGE_SIZE 256
#define __12K 0x3000
#define __8K 0x2000

/* cmd info */
#define DATA_86_UNIT_64 52
#define DATA_84_UNIT_64 52

namespace SiS
{
namespace DeviceIO
{

class ISiSDeviceIO_819_i2c : public ISiSDeviceIO
{
public:
    explicit ISiSDeviceIO_819_i2c();
    virtual ~ISiSDeviceIO_819_i2c();

    virtual int burnToROM(unsigned int address, int size, unsigned char* binBuffer, int binSize, int chipAddress);

//protected:
    virtual int getMaxBufferSize();

    virtual int getReportIdIndex(bool isOut = true);
    virtual int getReportId(int size, bool isOut = true);
    virtual int getBufferSizeOfReportId(int reportId);

    virtual int call85(int data, int chipAddress);

    virtual int call86(unsigned int address, unsigned char* data, int size, int chipAddress);

    virtual int call82();

    virtual int call84(unsigned int pageAddress, unsigned char* data, int size, int chipAddress);
	
    virtual int call81(unsigned int pageStartAddress, unsigned int pageLength, int chipAddress);

    virtual int call83(unsigned int pageStartAddress, unsigned int pageLength, int chipAddress);

};

}
}

#endif // ISISDEVICEIO_819_I2C_H
