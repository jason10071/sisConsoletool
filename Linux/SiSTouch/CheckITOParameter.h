#ifndef __CHECKITOPARAMETER_H__
#define __CHECKITOPARAMETER_H__

#include "Parameter.h"
#include "ApplicationParameter.h"

class CheckITOParameter : public ApplicationParameter {
    public:
        // default constructor
        CheckITOParameter ();
        // destructor
        ~CheckITOParameter ();

        int parse(int argc, char** argv);
        bool parseArgument(char *arg);
        int check();
        void print_usage();

}; /* end of class CheckITOParameter  */

#endif /* end of __CHECKITOPARAMETER_H__ */
