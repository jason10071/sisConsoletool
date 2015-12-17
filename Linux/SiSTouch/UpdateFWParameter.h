#ifndef __UPDATEFWARGUMENT_H
#define __UPDATEFWARGUMENT_H

#include "Parameter.h"
#include "ApplicationParameter.h"
#include <cstring>
#include <string>
#include <vector>

/*===========================================================================*/
class UpdateFWParameter : public ApplicationParameter
{
    public:
        UpdateFWParameter();
        ~UpdateFWParameter();
        int parse( int argc, char** argv );
        bool parseArgument(char *arg);
        int check();
        void print_usage();
    public:
        std::vector<std::string> filenames;
        bool update_bootloader;
        bool update_bootloader_auto;
        bool reserve_RODATA;
        bool update_parameter;
        bool force_update;
        bool jump_check;
        int  wait_time;
}; /* end of class UpdateFWParameter */
/*===========================================================================*/

#endif
