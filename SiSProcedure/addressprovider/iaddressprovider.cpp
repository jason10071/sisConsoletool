#include "iaddressprovider.h"

#include "sislog.h"
#include "spexception/spexception.h"
#include <sstream>
#include <iostream>
#include <algorithm>
#include "stdio.h"

using namespace SiS::Procedure;

#define EXCEPTION_TITLE "IAddressProvider Exception : "
#define TAG "IAddressProvider"

#define AAC_FW_MEMORY_LAYOUT_STR        "AAC_FW_MEMORY_LAYOUT"
#define AAC_FW_ATTRIBUTE_STR            "AAC_FW_ATTRIBUTE"
#define AAC_UNDEFINED_STR               "AAC_UNDEFINED"
#define AAC_FW_MEMORY_LAYOUT_KEYWORD    "FML"
#define AAC_FW_ATTRIBUTE_KEYWORD        "FA"

IAddressProvider::IAddressProvider()
{

}

IAddressProvider::~IAddressProvider()
{

}

unsigned int
IAddressProvider::getAddress(std::string key)
{
    std::map<std::string, AddressAttribute>::iterator it = m_addressTable.find( key );

    if( it != m_addressTable.end() )
    {
        AddressAttribute addressAttribute = it->second;
        return addressAttribute.getAddress();
    }
    else
    {
        std::string msg = EXCEPTION_TITLE;
        msg.append("getAddress(key). Can't find key \"");
        msg.append(key);
        msg.append("\" !");
        throw SPException( msg );
    }
}

int
IAddressProvider::getLength(std::string key)
{
    std::map<std::string, AddressAttribute>::iterator it = m_addressTable.find( key );

    if( it != m_addressTable.end() )
    {
        AddressAttribute addressAttribute = it->second;
        return addressAttribute.getLength();
    }
    else
    {
        std::string msg = EXCEPTION_TITLE;
        msg.append("getLength(key). Can't find key \"");
        msg.append(key);
        msg.append("\" !");
        throw SPException( msg );
    }
}

void
IAddressProvider::showAddressTable()
{
    showAddressTable( AddressAttribute::AAC_FW_MEMORY_LAYOUT );
    showAddressTable( AddressAttribute::AAC_FW_ATTRIBUTE );
    showAddressTable( AddressAttribute::AAC_UNDEFINED );
}

void
IAddressProvider::showAddressTable(AddressAttribute::AddressAttributeCategory category)
{
    /* category */
    const char* categoryStr = AAC_UNDEFINED_STR;
    if( category == AddressAttribute::AAC_ALL)
    {
        showAddressTable();
        return;
    }
    else if( category == AddressAttribute::AAC_FW_MEMORY_LAYOUT )
    {
        categoryStr = AAC_FW_MEMORY_LAYOUT_STR;
    }
    else if( category == AddressAttribute::AAC_FW_ATTRIBUTE )
    {
        categoryStr = AAC_FW_ATTRIBUTE_STR;
    }
    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "/%s/", categoryStr );

    /* keys */
    std::vector<std::string> keys = getKeys(category);

    for (int i = 0; i < keys.size(); i++)
    {
        AddressAttribute addressAttribute = m_addressTable.find( keys[i] )->second;

        /* length */
        int length = addressAttribute.getLength();
        bool isKByte = false;
        const char* unit = "";
        if( length >= 1024 && length % 1024 == 0 )
        {
            isKByte = true;
            unit = "k";
            length = length / 1024;
        }

        /* show */
        SIS_LOG_I(SiSLog::getOwnerSiS(), TAG,
                  "%s : 0x%08x (%d%s)",
                  addressAttribute.getKey().c_str(),
                  addressAttribute.getAddress(),
                  length,
                  unit );
    }
}

std::vector<std::string>
IAddressProvider::getKeys(AddressAttribute::AddressAttributeCategory category)
{
    std::vector<std::string> keys;

    for (std::map<std::string, AddressAttribute>::iterator it = m_addressTable.begin(); it != m_addressTable.end(); ++it)
    {
        AddressAttribute addressAttribute = it->second;

        if( category == AddressAttribute::AAC_ALL || category == addressAttribute.getCategory())
        {
            keys.push_back( addressAttribute.getKey() );
        }
    }

    return keys;
}

bool myfunction (char lhs, char rhs)
{
  return (lhs == rhs) && (lhs == ' ');
}

std::vector<AddressAttribute>
IAddressProvider::parseAddressTableString(std::string addressTableString)
{
    std::vector<AddressAttribute> addressAttributes;

    std::istringstream f(addressTableString);
    std::string addressAttributeLine;
    while(std::getline(f, addressAttributeLine, '\n'))
    {
        /* parse line of addressAttribute */

        /* replace '\t' to ' ' */
        std::replace( addressAttributeLine.begin(), addressAttributeLine.end(), '\t', ' ');

        /* combine multi ' ' to ' ' */
        std::string::iterator new_end =
                std::unique( addressAttributeLine.begin(), addressAttributeLine.end(),
                             myfunction );
        addressAttributeLine.erase(new_end, addressAttributeLine.end());

        /* erase first space */
        if( addressAttributeLine.find(' ') == 0 )
        {
            addressAttributeLine.erase( addressAttributeLine.find(' '), 1 );
        }

        /* comment */
        if( addressAttributeLine.find('#') == 0 )
        {
            continue;
        }
        //SiSDebug::LOG_D("addressAttribute line: %s\n", addressAttributeLine.c_str());

        /* token */
        std::istringstream ff(addressAttributeLine);
        std::vector<std::string> addressAttributeTokens;
        std::string addressAttributeToken;
        while(std::getline(ff, addressAttributeToken, ' '))
        {
            addressAttributeTokens.push_back( addressAttributeToken );
        }

        /* assign to an AddressAttribute */
        if( addressAttributeTokens.size() >= 3 )
        {
            /* key */
            std::string key = addressAttributeTokens[0];

            /* address */
            std::string addressStr = addressAttributeTokens[1];
            unsigned int address = strtoul(addressStr.c_str(), NULL, 16);

            /* length */
            std::string lengthStr = addressAttributeTokens[2];
            bool isKByte = false;
            if( lengthStr.find('k') != std::string::npos || lengthStr.find('K') != std::string::npos )
            {
                isKByte = true;
                lengthStr.erase( std::remove(lengthStr.begin(), lengthStr.end(), 'k'), lengthStr.end() );
                lengthStr.erase( std::remove(lengthStr.begin(), lengthStr.end(), 'K'), lengthStr.end() );
            }
            int length = atoi(lengthStr.c_str());
            if(isKByte)
            {
                length = length * 1024;
            }

            /* category */
            AddressAttribute::AddressAttributeCategory category = AddressAttribute::AAC_UNDEFINED;
            if( addressAttributeLine.find( AAC_FW_MEMORY_LAYOUT_KEYWORD ) == 0 )
            {
                category = AddressAttribute::AAC_FW_MEMORY_LAYOUT;
            }
            else if( addressAttributeLine.find( AAC_FW_ATTRIBUTE_KEYWORD ) == 0 )
            {
                category = AddressAttribute::AAC_FW_ATTRIBUTE;
            }

            /* assign */
            addressAttributes.push_back( AddressAttribute(key, address, length, category) );
        }
    }

    return addressAttributes;
}

void
IAddressProvider::insertAddressAttribute(AddressAttribute addressAttribute)
{
    std::string key = addressAttribute.getKey();

    /* erase key if already exist */
    if( m_addressTable.find( key ) != m_addressTable.end() )
    {
        m_addressTable.erase( m_addressTable.find( key ) );
    }

    /* insert key */
    this->m_addressTable.insert( std::pair<std::string, AddressAttribute>(key, addressAttribute) );
}
