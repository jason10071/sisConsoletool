#ifndef __TOUCHFINDER_ARGUMENT_H
#define __TOUCHFINDER_ARGUMENT_H


#include "Parameter.h"
#include "ApplicationParameter.h"


class TouchFinderParameter : public ApplicationParameter
{
    public:
        // default constructor
        TouchFinderParameter();
        // destructor
        ~TouchFinderParameter();

        int parse(int argc, char** argv);
        bool parseArgument(char *arg);
        int check();
        void print_usage();

    public:
        char checkDevName[MAX_DEVNAME_LENGTH];
}; /* end of class TouchFinderParameter */

#endif
