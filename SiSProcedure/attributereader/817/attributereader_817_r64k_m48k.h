#ifndef ATTRIBUTEREADER_817_R64K_M48K_H
#define ATTRIBUTEREADER_817_R64K_M48K_H

#include "attributereader/817/attributereader_817_r64k.h"

namespace SiS
{
namespace Procedure
{

class AttributeReader_817_r64k_m48k : public AttributeReader_817_r64k
{
public:  
    explicit AttributeReader_817_r64k_m48k(SiSDeviceIO* sisDeviceIO);
    explicit AttributeReader_817_r64k_m48k(unsigned char* binBuf, int binSize);
    virtual ~AttributeReader_817_r64k_m48k();

    virtual IAddressProvider* getAddressProvider();
    virtual std::string getAddressProviderTableName();
};

} // Procedure
} // SiS

#endif // ATTRIBUTEREADER_817_R64K_M48K_H
