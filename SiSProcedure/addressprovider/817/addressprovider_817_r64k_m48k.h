#ifndef ADDRESSPROVIDER_817_R64K_M48K_H
#define ADDRESSPROVIDER_817_R64K_M48K_H

#include "addressprovider/817/addressprovider_817_r64k.h"

namespace SiS
{
namespace Procedure
{

class AddressProvider_817_r64k_m48k : public AddressProvider_817_r64k
{
public:
    explicit AddressProvider_817_r64k_m48k();
    virtual ~AddressProvider_817_r64k_m48k();

    virtual std::string getTableName();

protected:
    virtual void createAddressTable();
    virtual std::string getAddressTableString();
    virtual void loadAddressTableFromAddressTableString();
};

} // Procedure
} // SiS

#endif // ADDRESSPROVIDER_817_R64K_M48K_H
