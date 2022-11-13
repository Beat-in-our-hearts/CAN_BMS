#include "BPS_gpio.h"

void BPS_Contorl_IO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

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

