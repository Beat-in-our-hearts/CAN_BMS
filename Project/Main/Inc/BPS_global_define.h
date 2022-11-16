#ifndef __BPS_GLOBAL_DEFINE_H
#define __BPS_GLOBAL_DEFINE_H

#include "debug.h"

#define ADC_NBR_OF_CHANNEL 9
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
    float current_voltage_buffer[ADC_NBR_OF_CHANNEL];       // 暂存ADC转化后的实际电压电流数据
    float instantaneous_power[ADC_NBR_OF_CHANNEL - 1];      // 瞬时功率
    float history_max_power[ADC_NBR_OF_CHANNEL - 1];        // 峰值功率：历史最高功率
    uint32_t work_time[ADC_NBR_OF_CHANNEL - 1];             // 工作时间：单位s
    float cumulative_power[ADC_NBR_OF_CHANNEL - 1];         // 累计功耗 
    float on_average_power_buffer[ADC_NBR_OF_CHANNEL - 1];  // 平均功率
    float total_instantaneous_power;                        // 总瞬时功率
    float total_cumulative_power;                           // 总累计功耗
    uint8_t contorl_GPIO_status[ADC_NBR_OF_CHANNEL - 1];    // 通道状态

}CAN_BMS_Infomation_TypeDef;

extern CAN_BMS_Infomation_TypeDef CAN_BMS_Info;

#endif
