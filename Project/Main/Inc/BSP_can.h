/***************************************************************** (C) COPYRIGHT  *****************************************************************
 * File Name          : BSP_can.h
 * Author             : lzx
 * Version            : V1.0.0
 * Date               : 2021/11/25
 * Description        : 【宏定义】 Can总线采用的波特率 Can工作模式（默认正常模式）
 *                      【全局变量】 CAN_ID 为当前设备号 默认上电读取一次后不再读取
 *                      【函数说明】 BSP_CAN_Base_Init       开启时钟；初始IO；CAN初始化结构体
 *                                  BSP_CAN_Read_ID         读取设备ID（上电执行一次）
 *                                  BSP_CAN_NVIC_Config     设置中断优先级
 *                                  BSP_CAN_Init            主函数中依次调用上述函数
 *                                  BSP_CAN_Send_Msg        阻塞发送  
 *                                  BSP_CAN_Receive_Msg     读取FIFO缓存信息
 ************************************************************************************************************************************************/ 
#ifndef __BSP_CAN_H
#define __BSP_CAN_H

#include "debug.h"
#include "BSP_global_define.h"

// BSP =Fpclk1/((tpb1+1+tbs2+1+1)*brp) = 36M 
#define CAN_BSP_MODE        CAN_BSP_250KBSP
#define CAN_BSP_100KBSP     1
#define CAN_BSP_200KBSP     2
#define CAN_BSP_250KBSP     3
#define CAN_BSP_500KBSP     4
#define CAN_BSP_1000KBSP    5

#define CAN_SJW_TQ         CAN_SJW_1tq 

#define CAN_BASE_MODE      CAN_Mode_Normal // CAN_Mode_LoopBack CAN_Mode_Silent CAN_Mode_Silent_LoopBack

extern uint8_t CAN_ID;

void BSP_CAN_Base_Init(void);                       // CAN GPIO 重映射 模式选择 滤波器设置
void BSP_CAN_NVIC_Config(void);                     // 中断服务
void BSP_CAN_Init(void);                            // 设置CAN总线功能

uint8_t BSP_CAN_Send_Msg(uint8_t id, uint8_t cmd, uint8_t* msg, uint8_t len);  // 发送消息
uint8_t BSP_CAN_Receive_Msg(uint8_t* buf);          // 检查是否存在消息并接受
void BSP_CAN_Read_ID(void);

// void BSP_CAN_Test_Tx(void);

#endif

