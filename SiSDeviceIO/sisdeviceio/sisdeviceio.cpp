#include "sisdeviceio.h"

#include "sislog.h"
#include "sisdeviceio/sisdeviceexception.h"

#include "sisdeviceio/impl/819/sisdeviceio_819_usb_hid.h"
#include "sisdeviceio/impl/819/sisdeviceio_819_hid_over_i2c.h"

using namespace SiS;
using namespace SiS::DeviceIO;

#define TAG "SiSDeviceIO"

SiSDeviceIO::SiSDeviceIO() : 
    ISiSDeviceIO(),
    m_iSiSDeviceIO(0)
{
    m_sisDeviceMgr = new SiSDeviceMgr();
	m_sisIOAttribute = new SiSIOAttribute();
}

SiSDeviceIO::~SiSDeviceIO()
{
    delete m_sisDeviceMgr;
    delete m_sisIOAttribute;

    if(m_iSiSDeviceIO != 0)
    {
        delete m_iSiSDeviceIO;
        m_iSiSDeviceIO = 0;
    }
}

void 
SiSDeviceIO::boot(std::string deviceName)
{
    m_sisDeviceMgr->clearDeviceAttributeList();

    /* super */
    ISiSDeviceIO::boot(deviceName);

    SiSDeviceAttribute* opened = m_sisDeviceMgr->getOpened();

    /* instance an impl by con */
    if( opened->getConnectType() == SiSDeviceAttribute::CON_UNKNOW )
    {
        throw SiSDeviceException("boot fail, SiSDeviceMgr::CON_UNKNOW", -1);
    }
    else if( opened->getConnectType() == SiSDeviceAttribute::CON_819_USB_HID )
    {
        SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "instance CON_819_USB_HID" );
        m_iSiSDeviceIO = new SiSDeviceIO_819_usb_hid( m_sisDeviceMgr, m_sisIOAttribute );
    }
    else if( opened->getConnectType() == SiSDeviceAttribute::CON_819_HID_OVER_I2C )
    {
        SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "instance CON_819_HID_OVER_I2C" );
		m_iSiSDeviceIO = new SiSDeviceIO_819_hid_over_i2c( m_sisDeviceMgr, m_sisIOAttribute );
    }

    /* openDevice */
    openDevice();
}

void 
SiSDeviceIO::shutdown()
{
    /* super */
    ISiSDeviceIO::shutdown();

    m_sisDeviceMgr->clearDeviceAttributeList();

    //if(m_iSiSDeviceIO != 0)
    //{
    //    delete m_iSiSDeviceIO;
    //    m_iSiSDeviceIO = 0;
    //}
}

int
SiSDeviceIO::ctlReportToOs(bool isReport)
{
    if( m_iSiSDeviceIO == 0 )
    {
        throw SiSDeviceException("ctlReportToOs() : no impl m_iSiSDeviceIO", -1);
    }

    return m_iSiSDeviceIO->ctlReportToOs(isReport);
}

int
SiSDeviceIO::ctlSoftReset()
{
    if( m_iSiSDeviceIO == 0 )
    {
        throw SiSDeviceException("ctlSoftReset() : no impl m_iSiSDeviceIO", -1);
    }

    return m_iSiSDeviceIO->ctlSoftReset();
}

int 
SiSDeviceIO::jumpBootloader(int chipAddress)
{
    if( m_iSiSDeviceIO == 0 )
    {
        throw SiSDeviceException("jumpBootloader() : no impl m_iSiSDeviceIO", -1);
    }

    return m_iSiSDeviceIO->jumpBootloader(chipAddress);
}

bool 
SiSDeviceIO::readFromAddress(unsigned int address, int size, unsigned char* buffer, int chipAddress)
{
    if( m_iSiSDeviceIO == 0 )
    {
        throw SiSDeviceException("readFromAddress() : no impl m_iSiSDeviceIO", -1);
    }

    return m_iSiSDeviceIO->readFromAddress(address, size, buffer, chipAddress);
}

int
SiSDeviceIO::burnToROM(unsigned int address, int size, unsigned char* binBuffer, int binSize, int chipAddress)
{
    if( m_iSiSDeviceIO == 0 )
    {
        throw SiSDeviceException("burnToROM() : no impl m_iSiSDeviceIO", -1);
    }

    return m_iSiSDeviceIO->burnToROM(address, size, binBuffer, binSize, chipAddress);
}

int 
SiSDeviceIO::openDevice()
{
    if( m_iSiSDeviceIO == 0 )
    {
        throw SiSDeviceException("openDevice() : no impl m_iSiSDeviceIO", -1);
    }

    return m_iSiSDeviceIO->openDevice();
}

int 
SiSDeviceIO::closeDevice()
{
    if( m_iSiSDeviceIO == 0 )
    {
        throw SiSDeviceException("closeDevice() : no impl m_iSiSDeviceIO", -1);
    }

    return m_iSiSDeviceIO->closeDevice();
}

int
SiSDeviceIO::getMaxBufferSize()
{
    if( m_iSiSDeviceIO == 0 )
    {
        throw SiSDeviceException("getMaxBufferSize() : no impl m_iSiSDeviceIO", -1);
    }

    return m_iSiSDeviceIO->getMaxBufferSize();
}

int
SiSDeviceIO::writeData(unsigned char * buf, int size, int timeoutMsec)
{
    if( m_iSiSDeviceIO == 0 )
    {
        throw SiSDeviceException("writeData() : no impl m_iSiSDeviceIO", -1);
    }

    return m_iSiSDeviceIO->writeData(buf, size, timeoutMsec);
}

int
SiSDeviceIO::readData( unsigned char * buf, int size, int timeoutMsec)
{
    if( m_iSiSDeviceIO == 0 )
    {
        throw SiSDeviceException("readData() : no impl m_iSiSDeviceIO", -1);
    }

    return m_iSiSDeviceIO->readData(buf, size, timeoutMsec);
}

bool
SiSDeviceIO::isUsingIOCTL()
{
    if( m_iSiSDeviceIO == 0 )
    {
        throw SiSDeviceException("isUsingIOCTL() : no impl m_iSiSDeviceIO", -1);
    }

    return m_iSiSDeviceIO->isUsingIOCTL();
}

int
SiSDeviceIO::readSimpleAck(int size, int chipAddress)
{
    if( m_iSiSDeviceIO == 0 )
    {
        throw SiSDeviceException("readSimpleAck() : no impl m_iSiSDeviceIO", -1);
    }

    return m_iSiSDeviceIO->readSimpleAck(size, chipAddress);
}

int
SiSDeviceIO::readSimpleAckMaster(int size, int chipAddress)
{
    if( m_iSiSDeviceIO == 0 )
    {
        throw SiSDeviceException("readSimpleAckMaster() : no impl m_iSiSDeviceIO", -1);
    }

    return m_iSiSDeviceIO->readSimpleAckMaster(size, chipAddress);
}

int
SiSDeviceIO::readSimpleAckSlave(int size, int chipAddress)
{
    if( m_iSiSDeviceIO == 0 )
    {
        throw SiSDeviceException("readSimpleAckSlave() : no impl m_iSiSDeviceIO", -1);
    }

    return m_iSiSDeviceIO->readSimpleAckSlave(size, chipAddress);
}

int
SiSDeviceIO::getReportIdIndex(bool isOut)
{
    if( m_iSiSDeviceIO == 0 )
    {
        throw SiSDeviceException("getReportIdIndex() : no impl m_iSiSDeviceIO", -1);
    }

    return m_iSiSDeviceIO->getReportIdIndex(isOut);
}

int 
SiSDeviceIO::getReportId(int size, bool isOut)
{
    if( m_iSiSDeviceIO == 0 )
    {
        throw SiSDeviceException("getReportId() : no impl m_iSiSDeviceIO", -1);
    }

    return m_iSiSDeviceIO->getReportId(size, isOut);
}

int 
SiSDeviceIO::getBufferSizeOfReportId(int reportId)
{
    if( m_iSiSDeviceIO == 0 )
    {
        throw SiSDeviceException("getBufferSizeOfReportId() : no impl m_iSiSDeviceIO", -1);
    }

    return m_iSiSDeviceIO->getBufferSizeOfReportId(reportId);
}

int
SiSDeviceIO::makeCommonHeader(int cmd, int length, int chipAddress)
{
    if( m_iSiSDeviceIO == 0 )
    {
        throw SiSDeviceException("makeCommonHeader() : no impl m_iSiSDeviceIO", -1);
    }

    return m_iSiSDeviceIO->makeCommonHeader(cmd, length, chipAddress);
}

int 
SiSDeviceIO::makeCommonHeaderMaster(int cmd, int length, int chipAddress)
{
    if( m_iSiSDeviceIO == 0 )
    {
        throw SiSDeviceException("makeCommonHeaderMaster() : no impl m_iSiSDeviceIO", -1);
    }

    return m_iSiSDeviceIO->makeCommonHeaderMaster(cmd, length, chipAddress);
}

int
SiSDeviceIO::makeCommonHeaderSlave(int cmd, int length, int chipAddress)
{
    if( m_iSiSDeviceIO == 0 )
    {
        throw SiSDeviceException("makeCommonHeaderSlave() : no impl m_iSiSDeviceIO", -1);
    }

    return m_iSiSDeviceIO->makeCommonHeaderSlave(cmd, length, chipAddress);
}

void
SiSDeviceIO::generateOutputCRC(int chipAddress)
{
    if( m_iSiSDeviceIO == 0 )
    {
        throw SiSDeviceException("generateOutputCRC() : no impl m_iSiSDeviceIO", -1);
    }

    m_iSiSDeviceIO->generateOutputCRC(chipAddress);
}

void
SiSDeviceIO::generateOutputCRCMaster(int chipAddress)
{
    if( m_iSiSDeviceIO == 0 )
    {
        throw SiSDeviceException("generateOutputCRCMaster() : no impl m_iSiSDeviceIO", -1);
    }

    m_iSiSDeviceIO->generateOutputCRCMaster(chipAddress);
}

void
SiSDeviceIO::generateOutputCRCSlave(int chipAddress)
{
    if( m_iSiSDeviceIO == 0 )
    {
        throw SiSDeviceException("generateOutputCRCSlave() : no impl m_iSiSDeviceIO", -1);
    }

    m_iSiSDeviceIO->generateOutputCRCSlave(chipAddress);
}

int
SiSDeviceIO::makeBuffer85(int data, int chipAddress)
{
    if( m_iSiSDeviceIO == 0 )
    {
        throw SiSDeviceException("makeBuffer85() : no impl m_iSiSDeviceIO", -1);
    }

    return m_iSiSDeviceIO->makeBuffer85(data, chipAddress);
}

int
SiSDeviceIO::call85(int data, int chipAddress)
{
    if( m_iSiSDeviceIO == 0 )
    {
        throw SiSDeviceException("call85() : no impl m_iSiSDeviceIO", -1);
    }

    return m_iSiSDeviceIO->call85(data, chipAddress);
}

int
SiSDeviceIO::call85WithRetry(int data, int chipAddress)
{
    if( m_iSiSDeviceIO == 0 )
    {
        throw SiSDeviceException("call85WithRetry() : no impl m_iSiSDeviceIO", -1);
    }

    return m_iSiSDeviceIO->call85WithRetry(data, chipAddress);
}

int 
SiSDeviceIO::formatData86Unit(int size)
{
    if( m_iSiSDeviceIO == 0 )
    {
        throw SiSDeviceException("formatData86Unit() : no impl m_iSiSDeviceIO", -1);
    }

    return m_iSiSDeviceIO->formatData86Unit(size);
}

int
SiSDeviceIO::getData86StartIndex(int chipAddress)
{
    if( m_iSiSDeviceIO == 0 )
    {
        throw SiSDeviceException("getData86StartIndex() : no impl m_iSiSDeviceIO", -1);
    }

    return m_iSiSDeviceIO->getData86StartIndex(chipAddress);
}

int
SiSDeviceIO::makeBuffer86(unsigned int address, int size, int chipAddress)
{
    if( m_iSiSDeviceIO == 0 )
    {
        throw SiSDeviceException("makeBuffer86() : no impl m_iSiSDeviceIO", -1);
    }

    return m_iSiSDeviceIO->makeBuffer86(address, size, chipAddress);
}

int
SiSDeviceIO::call86(unsigned int address, unsigned char* data, int size, int chipAddress)
{
    if( m_iSiSDeviceIO == 0 )
    {
        throw SiSDeviceException("call86() : no impl m_iSiSDeviceIO", -1);
    }

    return m_iSiSDeviceIO->call86(address, data, size, chipAddress);
}

int
SiSDeviceIO::call86WithRetry(unsigned int address, unsigned char* data, int size, int chipAddress)
{
    if( m_iSiSDeviceIO == 0 )
    {
        throw SiSDeviceException("call86WithRetry() : no impl m_iSiSDeviceIO", -1);
    }

    return m_iSiSDeviceIO->call86WithRetry(address, data, size, chipAddress);
}

int
SiSDeviceIO::makeBuffer82()
{
    if( m_iSiSDeviceIO == 0 )
    {
        throw SiSDeviceException("makeBuffer82() : no impl m_iSiSDeviceIO", -1);
    }

    return m_iSiSDeviceIO->makeBuffer82();
}

int
SiSDeviceIO::call82()
{
    if( m_iSiSDeviceIO == 0 )
    {
        throw SiSDeviceException("call82() : no impl m_iSiSDeviceIO", -1);
    }

    return m_iSiSDeviceIO->call82();
}

int
SiSDeviceIO::call82WithRetry()
{
    if( m_iSiSDeviceIO == 0 )
    {
        throw SiSDeviceException("call82WithRetry() : no impl m_iSiSDeviceIO", -1);
    }

    return m_iSiSDeviceIO->call82WithRetry();
}

int
SiSDeviceIO::makeBuffer84(unsigned int pageAddress, unsigned char* data, int size, int chipAddress)
{
    if( m_iSiSDeviceIO == 0 )
    {
        throw SiSDeviceException("makeBuffer84() : no impl m_iSiSDeviceIO", -1);
    }

    return m_iSiSDeviceIO->makeBuffer84(pageAddress, data, size, chipAddress);
}

int 
SiSDeviceIO::call84(unsigned int pageAddress, unsigned char* data, int size, int chipAddress)
{
    if( m_iSiSDeviceIO == 0 )
    {
        throw SiSDeviceException("call84() : no impl m_iSiSDeviceIO", -1);
    }

    return m_iSiSDeviceIO->call84(pageAddress, data, size, chipAddress);
}

int
SiSDeviceIO::comparePageAddrAck(unsigned int pageAddress, int chipAddress)
{
    if( m_iSiSDeviceIO == 0 )
    {
        throw SiSDeviceException("comparePageAddrAck() : no impl m_iSiSDeviceIO", -1);
    }

    return m_iSiSDeviceIO->comparePageAddrAck(pageAddress, chipAddress);
}

int
SiSDeviceIO::comparePageAddrAckMaster(unsigned int pageAddress, int chipAddress)
{
    if( m_iSiSDeviceIO == 0 )
    {
        throw SiSDeviceException("comparePageAddrAckMaster() : no impl m_iSiSDeviceIO", -1);
    }

    return m_iSiSDeviceIO->comparePageAddrAckMaster(pageAddress, chipAddress);
}

int
SiSDeviceIO::comparePageAddrAckSlave(unsigned int pageAddress, int chipAddress)
{
    if( m_iSiSDeviceIO == 0 )
    {
        throw SiSDeviceException("comparePageAddrAckSlave() : no impl m_iSiSDeviceIO", -1);
    }

    return m_iSiSDeviceIO->comparePageAddrAckSlave(pageAddress, chipAddress);
}

int
SiSDeviceIO::makeBuffer81(unsigned int pageStartAddress, unsigned int pageLength, int chipAddress)
{
    if( m_iSiSDeviceIO == 0 )
    {
        throw SiSDeviceException("makeBuffer81() : no impl m_iSiSDeviceIO", -1);
    }

    return m_iSiSDeviceIO->makeBuffer81(pageStartAddress, pageLength, chipAddress);
}

int 
SiSDeviceIO::call81(unsigned int pageStartAddress, unsigned int pageLength, int chipAddress)
{
    if( m_iSiSDeviceIO == 0 )
    {
        throw SiSDeviceException("call81() : no impl m_iSiSDeviceIO", -1);
    }

    return m_iSiSDeviceIO->call81(pageStartAddress, pageLength, chipAddress);
}

int
SiSDeviceIO::makeBuffer83(unsigned int pageStartAddress, unsigned int pageLength, int chipAddress)
{
    if( m_iSiSDeviceIO == 0 )
    {
        throw SiSDeviceException("makeBuffer83() : no impl m_iSiSDeviceIO", -1);
    }

    return m_iSiSDeviceIO->makeBuffer83(pageStartAddress, pageLength, chipAddress);
}

int
SiSDeviceIO::call83(unsigned int pageStartAddress, unsigned int pageLength, int chipAddress)
{
    if( m_iSiSDeviceIO == 0 )
    {
        throw SiSDeviceException("call83() : no impl m_iSiSDeviceIO", -1);
    }

    return m_iSiSDeviceIO->call83(pageStartAddress, pageLength, chipAddress);
}

