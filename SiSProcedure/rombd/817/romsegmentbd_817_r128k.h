#ifndef ROMSEGMENTBD_817_R128K_H
#define ROMSEGMENTBD_817_R128K_H

#include "rombd/817/romsegmentbd_817_r64k.h"

namespace SiS
{
namespace Procedure
{

class RomSegmentBD_817_r128k : public RomSegmentBD_817_r64k
{
public:
    explicit RomSegmentBD_817_r128k(SiSDeviceIO* sisDeviceIO, BurnDump burnDump, RomSegment romSegment);
    virtual ~RomSegmentBD_817_r128k();

protected:
    virtual IAddressProvider* getAddressProvider();
    virtual std::string getAddressProviderTableName();
    virtual IAttributeReader* generateAttributeReader();
    virtual int getMaxBlockSize();
};

} // Procedure
} // SiS

#endif // ROMSEGMENTBD_817_R128K_H
