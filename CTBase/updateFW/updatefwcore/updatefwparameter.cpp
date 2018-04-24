#include "updatefwparameter.h"

#include "sislog.h"
#include "ctexception/ctexception.h"
#include "sisprocedure/isisprocedure.h"

using namespace CT;
using namespace CT::BFP;
using namespace CT::UFWP;
using namespace SiS;
using namespace SiS::Procedure;

#define EXCEPTION_TITLE "UpdateFWParameter Exception : "
#define TAG "UpdateFWParameter"

UpdateFWParameter::UpdateFWParameter() :
    BinFileParameter(),
    m_updateBootloader(false),
    m_updateBootloaderAuto(false),
    m_reserveROData(false),
    m_updateRegmem(false),
    m_forceUpdate(false),
    m_disableSoftReset(false),
    m_disableStamp(false),
    m_jumpCheckAll(false),
    m_jumpCheckBinSize(false),
    m_jumpCheckDeviceType(false),
    m_jumpCheckInterfaceID(false),
    m_jumpCheckSelectiveID(false),
    m_jumpCheckProductID(false),
    m_confirmUpdate(false),
    m_blockRetry(-1),
    m_allRetry(-1),
    m_ramSize(-1)
{

}

UpdateFWParameter::~UpdateFWParameter()
{           

}

void
UpdateFWParameter::showHelp()
{
    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s", ARG_UPDATE_BOOTLOADER.c_str() );
    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "\t#update Bootloader\n" );

    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s", ARG_UPDATE_BOOTLOADER_AUTO.c_str() );
    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "\t#auto update Bootloader\n" );

    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s", ARG_RESERVE_RODATA.c_str() );
    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "\t#reserve ROData\n" );

    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s", ARG_UPDATE_REGMEM.c_str() );
    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "\t#update Regmem\n" );

    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s", ARG_FORCE_UPDATE.c_str() );
    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "\t#force update\n" );

    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s", ARG_DISABLE_SOFT_RESET.c_str() );
    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "\t#disable softreset after update done. (note: only support non-broken FW)\n" );

    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s", ARG_DISABLE_STAMP.c_str() );
    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "\t#disable update mark\n" );

    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s", ARG_JUMP_CHECK_ALL.c_str() );
    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "\t#jump check all\n" );

    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s", ARG_JUMP_CHECK_BIN_SIZE.c_str() );
    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "\t#jump check bin size\n" );

    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s", ARG_JUMP_CHECK_DEVICE_TYPE.c_str() );
    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "\t#jump check device type\n" );

    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s", ARG_JUMP_CHECK_INTERFACE_ID.c_str() );
    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "\t#jump check interfaceID\n" );

    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s", ARG_JUMP_CHECK_SELECTIVE_ID.c_str() );
    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "\t#jump check selectiveID\n" );

    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s", ARG_JUMP_CHECK_PRODUCT_ID.c_str() );
    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "\t#jump check productID (different to PID)\n" );

    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s", ARG_CONFIRM_UPDATE.c_str() );
    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "\t#confirm update\n" );

    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s=<VALUE>", ARG_BLOCK_RETRY.c_str() );
    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "\t#set block retry\n" );

    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s=<VALUE>", ARG_ALL_RETRY.c_str() );
    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "\t#set all retry\n" );

    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s=<VALUE>", ARG_RAM_SIZE.c_str() );
    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "\t#set ram size (k) to a block\n" );

    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s=<FILE>", ARG_BIN_FILE.c_str() );
    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "\t@<FILE>=[*.bin]" );
    SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "\t#set master/slave%s bin file for updating FW. (EX: M.bin S0.bin S1.bin ...)\n", EXPRESSION_KEYWORD_SLAVE_INDEX );

    BinFileParameter::showHelp();
}

bool
UpdateFWParameter::isLegalArgument(ArgumentExpression* argumentExpression)
{
    /* super */
    if( BinFileParameter::isLegalArgument(argumentExpression) )
    {
        return true;
    }

    /* this */
    if( argumentExpression->getName().compare( ARG_UPDATE_BOOTLOADER ) == 0 &&
            argumentExpression->getAttribute() == ArgumentExpression::ARGUMENT_BOOL )
    {
        return true;
    }
    else if( argumentExpression->getName().compare( ARG_UPDATE_BOOTLOADER_AUTO ) == 0 &&
             argumentExpression->getAttribute() == ArgumentExpression::ARGUMENT_BOOL )
    {
        return true;
    }
    else if( argumentExpression->getName().compare( ARG_RESERVE_RODATA ) == 0 &&
             argumentExpression->getAttribute() == ArgumentExpression::ARGUMENT_BOOL )
    {
        return true;
    }
    else if( argumentExpression->getName().compare( ARG_UPDATE_REGMEM ) == 0 &&
             argumentExpression->getAttribute() == ArgumentExpression::ARGUMENT_BOOL )
    {
        return true;
    }
    else if( argumentExpression->getName().compare( ARG_FORCE_UPDATE ) == 0 &&
             argumentExpression->getAttribute() == ArgumentExpression::ARGUMENT_BOOL )
    {
        return true;
    }
    else if( argumentExpression->getName().compare( ARG_DISABLE_SOFT_RESET ) == 0 &&
             argumentExpression->getAttribute() == ArgumentExpression::ARGUMENT_BOOL )
    {
        return true;
    }
    else if( argumentExpression->getName().compare( ARG_DISABLE_STAMP ) == 0 &&
             argumentExpression->getAttribute() == ArgumentExpression::ARGUMENT_BOOL )
    {
        return true;
    }
    else if( argumentExpression->getName().compare( ARG_JUMP_CHECK_ALL ) == 0 &&
             argumentExpression->getAttribute() == ArgumentExpression::ARGUMENT_BOOL )
    {
        return true;
    }
    else if( argumentExpression->getName().compare( ARG_JUMP_CHECK_BIN_SIZE ) == 0 &&
             argumentExpression->getAttribute() == ArgumentExpression::ARGUMENT_BOOL )
    {
        return true;
    }
    else if( argumentExpression->getName().compare( ARG_JUMP_CHECK_DEVICE_TYPE ) == 0 &&
             argumentExpression->getAttribute() == ArgumentExpression::ARGUMENT_BOOL )
    {
        return true;
    }
    else if( argumentExpression->getName().compare( ARG_JUMP_CHECK_INTERFACE_ID ) == 0 &&
             argumentExpression->getAttribute() == ArgumentExpression::ARGUMENT_BOOL )
    {
        return true;
    }
    else if( argumentExpression->getName().compare( ARG_JUMP_CHECK_SELECTIVE_ID ) == 0 &&
             argumentExpression->getAttribute() == ArgumentExpression::ARGUMENT_BOOL )
    {
        return true;
    }
    else if( argumentExpression->getName().compare( ARG_JUMP_CHECK_PRODUCT_ID ) == 0 &&
             argumentExpression->getAttribute() == ArgumentExpression::ARGUMENT_BOOL )
    {
        return true;
    }
    else if( argumentExpression->getName().compare( ARG_CONFIRM_UPDATE ) == 0 &&
             argumentExpression->getAttribute() == ArgumentExpression::ARGUMENT_BOOL )
    {
        return true;
    }
    else if( argumentExpression->getName().compare( ARG_BLOCK_RETRY ) == 0 &&
             argumentExpression->getAttribute() == ArgumentExpression::ARGUMENT_VALUE )
    {
        return true;
    }
    else if( argumentExpression->getName().compare( ARG_ALL_RETRY ) == 0 &&
             argumentExpression->getAttribute() == ArgumentExpression::ARGUMENT_VALUE )
    {
        return true;
    }
    else if( argumentExpression->getName().compare( ARG_RAM_SIZE ) == 0 &&
             argumentExpression->getAttribute() == ArgumentExpression::ARGUMENT_VALUE )
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool
UpdateFWParameter::setArgument(ArgumentExpression* argumentExpression)
{
    /* this */
    if( contains(argumentExpression->getName()) )
    {
        if( argumentExpression->getName().compare( ARG_UPDATE_BOOTLOADER ) == 0 )
        {
            if( argumentExpression->getValueToBool() )
            {
                this->m_updateBootloader = true;
                SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "set updateBootloader : true");
            }
            else
            {
                this->m_updateBootloader = false;
                SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "set updateBootloader : false");
            }
            return true;
        }
        else if( argumentExpression->getName().compare( ARG_UPDATE_BOOTLOADER_AUTO ) == 0 )
        {
            if( argumentExpression->getValueToBool() )
            {
                this->m_updateBootloaderAuto = true;
                SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "set updateBootloaderAuto : true");
            }
            else
            {
                this->m_updateBootloaderAuto = false;
                SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "set updateBootloaderAuto : false");
            }
            return true;
        }
        else if( argumentExpression->getName().compare( ARG_RESERVE_RODATA ) == 0 )
        {
            if( argumentExpression->getValueToBool() )
            {
                this->m_reserveROData = true;
                SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "set reserveROData : true");
            }
            else
            {
                this->m_reserveROData = false;
                SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "set reserveROData : false");
            }
            return true;
        }
        else if( argumentExpression->getName().compare( ARG_UPDATE_REGMEM ) == 0 )
        {
            if( argumentExpression->getValueToBool() )
            {
                this->m_updateRegmem = true;
                SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "set updateRegmem : true");
            }
            else
            {
                this->m_updateRegmem = false;
                SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "set updateRegmem : false");
            }
            return true;
        }
        else if( argumentExpression->getName().compare( ARG_FORCE_UPDATE ) == 0 )
        {
            if( argumentExpression->getValueToBool() )
            {
                this->m_forceUpdate = true;
                SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "set forceUpdate : true");
            }
            else
            {
                this->m_forceUpdate = false;
                SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "set forceUpdate : false");
            }
            return true;
        }
        else if( argumentExpression->getName().compare( ARG_DISABLE_SOFT_RESET ) == 0 )
        {
            if( argumentExpression->getValueToBool() )
            {
                this->m_disableSoftReset = true;
                SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "set disableSoftReset : true");
            }
            else
            {
                this->m_disableSoftReset = false;
                SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "set disableSoftReset : false");
            }
            return true;
        }
        else if( argumentExpression->getName().compare( ARG_DISABLE_STAMP ) == 0 )
        {
            if( argumentExpression->getValueToBool() )
            {
                this->m_disableStamp = true;
                SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "set disableUpdateMark : true");
            }
            else
            {
                this->m_disableStamp = false;
                SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "set disableUpdateMark : false");
            }
            return true;
        }
        else if( argumentExpression->getName().compare( ARG_JUMP_CHECK_ALL ) == 0 )
        {
            if( argumentExpression->getValueToBool() )
            {
                this->m_jumpCheckAll = true;
                SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "set jumpCheckAll : true");
            }
            else
            {
                this->m_jumpCheckAll = false;
                SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "set jumpCheckAll : false");
            }
            return true;
        }
        else if( argumentExpression->getName().compare( ARG_JUMP_CHECK_BIN_SIZE ) == 0 )
        {
            if( argumentExpression->getValueToBool() )
            {
                this->m_jumpCheckBinSize = true;
                SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "set jumpCheckBinSize : true");
            }
            else
            {
                this->m_jumpCheckBinSize = false;
                SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "set jumpCheckBinSize : false");
            }
            return true;
        }
        else if( argumentExpression->getName().compare( ARG_JUMP_CHECK_DEVICE_TYPE ) == 0 )
        {
            if( argumentExpression->getValueToBool() )
            {
                this->m_jumpCheckDeviceType = true;
                SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "set jumpCheckDeviceType : true");
            }
            else
            {
                this->m_jumpCheckDeviceType = false;
                SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "set jumpCheckDeviceType : false");
            }
            return true;
        }
        else if( argumentExpression->getName().compare( ARG_JUMP_CHECK_INTERFACE_ID ) == 0 )
        {
            if( argumentExpression->getValueToBool() )
            {
                this->m_jumpCheckInterfaceID = true;
                SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "set jumpCheckInterfaceID : true");
            }
            else
            {
                this->m_jumpCheckInterfaceID = false;
                SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "set jumpCheckInterfaceID : false");
            }
            return true;
        }
        else if( argumentExpression->getName().compare( ARG_JUMP_CHECK_SELECTIVE_ID ) == 0 )
        {
            if( argumentExpression->getValueToBool() )
            {
                this->m_jumpCheckSelectiveID = true;
                SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "set jumpCheckSelectiveID : true");
            }
            else
            {
                this->m_jumpCheckSelectiveID = false;
                SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "set jumpCheckSelectiveID : false");
            }
            return true;
        }
        else if( argumentExpression->getName().compare( ARG_JUMP_CHECK_PRODUCT_ID ) == 0 )
        {
            if( argumentExpression->getValueToBool() )
            {
                this->m_jumpCheckProductID = true;
                SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "set jumpCheckProductID : true");
            }
            else
            {
                this->m_jumpCheckProductID = false;
                SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "set jumpCheckProductID : false");
            }
            return true;
        }
        else if( argumentExpression->getName().compare( ARG_CONFIRM_UPDATE ) == 0 )
        {
            if( argumentExpression->getValueToBool() )
            {
                this->m_confirmUpdate = true;
                SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "set confirmUpdate : true");
            }
            else
            {
                this->m_confirmUpdate = false;
                SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "set confirmUpdate : false");
            }
            return true;
        }
        else if( argumentExpression->getName().compare( ARG_BLOCK_RETRY ) == 0 )
        {
            this->m_blockRetry = argumentExpression->getValueToInt();

            if( m_blockRetry > 0 )
            {
                SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "set block retry : %d", m_blockRetry);
                return true;
            }
            else
            {
                std::string msg = EXCEPTION_TITLE;
                char errorMsg[1024] = "";
                sprintf(errorMsg, "set block retry FAIL : can't be %d\n", m_blockRetry);
                msg.append(errorMsg);
                throw CTException( msg );
            }
            return true;
        }
        else if( argumentExpression->getName().compare( ARG_ALL_RETRY ) == 0 )
        {
            this->m_allRetry = argumentExpression->getValueToInt();

            if( m_allRetry > 0 )
            {
                SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "set all retry : %d", m_allRetry);
                return true;
            }
            else
            {
                std::string msg = EXCEPTION_TITLE;
                char errorMsg[1024] = "";
                sprintf(errorMsg, "set all retry FAIL : can't be %d\n", m_allRetry);
                msg.append(errorMsg);
                throw CTException( msg );
            }
            return true;
        }
        else if( argumentExpression->getName().compare( ARG_RAM_SIZE ) == 0 )
        {
            this->m_ramSize = argumentExpression->getValueToInt();

            if( m_ramSize > 0 )
            {
                SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "set ram size : %d", m_ramSize);
                return true;
            }
            else
            {
                std::string msg = EXCEPTION_TITLE;
                char errorMsg[1024] = "";
                sprintf(errorMsg, "set ram size FAIL : can't be %d\n", m_ramSize);
                msg.append(errorMsg);
                throw CTException( msg );
            }
            return true;
        }
    }

    /* super */
    if( BinFileParameter::setArgument(argumentExpression) )
    {
        return true;
    }

    return false;
}

bool
UpdateFWParameter::hasArgumentConflict()
{
    /* super */
    if( BinFileParameter::hasArgumentConflict() )
    {
        return true;
    }

    /* this */
    if(m_updateRegmem && m_updateBootloader)
    {
        SIS_LOG_E(SiSLog::getOwnerSiS(), TAG, "-r and -b is conflict");
        return true;
    }

    if(m_updateRegmem && m_reserveROData)
    {
        SIS_LOG_E(SiSLog::getOwnerSiS(), TAG, "-r and -g is conflict");
        return true;
    }

    return false;
}

bool
UpdateFWParameter::getUpdateBootloader()
{
    return this->m_updateBootloader;
}

bool
UpdateFWParameter::getUpdateBootloaderAuto()
{
    return this->m_updateBootloaderAuto;
}

bool
UpdateFWParameter::getReserveROData()
{
    return this->m_reserveROData;
}

bool
UpdateFWParameter::getUpdateRegmem()
{
    return this->m_updateRegmem;
}

bool
UpdateFWParameter::getForceUpdate()
{
    return this->m_forceUpdate;
}

bool
UpdateFWParameter::getDisableSoftReset()
{
    return this->m_disableSoftReset;
}

bool
UpdateFWParameter::getDisableStamp()
{
    return this->m_disableStamp;
}

bool
UpdateFWParameter::getJumpCheckAll()
{
    return this->m_jumpCheckAll;
}

bool
UpdateFWParameter::getJumpCheckBinSize()
{
    return this->m_jumpCheckBinSize;
}

bool
UpdateFWParameter::getJumpCheckDeviceType()
{
    return this->m_jumpCheckDeviceType;
}

bool
UpdateFWParameter::getJumpCheckInterfaceID()
{
    return this->m_jumpCheckInterfaceID;
}

bool
UpdateFWParameter::getJumpCheckSelectiveID()
{
    return this->m_jumpCheckSelectiveID;
}

bool
UpdateFWParameter::getJumpCheckProductID()
{
    return this->m_jumpCheckProductID;
}

bool
UpdateFWParameter::getConfirmUpdate()
{
    return this->m_confirmUpdate;
}

int
UpdateFWParameter::getBlockRetry()
{
    return this->m_blockRetry;
}

int
UpdateFWParameter::getAllRetry()
{
    return this->m_allRetry;
}

int
UpdateFWParameter::getRamSize()
{
    return this->m_ramSize;
}

void
UpdateFWParameter::setUpdateBootloader(bool updateBootloader)
{
    this->m_updateBootloader = updateBootloader;
}

void
UpdateFWParameter::setUpdateBootloaderAuto(bool updateBootloaderAuto)
{
    this->m_updateBootloaderAuto = updateBootloaderAuto;
}

void
UpdateFWParameter::setReserveROData(bool reserveROData)
{
    this->m_reserveROData = reserveROData;
}

void
UpdateFWParameter::setUpdateRegmem(bool updateRegmem)
{
    this->m_updateRegmem = updateRegmem;
}

void
UpdateFWParameter::setForceUpdate(bool forceUpdate)
{
    this->m_forceUpdate = forceUpdate;
}

void
UpdateFWParameter::setDisableSoftReset(bool disableSoftReset)
{
    this->m_disableSoftReset = disableSoftReset;
}

void
UpdateFWParameter::setDisableStamp(bool disableStamp)
{
    this->m_disableStamp = disableStamp;
}

void
UpdateFWParameter::setJumpCheckAll(bool jumpCheckAll)
{
    this->m_jumpCheckAll = jumpCheckAll;
}

void
UpdateFWParameter::setJumpCheckBinSize(bool jumpCheckBinSize)
{
    this->m_jumpCheckBinSize = jumpCheckBinSize;
}

void
UpdateFWParameter::setJumpCheckDeviceType(bool jumpCheckDeviceType)
{
    this->m_jumpCheckDeviceType = jumpCheckDeviceType;
}

void
UpdateFWParameter::setJumpCheckInterfaceID(bool jumpCheckInterfaceID)
{
    this->m_jumpCheckInterfaceID = jumpCheckInterfaceID;
}

void
UpdateFWParameter::setJumpCheckSelectiveID(bool jumpCheckSelectiveID)
{
    this->m_jumpCheckSelectiveID = jumpCheckSelectiveID;
}

void
UpdateFWParameter::setJumpCheckProductID(bool jumpCheckProductID)
{
    this->m_jumpCheckProductID = jumpCheckProductID;
}

void
UpdateFWParameter::setConfirmUpdate(bool confirmUpdate)
{
    this->m_confirmUpdate = confirmUpdate;
}

void
UpdateFWParameter::setBlockRetry(int blockRetry)
{
    this->m_blockRetry = blockRetry;
}

void
UpdateFWParameter::setAllRetry(int allRetry)
{
    this->m_allRetry = allRetry;
}

void
UpdateFWParameter::setRamSize(int ramSize)
{
    this->m_ramSize = ramSize;
}
