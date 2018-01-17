#include "shellcommand.h"

#include <iostream>
#include <stdexcept>
#include <stdio.h>
#include <string>

ShellCommand::ShellCommand()
{

}

ShellCommand::~ShellCommand()
{

}

std::string
ShellCommand::exec(const char* cmd)
{
    char buffer[128];
    std::string result = "";
    FILE* pipe = popen(cmd, "r");
	
    if(!pipe)
    {
        printf("ShellCommand::exec, popen() failed!\n");
    }
	
    try 
    {
        while(!feof(pipe))
        {
            if(fgets(buffer, 128, pipe) != NULL)
            {
                result += buffer;
            }  
        }
    } 
    catch(...)
    {
        pclose(pipe);
        printf("ShellCommand::exec, failed! (...)\n");
    }
    pclose(pipe);
	
    return result;
}
