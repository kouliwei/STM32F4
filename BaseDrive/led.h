#ifndef _LED_H_
#define _LED_H_


#include "user.h"


	

#define LED0_ON()  	GPIO_ResetBits(GPIOB,GPIO_Pin_4)
#define LED0_OFF()  GPIO_SetBits(GPIOB,GPIO_Pin_4)
#define LED0_TOG()  GPIO_ToggleBits(GPIOB,GPIO_Pin_4)

#define LED1_ON() 	GPIO_ResetBits(GPIOB,GPIO_Pin_5)
#define LED1_OFF()	GPIO_SetBits(GPIOB,GPIO_Pin_5)
#define LED1_TOG()  GPIO_ToggleBits(GPIOB,GPIO_Pin_5)


#define LED2_ON() 	GPIO_ResetBits(GPIOB,GPIO_Pin_6)
#define LED2_OFF()		GPIO_SetBits(GPIOB,GPIO_Pin_6)
#define LED2_TOG()  GPIO_ToggleBits(GPIOB,GPIO_Pin_6)

#define LED3_ON() 	GPIO_ResetBits(GPIOB,GPIO_Pin_7)
#define LED3_OFF()	GPIO_SetBits(GPIOB,GPIO_Pin_7)
#define LED3_TOG()  GPIO_ToggleBits(GPIOB,GPIO_Pin_7)


#define Relay1_On()  GPIO_SetBits(GPIOF,GPIO_Pin_3)
#define Relay1_Off() GPIO_ResetBits(GPIOF,GPIO_Pin_3)
#define Relay1_TOG() GPIO_ToggleBits(GPIOF,GPIO_Pin_3)



#define Relay2_On()  GPIO_SetBits(GPIOG,GPIO_Pin_13)
#define Relay2_Off() GPIO_ResetBits(GPIOG,GPIO_Pin_13)
#define Relay2_TOG() GPIO_ToggleBits(GPIOG,GPIO_Pin_13)


#define Relay3_On()  GPIO_SetBits(GPIOG,GPIO_Pin_14)
#define Relay3_Off() GPIO_ResetBits(GPIOG,GPIO_Pin_14)
#define Relay3_TOG() GPIO_ToggleBits(GPIOG,GPIO_Pin_14)


#define Relay4_On()  GPIO_SetBits(GPIOF,GPIO_Pin_4)
#define Relay4_Off() GPIO_ResetBits(GPIOF,GPIO_Pin_4)
#define Relay4_TOG() GPIO_ToggleBits(GPIOF,GPIO_Pin_4)




void Relay_Init(void);
void LED_Init(void);
void Led_Driver(uint8_t i);
void Delay_Nms(uint16_t i);
void Circle_Led(void);
void NVIC_Config(void);
void SystemInitUser(void);

#endif


