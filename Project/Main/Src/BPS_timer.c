#include "BPS_timer.h"

void BPS_Timer_Interrupt(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure = {0};

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

#if (ADC_Freq_Mode == ADC_Freq_1KHz)
    TIM_TimeBaseInitStructure.TIM_Period = 1000 - 1;
    TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;
#if (ADC_Freq_Mode == ADC_Freq_2KHz)
    TIM_TimeBaseInitStructure.TIM_Period = 500 - 1;
    TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;
#elif (ADC_Freq_Mode == ADC_Freq_5KHz)
    TIM_TimeBaseInitStructure.TIM_Period = 200 - 1;
    TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;
#elif (ADC_Freq_Mode == ADC_Freq_10KHz)
    TIM_TimeBaseInitStructure.TIM_Period = 100 - 1;
    TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;
#elif (ADC_Freq_Mode == ADC_Freq_20KHz)
    TIM_TimeBaseInitStructure.TIM_Period = 50 - 1;
    TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;
#endif
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0x00;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);

    TIM_SelectMasterSlaveMode(TIM3, TIM_MasterSlaveMode_Disable); // 关闭主从模式
    TIM_SelectOutputTrigger(TIM3, TIM_TRGOSource_Update);         // 选择TRGO触发源为更新事件
    TIM_Cmd(TIM3, ENABLE);                                        // TIM3 使能
}