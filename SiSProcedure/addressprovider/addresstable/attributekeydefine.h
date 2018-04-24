#ifndef ATTRIBUTEKEYDEFINE
#define ATTRIBUTEKEYDEFINE

/* FW Attribute */
#define KEY_FA_BOOT_FLAG_ROM                            "FA_BOOT_FLAG_ROM"
#define KEY_FA_BOOT_FLAG_XRAM                           "FA_BOOT_FLAG_XRAM"
#define EXPECT_SIZE_FA_BOOT_FLAG_XRAM                   4

#define KEY_FA_0X0238_ROM                               "FA_0X0238_ROM"
#define KEY_FA_0X0238_XRAM                              "FA_0X0238_XRAM"
#define EXPECT_SIZE_FA_0X0238_XRAM                      1

#define KEY_FA_0XC04C_ROM                               "FA_0XC04C_ROM"
#define KEY_FA_0XC04C_XRAM                              "FA_0XC04C_XRAM"
#define EXPECT_SIZE_FA_0XC04C_XRAM                      1

#define KEY_FA_MULTI_SLAVE_NUMBER_ROM                   "FA_MULTI_SLAVE_NUMBER_ROM"
#define KEY_FA_MULTI_SLAVE_NUMBER_XRAM                  "FA_MULTI_SLAVE_NUMBER_XRAM"
#define EXPECT_SIZE_FA_MULTI_SLAVE_NUMBER_XRAM          1

#define KEY_FA_INTERFACE_ID_ROM                         "FA_INTERFACE_ID_ROM"
#define KEY_FA_INTERFACE_ID_XRAM                        "FA_INTERFACE_ID_XRAM"
#define EXPECT_SIZE_FA_INTERFACE_ID_XRAM                1

#define KEY_FA_MULTI_CHIP_SELECTIVE_ID_ROM              "FA_MULTI_CHIP_SELECTIVE_ID_ROM"
#define KEY_FA_MULTI_CHIP_SELECTIVE_ID_XRAM             "FA_MULTI_CHIP_SELECTIVE_ID_XRAM"
#define EXPECT_SIZE_FA_MULTI_CHIP_SELECTIVE_ID_XRAM     1


#define KEY_FA_CALIBRATION_FLAG_ROM                     "FA_CALIBRATION_FLAG_ROM"
#define KEY_FA_CALIBRATION_FLAG_XRAM                    "FA_CALIBRATION_FLAG_XRAM"
#define EXPECT_SIZE_FA_CALIBRATION_FLAG_XRAM            4

#define KEY_FA_0X0200_ROM                               "FA_0X0200_ROM"
#define KEY_FA_0X0200_XRAM                              "FA_0X0200_XRAM"
#define EXPECT_SIZE_FA_0X0200_XRAM                      6

#define KEY_FA_0X020C_ROM                               "FA_0X020C_ROM"
#define KEY_FA_0X020C_XRAM                              "FA_0X020C_XRAM"
#define EXPECT_SIZE_FA_0X020C_XRAM                      4

#define KEY_FA_FIRMWARE_ID_ROM                          "FA_FIRMWARE_ID_ROM"
#define KEY_FA_FIRMWARE_ID_XRAM                         "FA_FIRMWARE_ID_XRAM"
#define EXPECT_SIZE_FA_FIRMWARE_ID_XRAM                 12
#define EXPECT_SIZE_FA_FIRMWARE_ID_XRAM_819             14

#define KEY_FA_BOOTLOADER_ID_ROM                        "FA_BOOTLOADER_ID_ROM"
#define KEY_FA_BOOTLOADER_ID_XRAM                       "FA_BOOTLOADER_ID_XRAM"
#define EXPECT_SIZE_FA_BOOTLOADER_ID_XRAM               4

#define KEY_FA_BOOT_CODE_CRC_ROM                        "FA_BOOT_CODE_CRC_ROM"
#define KEY_FA_BOOT_CODE_CRC_XRAM                       "FA_BOOT_CODE_CRC_XRAM"
#define EXPECT_SIZE_FA_BOOT_CODE_CRC_XRAM               4

#define KEY_FA_MAIN_CODE_CRC_ROM                        "FA_MAIN_CODE_CRC_ROM"
#define KEY_FA_MAIN_CODE_CRC_XRAM                       "FA_MAIN_CODE_CRC_XRAM"
#define EXPECT_SIZE_FA_MAIN_CODE_CRC_XRAM               4

#define KEY_FA_LAST_ID_ROM                              "FA_LAST_ID_ROM"
#define KEY_FA_LAST_ID_XRAM                             "FA_LAST_ID_XRAM"
#define EXPECT_SIZE_FA_LAST_ID_XRAM                     12
#define EXPECT_SIZE_FA_LAST_ID_XRAM_819                 14

#define KEY_FA_LAST_TIME_ROM                            "FA_LAST_TIME_ROM"
#define KEY_FA_LAST_TIME_XRAM                           "FA_LAST_TIME_XRAM"
#define EXPECT_SIZE_FA_LAST_TIME_XRAM                   4

#define KEY_FA_PRIOR_LAST_ID_ROM                        "FA_PRIOR_LAST_ID_ROM"
#define KEY_FA_PRIOR_LAST_ID_XRAM                       "FA_PRIOR_LAST_ID_XRAM"
#define EXPECT_SIZE_FA_PRIOR_LAST_ID_XRAM               12
#define EXPECT_SIZE_FA_PRIOR_LAST_ID_XRAM_819           14

#define KEY_FA_PRIOR_LAST_TIME_ROM                      "FA_PRIOR_LAST_TIME_ROM"
#define KEY_FA_PRIOR_LAST_TIME_XRAM                     "FA_PRIOR_LAST_TIME_XRAM"
#define EXPECT_SIZE_FA_PRIOR_LAST_TIME_XRAM             4

#define KEY_FA_MAGIC_0X0200_XRAM                        "FA_MAGIC_0X0200_XRAM"
#define EXPECT_SIZE_FA_MAGIC_0X0200_XRAM                16

#define KEY_FA_MAGIC_0X10200_XRAM                       "FA_MAGIC_0X10200_XRAM"
#define EXPECT_SIZE_FA_MAGIC_0X10200_XRAM               16

#define KEY_FA_TASK_ID_ROM                              "FA_TASK_ID_ROM"
#define KEY_FA_TASK_ID_XRAM                             "FA_TASK_ID_XRAM"
#define EXPECT_SIZE_FA_TASK_ID_XRAM                     2

#define KEY_FA_FW_VERSION_ROM                           "FA_FW_VERSION_ROM"
#define KEY_FA_FW_VERSION_XRAM                          "FA_FW_VERSION_XRAM"
#define EXPECT_SIZE_FA_FW_VERSION_XRAM                  2

#define KEY_FA_PRODUCT_ID_IN_FW_ROM                     "FA_PRODUCT_ID_IN_FW_ROM"
#define KEY_FA_PRODUCT_ID_IN_124K_XRAM                  "FA_PRODUCT_ID_IN_124K_XRAM"
#define EXPECT_SIZE_FA_PRODUCT_ID_IN_124K_XRAM          8

#define KEY_FA_UPDATE_MARK_ROM                          "FA_UPDATE_MARK_ROM"
#define KEY_FA_UPDATE_MARK_XRAM                         "FA_UPDATE_MARK_XRAM"
#define EXPECT_SIZE_FA_UPDATE_MARK_XRAM                 4
#define EXPECT_SIZE_FA_UPDATE_MARK_XRAM_819             2

#define KEY_FA_IS_UPDATE_BOOTLOADER_INFO_ROM            "FA_IS_UPDATE_BOOTLOADER_INFO_ROM"
#define KEY_FA_IS_UPDATE_BOOTLOADER_INFO_XRAM           "FA_IS_UPDATE_BOOTLOADER_INFO_XRAM"
#define EXPECT_SIZE_FA_IS_UPDATE_BOOTLOADER_INFO_XRAM   2

#define KEY_FA_LAST_UPDATE_MARK_ROM_819                 "FA_LAST_UPDATE_MARK_ROM"
#define KEY_FA_LAST_UPDATE_MARK_XRAM_819                "FA_LAST_UPDATE_MARK_XRAM"
#define EXPECT_SIZE_FA_LAST_UPDATE_MARK_XRAM_819        2

#define KEY_FA_PRIOR_LAST_UPDATE_MARK_ROM_819           "FA_PRIOR_LAST_UPDATE_MARK_ROM"
#define KEY_FA_PRIOR_LAST_UPDATE_MARK_XRAM_819          "FA_PRIOR_LAST_UPDATE_MARK_XRAM"
#define EXPECT_SIZE_FA_PRIOR_LAST_UPDATE_MARK_XRAM_819  2

#endif // ATTRIBUTEKEYDEFINE

