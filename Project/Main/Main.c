/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2019/10/15
 * Description        : Main program body.
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * SPDX-License-Identifier: Apache-2.0
 *******************************************************************************/


#include "debug.h"
#include "oled.h"
#include "BSP_global_define.h" // 定义绝大多数宏

#include "BSP_adc.h"
#include "BSP_gpio.h"
#include "BSP_can.h"
#include "BSP_timer.h"
#include "BSP_flash.h"

#include "BSP_control.h"

uint8_t can_test_buffer[8] = {0x01,0x02,0x01,0x02,0x01,0x02,0x01,0x02};

/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
int main(void)
{
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  Delay_Init();
  USART_Printf_Init(115200);

  printf("SystemClk:%d\r\n", SystemCoreClock);
  printf("GPIO Toggle TEST\r\n");

  Delay_Ms(1000);
  
  OLED_Init(); 
  OLED_ColorTurn(0);
  OLED_DisplayTurn(1);
  OLED_Clear();
  OLED_ShowString(0, 0, "OLED Loading", 24, 1); // OLED TEST
  OLED_Refresh();

  BSP_Flash_Init();       // 初始化Flash，读取设定参数
  BSP_Contorl_IO_Init();  // IO
  BSP_ID_IO_Init();
  BSP_ADC_Init();         // adc功能
  BSP_CAN_Init();         // can协议
  BSP_TIM_Init();         // 定时器 【主循环】
  OLED_Clear();

  Delay_Ms(1000);
  while (1)
  {
    Delay_Ms(250);
  }
}
