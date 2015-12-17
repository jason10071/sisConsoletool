#ifndef __ISBROKENFWARGUMENT_H
#define __ISBROKENFWARGUMENT_H

#include "Parameter.h"
#include "ApplicationParameter.h"
#include <cstring>

class OKFWParameter : public ApplicationParameter
{
    public:
        // default constructor
        OKFWParameter();
        // destructor
        ~OKFWParameter();
        int parse( int argc, char** argv );
        bool parseArgument(char *arg);
        int check();
        void print_usage();

    private:
        /* data_m */
}; /* end of class isBrokenFWParameter */

#endif
