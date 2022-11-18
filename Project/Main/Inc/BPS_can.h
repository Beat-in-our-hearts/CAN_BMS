#ifndef __BPS_CAN_H
#define __BPS_CAN_H

#include "debug.h"
#include "BPS_global_define.h"

// Bps =Fpclk1/((tpb1+1+tbs2+1+1)*brp) = 36M 
#define CAN_BPS_MODE        CAN_BPS_250Kbps
#define CAN_BPS_100Kbps     1
#define CAN_BPS_200Kbps     2
#define CAN_BPS_250Kbps     3
#define CAN_BPS_500Kbps     4
#define CAN_BPS_1000Kbps    5

#define CAN_SJW_TQ         CAN_SJW_1tq 

#define CAN_BASE_MODE      CAN_Mode_Normal // CAN_Mode_LoopBack CAN_Mode_Silent CAN_Mode_Silent_LoopBack

extern uint8_t CAN_ID;

uint8_t BPS_CAN_Send_Msg(uint8_t id, uint8_t cmd, uint8_t* msg, uint8_t len);  // 发送消息
uint8_t BPS_CAN_Receive_Msg(uint8_t* buf);          // 检查是否存在消息并接受，非阻塞
void BPS_CAN_Base_Init(void);                       // CAN GPIO 重映射 模式选择 滤波器设置
void BPS_CAN_NVIC_Config(void);                     // 中断服务
void BPS_CAN_Init(void);                            // 设置CAN总线功能

void BPS_CAN_Read_ID(void);

// void BPS_CAN_Test_Tx(void);

#endif

