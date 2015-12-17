#include "GetFwModeParameter.h"

/*===========================================================================*/
GetFwModeParameter::GetFwModeParameter() :
        isDumpLog(false)
{
	// isDumpLog = false;
	strcpy( filename, "getFwMode.log" );
}
/*===========================================================================*/
GetFwModeParameter::~GetFwModeParameter()
{
}
/*===========================================================================*/
int GetFwModeParameter::parse( int argc, char** argv )
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
bool GetFwModeParameter::parseArgument(char *arg)
{
    if(conParameter.parseArgument(arg)) { return true; }

    if ( strstr( arg, "-o=" ) == arg ) {
    	isDumpLog = true;
        strcpy( filename, arg + 3 );
        printf( "log file: %s\n", filename );
    } else {
        return false;
    }

    return true;

}
/*===========================================================================*/
int GetFwModeParameter::check()
{
    if(conParameter.check() == 0)
    {
        return 0;
    }

    return 1;
}
/*===========================================================================*/
void GetFwModeParameter::print_usage()
{
    printf("\nUsage: getFwMode [-o=FILENAME]\n"
           "         FILENAME: output file name\n" );
}
/*===========================================================================*/
