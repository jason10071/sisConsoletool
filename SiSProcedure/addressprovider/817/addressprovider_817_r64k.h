#ifndef ADDRESSPROVIDER_817_R64K_H
#define ADDRESSPROVIDER_817_R64K_H

#include "addressprovider/iaddressprovider.h"

namespace SiS
{
namespace Procedure
{

class AddressProvider_817_r64k : public IAddressProvider
{
public:
    explicit AddressProvider_817_r64k();
    virtual ~AddressProvider_817_r64k();

    virtual std::string getTableName();

protected:
    virtual void createAddressTable();
    virtual std::string getAddressTableString();
    virtual void loadAddressTableFromAddressTableString();
};

} // Procedure
} // SiS

#endif // ADDRESSPROVIDER_817_R64K_H
