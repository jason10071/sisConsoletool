#ifndef ATTRIBUTEREADER_817_R64K_H
#define ATTRIBUTEREADER_817_R64K_H

#include "attributereader/iattributereader.h"

namespace SiS
{
namespace Procedure
{

class AttributeReader_817_r64k : public IAttributeReader
{
public:
    explicit AttributeReader_817_r64k(SiSDeviceIO* sisDeviceIO);
    explicit AttributeReader_817_r64k(unsigned char* binBuf, int binSize);
    virtual ~AttributeReader_817_r64k();

    virtual IAddressProvider* getAddressProvider();
    virtual std::string getAddressProviderTableName();
    virtual int mappingChipAddress(int chipIndex);

protected:
    virtual void readAddressFromSiSCore(unsigned int address, unsigned char* dataBuf, int dataSize);
};

} // Procedure
} // SiS

#endif // ATTRIBUTEREADER_817_R64K_H
