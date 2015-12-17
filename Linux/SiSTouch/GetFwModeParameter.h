#ifndef __GETFWMODEPARAMETER_H__
#define __GETFWMODEPARAMETER_H__

#include "Parameter.h"
#include "ApplicationParameter.h"

class GetFwModeParameter : public ApplicationParameter {
    public:
        // default constructor
		GetFwModeParameter();
        // destructor
        ~GetFwModeParameter();

        int parse(int argc, char** argv);
        bool parseArgument(char *arg);
        int check();
        void print_usage();

    public:
        bool isDumpLog;
        char filename[MAX_FILENAME_LENGTH];
}; /* end of class GetFwModeParameter */

#endif /* end of __GETFWMODEPARAMETER_H__ */
