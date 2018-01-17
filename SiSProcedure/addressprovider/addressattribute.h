#ifndef ADDRESSATTRIBUTE_H
#define ADDRESSATTRIBUTE_H

#include "stdio.h"
#include "string"

namespace SiS
{
namespace Procedure
{

class AddressAttribute
{
public:
    enum AddressAttributeCategory
    {
        AAC_UNDEFINED = 0,
        AAC_FW_MEMORY_LAYOUT,
        AAC_FW_ATTRIBUTE,
        AAC_ALL
    };

    AddressAttribute(std::string key, unsigned int address, int length, AddressAttributeCategory category);
    ~AddressAttribute();

    std::string getKey();
    unsigned int getAddress();
    int getLength();
    AddressAttributeCategory getCategory();

private:
    std::string m_key;
    unsigned int m_address;
    int m_length;
    AddressAttributeCategory m_category;
};

} // Procedure
} // SiS

#endif // ADDRESSATTRIBUTE_H
