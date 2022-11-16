#ifndef __BPS_GPIO_H
#define __BPS_GPIO_H

#include "debug.h"
#include "BPS_global_define.h"


// TODO: 合并通道当前开通状态到 总信息结构体中
// extern uint8_t Contorl_GPIO_Status[8];

void BPS_Contorl_IO_Init(void);

void BPS_ID_IO_Init(void);

void BPS_Set_Contorl_IO_Status(CAN_BMS_Infomation_TypeDef *CAN_BMS_Infomation, uint8_t channel_num, FunctionalState NewState);
uint8_t BPS_Check_All_Channel_Reset(CAN_BMS_Infomation_TypeDef *CAN_BMS_Infomation); // IO端口为Reset返回1

#endif
