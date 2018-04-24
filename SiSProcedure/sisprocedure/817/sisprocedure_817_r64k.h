#ifndef SISPROCEDURE_817_R64K_H
#define SISPROCEDURE_817_R64K_H

#include "sisprocedure/isisprocedure.h"

namespace SiS
{
namespace Procedure
{

class SiSProcedure_817_r64k : public ISiSProcedure
{
public:
    explicit SiSProcedure_817_r64k();
    virtual ~SiSProcedure_817_r64k();

    virtual IAttributeReader* generateAttributeReader(BinWrap* binWrap = 0);
    virtual IAttributeWriter* generateAttributeWriter(BinWrap* binWrap = 0);
    virtual IRomBDArranger* generateRomBDArranger(IRomSegmentBD::BurnDump burnDump, IRomBDArranger::RomBDPolicy romBDPolicy);

    virtual void disableCtlReportToOs();
    virtual void enableCtlReportToOs();
    virtual void jumpBootloader(int chipIndex) {} // not support
    virtual void softReset();
    virtual void reCalibration(int waitResetSec);

    virtual bool readBroken(ReferenceSource rs, int chipIndex);
    virtual FWSizeType readFWSizeType(ReferenceSource rs, int chipIndex, bool isBroken);
    virtual unsigned char readInterfaceID(ReferenceSource rs, int chipIndex);
    virtual int readSlaveNum(ReferenceSource rs);
    virtual unsigned char readMultiChipSelectiveID(ReferenceSource rs, int chipIndex);
    virtual bool readCalibrationFlag(ReferenceSource rs, int chipIndex);
    virtual SerialData* readFirmwareID(ReferenceSource rs, int chipIndex);
    virtual SerialData* readBootloaderID(ReferenceSource rs, int chipIndex);
    virtual SerialData* readBootCodeCRC(ReferenceSource rs, int chipIndex);
    virtual SerialData* readMainCodeCRC(ReferenceSource rs, int chipIndex);
    virtual SerialData* readLastID(ReferenceSource rs, int chipIndex);
    virtual SerialData* readLastTime(ReferenceSource rs, int chipIndex);
    virtual SerialData* readPriorLastID(ReferenceSource rs, int chipIndex);
    virtual SerialData* readPriorLastTime(ReferenceSource rs, int chipIndex);
    virtual SerialData* readProductID(ReferenceSource rs, int chipIndex);
    virtual SerialData* readTaskID(ReferenceSource rs, int chipIndex);
    virtual FwVersion readFwVersion(ReferenceSource rs, int chipIndex);
    virtual FWSizeType readRomFWSizeType(int chipIndex);
    virtual SerialData* readXramAttribute(std::string key, int chipIndex);
    virtual SerialData* readXramAddress(unsigned int address, int dataSize, int chipIndex);
	virtual SerialData* readNoneSiSCmdViaBridge(SerialData * readcommand, const size_t len, const int transitionID);
    virtual SerialData* readUpdateMark(ReferenceSource rs, int chipIndex) {return 0;} // not support
    virtual SerialData* readLastUpdateMark(ReferenceSource rs, int chipIndex) {return 0;} // not support
    virtual SerialData* readPriorLastUpdateMark(ReferenceSource rs, int chipIndex) {return 0;} // not support

    virtual void writeUpdateMark(ReferenceSource rs, int chipIndex, SerialData* serialData);
    virtual void writeIsUpdateBootloaderInfo(ReferenceSource rs, int chipIndex, SerialData* serialData);
    virtual void writeLastUpdateMark(ReferenceSource rs, int chipIndex, SerialData* serialData) {} // not support
    virtual void writePriorLastUpdateMark(ReferenceSource rs, int chipIndex, SerialData* serialData) {} // not support
    virtual void writeLastID(ReferenceSource rs, int chipIndex, SerialData* serialData);
    virtual void writeLastTime(ReferenceSource rs, int chipIndex, SerialData* serialData);
    virtual void writePriorLastID(ReferenceSource rs, int chipIndex, SerialData* serialData);
    virtual void writePriorLastTime(ReferenceSource rs, int chipIndex, SerialData* serialData);    
	virtual void writeNoneSiSCmdViaBridge( 
			SerialData * data, const int transitionID);
    virtual void burn(RomBurnFlag romBurnFlag, int chipIndex);
    virtual void clearCalibration(int chipIndex);
    virtual IRomBDArranger::RomBDPolicy getPolicy(RomBurnFlag romBurnFlag);
};

} // Procedure
} // SiS

#endif // SISPROCEDURE_817_R64K_H
