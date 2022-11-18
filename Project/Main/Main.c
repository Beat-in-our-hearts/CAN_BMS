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
#include "BPS_global_define.h"
#include "BPS_adc.h"
#include "BPS_gpio.h"
#include "BPS_can.h"
#include "BPS_timer.h"
#include "BPS_control.h"


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

  OLED_ShowString(0, 0, "OLED TEST", 24, 1); // OLED TEST
  OLED_Refresh();

  Delay_Ms(1000);
  BPS_Contorl_IO_Init();
  BPS_ID_IO_Init();
  BPS_ADC_Init();
  BPS_CAN_Init();
  BPS_TIM_Init();
  OLED_Clear();

  Delay_Ms(1000);
  while (1)
  {
    Delay_Ms(250);
  }
}
