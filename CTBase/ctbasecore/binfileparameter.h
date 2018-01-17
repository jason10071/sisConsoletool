#ifndef BINFILEPARAMETER_H
#define BINFILEPARAMETER_H

#include "ctbasedllexportdefine.h"
#include "ctbasecore/ctbaseparameter.h"
#include "binloader/binwrap.h"
#include "stdio.h"
#include <map>

using namespace SE;
using namespace SiS::Procedure;

namespace CT
{

namespace BFP
{

const std::string ARG_BIN_FILE =        "{BIN_FILE}";               // m_binWrapMap
const std::string ARG_BIN_DIR =         "-bindir";                  // m_binDir

}

class CTBASE_EXPORT BinFileParameter : public CTBaseParameter
{
public:
    explicit BinFileParameter();
    virtual ~BinFileParameter();

    virtual void showHelp();

    /* get function */
    std::map<ChipIndexKey, BinWrap*>& getBinWrapMap();
    BinWrap* getBinWrapMap(int chipIndex);
    std::string getBinDir();

    /* set function */
    void setBinWrapMap(std::map<ChipIndexKey, BinWrap*> binWrapMap);
    void setBinWrapMap(int chipIndex, BinWrap* binWrap);
    void clearBinWrapMap();
    void setBinDir(std::string binDir);

protected:
    virtual bool isLegalArgument(ArgumentExpression* argumentExpression);
    virtual bool setArgument(ArgumentExpression* argumentExpression);
    virtual bool hasArgumentConflict();

private:
    void assignBinWrapMapFromBinDir(std::string binDir);

protected:
    std::map<ChipIndexKey, BinWrap*> m_binWrapMap; //arg
    std::string m_binDir; //arg
};

} // CT

#endif // BINFILEPARAMETER_H
