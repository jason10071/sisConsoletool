#ifndef SISIOATTRIBUTE_H
#define SISIOATTRIBUTE_H

#include "stdio.h"
#include "string"

/* retry */
#define CMD_85_RETRY_MAX 5
#define CMD_86_RETRY_MAX 5
#define CMD_82_RETRY_MAX 5
#define CMD_83_RETRY_MAX 5

/* delay */
#define CMD_RETRY_DELAY 100				// ms
#define CHANGE_85_MODE_DELAY 100		// ms

class SiSIOAttribute
{
public:
    SiSIOAttribute();
    ~SiSIOAttribute();

	int getIODelay();
    void setIODelay(int ioDelay);

    int getIODelayOf81();
    void setIODelayOf81(int ioDelayOf81);

    int getBlockRetry();
    void setBlockRetry(int bolckRetry);

private:
    int m_ioDelay; 		// ms	
    int m_ioDelayOf81; 	// ms	
    int m_bolckRetry;

};

#endif // SISIOATTRIBUTE_H
