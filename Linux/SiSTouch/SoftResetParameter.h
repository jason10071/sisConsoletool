#ifndef __SOFTRESETPARAMETER_H__
#define __SOFTRESETPARAMETER_H__

#include "Parameter.h"
#include "ApplicationParameter.h"

class SoftResetParameter : public ApplicationParameter {
    public:
        // default constructor
        SoftResetParameter();
        // destructor
        ~SoftResetParameter();

        int parse(int argc, char** argv);
        bool parseArgument(char *arg);
        int check();
        void print_usage();

        void showFileNames();

}; /* end of class SoftResetParameter */

#endif /* end of __SOFTRESETPARAMETER_H__ */
