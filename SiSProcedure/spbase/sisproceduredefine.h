#ifndef SISPROCEDUREDEFINE
#define SISPROCEDUREDEFINE

#define DEFAULT_CTL_REPORT_TO_OS    false
#define DEFAULT_BYTE_DATA           0xde
#define DEFAULT_BROKEN              true
#define DEFAULT_CALIBRATION_FLAG    false

/* device interface */
#define DEVICE_INTERFACE_I2C        "I2C"
#define DEVICE_INTERFACE_HID_I2C    "HID_I2C"
#define DEVICE_INTERFACE_HID_USB    "HID_USB"
#define DEVICE_INTERFACE_DUMMY      "DUMMY"
#define INTERFACE_ID_I2C            0x1
#define INTERFACE_ID_USB            0x2
#define INTERFACE_ID_SPI_819        0x3
#define INTERFACE_ID_USB_I2C_819    0x4
#define INTERFACE_ID_DUMMY_817      0x3
#define INTERFACE_ID_DUMMY_819      0xdd

/* device selective ID */
#define NON_MULTI_DEVICE_FLAG   0x0
//#define SLAVE_DEVICE0_FLAG      0x5
//#define SLAVE_DEVICE1_FLAG      0x6
//#define SLAVE_DEVICE2_FLAG      0x7
#define MASTER_DEVICE_FLAG      0x1
#define BRIDGE_DEVICE_FLAG      0xb

/* size */
#define _64K            0x10000
#define LOW_BOUND_64K   0x1
#define _128K           0x20000
#define LOW_BOUND_128K  0x10001
#define _4K             0x1000
#define _8K             0x2000
#define _12K            0x3000
#define _16K            0x4000

namespace SiS
{
namespace Procedure
{

typedef int ChipIndexKey;

struct FwVersion
{
    int major;
    int minor;
};

enum ReferenceSource
{
    RS_BIN = 0,
    RS_ROM,
    RS_XRAM,
    RS_COUNT,
};

enum DeviceType
{
    DT_INVALID = 0,
    DT_817,
    DT_819,
    DT_BRIDGE,
};

enum DeviceInterface
{
    DI_INVALID = 0,
    DI_I2C,
    DI_HID_I2C,
    DI_HID_USB,
    DI_SPI,
    DI_DUMMY,
};

enum FWSizeType
{
    FST_INVALID = 0,
    FST_817_R64K,
    FST_817_R64K_M48K,
    FST_817_R128K,
    FST_819_R128K,
    FST_R64K,
    FST_R128K,
};

enum ChipIndex
{
    CI_MASTER = 0,
    CI_SLAVE_0 = 1,
};

} // Procedure
} // SiS

#endif // SISPROCEDUREDEFINE

