#include "BSP_flash.h"



void BSP_Flash_Test_Fast(void)
{
    uint8_t i, Verity_Flag = 0;
    uint32_t buf[32] = {0};

    for (i = 0; i < 32; i++) {
        buf[i] = i;
    }

    FLASH_Unlock_Fast(); // 快速模式解锁

    FLASH_ErasePage_Fast(0x0800E000); // 快速擦除 128 字节 = 32 个字

    printf("128Byte Page Erase Sucess\r\n");

    FLASH_BufReset(); // 清缓存
    FLASH_BufLoad(0x0800E000, buf[0], buf[1], buf[2], buf[3]);
    FLASH_BufLoad(0x0800E000 + 0x10, buf[4], buf[5], buf[6], buf[7]);
    FLASH_BufLoad(0x0800E000 + 0x20, buf[8], buf[9], buf[10], buf[11]);
    FLASH_BufLoad(0x0800E000 + 0x30, buf[12], buf[13], buf[14], buf[15]);
    FLASH_BufLoad(0x0800E000 + 0x40, buf[16], buf[17], buf[18], buf[19]);
    FLASH_BufLoad(0x0800E000 + 0x50, buf[20], buf[21], buf[22], buf[23]);
    FLASH_BufLoad(0x0800E000 + 0x60, buf[24], buf[25], buf[26], buf[27]);
    FLASH_BufLoad(0x0800E000 + 0x70, buf[28], buf[29], buf[30], buf[31]);
    FLASH_ProgramPage_Fast(0x0800E000); // 写缓存

    printf("128Byte Page Program Sucess\r\n");

    FLASH_Lock_Fast(); // 锁

    for (i = 0; i < 32; i++) {
        if (buf[i] == *(uint32_t *)(0x0800E000 + 4 * i)) {
            Verity_Flag = 0;
        } else {
            Verity_Flag = 1;
            break;
        }
    }

    if (Verity_Flag) {
        printf("128Byte Page Verity Fail\r\n");
    } else {
        printf("128Byte Page Verity Sucess\r\n");
    }
}

void BSP_Flash_Read_Setting(CAN_BMS_Infomation_TypeDef *CAN_BMS_InfomationStructure)
{
    // SETTING_START_ADDR 首地址
    uint8_t i = 0;
    for (i = 0; i < 8; ++i)
        CAN_BMS_InfomationStructure->Module_Type[i] = *(uint8_t *)(SETTING_START_ADDR + i);
    for (i = 0; i < 2; ++i)
        CAN_BMS_InfomationStructure->Firmware_Version[i] = *(uint8_t *)(SETTING_START_ADDR + 0x10 + i);
    for (i = 0; i < ADC_NBR_OF_CHANNEL - 1; ++i)
        CAN_BMS_InfomationStructure->Protection_Policy[i] = *(uint8_t *)(SETTING_START_ADDR + 0x20 + i);
    for (i = 0; i < ADC_NBR_OF_CHANNEL - 1; ++i)
        CAN_BMS_InfomationStructure->Channel_Max_Current[i] = *(float *)(SETTING_START_ADDR + 0x30 + 4 * i);
    for (i = 0; i < ADC_NBR_OF_CHANNEL - 1; ++i)
        CAN_BMS_InfomationStructure->Channel_Max_Power[i] = *(float *)(SETTING_START_ADDR + 0x50 + 4 * i);
    for (i = 0; i < 2; ++i)
        CAN_BMS_InfomationStructure->Min_Max_Voltage[i] = *(float *)(SETTING_START_ADDR + 0x70 + 4 * i);
    CAN_BMS_InfomationStructure->Total_Max_Power = *(float *)(SETTING_START_ADDR + 0x78);
}

void BSP_Flash_Write_Setting(CAN_BMS_Infomation_TypeDef *CAN_BMS_InfomationStructure)
{
    uint32_t buf[32] = {0};
    buf[0] = ((uint32_t *)(CAN_BMS_InfomationStructure->Module_Type))[0];
    buf[1] = ((uint32_t *)(CAN_BMS_InfomationStructure->Module_Type))[1];

    buf[4] = CAN_BMS_InfomationStructure->Firmware_Version[0] << 24 + CAN_BMS_InfomationStructure->Firmware_Version[1] << 16;

    buf[8] = ((uint32_t *)(CAN_BMS_InfomationStructure->Protection_Policy))[0];
    buf[9] = ((uint32_t *)(CAN_BMS_InfomationStructure->Protection_Policy))[1];

    buf[12] = ((uint32_t *)(CAN_BMS_InfomationStructure->Channel_Max_Current))[0];
    buf[13] = ((uint32_t *)(CAN_BMS_InfomationStructure->Channel_Max_Current))[1];
    buf[14] = ((uint32_t *)(CAN_BMS_InfomationStructure->Channel_Max_Current))[2];
    buf[15] = ((uint32_t *)(CAN_BMS_InfomationStructure->Channel_Max_Current))[3];
    buf[16] = ((uint32_t *)(CAN_BMS_InfomationStructure->Channel_Max_Current))[4];
    buf[17] = ((uint32_t *)(CAN_BMS_InfomationStructure->Channel_Max_Current))[5];
    buf[18] = ((uint32_t *)(CAN_BMS_InfomationStructure->Channel_Max_Current))[6];
    buf[19] = ((uint32_t *)(CAN_BMS_InfomationStructure->Channel_Max_Current))[7];

    buf[20] = ((uint32_t *)(CAN_BMS_InfomationStructure->Channel_Max_Power))[0];
    buf[21] = ((uint32_t *)(CAN_BMS_InfomationStructure->Channel_Max_Power))[1];
    buf[22] = ((uint32_t *)(CAN_BMS_InfomationStructure->Channel_Max_Power))[2];
    buf[23] = ((uint32_t *)(CAN_BMS_InfomationStructure->Channel_Max_Power))[3];
    buf[24] = ((uint32_t *)(CAN_BMS_InfomationStructure->Channel_Max_Power))[4];
    buf[25] = ((uint32_t *)(CAN_BMS_InfomationStructure->Channel_Max_Power))[5];
    buf[26] = ((uint32_t *)(CAN_BMS_InfomationStructure->Channel_Max_Power))[6];
    buf[27] = ((uint32_t *)(CAN_BMS_InfomationStructure->Channel_Max_Power))[7];

    buf[28] = ((uint32_t *)(CAN_BMS_InfomationStructure->Min_Max_Voltage))[0];
    buf[29] = ((uint32_t *)(CAN_BMS_InfomationStructure->Min_Max_Voltage))[1];

    buf[30] = *((uint32_t *)(&CAN_BMS_InfomationStructure->Total_Max_Power));

    FLASH_Unlock_Fast();                      // 快速模式解锁
    FLASH_ErasePage_Fast(SETTING_START_ADDR); // 快速擦除 128 字节 = 32 个字
    FLASH_BufReset();                         // 清缓存
    FLASH_BufLoad(SETTING_START_ADDR, buf[0], buf[1], buf[2], buf[3]);
    FLASH_BufLoad(SETTING_START_ADDR + 0x10, buf[4], buf[5], buf[6], buf[7]);
    FLASH_BufLoad(SETTING_START_ADDR + 0x20, buf[8], buf[9], buf[10], buf[11]);
    FLASH_BufLoad(SETTING_START_ADDR + 0x30, buf[12], buf[13], buf[14], buf[15]);
    FLASH_BufLoad(SETTING_START_ADDR + 0x40, buf[16], buf[17], buf[18], buf[19]);
    FLASH_BufLoad(SETTING_START_ADDR + 0x50, buf[20], buf[21], buf[22], buf[23]);
    FLASH_BufLoad(SETTING_START_ADDR + 0x60, buf[24], buf[25], buf[26], buf[27]);
    FLASH_BufLoad(SETTING_START_ADDR + 0x70, buf[28], buf[29], buf[30], buf[31]);
    FLASH_ProgramPage_Fast(SETTING_START_ADDR); // 写缓存
    FLASH_Lock_Fast();                          // 锁
}

void BSP_Flash_Init(void) // 读取设定值 检查固件版本等
{
    uint8_t update_check_flag = 0;
    uint8_t i = 0;
    BSP_Flash_Read_Setting(&CAN_BMS_Info);
    for (i = 0; i < 8; ++i) {
        if (CAN_BMS_Info.Module_Type[i] != Type_Name[i]) {
            CAN_BMS_Info.Module_Type[i] = Type_Name[i];
            update_check_flag = 1;
        }
    }
    for (i = 0; i < 2; ++i) {
        if (CAN_BMS_Info.Firmware_Version[i] != Version[i]) {
            CAN_BMS_Info.Firmware_Version[i] = Version[i];
            update_check_flag = 1;
            
        }
    }
    if (update_check_flag) {
        BSP_Flash_Write_Setting(&CAN_BMS_Info);
    }
}
