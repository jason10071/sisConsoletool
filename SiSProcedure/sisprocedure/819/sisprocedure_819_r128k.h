#ifndef SISPROCEDURE_819_R128K_H
#define SISPROCEDURE_819_R128K_H

#include "sisprocedure/817/sisprocedure_817_r64k.h"

namespace SiS
{
namespace Procedure
{

class SiSProcedure_819_r128k : public SiSProcedure_817_r64k
{
public:
    explicit SiSProcedure_819_r128k();
    virtual ~SiSProcedure_819_r128k();

    virtual IAttributeReader* generateAttributeReader(BinWrap* binWrap = 0);
    virtual IAttributeWriter* generateAttributeWriter(BinWrap* binWrap = 0);
    virtual IRomBDArranger* generateRomBDArranger(IRomSegmentBD::BurnDump burnDump, IRomBDArranger::RomBDPolicy romBDPolicy);

//    virtual void disableCtlReportToOs();
//    virtual void enableCtlReportToOs();
    virtual void jumpBootloader(int chipIndex);
    virtual void softReset();
//    virtual void reCalibration();

    virtual FWSizeType readFWSizeType(ReferenceSource rs, int chipIndex, bool isBroken);
//    virtual int readSlaveNum(ReferenceSource rs);
    virtual SerialData* readProductID(ReferenceSource rs, int chipIndex);
    virtual FwVersion readFwVersion(ReferenceSource rs, int chipIndex);
    virtual FWSizeType readRomFWSizeType(int chipIndex);
	virtual SerialData* readNoneSiSCmdViaBridge(SerialData * readcommand, const size_t len, const int transitionID);
	virtual void writeNoneSiSCmdViaBridge( 
			SerialData * data, const int transitionID);

    virtual SerialData* readUpdateMark(ReferenceSource rs, int chipIndex);
    virtual SerialData* readLastUpdateMark(ReferenceSource rs, int chipIndex);
    virtual SerialData* readPriorLastUpdateMark(ReferenceSource rs, int chipIndex);
    virtual void writeLastUpdateMark(ReferenceSource rs, int chipIndex, SerialData* serialData);
    virtual void writePriorLastUpdateMark(ReferenceSource rs, int chipIndex, SerialData* serialData);
};

} // Procedure
} // SiS

#endif // SISPROCEDURE_819_R128K_H
