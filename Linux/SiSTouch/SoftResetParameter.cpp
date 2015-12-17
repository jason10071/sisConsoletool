#include "SoftResetParameter.h"

/*===========================================================================*/
SoftResetParameter::SoftResetParameter()
{

}
/*===========================================================================*/
SoftResetParameter::~SoftResetParameter()
{
}
/*===========================================================================*/
int SoftResetParameter::parse( int argc, char** argv )
{
    printf( "parsing command parameter\n" );

    for (int i = 1; i < argc; i++) {
        printf( "%d: [%s] ", i, argv[i] );

        if (!parseArgument(argv[i])) {
            printf("unknown parameter: %s\n", argv[i]);
            return 0;
        }
    }

    return check();
}
/*===========================================================================*/
bool SoftResetParameter::parseArgument(char *arg)
{
    return conParameter.parseArgument(arg) ? true : false;
}
/*===========================================================================*/
int SoftResetParameter::check()
{
    return (conParameter.check() == 0) ? 0 : 1;
}
/*===========================================================================*/
void SoftResetParameter::print_usage()
{
    printf("\nUsage: softReset [-l]\n"
           "                 -l: verbose mode\n" );
}
/*===========================================================================*/
