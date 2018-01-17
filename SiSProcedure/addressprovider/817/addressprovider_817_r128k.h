#ifndef ADDRESSPROVIDER_817_R128K_H
#define ADDRESSPROVIDER_817_R128K_H

#include "addressprovider/817/addressprovider_817_r64k.h"

namespace SiS
{
namespace Procedure
{

class AddressProvider_817_r128k : public AddressProvider_817_r64k
{
public:
    explicit AddressProvider_817_r128k();
    virtual ~AddressProvider_817_r128k();

    virtual std::string getTableName();

protected:
    virtual void createAddressTable();
    virtual std::string getAddressTableString();
    virtual void loadAddressTableFromAddressTableString();
};

} // Procedure
} // SiS

#endif // ADDRESSPROVIDER_817_R128K_H
