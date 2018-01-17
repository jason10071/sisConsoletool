#ifndef IROMBDARRANGER_H
#define IROMBDARRANGER_H

#include "rombd/iromsegmentbd.h"

using namespace SiS::DeviceIO;

namespace SiS
{
namespace Procedure
{

class IRomBDArranger
{
public:
    enum RomBDPolicy
    {
        RBDP_CLEAR_BOOT_FLAG = 0,
        RBDP_CLEAR_CALIBRATION_FLAG,
        RBDP_COMPLETE,
        RBDP_BOOTLOADER,
        RBDP_MAIN,
        RBDP_MAIN_WITH_RESERVE_PARTIAL,
        RBDP_MAIN_PARTIAL,
    };

    explicit IRomBDArranger(SiSDeviceIO* sisDeviceIO, IRomSegmentBD::BurnDump burnDump, RomBDPolicy romBDPolicy);
    virtual ~IRomBDArranger();

    virtual void exec(unsigned char* binBuffer, int binSize) = 0;

    void setChipIndex(int chipIndex);
    void addMainReservePartial(IRomSegmentBD::RomSegment romSegment);
    void addMainPartial(IRomSegmentBD::RomSegment romSegment);

protected:
    virtual void createRomLayout();
    bool hasRomSegment(IRomSegmentBD::RomSegment romSegment);
    virtual IRomSegmentBD* generateRomSegmentBD(IRomSegmentBD::BurnDump burnDump, IRomSegmentBD::RomSegment romSegment) = 0;
    virtual void prepare();
    virtual void prepareClearBootFlag() = 0;
    virtual void prepareClearCalibrationFlag() = 0;
    virtual void preparePolicyComplete() = 0;
    virtual void preparePolicyBootloader() = 0;
    virtual void preparePolicyMain() = 0;
    virtual void preparePolicyMainWithReservePartial() = 0;
    virtual void preparePolicyMainPartial() = 0;

protected:
    SiSDeviceIO* m_sisDeviceIO;
    IRomSegmentBD::BurnDump m_burnDump;
    RomBDPolicy m_romBDPolicy;
    std::list<IRomSegmentBD::RomSegment> m_romLayout;
    std::list<IRomSegmentBD::RomSegment> m_mainReservePartial;
    std::list<IRomSegmentBD::RomSegment> m_mainPartial;
    std::list<IRomSegmentBD*> m_romSegmentBDQueue;
    int m_chipIndex;

};

} // Procedure
} // SiS

#endif // IROMBDARRANGER_H
