#ifndef __OPENSHORTARGUMENT_H
#define __OPENSHORTARGUMENT_H

#include <cstdio>
#include <cstring>
#include "SiSTouchAdapter.h"
#include "Parameter.h"
#include "ApplicationParameter.h"

class OpenShortParameter : public ApplicationParameter
{
    public:
        OpenShortParameter();
        ~OpenShortParameter();
        int parse( int argc, char** argv );
        bool parseArgument(char *arg);
        int check();
        void print_usage();

    public:
        char samplefilename[MAX_FILENAME_LENGTH];
        char ouputfilename[MAX_FILENAME_LENGTH];
        char configfilename[MAX_FILENAME_LENGTH];
        bool dumpError;
        bool resetFlag;
        bool noquery;
        int time;
        /* data_m */
}; /* end of class OpenShortParameter */

#endif
