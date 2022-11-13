#ifndef __BPS_ADC_H
#define __BPS_ADC_H

#include "debug.h"
#include "BPS_global_define.h"


#define Used_ADC_NbrOfChannel 9


/* TIM_ADC_Freq */
#define ADC_Freq_Mode ADC_Freq_1KHz
#define ADC_Freq_1KHz 0x0001
#define ADC_Freq_2KHz 0x0002
#define ADC_Freq_5KHz 0x0003
#define ADC_Freq_10KHz 0x0004
#define ADC_Freq_20KHz 0x0005

extern int16_t ADC_Buffer[Used_ADC_NbrOfChannel];
extern int16_t Calibrattion_Val;
void BPS_ADC_Base_Init(void);   // 设置ADC 基本功能
void BPS_DMA_ADC_Tx_Init(void); // 设置DMA对应的内存地址和长度
void BPS_Set_ADC_Freq(void);    // 设置定时器TIM3 TRGO 触发ADC采集的频率
void BPS_ADC_NVIC_Config(void); // 设置所需的中断函数优先级
void BPS_ADC_Init(void);        // 设置ADC功能：端口初始化、ADC初始化、DMA初始化、定时器初始化

#endif
