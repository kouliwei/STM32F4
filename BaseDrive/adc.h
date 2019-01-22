/* * USER_ADC.h * *  Created on: 2017-9-18 *      Author: koukou */ 
#ifndef ADC_H_ 
#define ADC_H_ 
#include "user.h" 

extern uint8_t  Voltage[5];
extern uint16_t uAD_Buff_A;
extern uint16_t uAD_Buff_B;

void USER_ADC_Init(void);
void PC_ReadVol(void);
void ADC_GPIO_Configuration(void) ;      
void TIM2_Configuration(void);
void ADC3_Init(void);
void ADCInit_DMA(void);

#endif /* USER_ADC_H_ */
