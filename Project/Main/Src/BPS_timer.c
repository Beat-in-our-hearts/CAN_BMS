#include "BPS_timer.h"

void BPS_TIM2_Base_Init(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure = {0};

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);  
    TIM_TimeBaseInitStructure.TIM_Period = 1000 - 1;
    TIM_TimeBaseInitStructure.TIM_Prescaler = 7200 - 1; // 10Hz
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0x00;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
    // TIM_SelectMasterSlaveMode(TIM2, TIM_MasterSlaveMode_Disable); // 关闭主从模式
    // TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_Update);         // 选择TRGO触发源为更新事件 

    TIM_Cmd(TIM2, ENABLE);                          // TIM2 使能
    // TIM_ClearFlag(TIM2, TIM_FLAG_Update);           // 清除标志位，以免一启用中断后立即产生中断
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);      // TIM2 更新中断 使能  
}


void BPS_TIM3_Base_Init(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure = {0};
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
#if (SAMPLE_FREQ_MODE == SAMPLE_FREQ_100Hz)
    TIM_TimeBaseInitStructure.TIM_Period = 100 - 1;
    TIM_TimeBaseInitStructure.TIM_Prescaler = 7200 - 1;
#elif (SAMPLE_FREQ_MODE == SAMPLE_FREQ_200Hz)
    TIM_TimeBaseInitStructure.TIM_Period = 50 - 1;
    TIM_TimeBaseInitStructure.TIM_Prescaler = 7200 - 1;
#elif (SAMPLE_FREQ_MODE == SAMPLE_FREQ_500Hz)
    TIM_TimeBaseInitStructure.TIM_Period = 20 - 1;
    TIM_TimeBaseInitStructure.TIM_Prescaler = 7200 - 1;
#elif (SAMPLE_FREQ_MODE == SAMPLE_FREQ_1KHz)
    TIM_TimeBaseInitStructure.TIM_Period = 10 - 1;
    TIM_TimeBaseInitStructure.TIM_Prescaler = 7200 - 1;
#elif (SAMPLE_FREQ_MODE == SAMPLE_FREQ_2KHz)
    TIM_TimeBaseInitStructure.TIM_Period = 5 - 1;
    TIM_TimeBaseInitStructure.TIM_Prescaler = 7200 - 1;
#elif (SAMPLE_FREQ_MODE == SAMPLE_FREQ_5KHz)
    TIM_TimeBaseInitStructure.TIM_Period = 2 - 1;
    TIM_TimeBaseInitStructure.TIM_Prescaler = 7200 - 1;
#endif
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0x00;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);

    // TIM_SelectMasterSlaveMode(TIM3, TIM_MasterSlaveMode_Disable); // 关闭主从模式
    // TIM_SelectOutputTrigger(TIM3, TIM_TRGOSource_Update);         // 选择TRGO触发源为更新事件
 
    TIM_Cmd(TIM3, ENABLE);                      // TIM3 使能
    // TIM_ClearFlag(TIM3, TIM_FLAG_Update);       // 清除标志位，以免一启用中断后立即产生中断
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);  // TIM3 更新中断 使能  
}

void BPS_TIM_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure = {0};
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;           // TIM3 global Interrupt
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; // 占先优先级：0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;        // 抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           // 中断使能
    NVIC_Init(&NVIC_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;           // TIM3 global Interrupt
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; // 占先优先级：0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;        // 抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           // 中断使能
    NVIC_Init(&NVIC_InitStructure);
}

void BPS_TIM_Init(void)
{
    BPS_TIM_NVIC_Config();
    BPS_TIM2_Base_Init();
    BPS_TIM3_Base_Init();
}

