#ifndef __BPS_CONTROL_H
#define __BPS_CONTROL_H

#include "debug.h"
#include "BPS_global_define.h"
#include "BPS_adc.h"
#include "BPS_can.h"
#include "BPS_gpio.h"
#include "BPS_timer.h"
#include "oled.h"

#define DELTA_T    0.001
#define MAX_CURRENT 5.0f
#define MAX_VOLTAGE 25.2f 
#define MIN_VOLTAGE 21.0f





void BPS_Current_Voltage_Protection_Check(CAN_BMS_Infomation_TypeDef * CAN_BMS_Infomation);

// TODO: 瞬时功率 or 全部功率计算
void BPS_CAN_BMS_Info_Update(CAN_BMS_Infomation_TypeDef * CAN_BMS_Infomation); // 计算功率


#endif



