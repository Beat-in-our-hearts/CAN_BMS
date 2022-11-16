/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2019/10/15
 * Description        : Main program body.
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * SPDX-License-Identifier: Apache-2.0
 *******************************************************************************/

/*
 *@Note
 GPIO例程：
 PA0推挽输出。
*/

#include "debug.h"
#include "oled.h"
#include "BPS_global_define.h"
#include "BPS_adc.h"
#include "BPS_gpio.h"
#include "BPS_can.h"
#include "BPS_timer.h"
#include "BPS_control.h"

//#include "bmp.h"
/* Global define */

/*********************************************************************
 * @fn      GPIO_Toggle_INIT
 *
 * @brief   Initializes GPIOA.0
 *
 * @return  none
 */
// void GPIO_Toggle_INIT(void)
// {
//   GPIO_InitTypeDef GPIO_InitStructure = {0};

//   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
//   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
//   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//   GPIO_Init(GPIOB, &GPIO_InitStructure);

//   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
//   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//   GPIO_Init(GPIOB, &GPIO_InitStructure);

//   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
//   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//   GPIO_Init(GPIOB, &GPIO_InitStructure);
// }

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
  //// GPIO_Toggle_INIT();
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
 
  //        OLED_ShowCHinese(16,0,0);// ¼¼
  //        OLED_ShowCHinese(32,0,1);// ÐÂ
  //        OLED_ShowCHinese(48,0,2);// µç
  //        OLED_ShowCHinese(64,0,3);// ×Ó
  //        OLED_ShowCHinese(80,0,4);// ¿Æ
  //        OLED_ShowCHinese(96,0,5);// ¼¼
  //    for(i = 0;i<3;++i)
  Delay_Ms(1000);
  //    OLED_DrawBMP(0,0,128,8,(unsigned char*)BMP1);
  // GPIO_SetBits(Control_IO_CH5_8_GPIO_Port, Control_IO_CH5_Pin);
  while (1)
  {
    // sprintf((char *)oled_string, "ADC_CH5:%d", ADC_Buffer[4]);
    // OLED_ShowString(0, 0, oled_string, 8, 1);
    // sprintf((char *)oled_string, "ADC_CH2:%d", ADC_Buffer[1]);
    // OLED_ShowString(0, 8, oled_string, 8, 1);
    // sprintf((char *)oled_string, "V_CH:%d", ADC_Buffer[8]);
    // OLED_ShowString(0, 16, oled_string, 8, 1);
    // OLED_Refresh();
    // BPS_CAN_Test_Tx();
    // BPS_CAN_Test_Rx();
    Delay_Ms(250);
    // GPIO_SetBits(Control_IO_CH1_4_GPIO_Port, Control_IO_CH1_Pin);
    // PAout( 0 ) ^= ( 1 << 0 );
    //  PBout(3) ^= (1<<0);
    //  Delay_Ms( 250 );
    //  PBout(4) ^= (1<<0);
  }
}
