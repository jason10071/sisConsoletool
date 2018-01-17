#ifndef SEDEBUG_H
#define SEDEBUG_H

#include <iostream>
#include <string>

using namespace std;

namespace SE
{

class SEDebug
{
public:
    SEDebug();
    ~SEDebug();

    static void setDebug(bool debug);
    static void printLog(string log);

private:
    static bool m_debug;
};

} // SE

#endif // SEDEBUG_H
