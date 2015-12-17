#ifndef __SHOWIDARGUMENT_H
#define __SHOWIDARGUMENT_H


#include "Parameter.h"
#include "ApplicationParameter.h"


class GetFirmwareIdParameter : public ApplicationParameter
{
    public:
        // default constructor
        GetFirmwareIdParameter();
        // destructor
        ~GetFirmwareIdParameter();

        int parse(int argc, char** argv);
        bool parseArgument(char *arg);
        int check();
        void print_usage();

    public:
        bool fromFile;
        bool fromInput;
        char filename[MAX_FILENAME_LENGTH];
        char slave_filenames[MAX_SLAVE_NUM][MAX_FILENAME_LENGTH];
        char input[MAX_FILENAME_LENGTH];
        char slave_input[MAX_SLAVE_NUM][MAX_FILENAME_LENGTH];
}; /* end of class GetFirmwareIdParameter */

#endif
