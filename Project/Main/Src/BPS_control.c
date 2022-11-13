#include "BPS_control.h"

void Current_Protection_Check(float *current_buf)
{
    if(current_buf[0] > MAX_Current)
    {
        GPIO_ResetBits(Control_IO_CH1_4_GPIO_Port,Control_IO_CH1_Pin)
    }
    if(current_buf[1] > MAX_Current)
    {
        GPIO_ResetBits(Control_IO_CH1_4_GPIO_Port,Control_IO_CH2_Pin)
    }
    if(current_buf[2] > MAX_Current)
    {
        GPIO_ResetBits(Control_IO_CH1_4_GPIO_Port,Control_IO_CH3_Pin)
    }
    if(current_buf[3] > MAX_Current)
    {
        GPIO_ResetBits(Control_IO_CH1_4_GPIO_Port,Control_IO_CH4_Pin)
    }
    if(current_buf[4] > MAX_Current)
    {
        GPIO_ResetBits(Control_IO_CH5_8_GPIO_Port,Control_IO_CH5_Pin)
    }
    if(current_buf[5] > MAX_Current)
    {
        GPIO_ResetBits(Control_IO_CH5_8_GPIO_Port,Control_IO_CH6_Pin)
    }
    if(current_buf[6] > MAX_Current)
    {
        GPIO_ResetBits(Control_IO_CH5_8_GPIO_Port,Control_IO_CH7_Pin)
    }
    if(current_buf[7] > MAX_Current)
    {
        GPIO_ResetBits(Control_IO_CH5_8_GPIO_Port,Control_IO_CH8_Pin)
    }
}

void Voltage_Protection_Check(float *current_buf)
{
    if(current_buf[8] > MAX_Voltage)
    {
        GPIO_ResetBits(Control_IO_CH1_4_GPIO_Port,Control_IO_CH1_Pin);
        GPIO_ResetBits(Control_IO_CH1_4_GPIO_Port,Control_IO_CH2_Pin);
        GPIO_ResetBits(Control_IO_CH1_4_GPIO_Port,Control_IO_CH3_Pin);
        GPIO_ResetBits(Control_IO_CH1_4_GPIO_Port,Control_IO_CH4_Pin);
        GPIO_ResetBits(Control_IO_CH5_8_GPIO_Port,Control_IO_CH5_Pin);
        GPIO_ResetBits(Control_IO_CH5_8_GPIO_Port,Control_IO_CH6_Pin);
        GPIO_ResetBits(Control_IO_CH5_8_GPIO_Port,Control_IO_CH7_Pin);
        GPIO_ResetBits(Control_IO_CH5_8_GPIO_Port,Control_IO_CH8_Pin);
    }
    else if(current_buf[8] < MIN_Voltage)
    {
        GPIO_ResetBits(Control_IO_CH1_4_GPIO_Port,Control_IO_CH1_Pin);
        GPIO_ResetBits(Control_IO_CH1_4_GPIO_Port,Control_IO_CH2_Pin);
        GPIO_ResetBits(Control_IO_CH1_4_GPIO_Port,Control_IO_CH3_Pin);
        GPIO_ResetBits(Control_IO_CH1_4_GPIO_Port,Control_IO_CH4_Pin);
        GPIO_ResetBits(Control_IO_CH5_8_GPIO_Port,Control_IO_CH5_Pin);
        GPIO_ResetBits(Control_IO_CH5_8_GPIO_Port,Control_IO_CH6_Pin);
        GPIO_ResetBits(Control_IO_CH5_8_GPIO_Port,Control_IO_CH7_Pin);
        GPIO_ResetBits(Control_IO_CH5_8_GPIO_Port,Control_IO_CH8_Pin);
    }
}


void TIM3_IRQHandler(void) // 定时中断检测ADC
{
    static uint32_t CNT = 0;
    static float adc_res[Used_ADC_NbrOfChannel], power_res[Used_ADC_NbrOfChannel - 1];
    uint8_t i = 0;
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update); // 清TIM3溢出中断标志位
        BPS_Get_Float_ADC(ADC_Buffer,adc_res);
        Current_Protection_Check(adc_res);
        BPS_Count_Power(adc_res, Deleta_T, power_res);


    }
}