#ifndef SISPROCEDURE_817_R128K_H
#define SISPROCEDURE_817_R128K_H

#include "sisprocedure/817/sisprocedure_817_r64k.h"

namespace SiS
{
namespace Procedure
{

class SiSProcedure_817_r128k : public SiSProcedure_817_r64k
{
public:
    explicit SiSProcedure_817_r128k();
    virtual ~SiSProcedure_817_r128k();

    virtual IAttributeReader* generateAttributeReader(BinWrap* binWrap = 0);
    virtual IAttributeWriter* generateAttributeWriter(BinWrap* binWrap = 0);
    virtual IRomBDArranger* generateRomBDArranger(IRomSegmentBD::BurnDump burnDump, IRomBDArranger::RomBDPolicy romBDPolicy);
};

} // Procedure
} // SiS

#endif // SISPROCEDURE_817_R128K_H
