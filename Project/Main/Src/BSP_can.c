#include "BSP_can.h"

uint8_t CAN_ID = 0;

void BSP_CAN_Base_Init(void)
{
    GPIO_InitTypeDef GPIO_InitSturcture = {0};
    CAN_InitTypeDef CAN_InitSturcture = {0};
    CAN_FilterInitTypeDef CAN_FilterInitSturcture = {0};

    RCC_APB2PeriphClockCmd( RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOB, ENABLE ); 
    RCC_APB1PeriphClockCmd( RCC_APB1Periph_CAN1, ENABLE );
    GPIO_PinRemapConfig( GPIO_Remap1_CAN1, ENABLE ); // 重映射 Can1

    //GPIO init
    GPIO_InitSturcture.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitSturcture.GPIO_Mode = GPIO_Mode_AF_PP; // 复用推挽输出
    GPIO_InitSturcture.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init( GPIOB, &GPIO_InitSturcture );

    GPIO_InitSturcture.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitSturcture.GPIO_Mode = GPIO_Mode_IPU; // 上拉输入
    GPIO_Init( GPIOB, &GPIO_InitSturcture );

    // Can setting 
    CAN_InitSturcture.CAN_TTCM = DISABLE;       // 非时间触发模式
    CAN_InitSturcture.CAN_ABOM = DISABLE;       // 软件自动离线管理
    CAN_InitSturcture.CAN_AWUM = DISABLE;       // 睡眠模式通过软件唤醒
    CAN_InitSturcture.CAN_NART = ENABLE;        // 禁止报文自动传送
    CAN_InitSturcture.CAN_RFLM = DISABLE;       // 报文不锁定，新的会覆盖
    CAN_InitSturcture.CAN_TXFP = DISABLE;       // 优先级由报文flag决定
    CAN_InitSturcture.CAN_Mode = CAN_BASE_MODE; // 0：普通模式 1：回环模式
    CAN_InitSturcture.CAN_SJW  = CAN_SJW_TQ;    // 重新同步跳跃宽度
    // CAN_InitSturcture.CAN_BS1 = tbs1;        // 时间段1 占用时间
    // CAN_InitSturcture.CAN_BS2 = tbs2;        // 时间段2 占用时间
    // CAN_InitSturcture.CAN_Prescaler = brp;   // 分频系数
#if (CAN_BSP_MODE == CAN_BSP_100KBSP)
    CAN_InitSturcture.CAN_BS1 = CAN_BS1_8tq;
    CAN_InitSturcture.CAN_BS2 = CAN_BS2_6tq;
    CAN_InitSturcture.CAN_Prescaler = 24;
#elif (CAN_BSP_MODE == CAN_BSP_200KBSP)
    CAN_InitSturcture.CAN_BS1 = CAN_BS1_5tq;
    CAN_InitSturcture.CAN_BS2 = CAN_BS2_4tq;
    CAN_InitSturcture.CAN_Prescaler = 18;
#elif (CAN_BSP_MODE == CAN_BSP_250KBSP)
    CAN_InitSturcture.CAN_BS1 = CAN_BS1_6tq;
    CAN_InitSturcture.CAN_BS2 = CAN_BS2_5tq;
    CAN_InitSturcture.CAN_Prescaler = 12;
#elif (CAN_BSP_MODE == CAN_BSP_500KBSP)
    CAN_InitSturcture.CAN_BS1 = CAN_BS1_6tq;
    CAN_InitSturcture.CAN_BS2 = CAN_BS2_5tq;
    CAN_InitSturcture.CAN_Prescaler = 6;
#elif (CAN_BSP_MODE == CAN_BSP_1000KBSP)
    CAN_InitSturcture.CAN_BS1 = CAN_BS1_9tq;
    CAN_InitSturcture.CAN_BS2 = CAN_BS2_8tq;
    CAN_InitSturcture.CAN_Prescaler = 2;
#endif
    CAN_Init( CAN1, &CAN_InitSturcture ); 

    CAN_FilterInitSturcture.CAN_FilterNumber = 0; // 过滤器0

    /* identifier/mask mode, One 32-bit filter, StdId: 0x317 */
    CAN_FilterInitSturcture.CAN_FilterMode = CAN_FilterMode_IdMask;
    CAN_FilterInitSturcture.CAN_FilterScale = CAN_FilterScale_32bit;
    CAN_FilterInitSturcture.CAN_FilterIdHigh = CAN_ID<<10; // 标准帧前11为有效 00/11(2) ID(4) CMD(5)0 0000(5)
    CAN_FilterInitSturcture.CAN_FilterIdLow = 0;
    CAN_FilterInitSturcture.CAN_FilterMaskIdHigh = 0xFC00; // 强制识别的位号 1111 1100 000|0 0000 ID标识位强制识别 3C00为不识别前两位
    CAN_FilterInitSturcture.CAN_FilterMaskIdLow = 0x000E; 

    CAN_FilterInitSturcture.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;
    CAN_FilterInitSturcture.CAN_FilterActivation = ENABLE;
    CAN_FilterInit( &CAN_FilterInitSturcture ); 
}

uint8_t BSP_CAN_Send_Msg(uint8_t id, uint8_t cmd, uint8_t* msg, uint8_t len)
{
    uint8_t mbox;
    uint16_t i = 0;

    CanTxMsg CanTxStructure = {0};

    CanTxStructure.StdId = (uint32_t)((0x11<<9) & (((uint32_t)id)<<5) & ((uint32_t)cmd));
    CanTxStructure.ExtId = 0x00;
    CanTxStructure.IDE = CAN_Id_Standard;   // 标准帧
    CanTxStructure.RTR = CAN_RTR_Data;      // 数据帧
    CanTxStructure.DLC = len;               // 数据长度
    for( i = 0; i < len; i++ )
		{
			CanTxStructure.Data[i] = msg[i];
		} // 写数据

    mbox = CAN_Transmit( CAN1, &CanTxStructure );

    i = 0;
    while( ( CAN_TransmitStatus( CAN1, mbox ) != CAN_TxStatus_Ok ) && ( i < 0xFFF ) ){i++;}
    if( i == 0xFFF )
        return 1;
    return 0;

}


uint8_t BSP_CAN_Receive_Msg(uint8_t* buf)
{
    uint8_t i;

    CanRxMsg CanRxStructure = {0};

    if( CAN_MessagePending( CAN1, CAN_FIFO0 ) == 0 ) // 有没有消息队列 
    {
        return 0;
    }

    CAN_Receive( CAN1, CAN_FIFO0, &CanRxStructure ); // 读取队列信息

    for( i = 0; i < 8; i++ ){
        buf[i] = CanRxStructure.Data[i];
    }

    return CanRxStructure.DLC;
}

void BSP_CAN_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure = {0};
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;  // TIM3 global Interrupt
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;   // 占先优先级：1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;          // 抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             // 中断使能
    NVIC_Init(&NVIC_InitStructure);

    CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);  // 使能CAN1 FIFO0 中断
}

void BSP_CAN_Init(void)
{
    BSP_CAN_Read_ID();      // 读取设备ID
    BSP_CAN_Base_Init();    // 初始化CAN端口
    BSP_CAN_NVIC_Config();  // 中断管理
}




void BSP_CAN_Read_ID(void)
{
    uint8_t i = 0;
    i += (GPIO_ReadInputDataBit(ID0_GPIO_Port, ID0_Pin)<< 0);
    i += (GPIO_ReadInputDataBit(ID1_GPIO_Port, ID1_Pin)<< 1);
    i += (GPIO_ReadInputDataBit(ID2_GPIO_Port, ID2_Pin)<< 2);
    i += (GPIO_ReadInputDataBit(ID3_GPIO_Port, ID3_Pin)<< 3);
    CAN_ID = i;
}

/*
//void BSP_CAN_Test_Tx(void)
//{
//    static uint8_t can_test_buffer[8] = {0x01,0x02,0x01,0x02,0x01,0x02,0x01,0x02};
//    uint8_t i = 0;
//    BSP_CAN_Send_Msg(can_test_buffer,8);
//    for(i=0;i<8;++i)
//        can_test_buffer[i]++;
//}
*/

// Can 滤波器示例

// #if (Frame_Format == Standard_Frame)
//     /* identifier/mask mode, One 32-bit filter, StdId: 0x317 */
//     CAN_FilterInitSturcture.CAN_FilterMode = CAN_FilterMode_IdMask;
//     CAN_FilterInitSturcture.CAN_FilterScale = CAN_FilterScale_32bit;
//     CAN_FilterInitSturcture.CAN_FilterIdHigh = 0x62E0;     // 标准帧前11为有效 0110 0010 1110 0000 -> 011 0001 0111
//     CAN_FilterInitSturcture.CAN_FilterIdLow = 0;
//     CAN_FilterInitSturcture.CAN_FilterMaskIdHigh = 0xFFE0; // 强制识别的位号 1111 1111 1110 0000 -> 前11为ID强制识别
//     CAN_FilterInitSturcture.CAN_FilterMaskIdLow = 0x0006; 

/* identifier/mask mode, Two 16-bit filters, StdId: 0x317£¬0x316 */
//	CAN_FilterInitSturcture.CAN_FilterMode = CAN_FilterMode_IdMask;
//	CAN_FilterInitSturcture.CAN_FilterScale = CAN_FilterScale_16bit;
//	CAN_FilterInitSturcture.CAN_FilterIdHigh = 0x62E0;
//	CAN_FilterInitSturcture.CAN_FilterIdLow = 0xFFF8;
//	CAN_FilterInitSturcture.CAN_FilterMaskIdHigh = 0x62C0;
//	CAN_FilterInitSturcture.CAN_FilterMaskIdLow = 0xFFF8;

/* identifier list mode, One 32-bit filter, StdId: 0x317£¬0x316 */
//	CAN_FilterInitSturcture.CAN_FilterMode = CAN_FilterMode_IdList;
//	CAN_FilterInitSturcture.CAN_FilterScale = CAN_FilterScale_32bit;
//	CAN_FilterInitSturcture.CAN_FilterIdHigh = 0x62E0;
//	CAN_FilterInitSturcture.CAN_FilterIdLow = 0;
//	CAN_FilterInitSturcture.CAN_FilterMaskIdHigh = 0x62C0;
//	CAN_FilterInitSturcture.CAN_FilterMaskIdLow = 0;

/* identifier list mode, Two 16-bit filters, StdId: 0x317,0x316,0x315,0x314 */
//	CAN_FilterInitSturcture.CAN_FilterMode = CAN_FilterMode_IdList;
//	CAN_FilterInitSturcture.CAN_FilterScale = CAN_FilterScale_16bit;
//	CAN_FilterInitSturcture.CAN_FilterIdHigh = 0x62E0;
//	CAN_FilterInitSturcture.CAN_FilterIdLow = 0x62C0;
//	CAN_FilterInitSturcture.CAN_FilterMaskIdHigh = 0x62A0;
//	CAN_FilterInitSturcture.CAN_FilterMaskIdLow = 0x6280;

// #elif (Frame_Format == Extended_Frame)
//     /* identifier/mask mode, One 32-bit filter, ExtId: 0x12124567 */
//     CAN_FilterInitSturcture.CAN_FilterMode = CAN_FilterMode_IdMask;
//     CAN_FilterInitSturcture.CAN_FilterScale = CAN_FilterScale_32bit;
//     CAN_FilterInitSturcture.CAN_FilterIdHigh = 0x9092;
//     CAN_FilterInitSturcture.CAN_FilterIdLow = 0x2B3C;
//     CAN_FilterInitSturcture.CAN_FilterMaskIdHigh = 0xFFFF;
//     CAN_FilterInitSturcture.CAN_FilterMaskIdLow = 0xFFFE;

// #endif
