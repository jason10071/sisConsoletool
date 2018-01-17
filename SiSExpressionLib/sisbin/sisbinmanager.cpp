#include "sisbinmanager.h"

#include "sedebug/sedebug.h"

#include <iostream>
#include <string>
#include <sstream>

#ifdef WIN32
    #include <windows.h>
#else
    #include "dirent.h"
#endif

using namespace SE;

SiSBinManager::SiSBinManager()
{

}

SiSBinManager::~SiSBinManager()
{

}

std::map<BinIndex, BinName>
SiSBinManager::getBinMap(std::string binDir)
{
    stringstream log;
    log << "[SiSBinManager::getBinMap(\"" << binDir << "\")]";
    SEDebug::printLog( log.str() );

    /* replace "\\" to "/" */
    binDir = SiSBinManager::replaceAll(binDir, "\\", "/");

    std::map<BinIndex, BinName> binMap;
    BinIndex binIndex = 0;

#ifdef WIN32
    HANDLE hFind;
    WIN32_FIND_DATA data;

    /* append * for search file */
    std::string tmpBinDir = binDir;
    tmpBinDir.append( "/*" );

    /* str to widestr */
    std::wstring widestr = std::wstring(tmpBinDir.begin(), tmpBinDir.end());
    const wchar_t* path = widestr.c_str();

    hFind = FindFirstFile( path,  &data );
    if(hFind != INVALID_HANDLE_VALUE)
    {
        do
        {
            /* file */
            if( data.dwFileAttributes == FILE_ATTRIBUTE_ARCHIVE )
            {
                /* convert from wide char to narrow char array */
                char ch[260];
                char DefChar = ' ';
                WideCharToMultiByte(CP_ACP, 0 ,data.cFileName, -1, ch, 260, &DefChar, NULL);

                /* A std:string  using the char* constructor. */
                std::string ss(ch);
                std::string binFile = binDir;
                binFile.append("/");
                binFile.append(ss);

                /* insert */
                if( SiSBinManager::endsWith(binFile, ".bin") )
                {
                    binMap[binIndex] = binFile;
                    binIndex++;
                }
            }
        } while(FindNextFile(hFind, &data));

        FindClose(hFind);
    }
    else
    {
        stringstream log;
        log << "[SiSBinManager::getBinMap(\"" << binDir << "\")], Fail";
        SEDebug::printLog( log.str() );
    }
#else
    DIR *dir;
    struct dirent *ent;
    if( ( dir = opendir(binDir.c_str())) != NULL )
    {
        /* all the files and directories within directory */
        while( (ent = readdir(dir)) != NULL )
        {
            /* file */
            if( ent->d_type == DT_REG )
            {
                /* A std:string  using the char* constructor. */
                std::string ss(ent->d_name);
                std::string binFile = binDir;
                binFile.append("/");
                binFile.append(ss);

                /* insert */
                if( SiSBinManager::endsWith(binFile, ".bin") )
                {
                    binMap[binIndex] = binFile;
                    binIndex++;
                }
            }
        }

        closedir (dir);
    }
    else
    {
        stringstream log;
        log << "[SiSBinManager::getBinMap(\"" << binDir << "\")], Fail";
        SEDebug::printLog( log.str() );
    }
#endif

    return sortBinMapByName(binMap);
}

std::map<BinIndex, BinName>
SiSBinManager::sortBinMapByName(std::map<BinIndex, BinName> oldBinMap)
{
//    for(std::map<BinIndex,BinName>::iterator iter = oldBinMap.begin(); iter != oldBinMap.end(); ++iter)
//    {
//        BinIndex binIndex = iter->first;
//        BinName binName = iter->second;

//        printf("old [%d] %s\n", binIndex, binName.c_str());
//    }

    /* It's not sort needed */
    if( oldBinMap.size() <= 1 )
    {
        return oldBinMap;
    }

    /* sort */
    std::map<BinIndex, BinName> newBinMap;
    std::map<BinIndex, BinName> tmpBinMap = oldBinMap;

    for(int binIndex = 0; binIndex < oldBinMap.size(); binIndex++)
    {
        std::string keyword = "";
        if( binIndex == 0 )
        {
            keyword.append("M.bin");
        }
        else
        {
            keyword.append("S");
            keyword.append( SiSBinManager::intToString(binIndex - 1) );
            keyword.append(".bin");
        }

        /* find and match */
        std::map<BinIndex,BinName>::iterator iter;
        bool isFound = false;
        for(iter = tmpBinMap.begin(); iter != tmpBinMap.end(); ++iter)
        {
            BinName binName = iter->second;
            if( SiSBinManager::endsWith(binName, keyword) )
            {
                newBinMap[binIndex] = binName;
                isFound = true;
                break;
            }
        }
        if(isFound) { tmpBinMap.erase(iter->first); } // remove found
    }

//    for(std::map<BinIndex,BinName>::iterator iter = newBinMap.begin(); iter != newBinMap.end(); ++iter)
//    {
//        BinIndex binIndex = iter->first;
//        BinName binName = iter->second;

//        printf("new [%d] %s\n", binIndex, binName.c_str());
//    }

    return newBinMap;
}

std::string
SiSBinManager::intToString(int a)
{
    ostringstream temp;
    temp << a;
    return temp.str();
}

bool
SiSBinManager::endsWith(std::string s, std::string suffix)
{
    return s.rfind(suffix) == (s.size() - suffix.size());
}

std::string
SiSBinManager::replaceAll(std::string str, const std::string from, const std::string to)
{
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos)
    {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
    }
    return str;
}
