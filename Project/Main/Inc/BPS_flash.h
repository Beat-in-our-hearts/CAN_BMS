#ifndef __BPS_FLASH_H
#define __BPS_FLASH_H

#include "debug.h"
#include "BPS_global_define.h"


/* Global define */
typedef enum {FAILED = 0, PASSED = !FAILED}TestStatus;
#define PAGE_WRITE_START_ADDR  ((uint32_t)0x0800F000) /* Start from 60K */
#define PAGE_WRITE_END_ADDR    ((uint32_t)0x08010000) /* End at 63K */
#define FLASH_PAGE_SIZE                   1024
#define FLASH_PAGES_TO_BE_PROTECTED FLASH_WRProt_Pages60to63
#define SETTING_START_ADDR  ((uint32_t)0x0800E000)

/* Global Variable */
uint32_t EraseCounter = 0x0, Address = 0x0;
uint16_t Data = 0xAAAA;
uint32_t WRPR_Value = 0xFFFFFFFF, ProtectedPages = 0x0;
uint32_t NbrOfPage;
volatile FLASH_Status FLASHStatus = FLASH_COMPLETE;
volatile TestStatus MemoryProgramStatus = PASSED;
volatile TestStatus MemoryEraseStatus = PASSED;
u32 buf[32];


void BPS_Flash_Test_Fast(void);
void BPS_Flash_Read_Setting(CAN_BMS_Infomation_TypeDef * CAN_BMS_InfomationStructure);
void BPS_Flash_Write_Setting(CAN_BMS_Infomation_TypeDef * CAN_BMS_InfomationStructure);



#endif


