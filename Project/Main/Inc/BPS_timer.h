/***************************************************************** (C) COPYRIGHT  *****************************************************************
 * File Name          : BPS_timer.h
 * Author             : lzx
 * Version            : V1.0.0
 * Date               : 2021/11/25
 * Description        : 【宏定义】 定时中断频率 以及对应的单位时间数值
 *                      【全局变量】 CAN_ID 为当前设备号 默认上电读取一次后不再读取
 *                      【函数说明】 BPS_TIM2_Base_Init      10Hz刷屏程序，修改频率在.c文件修改
 *                                  BPS_TIM3_Base_Init      定时中断
 *                                  BPS_TIM_NVIC_Config     设置中断优先级
 *                                  BPS_TIM_Init            主函数中依次调用上述函数
 ************************************************************************************************************************************************/ 
#ifndef __BPS_TIMER_H
#define __BPS_TIMER_H

#include "debug.h"
#include "BPS_global_define.h"

/* TIM_ADC_Freq */
#define SAMPLE_FREQ_MODE    SAMPLE_FREQ_1KHz // 数值乘以100为实际频率
#define SAMPLE_FREQ_100Hz   0x0001
#define SAMPLE_FREQ_200Hz   0x0002
#define SAMPLE_FREQ_500Hz   0x0005  
#define SAMPLE_FREQ_1KHz    0x000A
#define SAMPLE_FREQ_2KHz    0x0014
#define SAMPLE_FREQ_5KHz    0x0032
// #define SAMPLE_FREQ_10KHz   0x0004
// #define SAMPLE_FREQ_20KHz   0x0005

#define DELTA_T    1.0f/100/SAMPLE_FREQ_MODE    // s计算的单位定时时间  单精度浮点
#define DELTA_T_ns 10000/SAMPLE_FREQ_MODE       // ns计算的单位定时时间 整形

void BPS_TIM2_Base_Init(void);
void BPS_TIM3_Base_Init(void);
void BPS_TIM_NVIC_Config(void);
void BPS_TIM_Init(void);

#endif

