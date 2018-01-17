#ifndef ADDRESSPROVIDER_819_R128K_H
#define ADDRESSPROVIDER_819_R128K_H

#include "addressprovider/817/addressprovider_817_r64k.h"

namespace SiS
{
namespace Procedure
{

class AddressProvider_819_r128k : public AddressProvider_817_r64k
{
public:
    AddressProvider_819_r128k();
    ~AddressProvider_819_r128k();

    virtual std::string getTableName();

protected:
    virtual void createAddressTable();
    virtual std::string getAddressTableString();
    virtual void loadAddressTableFromAddressTableString();
};

} // Procedure
} // SiS

#endif // ADDRESSPROVIDER_819_R128K_H
