#ifndef BINWRAP_H
#define BINWRAP_H

#include <string>
#include "sisproceduredllexportdefine.h"

namespace SiS
{
namespace Procedure
{

class SISPROCEDURE_EXPORT BinWrap
{
public:
    explicit BinWrap(std::string name, int chipIndex);
    virtual ~BinWrap();

    void loadBin();

    std::string getName();
    int getChipIndex();
    unsigned char* getBuf();
    int getSize();

    void setBuf(unsigned char* buf);
    void setSize(int size);

private:
    std::string m_name;
    int m_chipIndex;
    unsigned char* m_buf;
    int m_size;

};

} // Procedure
} // SiS

#endif // BINWRAP_H
