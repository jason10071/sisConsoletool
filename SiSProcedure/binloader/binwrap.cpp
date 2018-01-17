#include "binwrap.h"

#include "binloader/binloader.h"
#include "spbase/sisproceduredefine.h"
#include "sislog.h"
#include "sisprocedure/isisprocedure.h"

using namespace SiS::Procedure;

#define TAG "BinWrap"

BinWrap::BinWrap(std::string name, int chipIndex) :
    m_name(name),
    m_chipIndex(chipIndex),
    m_buf(0),
    m_size(0)
{

}

BinWrap::~BinWrap()
{
    if( m_buf != 0 )
    {
        delete [] m_buf;
        m_buf = 0;
    }
}

void
BinWrap::loadBin()
{
    if( m_buf != 0 )
    {
        delete [] m_buf;
        m_buf = 0;
    }   

    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "load %s-bin", ISiSProcedure::getCIStr(m_chipIndex).c_str() );

    BinLoader binLoader;
    binLoader.read( m_name, m_buf, m_size );
}

std::string
BinWrap::getName()
{
    return this->m_name;
}

int
BinWrap::getChipIndex()
{
    return this->m_chipIndex;
}

unsigned char*
BinWrap::getBuf()
{
    return this->m_buf;
}

int
BinWrap::getSize()
{
    return this->m_size;
}

void
BinWrap::setBuf(unsigned char* buf)
{
    this->m_buf = buf;
}

void
BinWrap::setSize(int size)
{
    this->m_size = size;
}
