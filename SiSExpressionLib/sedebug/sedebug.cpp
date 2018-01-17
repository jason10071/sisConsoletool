#include "sedebug.h"

using namespace SE;

bool SEDebug::m_debug = false;

SEDebug::SEDebug()
{

}

SEDebug::~SEDebug()
{

}

void
SEDebug::setDebug(bool debug)
{
    m_debug = debug;
}

void
SEDebug::printLog(string log)
{
    if(m_debug)
    {
        cout << "[SEDebug] " << log << endl;
    }
}
