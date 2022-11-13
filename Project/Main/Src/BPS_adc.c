#include "BPS_adc.h"

int16_t Calibrattion_Val = 0;
int16_t ADC_Buffer[Used_ADC_NbrOfChannel] = {0};

void BPS_ADC_Base_Init(void)
{
    ADC_InitTypeDef ADC_InitStructure = {0};
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    // 时钟配置 APB2外设时钟与ADC时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // 开启GPIOA通道时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); // 开启GPIOB通道时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);  // 开启ADC1时钟
    RCC_ADCCLKConfig(RCC_PCLK2_Div8);                     // 8分频  系统默认采用72M主频 72M/8=9MHz

    // init GPIO ：used define adc channel
    GPIO_InitStructure.GPIO_Pin = ADC_CH1_Pin | ADC_CH2_Pin | ADC_CH3_Pin | ADC_CH4_Pin | ADC_CH5_Pin | ADC_CH6_Pin | ADC_CH7_Pin | ADC_CH8_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; // Analog input
    GPIO_Init(ADC_CH1_8_GPIO_Port, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = V_CH_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; // Analog input
    GPIO_Init(V_CH_GPIO_Port, &GPIO_InitStructure);

    // init adc channel
    ADC_DeInit(ADC1);                                                      // clear adc setting
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;                     // Independent mode 独立模式
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;                 // data right 右对齐
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;                           // Scan enable 多通道模式
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;                    // 连续转换模式
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T3_TRGO; // Timer3 trigger 定时器3触发事件
    ADC_InitStructure.ADC_NbrOfChannel = Used_ADC_NbrOfChannel;            // 使用的通道总数
    ADC_Init(ADC1, &ADC_InitStructure);                                    // init adc

    // init RegularChannel 规则采集顺序
    ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 1, ADC_SampleTime_239Cycles5); // 1-8 Current ch
    ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 2, ADC_SampleTime_239Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 3, ADC_SampleTime_239Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 4, ADC_SampleTime_239Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 5, ADC_SampleTime_239Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 6, ADC_SampleTime_239Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 7, ADC_SampleTime_239Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 8, ADC_SampleTime_239Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 9, ADC_SampleTime_239Cycles5); // battary vol

    ADC_ExternalTrigConvCmd(ADC1, ENABLE);          // 外部触发使能【非常重要】
    ADC_DMACmd(ADC1, ENABLE);                       // 使能DMA ADC
    ADC_Cmd(ADC1, ENABLE);                          // 使能ADC

    ADC_ResetCalibration(ADC1);                     // 使能ADC1复位校准寄存器
    while (ADC_GetResetCalibrationStatus(ADC1));    // 复位完毕
    ADC_StartCalibration(ADC1);                     // 开始校准
    while (ADC_GetCalibrationStatus(ADC1));         // 校准完毕
    Calibrattion_Val = Get_CalibrationValue(ADC1);  // 获取校准ADC值
}

void BPS_DMA_ADC_Tx_Init(void)
{
    DMA_InitTypeDef DMA_InitStructure = {0};

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); // 开启DMA1时钟线

    DMA_DeInit(DMA1_Channel1);                                                  // clear dma setting
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->RDATAR;         // 外设地址：ADC规则数据寄存器
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ADC_Buffer;                // 内存地址
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;                          // 传输方向：外设到内存
    DMA_InitStructure.DMA_BufferSize = Used_ADC_NbrOfChannel;                   // DMA 缓存大小
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;            // 外设地址不变
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                     // 内存地址增
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; // 外设：半字 16位
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;         // 内存：半字 16位
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;                             // DMA_Mode_Normal;            // 设置DMA循环模式
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;                     // DMA优先级
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                                // 内存到内存：不使用
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);
    DMA_Cmd(DMA1_Channel1, ENABLE);

    // DMA_ITConfig(DMA1_Channel1,DMA_IT_TC,ENABLE); //´«Êä½áÊøÖÐ¶Ï
}

void BPS_Set_ADC_Freq(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure = {0};

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

#if (ADC_Freq_Mode == ADC_Freq_1KHz)
    TIM_TimeBaseInitStructure.TIM_Period = 1000 - 1;
    TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;
#elif (ADC_Freq_Mode == ADC_Freq_2KHz)
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

void BPS_ADC_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure = {0};
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;           // TIM3 global Interrupt
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; // 占先优先级：0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;        // 抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           // 中断使能
    NVIC_Init(&NVIC_InitStructure);

      
    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;     
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;          
    NVIC_Init(&NVIC_InitStructure);

    TIM_ClearFlag(TIM3, TIM_FLAG_Update);      //清除标志位，以免一启用中断后立即产生中断
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE); //使能TIM3溢出中断
}

void BPS_ADC_Init(void)
{
    BPS_ADC_Base_Init();
    BPS_DMA_ADC_Tx_Init();
    BPS_Set_ADC_Freq();
    BPS_ADC_NVIC_Config();
}

void TIM3_IRQHandler(void)
{
    static uint32_t CNT = 0;
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update); // 清TIM3溢出中断标志位
        CNT++;
        if (CNT == 1000)
        {
            // GPIO_SetBits(Control_IO_CH1_4_GPIO_Port, Control_IO_CH2_Pin);
        }
        else if (CNT == 2000)
        {
            // PIO_ResetBits(Control_IO_CH1_4_GPIO_Port, Control_IO_CH2_Pin);
            CNT = 0;
        }
    }
}


void DMA1_Channel1_IRQHandler(void)
{
    if(DMA_GetITStatus(DMA1_IT_TC1)) // 传输完成中断
    {
        DMA_ClearITPendingBit(DMA1_IT_GL1); //清除全部中断标志
        // 可以进行数字滤波

    }
}

