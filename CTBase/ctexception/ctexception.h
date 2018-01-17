#ifndef CTEXCEPTION_H
#define CTEXCEPTION_H

#include <exception>
#include <string>
#include "ctbasedllexportdefine.h"

namespace CT
{

class CTBASE_EXPORT CTException : public std::exception
{
public:
    CTException(const std::string& errMsg) :
        std::exception(),
        m_msg(errMsg) {}

    virtual ~CTException() throw(){}

    const char* what() const throw()
    {
        return m_msg.c_str();
    }

protected:
    CTException(){}
    std::string m_msg;
};

} // CT

#endif // CTEXCEPTION_H
