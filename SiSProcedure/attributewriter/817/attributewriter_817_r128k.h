#ifndef ATTRIBUTEWRITER_817_R128K_H
#define ATTRIBUTEWRITER_817_R128K_H

#include "attributewriter/817/attributewriter_817_r64k.h"

namespace SiS
{
namespace Procedure
{

class AttributeWriter_817_r128k : public AttributeWriter_817_r64k
{
public:
    explicit AttributeWriter_817_r128k(SiSDeviceIO* m_sisDeviceIO);
    explicit AttributeWriter_817_r128k(unsigned char* binBuf, int binSize);
    virtual ~AttributeWriter_817_r128k();

protected:
    virtual IAddressProvider* getAddressProvider();
    virtual std::string getAddressProviderTableName();
};

} // Procedure
} // SiS

#endif // ATTRIBUTEWRITER_817_R128K_H
