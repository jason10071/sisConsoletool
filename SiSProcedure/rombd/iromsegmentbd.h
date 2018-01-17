#ifndef IROMSEGMENTBD_H
#define IROMSEGMENTBD_H

#include "stdio.h"
#include <string>
#include <list>

#include "addressprovider/iaddressprovider.h"
#include "attributereader/iattributereader.h"
#include "sisdeviceio/sisdeviceio.h"
#include "spbase/sisproceduredefine.h"

using namespace SiS::DeviceIO;

namespace SiS
{
namespace Procedure
{

class IRomSegmentBD
{
public:
    enum BurnDump
    {
        BD_BURN_TO_ROM = 0,
        BD_DUMP_FROM_XRAM,
    };

    enum RomSegment
    {
        RS_CLEAR_BOOT_FLAG = 0, // clear boot flag
        RS_CLEAR_CALIBRATION_FLAG, // clear calibration flag
        RS_BOOT_CODE = 10,      // bootloader
        RS_UPDATE_CODE,
        RS_MAIN_CODE_1 = 100,   // main
        RS_MAIN_CODE_2,
        RS_MAIN_COMBINE_INFO_REG_DEF_THQA_DEV_RPT,
        RS_MAIN_COMBINE_THQA_DEV_RPT,
        RS_MAIN_COMBINE_INFO_REG,
        RS_RODATA_CODE,
        RS_REMAIN_SECTION,
    };

    enum RomSegmentFission
    {
        RSF_MAIN_FW_INFO = 0,
        RSF_MAIN_REG_MEM,
        RSF_MAIN_THQA,
        RSF_MAIN_USB_DEV_DESC,
        RSF_MAIN_USB_RPT_DESC,
    };

    explicit IRomSegmentBD(SiSDeviceIO* sisDeviceIO, BurnDump burnDump, RomSegment romSegment);
    virtual ~IRomSegmentBD();

    void setChipIndex(int chipIndex);
    void setClearBootFlagWithReserveROData(bool clearBootFlagWithReserveROData);
    void addRomSegmentFission(RomSegmentFission romSegmentFission);

    virtual void exec(unsigned char* binBuffer, int binSize) = 0;

protected:
    virtual IAddressProvider* getAddressProvider() = 0;
    virtual std::string getAddressProviderTableName() = 0;
    virtual IAttributeReader* generateAttributeReader() = 0;
    virtual int getMaxBlockSize() = 0;
    virtual int mappingChipAddress(int chipIndex) = 0;
    std::string getRomSegmentKey(BurnDump burnDump, RomSegment romSegment);
    virtual void execClearBootFlag(unsigned char* binBuffer, int binSize) = 0;
    virtual void execClearCalibrationFlag(int binSize) = 0;
    virtual void execIndependent(unsigned char* binBuffer, int binSize) = 0;
    virtual void execDependent(unsigned char* binBuffer, int binSize) = 0;

protected:
    SiSDeviceIO* m_sisDeviceIO;
    BurnDump m_burnDump;
    RomSegment m_romSegment;
    std::list<RomSegmentFission> m_romSegmentFissions;
    IAddressProvider* m_addressProvider;
    int m_chipIndex;
    bool m_clearBootFlagWithReserveROData;
};

} // Procedure
} // SiS

#endif // IROMSEGMENTBD_H
