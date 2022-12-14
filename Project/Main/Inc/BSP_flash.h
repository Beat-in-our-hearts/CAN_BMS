/***************************************************************** (C) COPYRIGHT  *****************************************************************
 * File Name          : BSP_flash.h
 * Author             : lzx
 * Version            : V1.0.0
 * Date               : 2021/11/25
 * Description        : 【宏定义】  存储首地址
 *                      【全局变量】 设备类型和固件版本号 每次修改程序以写入到flash中
 *                      【函数说明】 BSP_Flash_Init          读取历史设定值，并查看是否需要写入新的设备类型和固件版本
 *                                  BSP_Flash_Read_Setting  读取设定
 *                                  BSP_Flash_Write_Setting 写入设定
 ************************************************************************************************************************************************/ 

#ifndef __BSP_FLASH_H
#define __BSP_FLASH_H

#include "debug.h"
#include "BSP_global_define.h"


/* Global define */
// typedef enum {FAILED = 0, PASSED = !FAILED}TestStatus;
// #define PAGE_WRITE_START_ADDR  ((uint32_t)0x0800F000) /* Start from 60K */
// #define PAGE_WRITE_END_ADDR    ((uint32_t)0x08010000) /* End at 63K */
// #define FLASH_PAGE_SIZE                   1024
// #define FLASH_PAGES_TO_BE_PROTECTED FLASH_WRProt_Pages60to63


// /* Global Variable */
// uint32_t EraseCounter = 0x0, Address = 0x0;
// uint16_t Data = 0xAAAA;
// uint32_t WRPR_Value = 0xFFFFFFFF, ProtectedPages = 0x0;
// uint32_t NbrOfPage;
// volatile FLASH_Status FLASHStatus = FLASH_COMPLETE;
// volatile TestStatus MemoryProgramStatus = PASSED;
// volatile TestStatus MemoryEraseStatus = PASSED;
// u32 buf[32];



#define SETTING_START_ADDR  ((uint32_t)0x0800E000)


void BSP_Flash_Test_Fast(void);
void BSP_Flash_Read_Setting(CAN_BMS_Infomation_TypeDef * CAN_BMS_InfomationStructure);
void BSP_Flash_Write_Setting(CAN_BMS_Infomation_TypeDef * CAN_BMS_InfomationStructure);

void BSP_Flash_Init(void);


#endif


