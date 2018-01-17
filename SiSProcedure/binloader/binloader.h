#ifndef BINLOADER_H
#define BINLOADER_H

#include "stdio.h"
#include <string>

namespace SiS
{
namespace Procedure
{

class BinLoader
{
public:
    BinLoader();
    ~BinLoader();

    void read(std::string binFileName, unsigned char* &binBuf, int &binSize);

private:
    FILE* openFile(std::string fileName, int &size);
    int readFileToBuffer(FILE* file, long offset, unsigned char* buf, int size);
};

} // Procedure
} // SiS

#endif // BINLOADER_H
