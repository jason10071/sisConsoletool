#ifndef SISBINMANAGER_H
#define SISBINMANAGER_H

#include "stdio.h"
#include <string>
#include <string.h>
#include <map>

#include <vector>

namespace SE
{

typedef int BinIndex;
typedef std::string BinName;

class SiSBinManager
{
public:
    explicit SiSBinManager();
    virtual ~SiSBinManager();

    static std::map<BinIndex, BinName> getBinMap(std::string binDir);

private:
    static std::map<BinIndex, BinName> sortBinMapByName(std::map<BinIndex, BinName> oldBinMap);

    static std::string intToString(int a);
    static bool endsWith(std::string s, std::string suffix);
    static std::string replaceAll(std::string str, const std::string from, const std::string to);
};

} // SE

#endif // SISBINMANAGER_H
