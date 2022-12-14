#include "BSP_control.h"

void BSP_Protection_Policy_Check(CAN_BMS_Infomation_TypeDef * CAN_BMS_InfomationStructure)
{
    uint8_t i = 0;
    uint8_t current_flag = 0, power_flag = 0, low_voltage_flag = 0, high_voltage_flag = 0;
    for(i = 0; i < OUTPUT_CHANNEL; ++i) 
    {
        current_flag = (CAN_BMS_InfomationStructure->Protection_Policy[i] & 0x11000000)>>6;
        power_flag = (CAN_BMS_InfomationStructure->Protection_Policy[i] & 0x00110000)>>4;
        low_voltage_flag = (CAN_BMS_InfomationStructure->Protection_Policy[i] & 0x00001100)>>2;
        high_voltage_flag = CAN_BMS_InfomationStructure->Protection_Policy[i] & 0x00000011;
        switch (current_flag)
        {
        case None_Policy:
            if(CAN_BMS_InfomationStructure->CH[i].real_time_current > CAN_BMS_InfomationStructure->Channel_Max_Current[i])
            {
                BSP_Set_Contorl_IO_Status(CAN_BMS_InfomationStructure, i, OverCurrentOn);
            }
            break;
        case Trip_Policy:
            if(CAN_BMS_InfomationStructure->CH[i].real_time_current > CAN_BMS_InfomationStructure->Channel_Max_Current[i])
            {
                BSP_Set_Contorl_IO_Status(CAN_BMS_InfomationStructure, i, OverCurrentOFF);
            }
            break;
        case Trip_Recover_Policy:
            if(CAN_BMS_InfomationStructure->CH[i].real_time_current > CAN_BMS_InfomationStructure->Channel_Max_Current[i])
            {
                BSP_Set_Contorl_IO_Status(CAN_BMS_InfomationStructure, i, OverCurrentOFF);
            }
            // TODO :恢复
            break;
        default:
            break;
        }
        switch (power_flag)
        {
        case None_Policy:
            if(CAN_BMS_InfomationStructure->CH[i].read_time_power > CAN_BMS_InfomationStructure->Channel_Max_Power[i])
            {
                BSP_Set_Contorl_IO_Status(CAN_BMS_InfomationStructure, i, OverPowerOn);
            }
            break;
        case Trip_Policy:
            if(CAN_BMS_InfomationStructure->CH[i].read_time_power > CAN_BMS_InfomationStructure->Channel_Max_Power[i])
            {
                BSP_Set_Contorl_IO_Status(CAN_BMS_InfomationStructure, i, OverPowerOFF);
            }
            break;
        case Trip_Recover_Policy:
            if(CAN_BMS_InfomationStructure->CH[i].read_time_power > CAN_BMS_InfomationStructure->Channel_Max_Power[i])
            {
                BSP_Set_Contorl_IO_Status(CAN_BMS_InfomationStructure, i, OverPowerOFF);
            }
            // TODO :恢复
            break;
        default:
            break;
        }
        switch (low_voltage_flag)
        {
            case None_Policy:
                if(CAN_BMS_InfomationStructure->battery_voltage < CAN_BMS_InfomationStructure->Min_Max_Voltage[0])
                {
                    BSP_Set_Contorl_IO_Status(CAN_BMS_InfomationStructure, i, ON);
                }
                break;
            case Trip_Policy:
                if(CAN_BMS_InfomationStructure->battery_voltage < CAN_BMS_InfomationStructure->Min_Max_Voltage[0])
                {
                    BSP_Set_Contorl_IO_Status(CAN_BMS_InfomationStructure, i, LowVoltageOFF);
                }
                break;
            case Trip_Recover_Policy:
                if(CAN_BMS_InfomationStructure->battery_voltage < CAN_BMS_InfomationStructure->Min_Max_Voltage[0])
                {
                    BSP_Set_Contorl_IO_Status(CAN_BMS_InfomationStructure, i, LowVoltageOFF);
                }
                // TODO :恢复
                break;
            default:
                break;
        }

        switch (high_voltage_flag)
        {
            case None_Policy:
                if(CAN_BMS_InfomationStructure->battery_voltage > CAN_BMS_InfomationStructure->Min_Max_Voltage[1])
                {
                    BSP_Set_Contorl_IO_Status(CAN_BMS_InfomationStructure, i, ON);
                }
                break;
            case Trip_Policy:
                if(CAN_BMS_InfomationStructure->battery_voltage > CAN_BMS_InfomationStructure->Min_Max_Voltage[1])
                {
                    BSP_Set_Contorl_IO_Status(CAN_BMS_InfomationStructure, i, HighVoltageOFF);
                }
                break;
            case Trip_Recover_Policy:
                if(CAN_BMS_InfomationStructure->battery_voltage> CAN_BMS_InfomationStructure->Min_Max_Voltage[1])
                {
                    BSP_Set_Contorl_IO_Status(CAN_BMS_InfomationStructure, i, HighVoltageOFF);
                }
                // TODO :恢复
                break;
            default:
                break;
        }
    }
}



void BSP_CAN_BMS_Info_Update(CAN_BMS_Infomation_TypeDef * CAN_BMS_Infomation)
{
    uint8_t i = 0;
    float temp_total_instantaneous_power = 0;
    float temp_total_cumulative_power = 0;
    CH_State temp_state = OFF;
    for(i = 0; i < OUTPUT_CHANNEL; ++i)
    {
        CAN_BMS_Infomation->CH[i].read_time_power = CAN_BMS_Infomation->CH[i].real_time_current * CAN_BMS_Infomation->battery_voltage;
        if(CAN_BMS_Infomation->CH[i].read_time_power > CAN_BMS_Infomation->CH[i].history_max_power)
            CAN_BMS_Infomation->CH[i].history_max_power = CAN_BMS_Infomation->CH[i].read_time_power;
        temp_state = CAN_BMS_Infomation->CH[i].state;
        if(temp_state == ON || temp_state == OverCurrentOn ||  temp_state == OverPowerOn)
        {
            CAN_BMS_Infomation->CH[i].work_time += DELTA_T_ns;
            CAN_BMS_Infomation->CH[i].cumulative_power += CAN_BMS_Infomation->CH[i].read_time_power * DELTA_T;
            // TODO:on_average_power_buffer
        }
        temp_total_instantaneous_power += CAN_BMS_Infomation->CH[i].read_time_power;
        temp_total_cumulative_power += CAN_BMS_Infomation->CH[i].cumulative_power;
    }
    CAN_BMS_Infomation->total_read_time_power = temp_total_instantaneous_power;
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
            sprintf((char *)OLED_Show_Buffer_Line1, "CH1:%4d f_CH1:%.2f  ", ADC_Buffer[0], CAN_BMS_Info.CH[0].real_time_current);
            sprintf((char *)OLED_Show_Buffer_Line2, "CH2:%4d f_CH2:%.2f  ", ADC_Buffer[1], CAN_BMS_Info.CH[1].real_time_current);
            break;
        case 1:
            sprintf((char *)OLED_Show_Buffer_Line1, "CH3:%4d f_CH3:%.2f  ", ADC_Buffer[2], CAN_BMS_Info.CH[2].real_time_current);
            sprintf((char *)OLED_Show_Buffer_Line2, "CH4:%4d f_CH4:%.2f  ", ADC_Buffer[3], CAN_BMS_Info.CH[3].real_time_current);
            break;
        case 2:
            sprintf((char *)OLED_Show_Buffer_Line1, "CH5:%4d f_CH5:%.2f  ", ADC_Buffer[4], CAN_BMS_Info.CH[4].real_time_current);
            sprintf((char *)OLED_Show_Buffer_Line2, "CH6:%4d f_CH6:%.2f  ", ADC_Buffer[5], CAN_BMS_Info.CH[5].real_time_current);
            break;
        case 3:
            sprintf((char *)OLED_Show_Buffer_Line1, "CH7:%4d f_CH7:%.2f  ", ADC_Buffer[6], CAN_BMS_Info.CH[6].real_time_current);
            sprintf((char *)OLED_Show_Buffer_Line2, "CH8:%4d f_CH8:%.2f  ", ADC_Buffer[7], CAN_BMS_Info.CH[7].real_time_current);
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
        sprintf((char *)OLED_Show_Buffer_Line3, "Vol:%4d %2d  %2.2f", ADC_Buffer[8], ADC_Offset_Buffer[8],CAN_BMS_Info.battery_voltage);
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
        BSP_Get_Float_ADC(&CAN_BMS_Info);               // 计算电压电流数据
        BSP_CAN_BMS_Info_Update(&CAN_BMS_Info);         // 更新信息
        BSP_Protection_Policy_Check(&CAN_BMS_Info);     // 电压电流功率报警检测
        // // BSP_Current_Voltage_Protection_Check(adc_res);          // 电流报警检测
        // // BSP_Voltage_Protection_Check(adc_res);          // 电压报警检测
        // // BSP_Count_Power(adc_res, power_res);            // 


    }
}


void USB_LP_CAN1_RX0_IRQHandler(void) // CAN 通信中断处理
{
    uint8_t i = 0;
    uint8_t cmd = 0;
    static uint8_t can_buffer[8] ={0};
    CH_State temp_state = OFF;
    CanRxMsg CanRxStructure = {0};
    CAN_Receive(CAN1, CAN_FIFO0, &CanRxStructure); // 读取FIFO0中缓存的can数据
    // 接受到对应命令后传输对应CAN帧数据
    if(CanRxStructure.IDE == CAN_Id_Standard && CanRxStructure.RTR == CAN_RTR_Data)
    {
        cmd = CanRxStructure.StdId & 0x01F; // 后5位为指令位
        switch (cmd)
        {
        case 0x00:
            for(i = 0; i < OUTPUT_CHANNEL; ++i)
            {
                BSP_Set_Contorl_IO_Status(&CAN_BMS_Info, i, OFF); 
            }
            break;
        case 0x01:
            for(i = 0; i < 8; ++i)
            {
                switch (CanRxStructure.Data[i])
                {
                case 0:
                    temp_state = OFF;
                    break;
                case 1:
                    temp_state = ON;
                    break;
                case 2:
                    temp_state = OverCurrentOn;
                    break;
                case 3:
                    temp_state = OverPowerOn;
                    break;
                case 4:
                    temp_state = OverCurrentOFF;
                    break;
                case 5:
                    temp_state = OverPowerOFF;
                    break;
                case 6:
                    temp_state = LowVoltageOFF;
                    break;
                case 7:
                    temp_state = HighVoltageOFF;
                    break;
                default:
                    break;
                }
                BSP_Set_Contorl_IO_Status(&CAN_BMS_Info, i, temp_state); 
            }
            break;
        case 0x02:
            BSP_CAN_Send_Msg(CAN_ID, cmd, CAN_BMS_Info.Module_Type, 8);
            break;
        case 0x03:
            BSP_CAN_Send_Msg(CAN_ID, cmd, CAN_BMS_Info.Firmware_Version, 2);
            break;
        case 0x04:
            BSP_CAN_Send_Msg(CAN_ID, cmd, CAN_BMS_Info.Protection_Policy, 8);
            break;
        case 0x05:
            for(i = 0; i < OUTPUT_CHANNEL; ++i)
            {
                can_buffer[i] = CAN_BMS_Info.CH[i].state;
            }
            BSP_CAN_Send_Msg(CAN_ID, cmd, can_buffer, 8);
            break;
        case 0x06:
            BSP_CAN_Send_Msg(CAN_ID, cmd, (uint8_t*)&CAN_BMS_Info.battery_voltage, 4);
            break;
        case 0x07:
            BSP_CAN_Send_Msg(CAN_ID, cmd, (uint8_t*)&CAN_BMS_Info.total_read_time_power, 4);
            break;
        case 0x08:
            BSP_CAN_Send_Msg(CAN_ID, cmd, (uint8_t*)&CAN_BMS_Info.total_cumulative_power, 8); // double
            break;
        case 0x09:            
            BSP_CAN_Send_Msg(CAN_ID, cmd, (uint8_t*)&CAN_BMS_Info.CH[CanRxStructure.Data[0] - 1].real_time_current, 4);
            break;
        case 0x0A:            
            BSP_CAN_Send_Msg(CAN_ID, cmd, (uint8_t*)&CAN_BMS_Info.CH[CanRxStructure.Data[0] - 1].read_time_power, 4);
            break;
        case 0x0B:            
            BSP_CAN_Send_Msg(CAN_ID, cmd, (uint8_t*)&CAN_BMS_Info.CH[CanRxStructure.Data[0] - 1].cumulative_power, 8); // double
            break;
        case 0x0C:            
            BSP_CAN_Send_Msg(CAN_ID, cmd, (uint8_t*)&CAN_BMS_Info.Channel_Max_Current[CanRxStructure.Data[0] - 1], 4);
            break;
        case 0x0D:            
            BSP_CAN_Send_Msg(CAN_ID, cmd, (uint8_t*)&CAN_BMS_Info.Channel_Max_Power[CanRxStructure.Data[0] - 1], 4);
            break;
        case 0x0E:            
            BSP_CAN_Send_Msg(CAN_ID, cmd, (uint8_t*)CAN_BMS_Info.Min_Max_Voltage, 8);
            break;
        case 0x0F:            
            BSP_CAN_Send_Msg(CAN_ID, cmd, (uint8_t*)&CAN_BMS_Info.Total_Max_Power, 4);
            break;

        case 0x10:
            CAN_BMS_Info.Channel_Max_Power[CanRxStructure.Data[4] - 1] = ((float*)CanRxStructure.Data)[0];
            break;
        case 0x11:
            CAN_BMS_Info.Channel_Max_Current[CanRxStructure.Data[4] - 1] = ((float*)CanRxStructure.Data)[0];
            break;
        case 0x12:
            CAN_BMS_Info.Min_Max_Voltage[0] = ((float*)CanRxStructure.Data)[0];
            CAN_BMS_Info.Min_Max_Voltage[1] = ((float*)CanRxStructure.Data)[1];
            break;
        case 0x13:
            CAN_BMS_Info.Total_Max_Power = ((float*)CanRxStructure.Data)[0];
            break;
        case 0x14:
            for(i = 0; i < OUTPUT_CHANNEL; ++i)
            {
                CAN_BMS_Info.Protection_Policy[i] = CanRxStructure.Data[i]; 
            }
            break;
        default:
            break;
        }
        if(cmd == 0x10 || cmd == 0x11 || cmd == 0x12 || cmd == 0x13 || cmd == 0x14 ) // 写flash指令
        {
            BSP_Flash_Write_Setting(&CAN_BMS_Info);
        }

    }
}



void DMA1_Channel1_IRQHandler(void)
{
    static uint32_t cnt = 0;
    static uint32_t sum_offset[ADC_NBR_OF_CHANNEL] = {0};
    static uint8_t i = 0;
    if (DMA_GetITStatus(DMA1_IT_TC1)) // 传输完成中断
    {
        DMA_ClearITPendingBit(DMA1_IT_GL1); //清除全部中断标志
        // 可以进行数字滤波
        if(cnt < 100 && ADC_Offset_Check_Flag)
        {   
            for(i = 0; i < ADC_NBR_OF_CHANNEL; ++i)
            {
                sum_offset[i] += ADC_Buffer[i];
            }
            cnt ++;
        }
        else if(cnt == 100)
        {
            for(i = 0; i < ADC_NBR_OF_CHANNEL; ++i)
            {
                ADC_Offset_Buffer[i] = sum_offset[i] / 100;
            }
        }
    }
}
