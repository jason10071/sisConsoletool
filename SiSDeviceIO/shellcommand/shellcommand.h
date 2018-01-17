#ifndef SHELLCOMMAND_H
#define SHELLCOMMAND_H

#include "stdio.h"
#include "string"

class ShellCommand
{
public:
    ShellCommand();
    ~ShellCommand();

	static std::string exec(const char* cmd);
};

#endif // SHELLCOMMAND_H
