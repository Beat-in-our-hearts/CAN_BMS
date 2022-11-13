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
    ADC_DeInit(ADC1);                                                   // clear adc setting
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;                  // Independent mode 独立模式
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;              // data right 右对齐
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;                        // Scan enable 多通道模式
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;                  // 连续转换模式
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; // 软件触发
    ADC_InitStructure.ADC_NbrOfChannel = Used_ADC_NbrOfChannel;         // 使用的通道总数
    ADC_Init(ADC1, &ADC_InitStructure);                                 // init adc

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

    ADC_ExternalTrigConvCmd(ADC1, ENABLE);          // 外部触发使能 【非常重要】
    ADC_DMACmd(ADC1, ENABLE);                       // 使能DMA ADC 【注意使用了DMA后，不需要使能ADC中断】
    ADC_Cmd(ADC1, ENABLE);                          // 使能ADC

    ADC_ResetCalibration(ADC1);                     // 使能ADC1复位校准寄存器
    while (ADC_GetResetCalibrationStatus(ADC1));    // 复位完毕
    ADC_StartCalibration(ADC1);                     // 开始校准
    while (ADC_GetCalibrationStatus(ADC1));         // 校准完毕
    Calibrattion_Val = Get_CalibrationValue(ADC1);  // 获取校准ADC值

    ADC_SoftwareStartConvCmd(ADC1, ENABLE);         // 连续模式开始采集ADC
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

    DMA_Cmd(DMA1_Channel1, ENABLE);                     // DMA1 通道1 使能
    DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);     // DMA1 通道1 传输完成中断
}


void BPS_ADC_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure = {0};
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

    // NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;           // TIM3 global Interrupt
    // NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; // 占先优先级：0
    // NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;        // 抢占优先级
    // NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           // 中断使能
    // NVIC_Init(&NVIC_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void BPS_ADC_Init(void)
{
    BPS_ADC_NVIC_Config();
    BPS_DMA_ADC_Tx_Init();
    BPS_ADC_Base_Init();
    // BPS_Set_ADC_Freq();
}

void BPS_Get_Float_ADC(uint16_t *buf, float *res)
{
    uint8_t i = 0;
    for(i = 0; i < Used_ADC_NbrOfChannel - 1; ++i)
        res[i] = 3.3f * buf[i] / 4096.0f / Current_Gain / Current_R;
    res[i] = 3.3f * buf[i] / 4096.0f * V_Total_R / V_R2;
}



void BPS_Count_Power(float *adc_res, float deleta_t, folat *power_res)
{
    uint8_t i = 0;
    for(i = 0; i < Used_ADC_NbrOfChannel - 1; ++i)
        power_res[i] =  adc_res[i] * adc_res[Used_ADC_NbrOfChannel -1] / deleta_t;
}





void DMA1_Channel1_IRQHandler(void)
{
    static uint32_t cnt = 0;
    
    if (DMA_GetITStatus(DMA1_IT_TC1)) // 传输完成中断
    {
        DMA_ClearITPendingBit(DMA1_IT_GL1); //清除全部中断标志
        // 可以进行数字滤波




        // cnt++;
        // if(cnt == 10000)
        //     // GPIO_SetBits(Control_IO_CH5_8_GPIO_Port, Control_IO_CH6_Pin);
    }
}

