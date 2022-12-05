/***************************************************************** (C) COPYRIGHT  *****************************************************************
 * File Name          : BPS_contorl.h
 * Author             : lzx
 * Version            : V1.2.0
 * Date               : 2021/12/5
 * Description        : 
 *                      
 *                      【函数说明】 BPS_CAN_BMS_Info_Update        计算所有状态信息
 *                                  BPS_Protection_Policy_Check    根据保护策略，控制通道通断管理
 *                                  dma_adc 中断管理函数
 *                                  can 中断管理函数
 *                                  tim2 刷屏函数
 *                                  tim3 定时中断函数【主循环】
 ************************************************************************************************************************************************/ 
#ifndef __BPS_CONTROL_H
#define __BPS_CONTROL_H

#include "debug.h"
#include "BPS_global_define.h"
#include "BPS_adc.h"
#include "BPS_can.h"
#include "BPS_gpio.h"
#include "BPS_timer.h"
#include "BPS_flash.h"
#include "oled.h"


void BPS_Protection_Policy_Check(CAN_BMS_Infomation_TypeDef * CAN_BMS_Infomation);

// TODO: 瞬时功率 or 全部功率计算
void BPS_CAN_BMS_Info_Update(CAN_BMS_Infomation_TypeDef * CAN_BMS_Infomation); // 计算功率


#endif



