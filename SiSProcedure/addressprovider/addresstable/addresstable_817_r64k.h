#ifndef ADDRESSTABLE_817_R64K
#define ADDRESSTABLE_817_R64K

/*==============================================================================
* Note 1:
* [format]
* I. Comment : # comment[' '|'\t']\n
* II. AddressAttribute : KEY[' '|'\t']ADDRESS[' '|'\t']LENGTH{'k'}[' '|'\t']\n
*
* Note 2: base addressattribute
* ============================================================================*/
#define ADDRESS_TABLE_817_R64K \
"\
# FW Memory Layout                                                          \n\
FML_BOOT_CODE_ROM                               0x0000          4k          \n\
FML_BOOT_CODE_XRAM                              0xa0000000      4k          \n\
FML_MAIN_CODE_1_ROM                             0x1000          44k         \n\
FML_MAIN_CODE_1_XRAM                            0xa0001000      44k         \n\
FML_MAIN_COMBINE_INFO_REG_DEF_THQA_DEV_RPT_ROM  0xc000          4k          \n\
FML_MAIN_COMBINE_INFO_REG_DEF_THQA_DEV_RPT_XRAM 0xa000c000      4k          \n\
FML_FW_INFO_ROM                                 0xc000          256         \n\
FML_FW_INFO_XRAM                                0xa000c000      256         \n\
FML_REG_MEM_ROM                                 0xc100          2048        \n\
FML_REG_MEM_XRAM                                0xa000c100      2048        \n\
FML_THQA_ROM                                    0xc900          288         \n\
FML_THQA_XRAM                                   0xa000c900      288         \n\
FML_USB_DEV_DESC_ROM                            0xca20          288         \n\
FML_USB_DEV_DESC_XRAM                           0xa000ca20      288         \n\
FML_USB_RPT_DESC_ROM                            0xcb40          1216        \n\
FML_USB_RPT_DESC_XRAM                           0xa000cb40      1216        \n\
FML_RODATA_CODE_ROM                             0xd000          8k          \n\
FML_RODATA_CODE_XRAM                            0xa000d000      8k          \n\
FML_UPDATE_CODE_ROM                             0xf000          4k          \n\
FML_UPDATE_CODE_XRAM                            0xa000f000      4k          \n\
FML_CLEAR_BOOT_FLAG_ROM                         0xe000          4k          \n\
FML_CLEAR_BOOT_FLAG_XRAM                        0xa000e000      4k          \n\
FML_CLEAR_CALIBRATION_FLAG_ROM                  0xe000          4k          \n\
FML_CLEAR_CALIBRATION_FLAG_XRAM                 0xa000e000      4k          \n\
\
# FW Attribute                                                  \n\
FA_UPDATE_FW_INFO_ROM               0xe000          6           \n\
FA_BOOT_FLAG_ROM                    0xeff0          4           \n\
FA_BOOT_FLAG_XRAM                   0xa000eff0      4           \n\
FA_0X0238_ROM                       0x0238          1           \n\
FA_0X0238_XRAM                      0xa0000238      1           \n\
FA_0XC04C_ROM                       0xc04c          1           \n\
FA_0XC04C_XRAM                      0xa000c04c      1           \n\
FA_INTERFACE_ID_ROM                 0xc026          1           \n\
FA_INTERFACE_ID_XRAM                0xa000c026      1           \n\
FA_MULTI_CHIP_SELECTIVE_ID_ROM      0xc027          1           \n\
FA_MULTI_CHIP_SELECTIVE_ID_XRAM     0xa000c027      1           \n\
FA_MULTI_SLAVE_NUMBER_ROM           0xc028          1           \n\
FA_MULTI_SLAVE_NUMBER_XRAM          0xa000c028      1           \n\
FA_CALIBRATION_FLAG_ROM             0xeffc          4           \n\
FA_CALIBRATION_FLAG_XRAM            0xa000effc      4           \n\
FA_0X0200_ROM                       0x0200          6           \n\
FA_0X0200_XRAM                      0xa0000200      6           \n\
FA_0X020C_ROM                       0x020c          4           \n\
FA_0X020C_XRAM                      0xa000020c      4           \n\
FA_FIRMWARE_ID_ROM                  0xc004          12          \n\
FA_FIRMWARE_ID_XRAM                 0xa000c004      12          \n\
FA_BOOTLOADER_ID_ROM                0x0230          4           \n\
FA_BOOTLOADER_ID_XRAM               0xa0000230      4           \n\
FA_BOOT_CODE_CRC_ROM                0x0234          4           \n\
FA_BOOT_CODE_CRC_XRAM               0xa0000234      4           \n\
FA_MAIN_CODE_CRC_ROM                0xc044          4           \n\
FA_MAIN_CODE_CRC_XRAM               0xa000c044      4           \n\
FA_LAST_ID_ROM                      0xc0a0          12          \n\
FA_LAST_ID_XRAM                     0xa000c0a0      12          \n\
FA_LAST_TIME_ROM                    0xc0ac          4           \n\
FA_LAST_TIME_XRAM                   0xa000c0ac      4           \n\
FA_PRIOR_LAST_ID_ROM                0xc0b0          12          \n\
FA_PRIOR_LAST_ID_XRAM               0xa000c0b0      12          \n\
FA_PRIOR_LAST_TIME_ROM              0xc0bc          4           \n\
FA_PRIOR_LAST_TIME_XRAM             0xa000c0bc      4           \n\
FA_MAGIC_0X0200_XRAM                0xa0000200      16          \n\
FA_MAGIC_0X10200_XRAM               0xa0010200      16          \n\
FA_DEVICE_BUILD_INFO_XRAM           0xa0000200      48          \n\
FA_DEVICE_FW_INFO_XRAM              0xa000bfe0      112         \n\
FA_TASK_ID_ROM                      0xc00c          2           \n\
FA_TASK_ID_XRAM                     0xa000c00c      2           \n\
FA_FW_VERSION_ROM                   0xc00e          2           \n\
FA_FW_VERSION_XRAM                  0xa000c00e      2           \n\
FA_UPDATE_MARK_ROM                  0xc000          4           \n\
FA_UPDATE_MARK_XRAM                 0xa000c000      4           \n\
FA_IS_UPDATE_BOOTLOADER_INFO_ROM    0xc0d6          2           \n\
FA_IS_UPDATE_BOOTLOADER_INFO_XRAM   0xa000c0d6      2           \n\
\
";
//==============================================================================

#endif // ADDRESSTABLE_817_R64K

