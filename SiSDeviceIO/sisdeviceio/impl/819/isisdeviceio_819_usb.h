#ifndef ISISDEVICEIO_819_USB_H
#define ISISDEVICEIO_819_USB_H

#include "stdio.h"
#include "string"

#include "sisdeviceio/isisdeviceio.h"

#define MAX_BUFFER_SIZE_819_USB 832

/* report id : 0x21-64, 0x25-64\*5, 0x29-64\*9, 0x2d-64\*13 */
#define REPORT_ID_21 0x21
#define REPORT_ID_25 0x25
#define REPORT_ID_29 0x29
#define REPORT_ID_2D 0x2d
#define REPORT_ID_21_SIZE 64
#define REPORT_ID_25_SIZE 320
#define REPORT_ID_29_SIZE 576
#define REPORT_ID_2D_SIZE 832

/* burn info */
#define PAGE_SIZE 256
#define __16K 0x4000

/* cmd info */
#define DATA_86_UNIT_64 56
#define DATA_86_UNIT_832 768
#define DATA_84_UNIT_832 768

namespace SiS
{
namespace DeviceIO
{

class ISiSDeviceIO_819_usb : public ISiSDeviceIO
{
public:
    explicit ISiSDeviceIO_819_usb();
    virtual ~ISiSDeviceIO_819_usb();

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

	virtual int call83(unsigned int pageStartAddress, unsigned int pageLength, int chipAddress) {} // not suport 

protected:
    void initBuffer();

};

}
}

#endif // ISISDEVICEIO_819_USB_H
