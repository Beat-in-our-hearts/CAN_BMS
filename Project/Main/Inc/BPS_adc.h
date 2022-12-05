/***************************************************************** (C) COPYRIGHT  *****************************************************************
 * File Name          : BPS_adc.h
 * Author             : lzx
 * Version            : V1.0.0
 * Date               : 2021/11/25
 * Description        : 【宏定义】需要实际测量对应通道的采样电阻大小 以及电压采集电路分压电阻大小
 *                      【全局变量】adc校准数值、dma操作的循环写入地址ADC_Buffer、零偏adc采集数值ADC_Offset_Buffer、零偏采集成功标志符
 *                      【函数说明】 BPS_ADC_Base_Init       开启时钟；初始IO；设定循环采集通道；校准
 *                                  BPS_DMA_ADC_Tx_Init     DMA设置
 *                                  BPS_ADC_NVIC_Config     设置中断优先级
 *                                  BPS_ADC_Init            主函数中依次调用上述函数
 *                                  BPS_Get_Float_ADC       将缓冲区adc整型数值转化成浮点类型的电压电流信息
 ************************************************************************************************************************************************/ 
#ifndef __BPS_ADC_H
#define __BPS_ADC_H

#include "debug.h"

#include "BPS_global_define.h"
#include "BPS_gpio.h"


#define CURRENT_GAIN        100
#define CURRENT_R1          0.003f
#define CURRENT_R2          0.003f
#define CURRENT_R3          0.003f
#define CURRENT_R4          0.003f
#define CURRENT_R5          0.003f
#define CURRENT_R6          0.003f
#define CURRENT_R7          0.003f
#define CURRENT_R8          0.003f

#define VBUS_R1            47.0f
#define VBUS_R2            5.1f    
#define VBUS_TOTAL_R       (VBUS_R1 + VBUS_R2)   
#define VBUS_KP            (VBUS_TOTAL_R / VBUS_R2)

/* TIM_ADC_Freq */
// #define ADC_Freq_Mode ADC_Freq_1KHz
// #define ADC_Freq_1KHz 0x0001
// #define ADC_Freq_2KHz 0x0002
// #define ADC_Freq_5KHz 0x0003
// #define ADC_Freq_10KHz 0x0004
// #define ADC_Freq_20KHz 0x0005

extern int16_t Calibrattion_Val;                        // ADC校准 
extern uint16_t ADC_Buffer[ADC_NBR_OF_CHANNEL];         // 存储ADC数据 DMA操作地址
extern uint16_t ADC_Offset_Buffer[ADC_NBR_OF_CHANNEL];  // 读取零偏误差 上电即测
extern uint8_t ADC_Offset_Check_Flag;                   // 成功读取偏差值为1 否则为0
 

void BPS_ADC_Base_Init(void);       // 设置ADC 基本功能
void BPS_DMA_ADC_Tx_Init(void);     // 设置DMA对应的内存地址和长度
//// void BPS_Set_ADC_Freq(void);     // 设置定时器TIM3 TRGO 触发ADC采集的频率
void BPS_ADC_NVIC_Config(void);     // 设置所需的中断函数优先级
void BPS_ADC_Init(void);            // 设置ADC功能：端口初始化、ADC初始化、DMA初始化////定时器初始化

// 数组结构 前n-1为电流 最后1位电压
////void BPS_Get_Float_ADC(uint16_t *buf, float *res);  // ADC转实际电压
void BPS_Get_Float_ADC(CAN_BMS_Infomation_TypeDef *CAN_BMS_InfomationStructure);



// TODO : 注入组 或者 默认查询前100次转换
#endif
