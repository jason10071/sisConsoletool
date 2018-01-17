#include "sebase.h"

#define SISLOG_BEGIN ":~$ "

SeBase::SeBase()
{

}

SeBase::~SeBase()
{

}

std::string
SeBase::removeSiSLogBegin(std::string line)
{
    if ( line.find(SISLOG_BEGIN) != std::string::npos )
    {
        line.replace(line.begin(), line.begin() + line.find(SISLOG_BEGIN) + 4, "");
    }

    return line;
}
