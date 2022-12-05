/***************************************************************** (C) COPYRIGHT  *****************************************************************
 * File Name          : BSP_contorl.h
 * Author             : lzx
 * Version            : V1.2.0
 * Date               : 2021/12/5
 * Description        : 
 *                      
 *                      【函数说明】 BSP_CAN_BMS_Info_Update        计算所有状态信息
 *                                  BSP_Protection_Policy_Check    根据保护策略，控制通道通断管理
 *                                  dma_adc 中断管理函数
 *                                  can 中断管理函数
 *                                  tim2 刷屏函数
 *                                  tim3 定时中断函数【主循环】
 ************************************************************************************************************************************************/ 
#ifndef __BSP_CONTROL_H
#define __BSP_CONTROL_H

#include "debug.h"
#include "BSP_global_define.h"
#include "BSP_adc.h"
#include "BSP_can.h"
#include "BSP_gpio.h"
#include "BSP_timer.h"
#include "BSP_flash.h"
#include "oled.h"


void BSP_Protection_Policy_Check(CAN_BMS_Infomation_TypeDef * CAN_BMS_Infomation);

// TODO: 瞬时功率 or 全部功率计算
void BSP_CAN_BMS_Info_Update(CAN_BMS_Infomation_TypeDef * CAN_BMS_Infomation); // 计算功率


#endif



