/***************************************************************** (C) COPYRIGHT  *****************************************************************
 * File Name          : BSP_gpio.h
 * Author             : lzx
 * Version            : V1.0.0
 * Date               : 2021/11/25
 * Description        : 针对控制引脚和ID引脚，由于其没用对应的MCU外设功能，是人为定义的，所以需要单独拿出来设置，并对控制引脚进行包装
 *                      BSP_Contorl_IO_Init BSP_ID_IO_Init 初始化
 *                      BSP_Set_Contorl_IO_Status 设定通道状态，优化其输入参数，并保存设定状态信息
 *                      BSP_Check_All_Channel_Reset 在初始化读取adc偏移时，需要保证控制IO为关闭状态，读取偏移值后才能开启【默认开机关闭所有通道】
 ************************************************************************************************************************************************/ 

#ifndef __BSP_GPIO_H
#define __BSP_GPIO_H

#include "debug.h"
#include "BSP_global_define.h"



void BSP_Contorl_IO_Init(void);
void BSP_ID_IO_Init(void);

void BSP_Set_Contorl_IO_Status(CAN_BMS_Infomation_TypeDef *CAN_BMS_Infomation, uint8_t channel_num, CH_State NewState);
uint8_t BSP_Check_All_Channel_Reset(CAN_BMS_Infomation_TypeDef *CAN_BMS_Infomation); // IO端口为Reset返回1

#endif
