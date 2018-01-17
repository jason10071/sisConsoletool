#ifndef IATTRIBUTEREADER_H
#define IATTRIBUTEREADER_H

#include "stdio.h"
#include <string>
#include <string.h>

#include "addressprovider/iaddressprovider.h"
#include "sisdeviceio/sisdeviceio.h"

using namespace SiS::DeviceIO;

namespace SiS
{
namespace Procedure
{

class IAttributeReader
{
public:
    enum AttributeSource
    {
        AS_SISCORE = 0,
        AS_BIN,
    };

    explicit IAttributeReader(SiSDeviceIO* sisDeviceIO);
    explicit IAttributeReader(unsigned char* binBuf, int binSize);
    virtual ~IAttributeReader();

    void setCtlReportToOs(bool ctlReportToOsFlag);
    void setChipIndex(int chipIndex);

    void readAttribute(std::string key, unsigned char* &dataBuf, int &dataSize);
    void readAddress(unsigned int address, unsigned char* &dataBuf, int dataSize);

    static void checkDataSize(int expectSize, int realSize);
    static void milliSleep(int msec);

    virtual IAddressProvider* getAddressProvider() = 0;
    virtual std::string getAddressProviderTableName() = 0;
    virtual int mappingChipAddress(int chipIndex) = 0;

protected:
    virtual void readAddressFromSiSCore(unsigned int address, unsigned char* dataBuf, int dataSize) = 0;
    void readAddressFromBin(unsigned int address, unsigned char* dataBuf, int dataSize);

protected:
    AttributeSource m_attributeSource;
    SiSDeviceIO* m_sisDeviceIO;
    bool m_ctlReportToOsFlag;
    unsigned char* m_binBuf;
    int m_binSize;
    IAddressProvider* m_addressProvider;
    int m_chipIndex;
};

} // Procedure
} // SiS

#endif // IATTRIBUTEREADER_H
