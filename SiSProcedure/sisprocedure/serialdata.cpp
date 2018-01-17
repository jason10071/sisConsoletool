#include "serialdata.h"
#include "spexception/spexception.h"
#include "sislog.h"
#include "spbase/sisproceduredefine.h"

#include "string.h"

using namespace SiS::Procedure;

#define EXCEPTION_TITLE "SerialData Exception : "

SerialData::SerialData(unsigned char* data, int size) :
    m_data(data),
    m_size(size),
    m_address(0x0),
    m_keyword("")
{

}

SerialData::SerialData(int size) :
    m_data(0),
    m_size(size)
{
    m_data = new unsigned char[m_size];
    memset( m_data, DEFAULT_BYTE_DATA, m_size * sizeof(unsigned char) );
}

SerialData::~SerialData()
{
    if( m_data != 0 )
    {
        delete [] m_data;
        m_data = 0;
    }
}

unsigned char*
SerialData::getData()
{
    if( m_data == 0 )
    {
        std::string msg = EXCEPTION_TITLE;
        char errorMsg[1024] = "";
        sprintf(errorMsg, "getData(), m_data == 0");
        msg.append(errorMsg);
        throw SPException( msg );
    }

    return this->m_data;
}

std::string
SerialData::getSerial()
{
    if( m_data == 0 )
    {
        std::string msg = EXCEPTION_TITLE;
        char errorMsg[1024] = "";
        sprintf(errorMsg, "getSerial(), m_data == 0");
        msg.append(errorMsg);
        throw SPException( msg );
    }

    std::string dataSerial;
    for(int i = 0; i < m_size; i++)
    {
        char bData[10] = "";

        if( i != m_size - 1 )
        {
            sprintf(bData, "%02x ", m_data[i]);
        }
        else
        {
            sprintf(bData, "%02x", m_data[i]);
        }

        dataSerial.append(bData);
    }

    return dataSerial;
}

std::string
SerialData::getString()
{
    if( m_data == 0 )
    {
        std::string msg = EXCEPTION_TITLE;
        char errorMsg[1024] = "";
        sprintf(errorMsg, "getString(), m_data == 0");
        msg.append(errorMsg);
        throw SPException( msg );
    }

    std::string dataString;
    for(int i = 0; i < m_size; i++)
    {
        char bData[10] = "";
        sprintf(bData, "%02x", m_data[i]);
        dataString.append(bData);
    }

    return dataString;
}

int
SerialData::getSize()
{
    return this->m_size;
}

unsigned int
SerialData::getAddress()
{
    return this->m_address;
}

void
SerialData::setAddress(unsigned int address)
{
    this->m_address = address;
}

std::string
SerialData::getKeyword()
{
    return this->m_keyword;
}

void
SerialData::setKeyword(std::string keyword)
{
    this->m_keyword = keyword;
}
