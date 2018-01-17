#include "addressattribute.h"

using namespace SiS::Procedure;

AddressAttribute::AddressAttribute(std::string key, unsigned int address, int length, AddressAttributeCategory category) :
    m_key(key),
    m_address(address),
    m_length(length),
    m_category(category)
{

}

AddressAttribute::~AddressAttribute()
{

}

std::string
AddressAttribute::getKey()
{
    return this->m_key;
}

unsigned int
AddressAttribute::getAddress()
{
    return this->m_address;
}

int
AddressAttribute::getLength()
{
    return this->m_length;
}

AddressAttribute::AddressAttributeCategory
AddressAttribute::getCategory()
{
    return this->m_category;
}
