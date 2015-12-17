#ifndef __APPLICATIONPARAMETER_H__
#define __APPLICATIONPARAMETER_H__

#include "Parameter.h"

/*===========================================================================*/
class ApplicationParameter
{
    public:
        enum ApplicationType {
            CALIBRATION,
            CALIBRATION_WITH_OPENSHORT,
            OK_FW,
            OPEN_SHORT,
            GET_FW_ID,
            UPDATE_FW,
            DUMP_ROM,
            SOFT_RESET,
            GET_FW_MODE,
            CHECK_ITO
        };
        enum {
            DEFAULT_CALIBRATION_TIME = 5
        };
    public:
        ApplicationParameter() : conParameter(Parameter()) {}
        virtual ~ApplicationParameter() {}
    public:
        static ApplicationParameter * create(ApplicationType type);
    public:
        virtual int parse(int argc, char ** argv) = 0;
        virtual bool parseArgument(char *arg) = 0;
        virtual int check() = 0;
        virtual void print_usage() = 0;

    public:
        Parameter conParameter;
}; /* end of class ApplicationParameter */
/*===========================================================================*/
#endif /* end of __APPLICATIONPARAMETER_H__ */
