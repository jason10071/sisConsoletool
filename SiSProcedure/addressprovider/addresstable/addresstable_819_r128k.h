#ifndef ADDRESSTABLE_819_R128K
#define ADDRESSTABLE_819_R128K

/*==============================================================================
* Note 1: format same as addresstable_819_r64k.h
*
*
*
*
* Note 2: addressattribute inherit addresstable_817_r64k.h (override the same)
* ============================================================================*/
#define ADDRESS_TABLE_819_R128K \
"\
# FW Memory Layout                                                          \n\
FML_BOOT_CODE_ROM                               0x0000          16k         \n\
FML_BOOT_CODE_XRAM                              0xa0000000      16k         \n\
FML_MAIN_COMBINE_INFO_REG_DEF_THQA_DEV_RPT_ROM  0x4000          8k          \n\
FML_MAIN_COMBINE_INFO_REG_DEF_THQA_DEV_RPT_XRAM 0xa0004000      8k          \n\
FML_MAIN_CODE_2_ROM                             0x6000          4k          \n\
FML_MAIN_CODE_2_XRAM                            0xa0006000      4k          \n\
FML_MAIN_CODE_1_ROM                             0x7000          88k         \n\
FML_MAIN_CODE_1_XRAM                            0xa0007000      88k         \n\
FML_RODATA_CODE_ROM                             0x1d000         8k          \n\
FML_RODATA_CODE_XRAM                            0xa001d000      8k          \n\
FML_REMAIN_SECTION_ROM                          0x1f000         4k          \n\
FML_REMAIN_SECTION_XRAM                         0xa001f000      4k          \n\
FML_CLEAR_BOOT_FLAG_ROM                         0x1e000         4k          \n\
FML_CLEAR_BOOT_FLAG_XRAM                        0xa001e000      4k          \n\
FML_CLEAR_CALIBRATION_FLAG_ROM                  0x1e000         4k          \n\
FML_CLEAR_CALIBRATION_FLAG_XRAM                 0xa001e000      4k          \n\
\
# FW Attribute                                              \n\
FA_UPDATE_FW_INFO_ROM             0x1e000         6           \n\
FA_BOOT_FLAG_ROM                  0x1eff0         4           \n\
FA_BOOT_FLAG_XRAM                 0xa001eff0      4           \n\
FA_INTERFACE_ID_ROM               0x4026          1           \n\
FA_INTERFACE_ID_XRAM              0xa0004026      1           \n\
FA_MULTI_CHIP_SELECTIVE_ID_ROM    0x4027          1           \n\
FA_MULTI_CHIP_SELECTIVE_ID_XRAM   0xa0004027      1           \n\
FA_MULTI_SLAVE_NUMBER_ROM         0x4028          1           \n\
FA_MULTI_SLAVE_NUMBER_XRAM        0xa0004028      1           \n\
FA_CALIBRATION_FLAG_ROM           0x1effc         4           \n\
FA_CALIBRATION_FLAG_XRAM          0xa001effc      4           \n\
FA_FIRMWARE_ID_ROM                0x4002          14          \n\
FA_FIRMWARE_ID_XRAM               0xa0004002      14          \n\
FA_BOOTLOADER_ID_ROM              0x0230          4           \n\
FA_BOOTLOADER_ID_XRAM             0xa0000230      4           \n\
FA_BOOT_CODE_CRC_ROM              0x0234          4           \n\
FA_BOOT_CODE_CRC_XRAM             0xa0000234      4           \n\
FA_MAIN_CODE_CRC_ROM              0x4044          4           \n\
FA_MAIN_CODE_CRC_XRAM             0xa0004044      4           \n\
FA_LAST_TIME_ROM                  0x40a0          4           \n\
FA_LAST_TIME_XRAM                 0xa00040a0      4           \n\
FA_LAST_ID_ROM                    0x40b2          14          \n\
FA_LAST_ID_XRAM                   0xa00040b2      14          \n\
FA_PRIOR_LAST_TIME_ROM            0x40c0          4           \n\
FA_PRIOR_LAST_TIME_XRAM           0xa00040c0      4           \n\
FA_PRIOR_LAST_ID_ROM              0x40d2          14          \n\
FA_PRIOR_LAST_ID_XRAM             0xa00040d2      14          \n\
FA_DEVICE_BUILD_INFO_XRAM         0xa0000200      48          \n\
FA_DEVICE_FW_INFO_XRAM            0xa0003fe0      112         \n\
FA_TASK_ID_ROM                    0x400a          2           \n\
FA_TASK_ID_XRAM                   0xa000400a      2           \n\
FA_FW_VERSION_ROM                 0x400e          2           \n\
FA_FW_VERSION_XRAM                0xa000400e      2           \n\
FA_PRODUCT_ID_IN_FW_ROM           0x4050          8           \n\
FA_PRODUCT_ID_IN_124K_XRAM        0xa001f000      8           \n\
FA_UPDATE_MARK_ROM                0x4000          2           \n\
FA_UPDATE_MARK_XRAM               0xa0004000      2           \n\
FA_LAST_UPDATE_MARK_ROM           0x40b0          2           \n\
FA_LAST_UPDATE_MARK_XRAM          0xa00040b0      2           \n\
FA_PRIOR_LAST_UPDATE_MARK_ROM     0x40d0          2           \n\
FA_PRIOR_LAST_UPDATE_MARK_XRAM    0xa00040d0      2           \n\
FA_IS_UPDATE_BOOTLOADER_INFO_ROM  0x40a4          2           \n\
FA_IS_UPDATE_BOOTLOADER_INFO_XRAM 0xa00040a4      2           \n\
\
";
//==============================================================================

#endif // ADDRESSTABLE_819_R128K

