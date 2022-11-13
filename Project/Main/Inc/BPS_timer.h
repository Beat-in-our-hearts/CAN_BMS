#ifndef __BPS_TIMER_H
#define __BPS_TIMER_H

#include "debug.h"
#include "BPS_global_define.h"

/* TIM_ADC_Freq */
#define Sample_Freq_Mode    Sample_Freq_1KHz
#define Sample_Freq_100Hz   0x0001
#define Sample_Freq_200Hz   0x0002
#define Sample_Freq_500Hz   0x0003  
#define Sample_Freq_1KHz    0x0004
#define Sample_Freq_2KHz    0x0005
#define Sample_Freq_5KHz    0x0006
// #define Sample_Freq_10KHz   0x0004
// #define Sample_Freq_20KHz   0x0005

void BPS_TIM3_Base_Init(void);
void BPS_TIM_NVIC_Config(void);
void BPS_TIM_Init(void);

#endif