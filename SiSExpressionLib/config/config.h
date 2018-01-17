/*!
\file config.h
\brief To provide config.ini parser-related API
\author nf
*/

#ifndef _CONFIG_H
#define _CONFIG_H

#define MAXIMUM_SANE_FILESIZE (64 * 1024)
#define _cfgMAX_JUMPBLOCK_ELEMENTS 128

#include "hash.h"

//struct configFile *gcfgfile;

#ifdef   __cplusplus
extern   "C"
{
#endif   /*cplusplus*/



    enum _cfgFinding {
        _cfgSECTIONEND, _cfgCOMMENTEND, _cfgKEYSTART, _cfgKEYEND,
                        _cfgCOLON, _cfgVALSTART, _cfgVALEND
    }
                    ;

    /**
    *\brief The config file structure
    */
    struct configFile
    {

        hash_table *sections;

        unsigned char *bbdg;
        /* The Big Block of Delicious Goo */
        size_t bbdgSize;

    }
    ;


    struct configFile *readConfigFile (char *filename);
    struct configFile *readPlayList(char *filename);

//struct configFile *_cfgParseConfigFile (struct configFile *cfg);

//struct _cfgjumpBlock *_cfgNewJumpBlock (struct _cfgjumpBlock *current);


    int getConfigInt (struct configFile *cfg, char *section, char *key);

    char *getConfigString (struct configFile *cfg, char *section, char *key);

//int getConfigDouble (struct configFile *cfg, char *section, char *key);


    char *setConfigString(struct configFile *cfg, char *section, char *key, char *value);

    char *setConfigInt(struct configFile *cfg, char *section, char *key, int value);

//char *setConfigDouble(struct configFile *cfg, char *section, char *key,  double value);

    void saveConfigFile(struct configFile *cfg, const char* fileName);

    void printConfigFile(struct configFile *cfg);
    void unloadConfigFile (struct configFile *cfg);

//char *trimTailSpace(char *str); //by enya
#ifdef   __cplusplus
}
#endif   /*cplusplus*/


#endif
