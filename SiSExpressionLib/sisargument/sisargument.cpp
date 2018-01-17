#include "sisargument.h"
#include "sedebug/sedebug.h"
#include "sebase/sebase.h"
#include "config/config.h"

#include <stdio.h>

#include "string.h"

using namespace SE;

#define EXPRESSION_KEYWORD_MINUS            "-"
#define EXPRESSION_KEYWORD_EQUAL            "="
#define EXPRESSION_KEYWORD_SPACE            " "
#define EXPRESSION_KEYWORD_LESS_THAN        "<"
#define EXPRESSION_KEYWORD_GREATER_THAN     ">"
#define EXPRESSION_KEYWORD_PIPE             "|"
#define EXPRESSION_KEYWORD_SLAVE_INDEX      "$SLAVE_INDEX"
#define EXPRESSION_KEYWORD_VALUE            "<VALUE>"
#define EXPRESSION_KEYWORD_OPTION           "<OPTION>"
#define EXPRESSION_KEYWORD_DIRECTORY        "<DIRECTORY>"
#define EXPRESSION_KEYWORD_FILE             "<FILE>"
#define EXPRESSION_KEYWORD_TAB_SMALL_MOUSE  "\t@"
#define EXPRESSION_KEYWORD_TAB_POUND       "\t#"
#define EXPRESSION_KEYWORD_LEFT_BRACKET    "["
#define EXPRESSION_KEYWORD_RIGHT_BRACKET    "]"
#define EXPRESSION_KEYWORD_LEFT_CURLY      "{"
#define EXPRESSION_KEYWORD_RIGHT_CURLY      "}"

SiSArgument::SiSArgument()
{
    m_praseErrorSS << "";

}

SiSArgument::~SiSArgument()
{
    clear();
}

void
SiSArgument::parseArgs(int argc, char *argv[])
{
    stringstream log;
    log << "[SiSArgument::parseArgs(int argc, char *argv[])]";
    SEDebug::printLog( log.str() );

    list<string> argList;

    for(int i = 0; i < argc; i++)
    {
        argList.push_back(argv[i]);
    }

    parseArgs(argList);
}

void
SiSArgument::parseArgs(string args)
{
    stringstream log;
    log << "[SiSArgument::parseArgs(string args)]";
    SEDebug::printLog( log.str() );

    list<string> argList;

    /* split by space */
    char* pch = NULL;
    pch = strtok( (char*) args.c_str(), EXPRESSION_KEYWORD_SPACE );
    while( pch != NULL )
    {
        argList.push_back(pch);
        pch = strtok (NULL, EXPRESSION_KEYWORD_SPACE );
    }

    /* parse */
    parseArgs(argList);
}

void
SiSArgument::parseArgs(list<string> argList)
{
    stringstream log;
    log << "[SiSArgument::parseArgs(list<string> argList)]";
    SEDebug::printLog( log.str() );
    log.str("");
    log << "argList.size: " << argList.size();
    SEDebug::printLog( log.str() );

    for (list<string>::iterator it = argList.begin(); it != argList.end(); it++)
    {
        string arg = *it;
        ArgumentExpression* argumentExpression = parseArg(arg);

        /* insert ArgumentExpression */
        if( argumentExpression != 0 )
        {
            insert(argumentExpression);
        }
    }
}

ArgumentExpression*
SiSArgument::parseArg(string arg)
{
    stringstream log;
    log << "[SiSArgument::parseArg(string arg)]";
    SEDebug::printLog( log.str() );
    log.str("");
    log << "arg: \"" << arg << "\"";
    SEDebug::printLog( log.str() );

    /* init */
    ArgumentExpression::ExpressionType expressionType = ArgumentExpression::EXPRESSION_UNKNOW;
    string name = "";
    string value = "";
    ArgumentExpression::ArgumentAttribute attribute = ArgumentExpression::ARGUMENT_UNKNOW;

    /* parse */
    if( strstr( arg.c_str(), EXPRESSION_KEYWORD_MINUS ) != arg.c_str() )
    {
        /* EXPRESSION_UNKNOW */
        expressionType = ArgumentExpression::EXPRESSION_UNKNOW;
        name = arg;
        value = arg;
        attribute = ArgumentExpression::ARGUMENT_UNKNOW;
    }
    else if( strstr( arg.c_str(), EXPRESSION_KEYWORD_EQUAL ) == NULL )
    {
        /* EXPRESSION_1 */
        expressionType = ArgumentExpression::EXPRESSION_1;
        name = arg;
        value = EXPRESSION_VALUE_TRUE;
        attribute = ArgumentExpression::ARGUMENT_BOOL;
    }
    else
    {
        /* EXPRESSION_2 */
        char* pch = NULL;
        int pi = 0;
        pch = strtok( (char*) arg.c_str(), EXPRESSION_KEYWORD_EQUAL );

        while( pch != NULL && pi < 3 )
        {
            pi++;

            if( pi == 1 )
            {
                name = pch;
            }
            else if( pi == 2 )
            {
                value = pch;
            }

            pch = strtok(NULL, EXPRESSION_KEYWORD_EQUAL);
        }

        expressionType = ArgumentExpression::EXPRESSION_2;
        attribute = ArgumentExpression::ARGUMENT_VALUE;
    }

    /* new ArgumentExpression */
    ArgumentExpression* argumentExpression = new ArgumentExpression();
    argumentExpression->setExpressionType(expressionType);
    argumentExpression->setName(name);
    argumentExpression->setValue(value);
    argumentExpression->setAttribute(attribute);
    argumentExpression->setIndex( size() ); // index + 1

    return argumentExpression;
}

void
SiSArgument::parseHelp(string helpStr, int chipNum)
{
    stringstream log;
    log << "[SiSArgument::parseHelp(string helpStr)]";
    SEDebug::printLog( log.str() );
    log.str("");
    log << "helpStr: \"" << endl << helpStr << endl << "\"";
    SEDebug::printLog( log.str() );

    /* assign to sub-lines */
    list< list<string> > expLinesList;
    stringstream helpSS(helpStr);
    string line;
    while( std::getline(helpSS, line) )
    {
        line = SeBase::removeSiSLogBegin(line);

        /* lines of a expression */
        if( line.substr(0, 1) == EXPRESSION_KEYWORD_MINUS )
        {
            /* EXPRESSION_1 and EXPRESSION_2 */

            list<string> expLines;

            do
            {
                line = SeBase::removeSiSLogBegin(line);
                eraseEndWhitespaces( line ); // erase '\n'
                expLines.push_back( line );

                if( line.substr(0, 2) == EXPRESSION_KEYWORD_TAB_POUND || line.size() == 0 )
                {
                    /* end the expression */
                    break;
                }

            }while( std::getline(helpSS, line) );

            /* if has slave chip */
            if( line.find(EXPRESSION_KEYWORD_SLAVE_INDEX) != std::string::npos )
            {
                for(int chipIdx = 1; chipIdx < chipNum; chipIdx++)
                {
                    list<string> expLinesSlave;
                    for (list<string>::iterator it = expLines.begin(); it != expLines.end(); it++)
                    {
                        string line = *it;

                        int pos = line.find(EXPRESSION_KEYWORD_SLAVE_INDEX);
                        if( pos != std::string::npos )
                        {
                            stringstream slaveNumSS;
                            slaveNumSS << (chipIdx - 1);
                            line.replace( pos, sizeof(EXPRESSION_KEYWORD_SLAVE_INDEX) - 1, slaveNumSS.str() );
                        }

                        expLinesSlave.push_back(line);
                    }

                    expLinesList.push_back(expLinesSlave);
                }
            }
            else
            {
                expLinesList.push_back(expLines);
            }
        }
        else if( line.substr(0, 1) == EXPRESSION_KEYWORD_LEFT_CURLY && line.find(EXPRESSION_KEYWORD_RIGHT_CURLY) != std::string::npos )
        {
            /* EXPRESSION_3 */

            list<string> expLines;

            do
            {
                line = SeBase::removeSiSLogBegin(line);
                eraseEndWhitespaces( line ); // erase '\n'
                expLines.push_back( line );

                if( line.substr(0, 2) == EXPRESSION_KEYWORD_TAB_POUND || line.size() == 0 )
                {
                    /* end the expression */
                    break;
                }

            }while( std::getline(helpSS, line) );

            /* if has slave chip */
            if( line.find(EXPRESSION_KEYWORD_SLAVE_INDEX) != std::string::npos )
            {
                for(int chipIdx = 0; chipIdx < chipNum; chipIdx++)
                {
                    list<string> expLinesTmp;
                    for (list<string>::iterator it = expLines.begin(); it != expLines.end(); it++)
                    {
                        string line = *it;

                        if( line.substr(0, 1) == EXPRESSION_KEYWORD_LEFT_CURLY && line.find(EXPRESSION_KEYWORD_RIGHT_CURLY) != std::string::npos )
                        {
                            int pos = line.find(EXPRESSION_KEYWORD_RIGHT_CURLY);
                            string nonAppearArgument = line.substr(1, pos - 1);
                            stringstream tmpSS;

                            if( chipIdx == 0 )
                            {
                                tmpSS << nonAppearArgument << "-master";
                            }
                            else
                            {
                                if( (chipIdx - 1) > 0 && (chipIdx - 1) < 10 )
                                {
                                    tmpSS << nonAppearArgument << "-slave0" << (chipIdx - 1);
                                }
                                else
                                {
                                    tmpSS << nonAppearArgument << "-slave" << (chipIdx - 1);
                                }
                            }

                            line.replace( 1, pos - 1, tmpSS.str() );
                        }

                        expLinesTmp.push_back(line);
                    }

                    expLinesList.push_back(expLinesTmp);
                }
            }
            else
            {
                expLinesList.push_back(expLines);
            }
        }
    }

    /* parse each lines of a expression */
    for (list< list<string> >::iterator it = expLinesList.begin(); it != expLinesList.end(); it++)
    {
        list<string> expLines = *it;

        stringstream log;
        log << "== Start to parse lines of a expression == \"";
        SEDebug::printLog( log.str() );
        for (list<string>::iterator it = expLines.begin(); it != expLines.end(); it++)
        {
            string line = *it;
            stringstream log;
            log << line;
            SEDebug::printLog( log.str() );
        }
        log.str("");
        log << "\"";
        SEDebug::printLog( log.str() );

        /* check expression format */
        if( !checkExpFormat(expLines) )
        {
            stringstream log;
            log << "! checkExpFormat error";
            SEDebug::printLog( log.str() );

            /* save to praseErrorSS */
            m_praseErrorSS << "== checkExpFormat error == \"" << endl;
            for (list<string>::iterator it = expLines.begin(); it != expLines.end(); it++)
            {
                string line = *it;
                m_praseErrorSS << line << endl;
            }
            m_praseErrorSS << "\"" << endl;

            continue;
        }

        ArgumentExpression* argumentExpression = parseExpLines(expLines);

        /* insert ArgumentExpression */
        if( argumentExpression != 0 )
        {
            insert(argumentExpression);
        }
        else
        {
            stringstream log;
            log << "argumentExpression == 0";
            SEDebug::printLog( log.str() );
        }
    }

    argumentChanged();
}

bool
SiSArgument::checkExpFormat(list<string> expLines)
{
    string lineBegin = *expLines.begin();

    if( lineBegin.substr(0, 1) == EXPRESSION_KEYWORD_LEFT_CURLY && lineBegin.find(EXPRESSION_KEYWORD_RIGHT_CURLY)  != std::string::npos )
    {
        /* EXPRESSION_3 */
        return true;
    }
    else if( lineBegin.substr(0, 1) == EXPRESSION_KEYWORD_MINUS && lineBegin.find(EXPRESSION_KEYWORD_EQUAL) == std::string::npos )
    {
        /* EXPRESSION_1 */
        bool argumentLine = false;
        bool descriptionLine = false;

        for (list<string>::iterator it = expLines.begin(); it != expLines.end(); it++)
        {
            string line = *it;

            if( line.substr(0, 1) == EXPRESSION_KEYWORD_MINUS )
            {
                argumentLine = true;
            }
            else if( line.substr(0, 2) == EXPRESSION_KEYWORD_TAB_POUND )
            {
                /* \t# */
                descriptionLine = true;
            }
        }

        if( !argumentLine || !descriptionLine )
        {
            return false;
        }
    }
    else if( lineBegin.substr(0, 1) == EXPRESSION_KEYWORD_MINUS && lineBegin.find(EXPRESSION_KEYWORD_EQUAL) != std::string::npos )
    {
        /* EXPRESSION_2 */
        bool argumentLine = false;
        bool referencenLine = false;
        bool descriptionLine = false;

        for (list<string>::iterator it = expLines.begin(); it != expLines.end(); it++)
        {
            string line = *it;

            if( line.substr(0, 1) == EXPRESSION_KEYWORD_MINUS )
            {
                /* - */
                argumentLine = true;

                if( line.find(EXPRESSION_KEYWORD_VALUE) == std::string::npos &&
                        line.find(EXPRESSION_KEYWORD_OPTION) == std::string::npos &&
                        line.find(EXPRESSION_KEYWORD_DIRECTORY) == std::string::npos &&
                        line.find(EXPRESSION_KEYWORD_FILE) == std::string::npos )
                {
                    return false;
                }

                if( line.find(EXPRESSION_KEYWORD_VALUE) != std::string::npos ||
                        line.find(EXPRESSION_KEYWORD_DIRECTORY) != std::string::npos ||
                        line.find(EXPRESSION_KEYWORD_FILE) != std::string::npos )
                {
                    referencenLine = true; // may not needed
                }
            }
            else if( line.substr(0, 2) == EXPRESSION_KEYWORD_TAB_SMALL_MOUSE )
            {
                /* \t@ */
                if( line.find(EXPRESSION_KEYWORD_VALUE) == std::string::npos &&
                        line.find(EXPRESSION_KEYWORD_OPTION) == std::string::npos &&
                        line.find(EXPRESSION_KEYWORD_DIRECTORY) == std::string::npos &&
                        line.find(EXPRESSION_KEYWORD_FILE) == std::string::npos )
                {
                    return false;
                }

                if( line.find(EXPRESSION_KEYWORD_LEFT_BRACKET) == std::string::npos ||
                        line.find(EXPRESSION_KEYWORD_RIGHT_BRACKET) == std::string::npos ||
                        line.find(EXPRESSION_KEYWORD_RIGHT_BRACKET) <= line.find(EXPRESSION_KEYWORD_LEFT_BRACKET) )
                {
                    return false; // check if lost []
                }

                if( line.find(EXPRESSION_KEYWORD_OPTION) != std::string::npos )
                {
                    referencenLine = true; // must need
                }
            }
            else if( line.substr(0, 2) == EXPRESSION_KEYWORD_TAB_POUND )
            {
                /* \t# */
                descriptionLine = true;
            }
        }

        if( !argumentLine || !referencenLine || !descriptionLine )
        {
            return false;
        }
    }
    else
    {
        /* EXPRESSION_UNKNOW */
        return false;
    }

    return true;
}

ArgumentExpression*
SiSArgument::parseExpLines(list<string> expLines)
{
    stringstream log;
    log << "[SiSArgument::parseExpLines(list<string> expLines)]";
    SEDebug::printLog( log.str() );

    /* init */
    ArgumentExpression::ExpressionType expressionType = ArgumentExpression::EXPRESSION_UNKNOW;
    string name = "";
    string value = "";
    ArgumentExpression::ArgumentAttribute attribute = ArgumentExpression::ARGUMENT_UNKNOW;
    ArgumentExpression::ReferenceAttribute referenceAttribute = ArgumentExpression::REFERENCE_UNKNOW;
    list<string> referenceList[ArgumentExpression::REFERENCE_UNKNOW + 1];
    string description = "";

    /* parse */
    string lineBegin = *expLines.begin();

    if( lineBegin.substr(0, 1) == EXPRESSION_KEYWORD_LEFT_CURLY && lineBegin.find(EXPRESSION_KEYWORD_RIGHT_CURLY)  != std::string::npos )
    {
        /* EXPRESSION_3 */
        expressionType = ArgumentExpression::EXPRESSION_3;

        for (list<string>::iterator it = expLines.begin(); it != expLines.end(); it++)
        {
            string line = *it;

            if( line.substr(0, 1) == EXPRESSION_KEYWORD_LEFT_CURLY )
            {
                /* { */
                name = line.substr( 1, line.find(EXPRESSION_KEYWORD_RIGHT_CURLY) - 1 );

                string tmpAttribute = line.substr( line.find(EXPRESSION_KEYWORD_EQUAL), line.size() );
                if( tmpAttribute.find(EXPRESSION_KEYWORD_VALUE) != std::string::npos )
                {
                    attribute = ArgumentExpression::ARGUMENT_VALUE;
                }
                else if( tmpAttribute.find(EXPRESSION_KEYWORD_OPTION) != std::string::npos )
                {
                    attribute = ArgumentExpression::ARGUMENT_OPTION;
                }
                else if( tmpAttribute.find(EXPRESSION_KEYWORD_DIRECTORY) != std::string::npos )
                {
                    attribute = ArgumentExpression::ARGUMENT_DIRECTORY;
                }
                else if( tmpAttribute.find(EXPRESSION_KEYWORD_FILE) != std::string::npos )
                {
                    attribute = ArgumentExpression::ARGUMENT_FILE;
                }
                else
                {
                    return 0;
                }
            }
            else if( line.substr(0, 2) == EXPRESSION_KEYWORD_TAB_SMALL_MOUSE )
            {
                /* \t@ */
                if( line.find(EXPRESSION_KEYWORD_VALUE) != std::string::npos )
                {

                }
                else if( line.find(EXPRESSION_KEYWORD_OPTION) != std::string::npos )
                {
                    referenceAttribute = ArgumentExpression::REFERENCE_OPTION;
                    referenceList[referenceAttribute].push_back(EXPRESSION_VALUE_DEFAULT_OPTION);
                    value = EXPRESSION_VALUE_DEFAULT_OPTION;
                }
                else if( line.find(EXPRESSION_KEYWORD_DIRECTORY) != std::string::npos )
                {

                }
                else if( line.find(EXPRESSION_KEYWORD_FILE) != std::string::npos )
                {
                    referenceAttribute = ArgumentExpression::REFERENCE_FILE;
                }
                else
                {
                    return 0;
                }

                string tmpReference = line.substr( line.find(EXPRESSION_KEYWORD_LEFT_BRACKET) + 1,
                                                   line.find(EXPRESSION_KEYWORD_RIGHT_BRACKET) - line.find(EXPRESSION_KEYWORD_LEFT_BRACKET) - 1 );

                istringstream f(tmpReference);
                string s;
                while (getline(f, s, '|'))
                {
                    referenceList[referenceAttribute].push_back(s);
                }
            }
            else if( line.substr(0, 2) == EXPRESSION_KEYWORD_TAB_POUND )
            {
                /* \t# */
                description = line.substr(2, line.size());
            }
        }
    }
    else if( lineBegin.substr(0, 1) == EXPRESSION_KEYWORD_MINUS && lineBegin.find(EXPRESSION_KEYWORD_EQUAL) == std::string::npos )
    {
        /* EXPRESSION_1 */
        expressionType = ArgumentExpression::EXPRESSION_1;

        for (list<string>::iterator it = expLines.begin(); it != expLines.end(); it++)
        {
            string line = *it;

            if( line.substr(0, 1) == EXPRESSION_KEYWORD_MINUS )
            {
                /* - */
                name = line;
                value = EXPRESSION_VALUE_FALSE;
                attribute = ArgumentExpression::ARGUMENT_BOOL;
            }
            else if( line.substr(0, 2) == EXPRESSION_KEYWORD_TAB_POUND )
            {
                /* \t# */
                description = line.substr(2, line.size());
            }
        }
    }
    else if( lineBegin.substr(0, 1) == EXPRESSION_KEYWORD_MINUS && lineBegin.find(EXPRESSION_KEYWORD_EQUAL) != std::string::npos )
    {
        /* EXPRESSION_2 */
        expressionType = ArgumentExpression::EXPRESSION_2;

        for (list<string>::iterator it = expLines.begin(); it != expLines.end(); it++)
        {
            string line = *it;

            if( line.substr(0, 1) == EXPRESSION_KEYWORD_MINUS )
            {
                /* - */
                name = line.substr( 0, line.find(EXPRESSION_KEYWORD_EQUAL) );

                string tmpAttribute = line.substr( line.find(EXPRESSION_KEYWORD_EQUAL), line.size() );
                if( tmpAttribute.find(EXPRESSION_KEYWORD_VALUE) != std::string::npos )
                {
                    attribute = ArgumentExpression::ARGUMENT_VALUE;
                }
                else if( tmpAttribute.find(EXPRESSION_KEYWORD_OPTION) != std::string::npos )
                {
                    attribute = ArgumentExpression::ARGUMENT_OPTION;
                }
                else if( tmpAttribute.find(EXPRESSION_KEYWORD_DIRECTORY) != std::string::npos )
                {
                    attribute = ArgumentExpression::ARGUMENT_DIRECTORY;
                }
                else if( tmpAttribute.find(EXPRESSION_KEYWORD_FILE) != std::string::npos )
                {
                    attribute = ArgumentExpression::ARGUMENT_FILE;
                }
                else
                {
                    return 0;
                }
            }
            else if( line.substr(0, 2) == EXPRESSION_KEYWORD_TAB_SMALL_MOUSE )
            {
                /* \t@ */
                if( line.find(EXPRESSION_KEYWORD_VALUE) != std::string::npos )
                {

                }
                else if( line.find(EXPRESSION_KEYWORD_OPTION) != std::string::npos )
                {
                    referenceAttribute = ArgumentExpression::REFERENCE_OPTION;
                    referenceList[referenceAttribute].push_back(EXPRESSION_VALUE_DEFAULT_OPTION);
                    value = EXPRESSION_VALUE_DEFAULT_OPTION;
                }
                else if( line.find(EXPRESSION_KEYWORD_DIRECTORY) != std::string::npos )
                {

                }
                else if( line.find(EXPRESSION_KEYWORD_FILE) != std::string::npos )
                {
                    referenceAttribute = ArgumentExpression::REFERENCE_FILE;
                }
                else
                {
                    return 0;
                }

                string tmpReference = line.substr( line.find(EXPRESSION_KEYWORD_LEFT_BRACKET) + 1,
                                                   line.find(EXPRESSION_KEYWORD_RIGHT_BRACKET) - line.find(EXPRESSION_KEYWORD_LEFT_BRACKET) - 1 );

                istringstream f(tmpReference);
                string s;
                while (getline(f, s, '|'))
                {
                    referenceList[referenceAttribute].push_back(s);
                }
            }
            else if( line.substr(0, 2) == EXPRESSION_KEYWORD_TAB_POUND )
            {
                /* \t# */
                description = line.substr(2, line.size());
            }
        }
    }
    else
    {
        /* EXPRESSION_UNKNOW */
        return 0;
    }

    /* new ArgumentExpression */
    ArgumentExpression* argumentExpression = new ArgumentExpression();
    argumentExpression->setExpressionType(expressionType);
    argumentExpression->setName(name);
    argumentExpression->setValue(value);
    argumentExpression->setAttribute(attribute);
    argumentExpression->setIndex( size() ); // index + 1
    argumentExpression->setReferenceAttribute(referenceAttribute);
    for(int i = ArgumentExpression::REFERENCE_OPTION; i <= ArgumentExpression::REFERENCE_UNKNOW; i++)
    {
        argumentExpression->setReferenceList( (ArgumentExpression::ReferenceAttribute)i, referenceList[i] );
    }
    argumentExpression->setDescription(description);

    return argumentExpression;
}

void
SiSArgument::parseHelp(string helpStr, string iniPath, string section, int chipNum)
{
    stringstream log;
    log << "[SiSArgument::parseHelp(string helpStr, string iniPath, string section, int chipNum)]";
    SEDebug::printLog( log.str() );

    parseHelp(helpStr, chipNum);

    /* assign value from ini */
    configFile* cfgFile = readConfigFile( (char*) iniPath.c_str() );
    if(cfgFile)
    {
        stringstream log;
        log << "open iniPath: " << iniPath;
        SEDebug::printLog( log.str() );

        for(map<string, ArgumentExpression*>::iterator it = m_argumentMap.begin(); it != m_argumentMap.end(); ++it)
        {
            ArgumentExpression* argumentExpression = it->second;
            //argumentExpression->setValue("ini value");

            /* getConfigString */
            stringstream log;
            log << "[" << section << "] " << argumentExpression->getName();
            SEDebug::printLog( log.str() );

            char* str;
            str = getConfigString( cfgFile, (char *) section.c_str(), (char *) argumentExpression->getName().c_str() );
            if( str )
            {
                stringstream log;
                log << "value: " << str;
                SEDebug::printLog( log.str() );

                string value = str;
                argumentExpression->setValue( value );
            }
            else
            {
                stringstream log;
                log << "! not find";
                SEDebug::printLog( log.str() );
            }
        }

        unloadConfigFile(cfgFile);
    }
    else
    {
        stringstream log;
        log << "! fail to open iniPath: " << iniPath;
        SEDebug::printLog( log.str() );
    }

    argumentChanged();
}

int
SiSArgument::size()
{
    return this->m_argumentMap.size();
}

list<string>
SiSArgument::arguments()
{
    list<string> argumentList;

    for (int idx = 0; idx < size(); idx++)
    {
        if( contains(idx) )
        {
            ArgumentExpression* argumentExpression = find(idx);
            argumentList.push_back( argumentExpression->getName() );
        }
    }

    return argumentList;
}

bool
SiSArgument::contains(string name)
{
    if( m_argumentMap.find( name ) != m_argumentMap.end() )
    {
        return true;
    }

    return false;
}

bool
SiSArgument::contains(int index)
{
    if( find(index) != 0 )
    {
        return true;
    }

    return false;
}

ArgumentExpression*
SiSArgument::find(string name)
{
    ArgumentExpression* argumentExpression = 0;

    map<string, ArgumentExpression*>::const_iterator it = m_argumentMap.find( name );
    if( it != m_argumentMap.end() )
    {
        argumentExpression = it->second;
    }

    return argumentExpression;
}

ArgumentExpression*
SiSArgument::find(int index)
{
    ArgumentExpression* argumentExpression = 0;

    for(map<string, ArgumentExpression*>::iterator it = m_argumentMap.begin(); it != m_argumentMap.end(); ++it)
    {
        if( it->second->getIndex() == index )
        {
            argumentExpression = it->second;
            break;
        }
    }

    return argumentExpression;
}

void
SiSArgument::update(int index, string value)
{
    ArgumentExpression* argumentExpression = find( index );

    if( argumentExpression != 0 )
    {
        argumentExpression->setValue( value );

        argumentChanged();
    }
}

void
SiSArgument::update(string name, string value)
{
    ArgumentExpression* argumentExpression = find( name );

    if( argumentExpression != 0 )
    {
        argumentExpression->setValue( value );

        argumentChanged();
    }
}

void
SiSArgument::insert(ArgumentExpression* argumentExpression)
{
    string name = argumentExpression->getName();

    if( contains( name ) )
    {
        argumentExpression->setIndex( find( name )->getIndex() ); // use the same index
        erase( name );
    }
    else
    {
        argumentExpression->setIndex( size() ); // index + 1
    }

    m_argumentMap.insert( map<string, ArgumentExpression*>::value_type( name, argumentExpression) );

    argumentChanged();
}

void
SiSArgument::erase(string name)
{
    if( contains( name ) )
    {
        ArgumentExpression* argumentExpression = find( name );
        m_argumentMap.erase( name );
        delete argumentExpression;
    }

    argumentChanged();
}

void
SiSArgument::clear()
{
    for(map<string, ArgumentExpression*>::iterator it = m_argumentMap.begin(); it != m_argumentMap.end(); ++it)
    {
        ArgumentExpression* argumentExpression = it->second;
        delete argumentExpression;
    }

    m_argumentMap.clear();

    argumentChanged();
}

string
SiSArgument::getPraseErrorInformation()
{
    return this->m_praseErrorSS.str();
}

void
SiSArgument::argumentChanged()
{
    // do nothing
}

void
SiSArgument::showHelp()
{
    // do nothing
}

void
SiSArgument::eraseEndWhitespaces(string& str)
{
    std::string whitespaces (" \t\f\v\n\r");

    std::size_t found = str.find_last_not_of(whitespaces);
    if ( found != std::string::npos )
    {
        str.erase(found+1);
    }
    else
    {
        str.clear();    // str is all whitespace
    }
}
