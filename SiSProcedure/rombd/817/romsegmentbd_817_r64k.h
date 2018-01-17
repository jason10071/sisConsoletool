#ifndef ROMSEGMENTBD_817_R64K_H
#define ROMSEGMENTBD_817_R64K_H

#include "rombd/iromsegmentbd.h"

#define SYMBOL_CLEAR_BOOT_FLAG  "[C]"
#define SYMBOL_INDEPENDENT      "[I]"
#define SYMBOL_DEPENDENT        "[D]"

namespace SiS
{
namespace Procedure
{

class RomSegmentBD_817_r64k : public IRomSegmentBD
{
public:
    explicit RomSegmentBD_817_r64k(SiSDeviceIO* sisDeviceIO, BurnDump burnDump, RomSegment romSegment);
    virtual ~RomSegmentBD_817_r64k();

    virtual void exec(unsigned char* binBuffer, int binSize);

protected:
    virtual IAddressProvider* getAddressProvider();
    virtual std::string getAddressProviderTableName();
    virtual IAttributeReader* generateAttributeReader();
    virtual int getMaxBlockSize();
    virtual int mappingChipAddress(int chipIndex);
    virtual void execClearBootFlag(unsigned char* binBuffer, int binSize);
    virtual void execClearCalibrationFlag(int binSize);
    virtual void execIndependent(unsigned char* binBuffer, int binSize);
    virtual void execDependent(unsigned char* binBuffer, int binSize);
};

} // Procedure
} // SiS

#endif // ROMSEGMENTBD_817_R64K_H
