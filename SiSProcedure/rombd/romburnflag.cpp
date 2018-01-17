#include "romburnflag.h"

using namespace SiS::Procedure;

RomBurnFlag::RomBurnFlag()
{
    init();
}

RomBurnFlag::~RomBurnFlag()
{

}

void
RomBurnFlag::init()
{
    for( int ur = UR_UPDATE; ur < UR_COUNT; ur++ )
    {
        for( int rf = RF_CLEAR_BOOT_FLAG; rf < RF_COUNT; rf++ )
        {
            this->m_flag[ur][rf] = false;
        }
    }
}

bool
RomBurnFlag::getFlag(UpdateReserve ur, RomFlag rf)
{
    if( ur >= UR_COUNT || ur < UR_UPDATE || rf >= RF_COUNT || rf < RF_CLEAR_BOOT_FLAG )
    {
        return false; // error range
    }

    return this->m_flag[ur][rf];
}

void
RomBurnFlag::setFlag(UpdateReserve ur, RomFlag rf, bool flag)
{
    if( ur >= UR_COUNT || ur < UR_UPDATE || rf >= RF_COUNT || rf < RF_CLEAR_BOOT_FLAG )
    {
        return; // error range
    }

    this->m_flag[ur][rf] = flag;
}

