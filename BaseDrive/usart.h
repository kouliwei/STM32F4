#ifndef __USART_H
#define __USART_H
#include "user.h"
#define USART_Baud 115200
#define USART3_MAX_LEN 800
extern uint8_t USART3_RX_BUF[USART3_MAX_LEN]; 
extern uint8_t USART3_TempBUF[USART3_MAX_LEN];
extern uint16_t USART3_RX_STA ;
void USART1_Config(void);
void USART1_IRQHandler(void);	
extern void DataInter_Pc(void);
extern void DataInter_AHRS(void);
extern void Send_Datatest(void);
extern void Send_VoltageAlert(void);
extern void UART_SendSensor(void);
extern void UART_SendDis(void);
extern void UART_Send(void);
extern void Send_TimeOver(void);
extern void Send_SingleSensor(void);
extern void Sci_Init(void);
static void Scia_Init(void);
static void Scid_Init(void);
void SciGPS_Init(u32 bound);
static void Scib_Init(void);
extern void Scib_Xmit(uint8_t a);
extern void Scib_Msg(char *msg);
static void Scic_Init(void);
#endif
