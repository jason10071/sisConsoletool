#ifndef ADDRESSTABLE_817_R128K
#define ADDRESSTABLE_817_R128K

/*==============================================================================
* Note 1: format same as addresstable_817_r64k.h
*
*
*
*
* Note 2: addressattribute inherit addresstable_817_r64k.h (override the same)
* ============================================================================*/
#define ADDRESS_TABLE_817_R128K \
"\
# FW Memory Layout                                                          \n\
FML_BOOT_CODE_ROM                               0x0000          8k          \n\
FML_BOOT_CODE_XRAM                              0xa0000000      8k          \n\
FML_UPDATE_CODE_ROM                             0x2000          8k          \n\
FML_UPDATE_CODE_XRAM                            0xa0002000      8k          \n\
FML_MAIN_CODE_1_ROM                             0x4000          28k         \n\
FML_MAIN_CODE_1_XRAM                            0xa0004000      28k         \n\
FML_MAIN_COMBINE_INFO_REG_DEF_THQA_DEV_RPT_ROM  0xb000          8k          \n\
FML_MAIN_COMBINE_INFO_REG_DEF_THQA_DEV_RPT_XRAM 0xa000b000      8k          \n\
FML_MAIN_COMBINE_THQA_DEV_RPT_ROM               0xb000          4k          \n\
FML_MAIN_COMBINE_THQA_DEV_RPT_XRAM              0xa000b000      4k          \n\
FML_MAIN_COMBINE_INFO_REG_ROM                   0xc000          4k          \n\
FML_MAIN_COMBINE_INFO_REG_XRAM                  0xa000c000      4k          \n\
FML_THQA_ROM                                    0xb000          1k          \n\
FML_THQA_XRAM                                   0xa000b000      1k          \n\
FML_USB_DEV_DESC_ROM                            0xb400          1k          \n\
FML_USB_DEV_DESC_XRAM                           0xa000b400      1k          \n\
FML_USB_RPT_DESC_ROM                            0xb800          2k          \n\
FML_USB_RPT_DESC_XRAM                           0xa000b800      2k          \n\
FML_FW_INFO_ROM                                 0xc000          256         \n\
FML_FW_INFO_XRAM                                0xa000c000      256         \n\
FML_REG_MEM_ROM                                 0xc100          3840        \n\
FML_REG_MEM_XRAM                                0xa000c100      3840        \n\
FML_RODATA_CODE_ROM                             0xd000          8k          \n\
FML_RODATA_CODE_XRAM                            0xa000d000      8k          \n\
FML_MAIN_CODE_2_ROM                             0xf000          68k         \n\
FML_MAIN_CODE_2_XRAM                            0xa000f000      68k         \n\
\
# FW Attribute                                              \n\
FA_TEST_ADDRESS_ROM             0x0010          30          \n\
FA_TEST_ADDRESS_XRAM            0xa000c111      30          \n\
\
";
//==============================================================================

#endif // ADDRESSTABLE_817_R128K

