#ifndef GETFIRMWAREINFOPARAMETER_H
#define GETFIRMWAREINFOPARAMETER_H

#include "ctbasecore/ctbaseparameter.h"
#include "binloader/binwrap.h"
#include "stdio.h"
#include <map>

using namespace SE;
using namespace SiS::Procedure;

#define INVALID_ADDRESS 0xffffff

namespace CT
{

namespace GFINFOP
{

const std::string ARG_ADDRESS =             "-addr";    // m_address
const std::string ARG_LENGTH =              "-len";     // m_length
const std::string ARG_KEYWORD =             "-kw";      // m_keyword
const std::string ARG_SHOW_LIST =           "-list";    // m_showList
const std::string ARG_SHOW_ENGINEER_MODE =  "-eng";     // m_showEngineerMode
const std::string ARG_AUTOTOOL =            "-at";      // m_autotool

}

class GetFirmwareInfoParameter : public CTBaseParameter
{
public:
    explicit GetFirmwareInfoParameter();
    ~GetFirmwareInfoParameter();

    virtual void showHelp();

    /* get function */
    unsigned int getAddress();
    int getLength();
    std::string getKeyword();
    bool getShowList();
    bool getShowEngineerMode();
    bool getAutotool();

    /* set function */
    void setAddress(unsigned int address);
    void setLength(int length);
    void setKeyword(std::string keyword);
    void setShowList(bool showList);
    void setShowEngineerMode(bool showEngineerMode);
    void setAutotool(bool autotool);

private:
    virtual bool isLegalArgument(ArgumentExpression* argumentExpression);
    virtual bool setArgument(ArgumentExpression* argumentExpression);
    virtual bool hasArgumentConflict();

private:
    unsigned int m_address;     //arg
    int m_length;               //arg
    std::string m_keyword;      //arg
    bool m_showList;            //arg
    bool m_showEngineerMode;    //arg
    bool m_autotool;            //arg
};

} // CT

#endif // GETFIRMWAREINFOPARAMETER_H
