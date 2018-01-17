#include "sisioattribute.h"

/* retry */
#define BLOCK_RETRY_MAX 5

/* delay */
#define IO_DELAY 0			// ms
#define IO_DELAY_OF_81 0	// ms

SiSIOAttribute::SiSIOAttribute() :
    m_ioDelay(IO_DELAY),
    m_ioDelayOf81(IO_DELAY_OF_81),
    m_bolckRetry(BLOCK_RETRY_MAX)
{

}

SiSIOAttribute::~SiSIOAttribute()
{

}

int 
SiSIOAttribute::getIODelay()
{
    return m_ioDelay;
}

void 
SiSIOAttribute::setIODelay(int ioDelay)
{
    m_ioDelay = ioDelay;
}

int 
SiSIOAttribute::getIODelayOf81()
{
    return m_ioDelayOf81;
}

void 
SiSIOAttribute::setIODelayOf81(int ioDelayOf81)
{
    m_ioDelayOf81 = ioDelayOf81;
}

int 
SiSIOAttribute::getBlockRetry()
{
    return m_bolckRetry;
}

void 
SiSIOAttribute::setBlockRetry(int bolckRetry)
{
    m_bolckRetry = bolckRetry;
}

