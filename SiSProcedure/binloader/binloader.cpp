#include "binloader.h"

#include "sislog.h"
#include "spexception/spexception.h"
#include "binloader/binwrap.h"
#include "spbase/sisproceduredefine.h"

#include "string.h"

using namespace SiS::Procedure;

#define EXCEPTION_TITLE "BinLoader Exception : "
#define TAG "BinLoader"

BinLoader::BinLoader()
{

}

BinLoader::~BinLoader()
{

}

void
BinLoader::read(std::string binFileName, unsigned char* &binBuf, int &binSize)
{
    /* open */
    FILE* file = openFile(binFileName, binSize);

    int standardizationSize = 0;
    if( binSize >= LOW_BOUND_64K && binSize <= _64K)
    {
        standardizationSize = _64K;
    }
    else if( binSize >= LOW_BOUND_128K && binSize <= _128K)
    {
        standardizationSize = _128K;
    }
    else
    {
        std::string msg = EXCEPTION_TITLE;
        char errorMsg[1024] = "";
        sprintf(errorMsg, "illegal bin size : %d bytes. It should be 64k/128k (%d ~ %d ~ %d)",
                binSize, LOW_BOUND_64K, _64K, _128K);
        msg.append(errorMsg);
        throw SPException( msg, SPException::SP_SELF_EXCEPTION );
    }
    /* using standardizationSize */
    binSize = standardizationSize;
    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "standardizationSize : %d bytes", binSize );

    /* alloc */
    binBuf = new unsigned char[binSize];
    memset(binBuf, 0x0, binSize * sizeof(unsigned char) );

    /* read */
    readFileToBuffer(file, 0, binBuf, binSize);
}

FILE*
BinLoader::openFile(std::string fileName, int &size)
{
    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "opening file: \"%s\"", fileName.c_str() );

    FILE* file = fopen( fileName.c_str(), "r" );

    if ( !file )
    {
        std::string msg = EXCEPTION_TITLE;
        msg.append("file not found !");
        throw SPException( msg, SPException::SP_SELF_EXCEPTION );
    }

    fseek( file, 0, SEEK_END );
    int fileSize = ftell( file );
    size = fileSize;
    fseek( file, 0, SEEK_SET );

    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "pattern file contains %d bytes", size );
    return file;
}

int
BinLoader::readFileToBuffer(FILE* file, long offset, unsigned char* buf, int size)
{
    fseek(file, offset, SEEK_SET);
    return fread( buf, sizeof(unsigned char), size, file );
}
