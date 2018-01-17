#ifndef GETFIRMWAREIDPARAMETER_H
#define GETFIRMWAREIDPARAMETER_H

#include "ctbasecore/binfileparameter.h"

using namespace SE;
using namespace SiS::Procedure;

namespace CT
{

namespace GFIDP
{

const std::string ARG_INPUT_MASTER =    "-input";                   // m_firmwareIdMap, master
const std::string ARG_INPUT_SLAVE =     "-input-s$SLAVE_INDEX";     // m_firmwareIdMap, slave

}

class GetFirmwareIdParameter : public BinFileParameter
{
public:
    explicit GetFirmwareIdParameter();
    virtual ~GetFirmwareIdParameter();

    virtual void showHelp();

    /* get function */
    std::map<ChipIndexKey, std::string>& getFirmwareCheckIdMap();
    std::string getFirmwareCheckIdMap(int chipIndex);

    /* set function */
    void setFirmwareCheckIdMap(int chipIndex, std::string checkId);
    void clearFirmwareCheckIdMap();

private:
    virtual bool isLegalArgument(ArgumentExpression* argumentExpression);
    virtual bool setArgument(ArgumentExpression* argumentExpression);
    virtual bool hasArgumentConflict();

private:
    std::map<ChipIndexKey, std::string> m_firmwareCheckIdMap; //arg
};

} // CT

#endif // GETFIRMWAREIDPARAMETER_H
