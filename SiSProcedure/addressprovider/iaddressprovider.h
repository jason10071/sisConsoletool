#ifndef IADDRESSPROVIDER_H
#define IADDRESSPROVIDER_H

#include "map"
#include "vector"
#include "addressprovider/addressattribute.h"
#include "sisproceduredllexportdefine.h"

namespace SiS
{
namespace Procedure
{

class SISPROCEDURE_EXPORT IAddressProvider
{
public:
    explicit IAddressProvider();
    virtual ~IAddressProvider();

    unsigned int getAddress(std::string key);
    int getLength(std::string key);

    virtual std::string getTableName() = 0;
    void showAddressTable();
    void showAddressTable(AddressAttribute::AddressAttributeCategory category);
    std::vector<std::string> getKeys(AddressAttribute::AddressAttributeCategory category = AddressAttribute::AAC_ALL);

protected:
    virtual void createAddressTable() = 0;
    virtual std::string getAddressTableString() = 0;
    virtual void loadAddressTableFromAddressTableString() = 0;
    std::vector<AddressAttribute> parseAddressTableString(std::string addressTableString);
    void insertAddressAttribute(AddressAttribute addressAttribute);

protected:
    std::map<std::string, AddressAttribute> m_addressTable;
};

} // Procedure
} // SiS

#endif // IADDRESSPROVIDER_H
