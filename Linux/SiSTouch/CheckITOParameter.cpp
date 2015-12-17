#include "CheckITOParameter.h"

/*===========================================================================*/
CheckITOParameter::CheckITOParameter ()
{

}
/*===========================================================================*/
CheckITOParameter::~CheckITOParameter ()
{

}
/*===========================================================================*/
int CheckITOParameter::parse(int argc, char** argv)
{
    printf( "parsing command parameter\n" );

    for( int i = 1; i < argc; i++ )
    {
        printf( "%d: [%s] ", i, argv[i] );

        if(!parseArgument(argv[i]))
        {
            printf( "unknown parameter: %s\n", argv[i] );
            return 0;
        }
    }

    return check();
}
/*===========================================================================*/
bool CheckITOParameter::parseArgument(char *arg)
{
    if(conParameter.parseArgument(arg)) { return true; }

    if ( strcmp( arg, "-h" ) == 0 )
    {
        printf( "update bootloader\n");
    } else {
        return false;
    }


    return true;
}
/*===========================================================================*/
int CheckITOParameter::check()
{
    if(conParameter.check() == 0)
    {
        return 0;
    }

    return 1;

}
/*===========================================================================*/
void CheckITOParameter::print_usage()
{
    printf("\nUsage: checkITO [-l]\n"
           "                 -l: verbose mode\n" );

}
/*===========================================================================*/
