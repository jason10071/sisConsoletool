#ifndef ROMBURNFLAG_H
#define ROMBURNFLAG_H

#include "sisproceduredllexportdefine.h"

namespace SiS
{
namespace Procedure
{

class SISPROCEDURE_EXPORT RomBurnFlag
{
public:
    enum UpdateReserve
    {
        UR_UPDATE = 0,
        UR_RESERVE,
        UR_COUNT,
    };

    enum RomFlag
    {
        RF_CLEAR_BOOT_FLAG = 0,
        RF_BOOTLOADER,          // BootCode + UpdateCode
        RF_MAIN_CODE,           // MainCode_(1 + 2 + ...)
        RF_RODATA_CODE,
        RF_FW_INFO,             // info 1
        RF_REG_MEM,             // info 2
        RF_DEF_MEM,             // info 3
        RF_THQA,                // info 4
        RF_USB_DEV_DESC,        // info 5
        RF_USB_RPT_DESC,        // info 6
        RF_COUNT,
    };

    RomBurnFlag();
    ~RomBurnFlag();

    bool getFlag(UpdateReserve ur, RomFlag rf);
    void setFlag(UpdateReserve ur, RomFlag rf, bool flag);

private:
    void init();

private:
    bool m_flag[UR_COUNT][RF_COUNT];
};

} // Procedure
} // SiS

#endif // ROMBURNFLAG_H
