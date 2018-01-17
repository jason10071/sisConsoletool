#ifndef SISARGUMENT_H
#define SISARGUMENT_H

#include <iostream>
#include <map>
#include <list>
#include <string>
#include <sstream>

#include "sisargument/argumentexpression.h"

using namespace std;

namespace SE
{

class SiSArgument
{
public:
    explicit SiSArgument();
    virtual ~SiSArgument();

    void parseArgs(int argc, char *argv[]);
    void parseArgs(string args);
    void parseArgs(list<string> argList);
    void parseHelp(string helpStr, int chipNum = 1);
    void parseHelp(string helpStr, string iniPath, string section, int chipNum = 1);

    int size();
    list<string> arguments();
    bool contains(string name);
    bool contains(int index);
    ArgumentExpression* find(string name);
    ArgumentExpression* find(int index);
    void update(int index, string value);
    void update(string name, string value);
    void insert(ArgumentExpression* argumentExpression);
    void clear();

    string getPraseErrorInformation();

    virtual void argumentChanged();
    virtual void showHelp();

protected:
    void erase(string name);
    ArgumentExpression* parseArg(string arg);
    bool checkExpFormat(list<string> expLines);
    ArgumentExpression* parseExpLines(list<string> expLines);
    void eraseEndWhitespaces(string& str);

protected:
    map<string, ArgumentExpression*> m_argumentMap;
    stringstream m_praseErrorSS;
};

} // SE

#endif // SISARGUMENT_H
