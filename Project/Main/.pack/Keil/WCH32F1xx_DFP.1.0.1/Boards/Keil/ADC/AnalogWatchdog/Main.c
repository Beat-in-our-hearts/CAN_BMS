/***************COPYRIGHT(C)  2019 WCH. A11 rights reserved*********************
* File Name          : main.c
* Author             : WCH
* Version            : V1.0.0
* Date               : 2019/10/15
* Description        : Main program body.
*******************************************************************************/ 
#include"sys.h"

/*******************************************************************************
* Function Name  : ADC_Init
* Description    : Initializes ADC collection.
* Input          : None
* Return         : None
*******************************************************************************/ 
void Adc_Init(void)
{
	ADC_InitTypeDef ADC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE );	  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE );	  
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);  
                      
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	ADC_DeInit(ADC1);  
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;		
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	
	ADC_InitStructure.ADC_NbrOfChannel = 1;	
	ADC_Init(ADC1, &ADC_InitStructure);	 
  
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	
	ADC_InitStructure.ADC_NbrOfChannel = 1;	
	ADC_Init(ADC1, &ADC_InitStructure);	  

	ADC_RegularChannelConfig(ADC1,ADC_Channel_2, 1, ADC_SampleTime_239Cycles5 );	
	
/* Higher Threshold:3500, Lower Threshold:2000 */	
	ADC_AnalogWatchdogThresholdsConfig(ADC1, 3500, 2000);		
	ADC_AnalogWatchdogSingleChannelConfig( ADC1, ADC_Channel_2);	
	ADC_AnalogWatchdogCmd( ADC1, ADC_AnalogWatchdog_SingleRegEnable);	

	NVIC_InitStructure.NVIC_IRQChannel = ADC1_2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
  ADC_ITConfig( ADC1, ADC_IT_AWD, ENABLE);  
	ADC_Cmd(ADC1, ENABLE);	
		
	ADC_ResetCalibration(ADC1);   
	while(ADC_GetResetCalibrationStatus(ADC1));	
	ADC_StartCalibration(ADC1);	 
	while(ADC_GetCalibrationStatus(ADC1));	 	
}

/*******************************************************************************
* Function Name  : Get_Adc
* Description    : Returns ADCx conversion result data.
* Input          : ch: ADC channel.
*                    ADC_Channel_0: ADC Channel0 selected.
*                    ADC_Channel_1: ADC Channel1 selected.
*                    ADC_Channel_2: ADC Channel2 selected.
*                    ADC_Channel_3: ADC Channel3 selected.
*                    ADC_Channel_4: ADC Channel4 selected.
*                    ADC_Channel_5: ADC Channel5 selected.
*                    ADC_Channel_6: ADC Channel6 selected.
*                    ADC_Channel_7: ADC Channel7 selected.
*                    ADC_Channel_8: ADC Channel8 selected.
*                    ADC_Channel_9: ADC Channel9 selected.
*                    ADC_Channel_10: ADC Channel10 selected.
*                    ADC_Channel_11: ADC Channel11 selected.
*                    ADC_Channel_12: ADC Channel12 selected.
*                    ADC_Channel_13: ADC Channel13 selected.
*                    ADC_Channel_14: ADC Channel14 selected.
*                    ADC_Channel_15: ADC Channel15 selected.
*                    ADC_Channel_16: ADC Channel16 selected.
*                    ADC_Channel_17: ADC Channel17 selected.
* Return         : val: The Data conversion value.
*******************************************************************************/
u16 Get_Adc(u8 ch)   
{	 
  u16 val;
	
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);	
	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));

	val = ADC_GetConversionValue(ADC1);
	
	return val;	
}

/*******************************************************************************
* Function Name  : ADC1_2_IRQHandler
* Description    : This function handles analog wathdog exception.
* Input          : None
* Return         : None
*******************************************************************************/
void ADC1_2_IRQHandler()
{
	if(ADC_GetITStatus( ADC1, ADC_IT_AWD)){   		
		printf( "Enter AnalogWatchdog Interrupt\r\n" );
	}
	
	ADC_ClearITPendingBit( ADC1, ADC_IT_AWD); 
}

/*******************************************************************************
* Function Name  : main
* Description    : Main program.
* Input          : None
* Return         : None
*******************************************************************************/
int main(void)
{
	u16 ADC_val;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	Delay_Init();
	Uart_Printf_Init(115200);
	printf( "Start @Chip_ID:%08x\r\n", DBGMCU->IDCODE );

	Adc_Init();
	
	while(1)
	{	  
		ADC_val = Get_Adc( ADC_Channel_2);   		
		Delay_Ms(500);
		printf( "%04d\r\n", ADC_val );
		Delay_Ms(2);
	}	
}

