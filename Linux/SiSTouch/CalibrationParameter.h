#ifndef __CALIBRATIONARGUMENT_H
#define __CALIBRATIONARGUMENT_H

#include "Parameter.h"
#include "ApplicationParameter.h"
#include <cstring>

class CalibrationParameter : public ApplicationParameter
{
    public:
        CalibrationParameter();
        ~CalibrationParameter();
        int parse( int argc, char** argv );
        bool parseArgument(char *arg);
        int check();
        void print_usage();

    public:
    bool noquery;
    bool checkFlag;
	bool eraseFlag;
    int time;
}; /* end of class CalibrationParameter */

#endif
