#ifndef ROMBDARRANGER_817_R128K_H
#define ROMBDARRANGER_817_R128K_H

#include "rombd/817/rombdarranger_817_r64k.h"

namespace SiS
{
namespace Procedure
{

class RomBDArranger_817_r128k : public RomBDArranger_817_r64k
{
public:
    explicit RomBDArranger_817_r128k(SiSDeviceIO* sisDeviceIO, IRomSegmentBD::BurnDump burnDump, RomBDPolicy romBDPolicy);
    virtual ~RomBDArranger_817_r128k();

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

#endif // ROMBDARRANGER_817_R128K_H
