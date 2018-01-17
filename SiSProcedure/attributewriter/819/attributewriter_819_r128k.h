#ifndef ATTRIBUTEWRITER_819_R128K_H
#define ATTRIBUTEWRITER_819_R128K_H

#include "attributewriter/817/attributewriter_817_r64k.h"

namespace SiS
{
namespace Procedure
{

class AttributeWriter_819_r128k : public AttributeWriter_817_r64k
{
public:
    explicit AttributeWriter_819_r128k(SiSDeviceIO* m_sisDeviceIO);
    explicit AttributeWriter_819_r128k(unsigned char* binBuf, int binSize);
    virtual ~AttributeWriter_819_r128k();

protected:
    virtual IAddressProvider* getAddressProvider();
    virtual std::string getAddressProviderTableName();
    virtual int mappingChipAddress(int chipIndex);
};

} // Procedure
} // SiS

#endif // ATTRIBUTEWRITER_819_R128K_H
