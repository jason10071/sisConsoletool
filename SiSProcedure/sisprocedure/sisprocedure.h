#ifndef SISPROCEDURE_H
#define SISPROCEDURE_H

#include "sisproceduredllexportdefine.h"
#include "sisprocedure/isisprocedure.h"
#include "sisprocedure/817/sisprocedure_817_r64k.h"
#include "sisprocedure/817/sisprocedure_817_r64k_m48k.h"
#include "sisprocedure/817/sisprocedure_817_r128k.h"
#include "sisprocedure/819/sisprocedure_819_r128k.h"

using namespace SiS::DeviceIO;

namespace SiS
{
namespace Procedure
{

class SISPROCEDURE_EXPORT SiSProcedure : public ISiSProcedure
{
public:
    explicit SiSProcedure();
    virtual ~SiSProcedure();

    void switchSiSProcedure(DeviceType deviceType);
    void switchSiSProcedure(FWSizeType fwSizeType);

    virtual void setSiSCore(SiSDeviceIO* sisDeviceIO);
    virtual void setBinWrapMap(std::map<ChipIndexKey, BinWrap*> binWrapMap);
    virtual void setCtlReportToOs(bool ctlReportToOsFlag);

    virtual IAttributeReader* generateAttributeReader(BinWrap* binWrap = 0);
    virtual IAttributeWriter* generateAttributeWriter(BinWrap* binWrap = 0);
    virtual IRomBDArranger* generateRomBDArranger(IRomSegmentBD::BurnDump burnDump, IRomBDArranger::RomBDPolicy romBDPolicy);

    virtual void disableCtlReportToOs();
    virtual void enableCtlReportToOs();
    virtual void jumpBootloader(int chipIndex);
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
    virtual SerialData* readUpdateMark(ReferenceSource rs, int chipIndex);
    virtual SerialData* readLastUpdateMark(ReferenceSource rs, int chipIndex);
    virtual SerialData* readPriorLastUpdateMark(ReferenceSource rs, int chipIndex);

    virtual void writeUpdateMark(ReferenceSource rs, int chipIndex, SerialData* serialData);
    virtual void writeIsUpdateBootloaderInfo(ReferenceSource rs, int chipIndex, SerialData* serialData);
    virtual void writeLastUpdateMark(ReferenceSource rs, int chipIndex, SerialData* serialData);
    virtual void writePriorLastUpdateMark(ReferenceSource rs, int chipIndex, SerialData* serialData);
    virtual void writeLastID(ReferenceSource rs, int chipIndex, SerialData* serialData);
    virtual void writeLastTime(ReferenceSource rs, int chipIndex, SerialData* serialData);
    virtual void writePriorLastID(ReferenceSource rs, int chipIndex, SerialData* serialData);
    virtual void writePriorLastTime(ReferenceSource rs, int chipIndex, SerialData* serialData);    
	virtual void writeNoneSiSCmdViaBridge( 
			SerialData * data, const int transitionID);

    virtual void burn(RomBurnFlag romBurnFlag, int chipIndex);
    virtual void clearCalibration(int chipIndex);
    virtual IRomBDArranger::RomBDPolicy getPolicy(RomBurnFlag romBurnFlag);

private:
    SiSProcedure_817_r64k* getSisProcedure_817_r64k();
    SiSProcedure_817_r64k_m48k* getSisProcedure_817_r64k_m48k();
    SiSProcedure_817_r128k* getSisProcedure_817_r128k();
    SiSProcedure_819_r128k* getSisProcedure_819_r128k();

private:
    ISiSProcedure* m_iSiSProcedure;
    SiSProcedure_817_r64k* m_sisProcedure_817_r64k;
    SiSProcedure_817_r64k_m48k* m_sisProcedure_817_r64k_m48k;
    SiSProcedure_817_r128k* m_sisProcedure_817_r128k;
    SiSProcedure_819_r128k* m_sisProcedure_819_r128k;
};

} // Procedure
} // SiS

#endif // SISPROCEDURE_H
