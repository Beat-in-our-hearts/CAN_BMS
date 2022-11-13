#ifndef __BPS_ADC_H
#define __BPS_ADC_H

#include "debug.h"
#include "BPS_global_define.h"


#define Used_ADC_NbrOfChannel 9
#define Current_Gain        100
#define Current_R1          0.003f
#define Current_R2          0.003f
#define Current_R3          0.003f
#define Current_R4          0.003f
#define Current_R5          0.003f
#define Current_R6          0.003f
#define Current_R7          0.003f
#define Current_R8          0.003f


#define VBUS_R1            47.0f
#define VBUS_R2            5.1f    
#define VBUS_Total_R       (VBUS_R1 + VBUS_R2)   
#define VBUS_Kp            (VBUS_R2 / VBUS_Total_R)

/* TIM_ADC_Freq */
// #define ADC_Freq_Mode ADC_Freq_1KHz
// #define ADC_Freq_1KHz 0x0001
// #define ADC_Freq_2KHz 0x0002
// #define ADC_Freq_5KHz 0x0003
// #define ADC_Freq_10KHz 0x0004
// #define ADC_Freq_20KHz 0x0005

extern int16_t ADC_Buffer[Used_ADC_NbrOfChannel];
extern int16_t Calibrattion_Val;
void BPS_ADC_Base_Init(void);   // 设置ADC 基本功能
void BPS_DMA_ADC_Tx_Init(void); // 设置DMA对应的内存地址和长度
// void BPS_Set_ADC_Freq(void);    // 设置定时器TIM3 TRGO 触发ADC采集的频率
void BPS_ADC_NVIC_Config(void); // 设置所需的中断函数优先级
void BPS_ADC_Init(void);        // 设置ADC功能：端口初始化、ADC初始化、DMA初始化、定时器初始化

// 数组结构 前n-1为电流 最后1位电压
void BPS_Get_Float_ADC(uint16_t *buf, float *res);
void BPS_Count_Power(float *adc_res, float deleta_t, folat *power_res);

#endif
