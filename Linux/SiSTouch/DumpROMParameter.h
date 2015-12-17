#ifndef __DUMPDATAPARAMETER_H__
#define __DUMPDATAPARAMETER_H__

#include "Parameter.h"
#include "ApplicationParameter.h"

class DumpROMParameter : public ApplicationParameter {
    public:
        // default constructor
        DumpROMParameter();
        // destructor
        ~DumpROMParameter();

        int parse(int argc, char** argv);
        bool parseArgument(char *arg);
        int check();
        void print_usage();

        void showFileNames();

    public:
        char filename[MAX_FILENAME_LENGTH];
        char slave_filenames[MAX_SLAVE_NUM][MAX_FILENAME_LENGTH];

}; /* end of class DumpROMParameter */

#endif /* end of __DUMPDATAPARAMETER_H__ */
