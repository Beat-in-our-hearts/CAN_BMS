#ifndef __BPS_TIMER_H
#define __BPS_TIMER_H

#include "debug.h"
#include "BPS_global_define.h"

/* TIM_ADC_Freq */
#define SAMPLE_FREQ_MODE    SAMPLE_FREQ_1KHz
#define SAMPLE_FREQ_100Hz   0x0001
#define SAMPLE_FREQ_200Hz   0x0002
#define SAMPLE_FREQ_500Hz   0x0003  
#define SAMPLE_FREQ_1KHz    0x0004
#define SAMPLE_FREQ_2KHz    0x0005
#define SAMPLE_FREQ_5KHz    0x0006
// #define SAMPLE_FREQ_10KHz   0x0004
// #define SAMPLE_FREQ_20KHz   0x0005

void BPS_TIM2_Base_Init(void);
void BPS_TIM3_Base_Init(void);
void BPS_TIM_NVIC_Config(void);
void BPS_TIM_Init(void);

#endif

