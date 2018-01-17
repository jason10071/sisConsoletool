#include "iromsegmentbd.h"

using namespace SiS::DeviceIO;
using namespace SiS::Procedure;

IRomSegmentBD::IRomSegmentBD(SiSDeviceIO* sisDeviceIO, BurnDump burnDump, RomSegment romSegment) :
    m_sisDeviceIO(sisDeviceIO),
    m_burnDump(burnDump),
    m_romSegment(romSegment),
    m_addressProvider(0),
    m_chipIndex(CI_MASTER),
    m_clearBootFlagWithReserveROData(false)
{

}

IRomSegmentBD::~IRomSegmentBD()
{
    if( m_addressProvider != 0 )
    {
        delete m_addressProvider;
        m_addressProvider = 0;
    }
}

void
IRomSegmentBD::setChipIndex(int chipIndex)
{
    this->m_chipIndex = chipIndex;
}

void
IRomSegmentBD::setClearBootFlagWithReserveROData(bool clearBootFlagWithReserveROData)
{
    this->m_clearBootFlagWithReserveROData = clearBootFlagWithReserveROData;
}

void
IRomSegmentBD::addRomSegmentFission(RomSegmentFission romSegmentFission)
{
    m_romSegmentFissions.push_back(romSegmentFission);
}

std::string
IRomSegmentBD::getRomSegmentKey(BurnDump burnDump, RomSegment romSegment)
{
    switch (romSegment) {
    case RS_BOOT_CODE:
        return (burnDump == BD_BURN_TO_ROM ? "FML_BOOT_CODE_ROM" : "FML_BOOT_CODE_XRAM");
        break;
    case RS_UPDATE_CODE:
        return (burnDump == BD_BURN_TO_ROM ? "FML_UPDATE_CODE_ROM" : "FML_UPDATE_CODE_XRAM");
        break;
    case RS_MAIN_CODE_1:
        return (burnDump == BD_BURN_TO_ROM ? "FML_MAIN_CODE_1_ROM" : "FML_MAIN_CODE_1_XRAM");
        break;
    case RS_MAIN_CODE_2:
        return (burnDump == BD_BURN_TO_ROM ? "FML_MAIN_CODE_2_ROM" : "FML_MAIN_CODE_2_XRAM");
        break;
    case RS_MAIN_COMBINE_INFO_REG_DEF_THQA_DEV_RPT:
        return (burnDump == BD_BURN_TO_ROM ? "FML_MAIN_COMBINE_INFO_REG_DEF_THQA_DEV_RPT_ROM" : "FML_MAIN_COMBINE_INFO_REG_DEF_THQA_DEV_RPT_XRAM");
        break;
    case RS_MAIN_COMBINE_THQA_DEV_RPT:
        return (burnDump == BD_BURN_TO_ROM ? "FML_MAIN_COMBINE_THQA_DEV_RPT_ROM" : "FML_MAIN_COMBINE_THQA_DEV_RPT_XRAM");
        break;
    case RS_MAIN_COMBINE_INFO_REG:
        return (burnDump == BD_BURN_TO_ROM ? "FML_MAIN_COMBINE_INFO_REG_ROM" : "FML_MAIN_COMBINE_INFO_REG_XRAM");
        break;
    case RS_RODATA_CODE:
        return (burnDump == BD_BURN_TO_ROM ? "FML_RODATA_CODE_ROM" : "FML_RODATA_CODE_XRAM");
        break;
    case RS_CLEAR_BOOT_FLAG:
        return (burnDump == BD_BURN_TO_ROM ? "FML_CLEAR_BOOT_FLAG_ROM" : "FML_CLEAR_BOOT_FLAG_XRAM");
        break;
    case RS_CLEAR_CALIBRATION_FLAG:
        return (burnDump == BD_BURN_TO_ROM ? "FML_CLEAR_CALIBRATION_FLAG_ROM" : "FML_CLEAR_CALIBRATION_FLAG_XRAM");
        break;
    case RS_REMAIN_SECTION:
        return (burnDump == BD_BURN_TO_ROM ? "FML_REMAIN_SECTION_ROM" : "FML_REMAIN_SECTION_XRAM");
        break;
    default:
        return "UNKNOW_RomSegment";
        break;
    }
}
