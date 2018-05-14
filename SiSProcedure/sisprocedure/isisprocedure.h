#ifndef ISISPROCEDURE_H
#define ISISPROCEDURE_H

#include <string.h>
#include "sisproceduredllexportdefine.h"
#include "sisdeviceio/sisdeviceio.h"
#include "spbase/sisproceduredefine.h"
#include "attributereader/iattributereader.h"
#include "attributewriter/iattributewriter.h"
#include "binloader/binwrap.h"
#include "sisprocedure/serialdata.h"
#include "rombd/romburnflag.h"
#include "rombd/irombdarranger.h"

using namespace SiS::DeviceIO;

namespace SiS
{
namespace Procedure
{

class SISPROCEDURE_EXPORT ISiSProcedure
{
public:
    explicit ISiSProcedure();
    virtual ~ISiSProcedure();

    SiSDeviceIO* getSiSDeviceIO();

    virtual IAttributeReader* generateAttributeReader(BinWrap* binWrap = 0) = 0;
    virtual IAttributeWriter* generateAttributeWriter(BinWrap* binWrap = 0) = 0;
    virtual IRomBDArranger* generateRomBDArranger(IRomSegmentBD::BurnDump burnDump, IRomBDArranger::RomBDPolicy romBDPolicy) = 0;

    virtual void setSiSCore(SiSDeviceIO* m_sisDeviceIO);
    virtual void setBinWrapMap(std::map<ChipIndexKey, BinWrap*> binWrapMap);
    virtual void setCtlReportToOs(bool ctlReportToOsFlag);

    /* ctrl function */
    virtual std::string openDevice(std::string deviceID);
    virtual std::string reOpenDevice(std::string deviceIDOpened, int waitResetSec, bool isCheckDeviceBack = false);
    virtual void disableCtlReportToOs() = 0;
    virtual void enableCtlReportToOs() = 0;
    virtual void jumpBootloader(int chipIndex) = 0;
    virtual void softReset() = 0;
    virtual void reCalibration(int waitResetSec) = 0;
    virtual void resetDevice(int waitResetSec, bool isCheckDeviceBack = false);

    /* read function */
    virtual std::string readPID();
    virtual std::string readVID();
    virtual DeviceType readOSDeviceType();
    virtual DeviceType readSubProtocol();
    virtual DeviceInterface readOSDeviceInterface();
    virtual bool readBroken(ReferenceSource rs, int chipIndex) = 0;
    virtual FWSizeType readFWSizeType(ReferenceSource rs, int chipIndex, bool isBroken) = 0;
    virtual unsigned char readInterfaceID(ReferenceSource rs, int chipIndex) = 0;
    virtual int readSlaveNum(ReferenceSource rs) = 0;
    virtual unsigned char readMultiChipSelectiveID(ReferenceSource rs, int chipIndex) = 0;
    virtual bool readCalibrationFlag(ReferenceSource rs, int chipIndex) = 0;
    virtual SerialData* readFirmwareID(ReferenceSource rs, int chipIndex) = 0;
    virtual SerialData* readBootloaderID(ReferenceSource rs, int chipIndex) = 0;
    virtual SerialData* readBootCodeCRC(ReferenceSource rs, int chipIndex) = 0;
    virtual SerialData* readMainCodeCRC(ReferenceSource rs, int chipIndex) = 0;
    virtual SerialData* readLastID(ReferenceSource rs, int chipIndex) = 0;
    virtual SerialData* readLastTime(ReferenceSource rs, int chipIndex) = 0;
    virtual SerialData* readPriorLastID(ReferenceSource rs, int chipIndex) = 0;
    virtual SerialData* readPriorLastTime(ReferenceSource rs, int chipIndex) = 0;
    virtual SerialData* readProductID(ReferenceSource rs, int chipIndex) = 0;
    virtual SerialData* readTaskID(ReferenceSource rs, int chipIndex) = 0;
    virtual FwVersion readFwVersion(ReferenceSource rs, int chipIndex) = 0;
    virtual FWSizeType readRomFWSizeType(int chipIndex) = 0;
    DeviceType readBinDeviceType(int chipIndex);
    virtual SerialData* readXramAttribute(std::string key, int chipIndex) = 0;
    virtual SerialData* readXramAddress(unsigned int address, int dataSize, int chipIndex) = 0;
	virtual SerialData* readNoneSiSCmdViaBridge( SerialData * readcommand, const size_t len, const int transitionID) = 0;
    virtual SerialData* readUpdateMark(ReferenceSource rs, int chipIndex) = 0;
    virtual SerialData* readLastUpdateMark(ReferenceSource rs, int chipIndex) = 0;
    virtual SerialData* readPriorLastUpdateMark(ReferenceSource rs, int chipIndex) = 0;

    /* write function */
    virtual void writeUpdateMark(ReferenceSource rs, int chipIndex, SerialData* serialData) = 0;
    virtual void writeIsUpdateBootloaderInfo(ReferenceSource rs, int chipIndex, SerialData* serialData) = 0;
    virtual void writeLastUpdateMark(ReferenceSource rs, int chipIndex, SerialData* serialData) = 0;
    virtual void writePriorLastUpdateMark(ReferenceSource rs, int chipIndex, SerialData* serialData) = 0;
    virtual void writeLastID(ReferenceSource rs, int chipIndex, SerialData* serialData) = 0;
    virtual void writeLastTime(ReferenceSource rs, int chipIndex, SerialData* serialData) = 0;
    virtual void writePriorLastID(ReferenceSource rs, int chipIndex, SerialData* serialData) = 0;
    virtual void writePriorLastTime(ReferenceSource rs, int chipIndex, SerialData* serialData) = 0;
	virtual void writeNoneSiSCmdViaBridge( 
			SerialData * data, const int transitionID) = 0;

    /* burn function */
    virtual void burn(RomBurnFlag romBurnFlag, int chipIndex) = 0;
    virtual void clearCalibration(int chipIndex) = 0;
    virtual IRomBDArranger::RomBDPolicy getPolicy(RomBurnFlag romBurnFlag) = 0;

    /* unit function */
    bool compareData(unsigned char* dataA, int sizeA, unsigned char* dataB, int sizeB);
    static SerialData* getTimestamp();
    static std::string getFSTStr(FWSizeType fwSizeType);
    static std::string getDTStr(DeviceType deviceType);
    static std::string getDIStr(DeviceInterface deviceInterface);
    static std::string getRSStr(ReferenceSource rs);
    static std::string getCIStr(int chipIndex);

    /* get/set */
    int getIOInterval();
    void setIOInterval(int ioInterval);

    int getIOIntervalOf81();
    void setIOIntervalOf81(int ioIntervalOf81);

    int getBlockRetry();
    void setBlockRetry(int blockRetry);

    int getAllRetry();
    void setAllRetry(int allRetry);

    int getRamSize();
    void setRamSize(int ramSize);

protected:
    BinWrap* getBinWrap(int chipIndex);

protected:
    SiSDeviceIO* m_sisDeviceIO;
    std::map<ChipIndexKey, BinWrap*> m_binWrapMap;
    bool m_ctlReportToOsFlag;
    int m_ioInterval;
    int m_iOIntervalOf81;
    int m_blockRetry;
    int m_allRetry;
    int m_ramSize;
};

} // Procedure
} // SiS

#endif // ISISPROCEDURE_H
