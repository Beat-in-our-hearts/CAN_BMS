#include "BPS_gpio.h"

// uint8_t Contorl_GPIO_Status[8] = {0};

void BPS_Contorl_IO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    uint8_t i = 0;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // 开启GPIOA通道时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); // 开启GPIOB通道时钟

    // init GPIO ：used define adc channel
    GPIO_InitStructure.GPIO_Pin = Control_IO_CH1_Pin | Control_IO_CH2_Pin | Control_IO_CH3_Pin | Control_IO_CH4_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  // 推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // 端口速度
    GPIO_Init(Control_IO_CH1_4_GPIO_Port, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = Control_IO_CH5_Pin | Control_IO_CH6_Pin | Control_IO_CH7_Pin | Control_IO_CH8_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  // 推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // 端口速度
    GPIO_Init(Control_IO_CH5_8_GPIO_Port, &GPIO_InitStructure);

    // Reset All the Channel 
    for(i = 0; i < 8 ; ++i)
        BPS_Set_Contorl_IO_Status(&CAN_BMS_Info, i, OFF);
}


void BPS_ID_IO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);   // 开启GPIOA通道时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);   // 开启GPIOB通道时钟

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   // 浮空输入：外围电路有上拉电阻
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;       // 端口速度

    GPIO_InitStructure.GPIO_Pin = ID0_Pin;
    GPIO_Init(ID0_GPIO_Port, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = ID1_Pin;
    GPIO_Init(ID1_GPIO_Port, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = ID2_Pin;
    GPIO_Init(ID2_GPIO_Port, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = ID3_Pin;
    GPIO_Init(ID3_GPIO_Port, &GPIO_InitStructure);
}

void BPS_Set_Contorl_IO_Status(CAN_BMS_Infomation_TypeDef *CAN_BMS_Infomation, uint8_t channel_num, CH_State NewState)
{
    if( NewState == ON || NewState == OverCurrentOn ||  NewState == OverPowerOn)
    {
        switch (channel_num)
        {
        case 1:
            GPIO_SetBits(Control_IO_CH1_4_GPIO_Port, Control_IO_CH1_Pin);
            CAN_BMS_Infomation->CH[0].state = NewState;
            break;
        case 2:
            GPIO_SetBits(Control_IO_CH1_4_GPIO_Port, Control_IO_CH2_Pin);
            CAN_BMS_Infomation->CH[1].state = NewState;
            break;
        case 3:
            GPIO_SetBits(Control_IO_CH1_4_GPIO_Port, Control_IO_CH3_Pin);
            CAN_BMS_Infomation->CH[2].state = NewState;
            break;
        case 4:
            GPIO_SetBits(Control_IO_CH1_4_GPIO_Port, Control_IO_CH4_Pin);
            CAN_BMS_Infomation->CH[3].state = NewState;
            break;
        case 5:
            GPIO_SetBits(Control_IO_CH5_8_GPIO_Port, Control_IO_CH5_Pin);
            CAN_BMS_Infomation->CH[4].state = NewState;
            break;
        case 6:
            GPIO_SetBits(Control_IO_CH5_8_GPIO_Port, Control_IO_CH6_Pin);
            CAN_BMS_Infomation->CH[5].state = NewState;
            break;
        case 7:
            GPIO_SetBits(Control_IO_CH5_8_GPIO_Port, Control_IO_CH7_Pin);
            CAN_BMS_Infomation->CH[6].state = NewState;
            break;
        case 8:
            GPIO_SetBits(Control_IO_CH5_8_GPIO_Port, Control_IO_CH8_Pin);
            CAN_BMS_Infomation->CH[7].state = NewState;
            break;
        default:
            break;
        }
    }
    else
    {
        switch (channel_num)
        {
        case 1:
            GPIO_ResetBits(Control_IO_CH1_4_GPIO_Port, Control_IO_CH1_Pin);
            CAN_BMS_Infomation->CH[0].state = NewState;
            break;
        case 2:
            GPIO_ResetBits(Control_IO_CH1_4_GPIO_Port, Control_IO_CH2_Pin);
            CAN_BMS_Infomation->CH[1].state = NewState;
            break;
        case 3:
            GPIO_ResetBits(Control_IO_CH1_4_GPIO_Port, Control_IO_CH3_Pin);
            CAN_BMS_Infomation->CH[2].state = NewState;
            break;
        case 4:
            GPIO_ResetBits(Control_IO_CH1_4_GPIO_Port, Control_IO_CH4_Pin);
            CAN_BMS_Infomation->CH[3].state = NewState;
            break;
        case 5:
            GPIO_ResetBits(Control_IO_CH5_8_GPIO_Port, Control_IO_CH5_Pin);
            CAN_BMS_Infomation->CH[4].state = NewState;
            break;
        case 6:
            GPIO_ResetBits(Control_IO_CH5_8_GPIO_Port, Control_IO_CH6_Pin);
            CAN_BMS_Infomation->CH[5].state = NewState;
            break;
        case 7:
            GPIO_ResetBits(Control_IO_CH5_8_GPIO_Port, Control_IO_CH7_Pin);
            CAN_BMS_Infomation->CH[6].state = NewState;
            break;
        case 8:
            GPIO_ResetBits(Control_IO_CH5_8_GPIO_Port, Control_IO_CH8_Pin);
            CAN_BMS_Infomation->CH[7].state = NewState;
            break;
        default:
            break;
        }
    }
}

uint8_t BPS_Check_All_Channel_Reset(CAN_BMS_Infomation_TypeDef *CAN_BMS_Infomation)
{
    uint8_t i = 0;
    for(i = 0; i < 8; ++i)
    {
        if(CAN_BMS_Infomation->CH[i].state != OFF)
            return 0;
    }
    return 1;
}

