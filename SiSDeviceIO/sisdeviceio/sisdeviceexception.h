#ifndef SISDEVICEEXCEPTION_H_
#define SISDEVICEEXCEPTION_H_

#include <exception>
#include <string>

class SiSDeviceException : public std::exception
{
public:
	SiSDeviceException(const std::string & errMsg, const unsigned int errCode) : 
		std::exception(), m_msg(errMsg), m_errCode(errCode) {}

	SiSDeviceException(const std::string & errMsg) : 
		std::exception(), m_msg(errMsg), m_errCode(0) {}

	virtual ~SiSDeviceException() throw() {}

	virtual unsigned int getErrCode() const
	{
		return m_errCode;
	}

	const char* what() const throw()
	{
		return m_msg.c_str();
	}

private:
	SiSDeviceException() : m_errCode(0)
	{}

	std::string m_msg;
	unsigned int m_errCode;
};

#endif
