#include "BPS_adc.h"

int16_t Calibrattion_Val = 0;
uint16_t ADC_Buffer[ADC_NBR_OF_CHANNEL] = {0};
uint16_t ADC_Offset_Buffer[ADC_NBR_OF_CHANNEL] = {0};
uint8_t ADC_Offset_Check_Flag = 0;  

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
    ADC_InitStructure.ADC_NbrOfChannel = ADC_NBR_OF_CHANNEL;            // 使用的通道总数
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
    DMA_InitStructure.DMA_BufferSize = ADC_NBR_OF_CHANNEL;                      // DMA 缓存大小
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;            // 外设地址不变
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                     // 内存地址增
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; // 外设：半字 16位
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;         // 内存：半字 16位
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;                             // DMA_Mode_Normal 设置DMA循环模式
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;                     // DMA优先级
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                                // 内存到内存：不使用
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);

    DMA_Cmd(DMA1_Channel1, ENABLE);                     // DMA1 通道1 使能
    DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);     // DMA1 通道1 传输完成中断
}


void BPS_ADC_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure = {0};
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    //// NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;           // TIM3 global Interrupt
    //// NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; // 占先优先级：0
    //// NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;        // 抢占优先级
    //// NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           // 中断使能
    //// NVIC_Init(&NVIC_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void BPS_ADC_Init(void)
{
    ADC_Offset_Check_Flag = BPS_Check_All_Channel_Reset(&CAN_BMS_Info);
    BPS_ADC_NVIC_Config();
    BPS_DMA_ADC_Tx_Init();
    BPS_ADC_Base_Init();
    //// BPS_Set_ADC_Freq();
}

/*
// void BPS_Get_Float_ADC(uint16_t *buf, float *res)
// {
//     res[0] = 3.3f * (buf[0] - ADC_Offset_Buffer[0])  / 4096.0f / CURRENT_GAIN / CURRENT_R1;
//     res[1] = 3.3f * (buf[1] - ADC_Offset_Buffer[1])  / 4096.0f / CURRENT_GAIN / CURRENT_R2;
//     res[2] = 3.3f * (buf[2] - ADC_Offset_Buffer[2])  / 4096.0f / CURRENT_GAIN / CURRENT_R3;
//     res[3] = 3.3f * (buf[3] - ADC_Offset_Buffer[3])  / 4096.0f / CURRENT_GAIN / CURRENT_R4;
//     res[4] = 3.3f * (buf[4] - ADC_Offset_Buffer[4])  / 4096.0f / CURRENT_GAIN / CURRENT_R5;
//     res[5] = 3.3f * (buf[5] - ADC_Offset_Buffer[5])  / 4096.0f / CURRENT_GAIN / CURRENT_R6;
//     res[6] = 3.3f * (buf[6] - ADC_Offset_Buffer[6])  / 4096.0f / CURRENT_GAIN / CURRENT_R7;
//     res[7] = 3.3f * (buf[7] - ADC_Offset_Buffer[7])  / 4096.0f / CURRENT_GAIN / CURRENT_R8;
//     res[8] = 3.3f * (buf[8] - ADC_Offset_Buffer[8])  / 4096.0f * VBUS_KP;
// }
*/

void BPS_Get_Float_ADC(CAN_BMS_Infomation_TypeDef *CAN_BMS_InfomationStructure)
{
    CAN_BMS_InfomationStructure->CH[0].real_time_current = 3.3f * (ADC_Buffer[0] - ADC_Offset_Buffer[0])  / 4096.0f / CURRENT_GAIN / CURRENT_R1;
    CAN_BMS_InfomationStructure->CH[1].real_time_current = 3.3f * (ADC_Buffer[1] - ADC_Offset_Buffer[1])  / 4096.0f / CURRENT_GAIN / CURRENT_R2;
    CAN_BMS_InfomationStructure->CH[2].real_time_current = 3.3f * (ADC_Buffer[2] - ADC_Offset_Buffer[2])  / 4096.0f / CURRENT_GAIN / CURRENT_R3;
    CAN_BMS_InfomationStructure->CH[3].real_time_current = 3.3f * (ADC_Buffer[3] - ADC_Offset_Buffer[3])  / 4096.0f / CURRENT_GAIN / CURRENT_R4;
    CAN_BMS_InfomationStructure->CH[4].real_time_current = 3.3f * (ADC_Buffer[4] - ADC_Offset_Buffer[4])  / 4096.0f / CURRENT_GAIN / CURRENT_R5;
    CAN_BMS_InfomationStructure->CH[5].real_time_current = 3.3f * (ADC_Buffer[5] - ADC_Offset_Buffer[5])  / 4096.0f / CURRENT_GAIN / CURRENT_R6;
    CAN_BMS_InfomationStructure->CH[6].real_time_current = 3.3f * (ADC_Buffer[6] - ADC_Offset_Buffer[6])  / 4096.0f / CURRENT_GAIN / CURRENT_R7;
    CAN_BMS_InfomationStructure->CH[7].real_time_current = 3.3f * (ADC_Buffer[7] - ADC_Offset_Buffer[7])  / 4096.0f / CURRENT_GAIN / CURRENT_R8;
    CAN_BMS_InfomationStructure->battery_voltage = 3.3f * (ADC_Buffer[8] - ADC_Offset_Buffer[8])  / 4096.0f * VBUS_KP;
}




