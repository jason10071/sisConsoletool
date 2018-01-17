#ifndef ADDRESSTABLE_817_R64K_M48K
#define ADDRESSTABLE_817_R64K_M48K

/*==============================================================================
* Note 1: format same as addresstable_817_r64k.h
*
*
*
*
* Note 2: addressattribute inherit addresstable_817_r64k.h (override the same)
* ============================================================================*/
#define ADDRESS_TABLE_817_R64K_M48K \
"\
# FW Memory Layout                                          \n\
FML_MAIN_CODE_2_ROM             0xd000          4k          \n\
FML_MAIN_CODE_2_XRAM            0xa000d000      4k          \n\
FML_RODATA_CODE_ROM             0xe000          4k          \n\
FML_RODATA_CODE_XRAM            0xa000e000      4k          \n\
\
# FW Attribute                                              \n\
FA_TEST_ADDRESS_ROM             0x0020          20          \n\
FA_TEST_ADDRESS_XRAM            0xa000c016      20          \n\
\
";
//==============================================================================

#endif // ADDRESSTABLE_817_R64K_M48K

