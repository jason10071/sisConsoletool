#ifndef IATTRIBUTEWRITER_H
#define IATTRIBUTEWRITER_H

#include "attributereader/iattributereader.h"

namespace SiS
{
namespace Procedure
{

class IAttributeWriter
{
public:
    explicit IAttributeWriter(SiSDeviceIO* sisDeviceIO);
    explicit IAttributeWriter(unsigned char* binBuf, int binSize);
    virtual ~IAttributeWriter();

    void setCtlReportToOs(bool ctlReportToOsFlag);
    void setChipIndex(int chipIndex);

    void writeAttribute(std::string key, unsigned char* dataBuf, int dataSize);
    void writeAddress(unsigned int address, unsigned char* dataBuf, int dataSize);

protected:
    virtual IAddressProvider* getAddressProvider() = 0;
    virtual std::string getAddressProviderTableName() = 0;
    virtual int mappingChipAddress(int chipIndex) = 0;
    virtual void writeAddressFromSiSCore(unsigned int address, unsigned char* dataBuf, int dataSize) = 0;
    void writeAddressFromBin(unsigned int address, unsigned char* dataBuf, int dataSize);

protected:
    IAttributeReader::AttributeSource m_attributeSource;
    SiSDeviceIO* m_sisDeviceIO;
    bool m_ctlReportToOsFlag;
    unsigned char* m_binBuf;
    int m_binSize;
    IAddressProvider* m_addressProvider;
    int m_chipIndex;
};

} // Procedure
} // SiS

#endif // IATTRIBUTEWRITER_H
