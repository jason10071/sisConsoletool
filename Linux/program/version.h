#ifndef VERSION_H
#define VERSION_H

#define MAIN_VERSION 1
#define SUB_VERSION 8
#define LAST_VERSION 20
#define DESC "HIDRAW"

void printVersion()
{
#ifndef DESC
    printf("V%d.%d.%d\n", MAIN_VERSION, SUB_VERSION, LAST_VERSION);
#else
    printf("V%d.%d.%d %s\n", MAIN_VERSION, SUB_VERSION, LAST_VERSION, DESC);
#endif
}

#endif
