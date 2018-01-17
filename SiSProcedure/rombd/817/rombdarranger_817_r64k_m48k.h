#ifndef ROMBDARRANGER_817_R64K_M48K_H
#define ROMBDARRANGER_817_R64K_M48K_H

#include "rombd/817/rombdarranger_817_r64k.h"

namespace SiS
{
namespace Procedure
{

class RomBDArranger_817_r64k_m48k : public RomBDArranger_817_r64k
{
public:
    explicit RomBDArranger_817_r64k_m48k(SiSDeviceIO* sisDeviceIO, IRomSegmentBD::BurnDump burnDump, RomBDPolicy romBDPolicy);
    virtual ~RomBDArranger_817_r64k_m48k();

//    virtual void exec(unsigned char* binBuffer, int binSize);

protected:
    virtual void createRomLayout();
    virtual IRomSegmentBD* generateRomSegmentBD(IRomSegmentBD::BurnDump burnDump, IRomSegmentBD::RomSegment romSegment);
//    virtual void prepareClearBootFlag();
//    virtual void prepareClearCalibrationFlag();
    virtual void preparePolicyComplete();
//    virtual void preparePolicyBootloader();
    virtual void preparePolicyMain();
//    virtual void preparePolicyMainWithReservePartial();
//    virtual void preparePolicyMainPartial();
};

} // Procedure
} // SiS

#endif // ROMBDARRANGER_817_R64K_M48K_H
