//=============================================================================
// author: kouliwei
// date : 20190103
//=============================================================================
//Í·ÎÄ¼þ
#include "led.h"
void SystemInitUser(void)
{
        NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
        SystemInit();
        LED_Init();
        Relay_Init();
        Sci_Init();
        USER_ADC_Init();
        EPwm_Init();
        Timer_Init();
        Para_Init();
        imu_data_decode_init();
        NVIC_Config();
}
void NVIC_Config(void)
{
        NVIC_InitTypeDef NVIC_InitStructure;
        NVIC_InitStructure.NVIC_IRQChannel=TIM4_IRQn; 
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; 
        NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; 
        NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
        NVIC_Init(&NVIC_InitStructure);
        NVIC_InitStructure.NVIC_IRQChannel=USART6_IRQn; 
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x00; 
        NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x00; 
        NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
        NVIC_Init(&NVIC_InitStructure);
        NVIC_InitStructure.NVIC_IRQChannel=USART1_IRQn; 
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x00; 
        NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x01; 
        NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
        NVIC_Init(&NVIC_InitStructure);
        NVIC_InitStructure.NVIC_IRQChannel=UART4_IRQn; 
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x00; 
        NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x02; 
        NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
        NVIC_Init(&NVIC_InitStructure);
        NVIC_InitStructure.NVIC_IRQChannel=USART2_IRQn; 
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x00; 
        NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; 
        NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
        NVIC_Init(&NVIC_InitStructure);
        NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn; 
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x03; 
        NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; 
        NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
        NVIC_Init(&NVIC_InitStructure);
        NVIC_InitStructure.NVIC_IRQChannel=USART3_IRQn; 
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; 
        NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; 
        NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
        NVIC_Init(&NVIC_InitStructure);
        //	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream0_IRQn;  
        //	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;  
        //	NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;        
        //	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           
        //	NVIC_Init(&NVIC_InitStructure);    
}
void LED_Init(void)
{    	 
        GPIO_InitTypeDef  GPIO_InitStructure;
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
        GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 |GPIO_Pin_7;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
        GPIO_Init(GPIOB, &GPIO_InitStructure);
        GPIO_SetBits(GPIOB, GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 |GPIO_Pin_7);
} 
void Relay_Init(void)
{
        GPIO_InitTypeDef  GPIO_InitStructure;
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
        GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_13 |GPIO_Pin_14;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
        GPIO_Init(GPIOG, &GPIO_InitStructure);
        GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_3 |GPIO_Pin_4;
        GPIO_Init(GPIOF, &GPIO_InitStructure);
        GPIO_ResetBits(GPIOG, GPIO_Pin_13 |GPIO_Pin_14);
        GPIO_ResetBits(GPIOF, GPIO_Pin_3 |GPIO_Pin_4);
}
void Led_Driver(uint8_t i)
{
        if (i & 0x08)
                LED3_ON();
        else
                LED3_OFF();
        if (i & 0x04)
                LED2_ON();
        else
                LED2_OFF();
        if (i & 0x02)
                LED1_ON();
        else
                LED1_OFF();
        if (i & 0x01)
                LED0_ON();
        else
                LED0_OFF();
}
void Delay_1ms(void)
{
        unsigned int i = 100,j = 0;
        while(i--)
        {
                j = 430;
                while(j--);
        }
}
void Delay_Nms(uint16_t i)
{
        while(i--)
        {
                Delay_1ms();
        }
}
extern void Circle_Led(void)
{
        uint8_t i = 1,j = 0;
        while(1)
        {
                Led_Driver(i);
                if(i == 8)
                {
                        j = 1;
                }
                else if (i == 1)
                {
                        j = 0 ;
                }
                if (j == 0)
                {
                        i = i<<1;
                }
                else
                {
                        i = i>>1;
                }
                Delay_Nms(1000);
        }
}
