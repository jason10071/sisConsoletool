#ifndef __CALIBRATIONWITHOPENSHORTARGUMENT_H
#define __CALIBRATIONWITHOPENSHORTARGUMENT_H


#include "Parameter.h"
#include "ApplicationParameter.h"
#include <cstdio>
#include <cstring>

class CalibrationWithOpenShortParameter : public ApplicationParameter
{
    public:
        // default constructor
        CalibrationWithOpenShortParameter();
        // destructor
        ~CalibrationWithOpenShortParameter();
        int parse(int argc, char ** argv);
        bool parseArgument(char *arg);
        int check();
        void print_usage();

    public:
    bool noquery;    
    int time;
    bool dumpError;
    char ouputfilename[MAX_FILENAME_LENGTH];
	char ouputfilename2[MAX_FILENAME_LENGTH];
    char configfilename[MAX_FILENAME_LENGTH];
}; /* end of class CalibrationWithOpenShortParameter */

#endif
