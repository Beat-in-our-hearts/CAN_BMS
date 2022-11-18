#ifndef __BPS_GPIO_H
#define __BPS_GPIO_H

#include "debug.h"
#include "BPS_global_define.h"


void BPS_Contorl_IO_Init(void);
void BPS_ID_IO_Init(void);

void BPS_Set_Contorl_IO_Status(CAN_BMS_Infomation_TypeDef *CAN_BMS_Infomation, uint8_t channel_num, CH_State NewState);
uint8_t BPS_Check_All_Channel_Reset(CAN_BMS_Infomation_TypeDef *CAN_BMS_Infomation); // IO端口为Reset返回1

#endif
