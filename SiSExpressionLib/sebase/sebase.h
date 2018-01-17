#ifndef SEBASE_H
#define SEBASE_H

#include "stdio.h"
#include <string>

using namespace std;

class SeBase
{
public:
    SeBase();
    ~SeBase();

    static std::string removeSiSLogBegin(std::string line);
};

#endif // SEBASE_H
