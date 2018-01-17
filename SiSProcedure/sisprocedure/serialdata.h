#ifndef SERIALDATA_H
#define SERIALDATA_H

#include <string>
#include "sisproceduredllexportdefine.h"

class SISPROCEDURE_EXPORT SerialData
{
public:
    explicit SerialData(unsigned char* data, int size);
    explicit SerialData(int size);
    virtual ~SerialData();

    unsigned char* getData();
    std::string getSerial();
    std::string getString();
    int getSize();

    unsigned int getAddress();
    void setAddress(unsigned int address);

    std::string getKeyword();
    void setKeyword(std::string keyword);

private:
    unsigned char* m_data;
    int m_size;
    unsigned int m_address;
    std::string m_keyword;
};

#endif // SERIALDATA_H
