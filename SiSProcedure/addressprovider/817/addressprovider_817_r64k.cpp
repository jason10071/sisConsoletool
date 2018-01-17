#include "addressprovider_817_r64k.h"

#include "addressprovider/addresstable/addresstable_817_r64k.h"
#include "sislog.h"

using namespace SiS::Procedure;

AddressProvider_817_r64k::AddressProvider_817_r64k() :
    IAddressProvider()
{
    AddressProvider_817_r64k::createAddressTable();
}

AddressProvider_817_r64k::~AddressProvider_817_r64k()
{

}

std::string
AddressProvider_817_r64k::getTableName()
{
    return "ADDRESS_TABLE_817_R64K";
}

void
AddressProvider_817_r64k::createAddressTable()
{
    AddressProvider_817_r64k::loadAddressTableFromAddressTableString();
}

std::string
AddressProvider_817_r64k::getAddressTableString()
{
    return ADDRESS_TABLE_817_R64K;
}

void
AddressProvider_817_r64k::loadAddressTableFromAddressTableString()
{
    std::string addressTableString = AddressProvider_817_r64k::getAddressTableString();

    std::vector<AddressAttribute> addressAttributes = parseAddressTableString(addressTableString);

    for (int i = 0; i < addressAttributes.size(); i++)
    {
        insertAddressAttribute( addressAttributes[i] );
    }
}
