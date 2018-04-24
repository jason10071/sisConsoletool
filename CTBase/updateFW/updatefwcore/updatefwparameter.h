#ifndef UPDATEFWPARAMETER_H
#define UPDATEFWPARAMETER_H

#include "ctbasecore/binfileparameter.h"

using namespace SE;
using namespace SiS::Procedure;

namespace CT
{

namespace UFWP
{

const std::string ARG_UPDATE_BOOTLOADER =           "-b";                   // m_updateBootloader
const std::string ARG_UPDATE_BOOTLOADER_AUTO =      "-ba";                  // m_updateBootloaderAuto
const std::string ARG_RESERVE_RODATA =              "-g";                   // m_reserveROData
const std::string ARG_UPDATE_REGMEM =               "-r";                   // m_updateRegmem
const std::string ARG_FORCE_UPDATE =                "-f";                   // m_forceUpdate
const std::string ARG_DISABLE_SOFT_RESET =          "-disable-softreset";   // m_disableSoftReset
const std::string ARG_DISABLE_STAMP =               "-disable-stamp";       // m_disableStamp
const std::string ARG_JUMP_CHECK_ALL =              "-jcall";               // m_jumpCheckAll
const std::string ARG_JUMP_CHECK_BIN_SIZE =         "-jcbs";                // m_jumpCheckBinSize
const std::string ARG_JUMP_CHECK_DEVICE_TYPE =      "-jcd";                 // m_jumpCheckDeviceType
const std::string ARG_JUMP_CHECK_INTERFACE_ID =     "-jci";                 // m_jumpCheckInterfaceID
const std::string ARG_JUMP_CHECK_SELECTIVE_ID =     "-jcs";                 // m_jumpCheckSelectiveID
const std::string ARG_JUMP_CHECK_PRODUCT_ID =       "-jcp";                 // m_jumpCheckProductID
const std::string ARG_CONFIRM_UPDATE =              "-cu";                  // m_confirmUpdate
const std::string ARG_BLOCK_RETRY =                 "-block-retry";         // m_blockRetry
const std::string ARG_ALL_RETRY =                   "-all-retry";           // m_allRetry
const std::string ARG_RAM_SIZE =                    "-ram-size";            // m_ramSize

}

class UpdateFWParameter : public BinFileParameter
{
public:
    explicit UpdateFWParameter();
    virtual ~UpdateFWParameter();

    virtual void showHelp();

    /* get function */
    bool getUpdateBootloader();
    bool getUpdateBootloaderAuto();
    bool getReserveROData();
    bool getUpdateRegmem();
    bool getForceUpdate();
    bool getDisableSoftReset();
    bool getDisableStamp();
    bool getJumpCheckAll();
    bool getJumpCheckBinSize();
    bool getJumpCheckDeviceType();
    bool getJumpCheckInterfaceID();
    bool getJumpCheckSelectiveID();
    bool getJumpCheckProductID();
    bool getConfirmUpdate();
    int getBlockRetry();
    int getAllRetry();
    int getRamSize();

    /* set function */
    void setUpdateBootloader(bool updateBootloader);
    void setUpdateBootloaderAuto(bool updateBootloaderAuto);
    void setReserveROData(bool reserveROData);
    void setUpdateRegmem(bool updateRegmem);
    void setForceUpdate(bool forceUpdate);
    void setDisableSoftReset(bool disableSoftReset);
    void setDisableStamp(bool disableStamp);
    void setJumpCheckAll(bool jumpCheckAll);
    void setJumpCheckBinSize(bool jumpCheckBinSize);
    void setJumpCheckDeviceType(bool jumpCheckDeviceType);
    void setJumpCheckInterfaceID(bool jumpCheckInterfaceID);
    void setJumpCheckSelectiveID(bool jumpCheckSelectiveID);
    void setJumpCheckProductID(bool jumpCheckProductID);
    void setConfirmUpdate(bool confirmUpdate);
    void setBlockRetry(int blockRetry);
    void setAllRetry(int allRetry);
    void setRamSize(int ramSize);

private:
    virtual bool isLegalArgument(ArgumentExpression* argumentExpression);
    virtual bool setArgument(ArgumentExpression* argumentExpression);
    virtual bool hasArgumentConflict();

private:
    bool m_updateBootloader; //arg
    bool m_updateBootloaderAuto; //arg
    bool m_reserveROData; //arg
    bool m_updateRegmem; //arg
    bool m_forceUpdate; //arg
    bool m_disableSoftReset; //arg
    bool m_disableStamp; //arg

    bool m_jumpCheckAll; //arg
    bool m_jumpCheckBinSize; //arg
    bool m_jumpCheckDeviceType; //arg
    bool m_jumpCheckInterfaceID; //arg
    bool m_jumpCheckSelectiveID; //arg
    bool m_jumpCheckProductID; //arg , Note : different to PID

    bool m_confirmUpdate; //arg

    int m_blockRetry; //arg
    int m_allRetry; //arg
    int m_ramSize; //arg
};

} // CT

#endif // UPDATEFWPARAMETER_H
