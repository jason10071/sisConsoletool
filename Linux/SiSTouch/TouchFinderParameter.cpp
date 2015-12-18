#include "TouchFinderParameter.h"


/*===========================================================================*/
TouchFinderParameter::TouchFinderParameter()
{
    strcpy( checkDevName, "" );
}
/*===========================================================================*/
TouchFinderParameter::~TouchFinderParameter()
{
}
/*===========================================================================*/
int TouchFinderParameter::parse( int argc, char** argv )
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
bool TouchFinderParameter::parseArgument(char *arg)
{
    if(conParameter.parseArgument(arg))
    {
        return true;
    }

    if ( strstr( arg, "-cn=" ) == arg )
    {
        strcpy( checkDevName, arg + 4 );
        printf( "checkDevName set: %s\n", checkDevName );
    }
    else
    {
        printf( "unknown parameter\n" );
        return false;;
    }

    return true;
}
/*===========================================================================*/
int TouchFinderParameter::check()
{
    if(conParameter.check() == 0)
    {
        return 0;
    }

    return 1;
}
/*===========================================================================*/
void TouchFinderParameter::print_usage()
{
    printf( "\n" );
    printf( "Usage: touchFinder [-cn=checkDevName]\n" );
}
/*===========================================================================*/
