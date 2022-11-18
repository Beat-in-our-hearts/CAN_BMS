
/********************************** (C) COPYRIGHT  *******************************
 * File Name          : 
 * Author             : lzx
 * Version            : V1.0.0
 * Date               : 2022/11/16
 * Description        : 本文件保留适用全部工程的定义以及最重要的信息结构体, 外部文件需要引用本文件
 * 此外 其他头文件只保存校准的一些宏和特定功能的全局变量
 *******************************************************************************/
#ifndef __BPS_GLOBAL_DEFINE_H
#define __BPS_GLOBAL_DEFINE_H

#include "debug.h"

typedef enum 
{
    None_Policy = 0x00,
    Trip_Policy = 0x01,
    Trip_Recover_Policy = 0x10,
    Other_Policy = 0x11
}Policy;

typedef enum 
{
    OFF = 0,
    ON = 1,
    OverCurrentOn = 2,
    OverPowerOn = 3,
    OverCurrentOFF = 4,
    OverPowerOFF = 5,
    LowVoltageOFF = 6,
    HighVoltageOFF  = 7
}CH_State;


#define ADC_NBR_OF_CHANNEL 9
#define OUTPUT_CHANNEL 8
/* Control CH1-8 PIN */
#define Control_IO_CH1_4_GPIO_Port GPIOA
#define Control_IO_CH1_Pin GPIO_Pin_11
#define Control_IO_CH2_Pin GPIO_Pin_10
#define Control_IO_CH3_Pin GPIO_Pin_9
#define Control_IO_CH4_Pin GPIO_Pin_8

#define Control_IO_CH5_8_GPIO_Port GPIOB
#define Control_IO_CH5_Pin GPIO_Pin_7
#define Control_IO_CH6_Pin GPIO_Pin_6
#define Control_IO_CH7_Pin GPIO_Pin_5
#define Control_IO_CH8_Pin GPIO_Pin_4

/* ADC Current CH1-8 PIN */
#define ADC_CH1_8_GPIO_Port GPIOA
#define ADC_CH1_Pin GPIO_Pin_7
#define ADC_CH2_Pin GPIO_Pin_6
#define ADC_CH3_Pin GPIO_Pin_5
#define ADC_CH4_Pin GPIO_Pin_4
#define ADC_CH5_Pin GPIO_Pin_3
#define ADC_CH6_Pin GPIO_Pin_2
#define ADC_CH7_Pin GPIO_Pin_1
#define ADC_CH8_Pin GPIO_Pin_0

/* ADC VOl CH PIN */
#define V_CH_Pin GPIO_Pin_0
#define V_CH_GPIO_Port GPIOB


/* CAN ID PIN */
#define ID0_GPIO_Port GPIOB
#define ID0_Pin GPIO_Pin_1
#define ID1_GPIO_Port GPIOA
#define ID1_Pin GPIO_Pin_12
#define ID2_GPIO_Port GPIOA
#define ID2_Pin GPIO_Pin_15
#define ID3_GPIO_Port GPIOB
#define ID3_Pin GPIO_Pin_3

typedef struct
{
    float real_time_current;    // 实时电流
    float read_time_power;      // 实时功率
    float history_max_power;    // 峰值功率
    uint32_t work_time;         // 工作时间ms
    double cumulative_power;    // 累计功耗
    float on_average_power;     // 平均功耗
    CH_State state;              // 通道状态
}Singer_Channel_TypeDef;


typedef struct
{
    // real-time data
    Singer_Channel_TypeDef CH[OUTPUT_CHANNEL];
    float battery_voltage;
    float total_read_time_power;
    double total_cumulative_power;
    // board setting 占用Flash 128个字节空间
    uint8_t Module_Type[8];                     // 模块类型         【8/16】 buf0 buf1
    uint8_t Firmware_Version[2];                // 固件版本         【2/16】 buf4
    uint8_t Protection_Policy[OUTPUT_CHANNEL];  // 保护策略         【8/16】 buf8 buf9
    float Channel_Max_Current[OUTPUT_CHANNEL];  // 最大电流         【32/32】buf12 - 19
    float Channel_Max_Power[OUTPUT_CHANNEL];    // 最大功率         【32/32】buf20 - 27
    float Min_Max_Voltage[2];                   // 电压阈值 min-max 【8/8】  buf28 buf29
    float Total_Max_Power;                      // 最大总功耗       【4/8】  buf30


}CAN_BMS_Infomation_TypeDef;



extern CAN_BMS_Infomation_TypeDef CAN_BMS_Info;

#endif
