#ifndef SPEXCEPTION
#define SPEXCEPTION

#include <exception>
#include <string>
#include "sisproceduredllexportdefine.h"

namespace SiS
{
namespace Procedure
{

class SISPROCEDURE_EXPORT SPException : public std::exception
{
public:
    enum SpCode
    {
        SP_SISCORE_EXCEPTION = 0,
        SP_SELF_EXCEPTION,
    };

    SPException(const std::string& errMsg) :
        std::exception(),
        m_msg(errMsg),
        m_spCode(SP_SISCORE_EXCEPTION) {}

    SPException(const std::string& errMsg, SpCode spCode) :
        std::exception(),
        m_msg(errMsg),
        m_spCode(spCode) {}

    virtual ~SPException() throw(){}

    const char* what() const throw()
    {
        return m_msg.c_str();
    }

    SpCode spcode()
    {
        return this->m_spCode;
    }

protected:
    SPException(){}
    std::string m_msg;
    SpCode m_spCode;
};

} // Procedure
} // SiS

#endif // SPEXCEPTION

