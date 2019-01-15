/*
 * USER_Spi.c
 *
 *  Created on: 2017-2-3
 *      Author: kou
 */
#include "adc.h"

uint8_t Voltage[4]={0,0,0,0};

uint16_t uAD_Buff_A = 0;
uint16_t uAD_Buff_B = 0;
uint16_t CurrentBuffPtr = 0;

void USER_ADC_Init(void)
{
	ADC_GPIO_Configuration();
	ADC3_Init();
	ADCInit_DMA();
 

}




void PC_ReadVol(void)
{
	uint16_t checksum = 0;
	Scib_Xmit(0xA5);
	Scib_Xmit(0x5A);
	Scib_Xmit(0x08);
	Scib_Xmit(0x08);
	Scib_Xmit(Voltage[0]);
	Scib_Xmit(Voltage[1]);
	Scib_Xmit(Voltage[2]);
	Scib_Xmit(Voltage[3]);
	checksum=0x08+0x08+ Voltage[0] + Voltage[1] +  Voltage[2] + Voltage[3];
	checksum=checksum%256;
	Scib_Xmit(checksum);
	Scib_Xmit(0xAA);
//	Scib_Xmit(Voltage[0] / 10+0x30);
//	Scib_Xmit(Voltage[0] % 10+0x30);
//	Scib_Xmit(0x2E);
//	Scib_Xmit(Voltage[1] / 10+0x30);
//	Scib_Xmit(Voltage[1] % 10+0x30);
//	Scib_Xmit(0x9);
//	Scib_Xmit(Voltage[2] / 10+0x30);
//	Scib_Xmit(Voltage[2] % 10+0x30);
//	Scib_Xmit(0x2E);
//	Scib_Xmit(Voltage[3] / 10+0x30);
//	Scib_Xmit(Voltage[3] % 10+0x30);
}


void TIM2_Configuration(void)
{ 
 TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure; 

 RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE); 
 TIM_TimeBaseStructure.TIM_Period = 1999; //
 TIM_TimeBaseStructure.TIM_Prescaler = 83;
 TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
 TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
 TIM_TimeBaseInit(TIM2, & TIM_TimeBaseStructure);
    
// TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
// TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
// TIM_OCInitStructure.TIM_Pulse = 1000; 
// TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
// TIM_OC2Init(TIM2, & TIM_OCInitStructure);   
	
	
}



void ADC_GPIO_Configuration(void)        //ADC????
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);   
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);   
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;        //??2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;    //??????
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOF, &GPIO_InitStructure);     //GPIO?
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;        //??2
  GPIO_Init(GPIOA, &GPIO_InitStructure); 
}




void ADC3_Init(void)
{
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
  ADC_InitTypeDef ADC_InitStructure; 
  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
  
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);      //ADC3??
  RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE); //????
	
  RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3,ENABLE);      //ADC3??
  RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3,DISABLE); //????
	
	
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//????
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles; 
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; 
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;			
	ADC_CommonInit(&ADC_CommonInitStructure); 
	
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;

	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; 
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T2_TRGO;  
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;    
	ADC_InitStructure.ADC_NbrOfConversion = 1;    
	ADC_Init(ADC3, &ADC_InitStructure);  
	ADC_InitStructure.ADC_NbrOfConversion = 1;    
	ADC_Init(ADC1, &ADC_InitStructure);  
	

	ADC_RegularChannelConfig(ADC3, ADC_Channel_4, 1, ADC_SampleTime_15Cycles); //PF4,VIN2,??14,UB

  ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 1, ADC_SampleTime_15Cycles); //PF4,VIN2,??14,UB
	
	ADC_DMARequestAfterLastTransferCmd(ADC3, ENABLE); 
	ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE); 
	
	ADC_DMACmd(ADC3, ENABLE);  
	ADC_DMACmd(ADC1, ENABLE); 
	
	ADC_Cmd(ADC3, ENABLE);	
	ADC_Cmd(ADC1, ENABLE);	
	
	ADC_SoftwareStartConv(ADC3); 
	ADC_SoftwareStartConv(ADC1); 
}

static void ADCInit_DMA(void)
{
    DMA_InitTypeDef  DMA_InitStructure;
    
    //??
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
    
    //DMA??
    DMA_InitStructure.DMA_Channel = DMA_Channel_2; //?????
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(ADC3->DR); //??????,ADC_DR_DATA???????
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)(u16 *)(&uAD_Buff_A); //DMA?????,?????????
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory; //????,??->???
    DMA_InitStructure.DMA_BufferSize = 1 * 1; 
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //?????????
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; //??????????
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //?????????(16?)
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; //?????????(16?)
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; 
    DMA_InitStructure.DMA_Priority = DMA_Priority_High; 
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable; 
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull; 
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single; 
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single; 
    DMA_Init(DMA2_Stream0, &DMA_InitStructure); 
		
		DMA_InitStructure.DMA_Channel = DMA_Channel_0; //?????
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(ADC1->DR); //??????,ADC_DR_DATA???????
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)(u16 *)(&uAD_Buff_B); //DMA?????,?????????
    DMA_Init(DMA2_Stream4, &DMA_InitStructure);

//    DMA_DoubleBufferModeConfig(DMA2_Stream0,(uint32_t)(uint16_t *)(&uAD_Buff_B),DMA_Memory_0);    //DMA_Memory_0?????
//    DMA_DoubleBufferModeCmd(DMA2_Stream0,ENABLE);
    

//    DMA_ClearITPendingBit(DMA2_Stream0, DMA_IT_TC); //??????   
//    DMA_ITConfig(DMA2_Stream0, DMA_IT_TC, ENABLE); //??????                                       
    DMA_Cmd(DMA2_Stream0, ENABLE); //??DMA
		DMA_Cmd(DMA2_Stream4, ENABLE); //??DMA
    
}



