#include "BPS_control.h"

void BPS_Current_Voltage_Protection_Check(CAN_BMS_Infomation_TypeDef * CAN_BMS_Infomation)
{
    uint8_t i = 0;
    // !电流通道检测
    for(i = 0; i < ADC_NBR_OF_CHANNEL - 1; ++i) 
    {
        if(CAN_BMS_Infomation->current_voltage_buffer[i] > MAX_CURRENT) // 电流超限，关闭对应通道
        {
            BPS_Set_Contorl_IO_Status(CAN_BMS_Infomation, i, DISABLE);
            // TODO： 电流超限报警 can通信
        }
    }
    // !电压通道检测
    if(CAN_BMS_Infomation->current_voltage_buffer[ADC_NBR_OF_CHANNEL-1] > MAX_VOLTAGE) // 电压超限，关闭所有通道
    {
        for(i = 0; i < ADC_NBR_OF_CHANNEL-1; ++i)
        {
            BPS_Set_Contorl_IO_Status(CAN_BMS_Infomation, i, DISABLE); 
        }
        // TODO：电压超限报警 can通信
    }
    else if(CAN_BMS_Infomation->current_voltage_buffer[ADC_NBR_OF_CHANNEL-1] < MIN_VOLTAGE) // 电压过低，关闭所有通道
    {
        for(i = 0; i < ADC_NBR_OF_CHANNEL-1; ++i)
        {
            BPS_Set_Contorl_IO_Status(CAN_BMS_Infomation, i, DISABLE); 
        }
        // TODO：电压过低报警 can通信
    }
}



void BPS_CAN_BMS_Info_Update(CAN_BMS_Infomation_TypeDef * CAN_BMS_Infomation)
{
    uint8_t i = 0;
    float temp_total_instantaneous_power = 0;
    float temp_total_cumulative_power = 0;
    for(i = 0; i < ADC_NBR_OF_CHANNEL - 1; ++i)
    {
        CAN_BMS_Infomation->instantaneous_power[i] = CAN_BMS_Infomation->current_voltage_buffer[i] * CAN_BMS_Infomation->current_voltage_buffer[ADC_NBR_OF_CHANNEL -1]; // DELTA_T;
        if(CAN_BMS_Infomation->instantaneous_power[i] > CAN_BMS_Infomation->history_max_power[i])
            CAN_BMS_Infomation->history_max_power[i] = CAN_BMS_Infomation->instantaneous_power[i];
        if(CAN_BMS_Infomation -> contorl_GPIO_status[i])
        {
            CAN_BMS_Infomation-> work_time[i] ++;
            CAN_BMS_Infomation-> cumulative_power[i] += CAN_BMS_Infomation-> instantaneous_power[i] * DELTA_T;
            // TODO:on_average_power_buffer 
        }
        temp_total_instantaneous_power += CAN_BMS_Infomation->instantaneous_power[i];
        temp_total_cumulative_power += CAN_BMS_Infomation-> cumulative_power[i];
    }
    CAN_BMS_Infomation->total_instantaneous_power = temp_total_instantaneous_power;
    CAN_BMS_Infomation->total_cumulative_power = temp_total_cumulative_power;
}


void TIM2_IRQHandler(void) // OLED 显示
{
    static uint8_t OLED_Show_Buffer_Line1[30] = {0};
    static uint8_t OLED_Show_Buffer_Line2[30] = {0};
    static uint8_t OLED_Show_Buffer_Line3[30] = {0};
    static uint8_t OLED_page = 0;
    static uint32_t cnt = 0;
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);     // 清TIM2溢出中断标志位
        
        switch (OLED_page)
        {
        case 0:
            sprintf((char *)OLED_Show_Buffer_Line1, "CH1:%4d f_CH1:%.2f  ", ADC_Buffer[0], CAN_BMS_Info.current_voltage_buffer[0]);
            sprintf((char *)OLED_Show_Buffer_Line2, "CH2:%4d f_CH2:%.2f  ", ADC_Buffer[1], CAN_BMS_Info.current_voltage_buffer[1]);
            break;
        case 1:
            sprintf((char *)OLED_Show_Buffer_Line1, "CH3:%4d f_CH3:%.2f  ", ADC_Buffer[2], CAN_BMS_Info.current_voltage_buffer[2]);
            sprintf((char *)OLED_Show_Buffer_Line2, "CH4:%4d f_CH4:%.2f  ", ADC_Buffer[3], CAN_BMS_Info.current_voltage_buffer[3]);
            break;
        case 2:
            sprintf((char *)OLED_Show_Buffer_Line1, "CH5:%4d f_CH5:%.2f  ", ADC_Buffer[4], CAN_BMS_Info.current_voltage_buffer[4]);
            sprintf((char *)OLED_Show_Buffer_Line2, "CH6:%4d f_CH6:%.2f  ", ADC_Buffer[5], CAN_BMS_Info.current_voltage_buffer[5]);
            break;
        case 3:
            sprintf((char *)OLED_Show_Buffer_Line1, "CH7:%4d f_CH7:%.2f  ", ADC_Buffer[6], CAN_BMS_Info.current_voltage_buffer[6]);
            sprintf((char *)OLED_Show_Buffer_Line2, "CH8:%4d f_CH8:%.2f  ", ADC_Buffer[7], CAN_BMS_Info.current_voltage_buffer[7]);
            break;
        case 4:
            sprintf((char *)OLED_Show_Buffer_Line1, "O1:%4d O2:%4d       ", ADC_Offset_Buffer[0], ADC_Offset_Buffer[1]);
            sprintf((char *)OLED_Show_Buffer_Line2, "O3:%4d O4:%4d       ", ADC_Offset_Buffer[2], ADC_Offset_Buffer[3]);
            break;
        case 5:
            sprintf((char *)OLED_Show_Buffer_Line1, "O5:%4d O6:%4d       ", ADC_Offset_Buffer[4], ADC_Offset_Buffer[5]);
            sprintf((char *)OLED_Show_Buffer_Line2, "O7:%4d O8:%4d       ", ADC_Offset_Buffer[6], ADC_Offset_Buffer[7]);
            break;
        default:
            break;
        }
        sprintf((char *)OLED_Show_Buffer_Line3, "Vol:%4d %2d  %2.2f", ADC_Buffer[8], ADC_Offset_Buffer[8],CAN_BMS_Info.current_voltage_buffer[8]);
        cnt++;
        if(cnt % 20 == 0) // 2s换页
        {
            OLED_page++;
            if(OLED_page > 5)
            {
                OLED_page = 0;
                cnt = 0;
            }
        }
        // OLED_Clear();
        OLED_ShowString(0, 0, OLED_Show_Buffer_Line1, 8, 1);
        OLED_ShowString(0, 8, OLED_Show_Buffer_Line2, 8, 1);
        OLED_ShowString(0, 16, OLED_Show_Buffer_Line3, 8, 1);
        OLED_Refresh();
    }
}


void TIM3_IRQHandler(void) // 定时中断检测ADC
{
    // static uint32_t CNT = 0;
    // static float adc_res[ADC_NBR_OF_CHANNEL], power_res[ADC_NBR_OF_CHANNEL - 1];
    // uint8_t i = 0;
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);     // 清TIM3溢出中断标志位
        BPS_Get_Float_ADC(ADC_Buffer, CAN_BMS_Info.current_voltage_buffer);          // 计算电压电流数据
        // // BPS_Current_Voltage_Protection_Check(adc_res);          // 电流报警检测
        // // BPS_Voltage_Protection_Check(adc_res);          // 电压报警检测
        // // BPS_Count_Power(adc_res, power_res);            // 


    }
}


void USB_LP_CAN1_RX0_IRQHandler(void) // CAN 通信中断处理
{
    uint8_t i = 0;
    static uint8_t can_test_buffer[8] ={0};
    CanRxMsg CanRxStructure = {0};
    CAN_Receive(CAN1, CAN_FIFO0, &CanRxStructure); // 读取FIFO0中缓存的can数据
    for( i = 0; i < 8; i++ ){
        can_test_buffer[i] = CanRxStructure.Data[i];
    }
    // TODO: 接受到对应命令后传输对应CAN帧数据

		BPS_CAN_Send_Msg(can_test_buffer,8); 
    //// CanRxStructure.Data[i] 8 字节消息
}


