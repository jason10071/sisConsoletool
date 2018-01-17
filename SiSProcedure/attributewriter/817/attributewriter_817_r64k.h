#ifndef ATTRIBUTEWRITER_817_R64K_H
#define ATTRIBUTEWRITER_817_R64K_H

#include "attributewriter/iattributewriter.h"

namespace SiS
{
namespace Procedure
{

class AttributeWriter_817_r64k : public IAttributeWriter
{
public:
    explicit AttributeWriter_817_r64k(SiSDeviceIO* m_sisDeviceIO);
    explicit AttributeWriter_817_r64k(unsigned char* binBuf, int binSize);
    virtual ~AttributeWriter_817_r64k();

protected:
    virtual IAddressProvider* getAddressProvider();
    virtual std::string getAddressProviderTableName();
    virtual int mappingChipAddress(int chipIndex);
    virtual void writeAddressFromSiSCore(unsigned int address, unsigned char* dataBuf, int dataSize);
};

} // Procedure
} // SiS

#endif // ATTRIBUTEWRITER_817_R64K_H
