/* Unix */
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

/* C */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include "TouchFinderParameter.h"
#include "version.h"
#include "ExitStatus.h"
#include "SisTouchFinder.h"

void printVersion();
void print_sep();

/*===========================================================================*/
int main(int argc, char *argv[])
{
    ApplicationParameter *param_tmp= ApplicationParameter::create(ApplicationParameter::TOUCH_FINDER);
    TouchFinderParameter &param = dynamic_cast<TouchFinderParameter &>(*param_tmp);
    int exitValue = EXIT_NODEV;
	
    printVersion();
	
	if ( !param.parse( argc, argv ) )
    {
        param.print_usage();
        return EXIT_BADARGU;
    }
	
    print_sep();

	/* check devName is sis touch */
	const char* checkDevName = param.checkDevName;
	SisTouchFinder sisTouchFinder;
    if( sisTouchFinder.isSisTouchHid(checkDevName) )
    {
        printf("%s: sis touch\n", checkDevName);
		exitValue = EXIT_OK;
    }
	else
	{
		printf("%s: not sis touch\n", checkDevName);
	}

    return exitValue;
}
/*===========================================================================*/
void print_sep()
{
    printf( "-----\n" );
}
/*===========================================================================*/
